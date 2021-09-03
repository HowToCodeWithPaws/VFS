#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>

namespace TestTask {
    /// Структура для файла с полями:
    /// файл знает свои начало и размеры в большом блоке
    /// и разрешения на чтение и запись
    struct File {
        size_t beginning;
        size_t actSize;
        size_t maxSize;
        bool isOpenForReading = false;
        bool isOpenForWriting = false;

        File(size_t beg, size_t act, size_t max) : beginning(beg), actSize(act), maxSize(max) {
        }
    };

    struct IVFS {

        /// Открытие в readonly, если файла нет - nullptr
        /// Так как файлы только виртуальные и на самом деле
        /// хранятся в одном большом, открывать что-то отдельно
        /// не получается, поэтому здесь просто ищется файл в мапе
        /// и задается поле доступа
        File *Open(const char *name) {
            File *filePtr = nullptr;

            if (nameMap[name] != nullptr) {

                filePtr = nameMap[name];

                if (filePtr->isOpenForReading) {
                    return filePtr;
                }
                filePtr->isOpenForReading = true;
            }
            return filePtr;
        }

        /// Открытие или создание writeonly
        /// Для записи файл тоже либо находится в мапе уже сущестующих,
        /// либо дописывается в конец большого блока. Для того, чтобы
        /// не возникало конфликтов при многопоточности, часть с записью
        /// огорожена мьютексом
        File *Create(const char *name) {
            File *filePtr = nullptr;

            /// если файл есть, то ему разрешается открываться на запись
            if (nameMap[name] != nullptr) {
                filePtr = nameMap[name];

                if (filePtr->isOpenForWriting) {
                    return filePtr;
                }
                filePtr->isOpenForWriting = true;
            } else {
                /// если файла нет, он создается в конце блока
                taskMutex.lock();
                filePtr = new File(blobSize, 0, 4096);
                char endBuff[4096] = {0};
                streamToBlob.write(endBuff, 4096);
                blobSize += 4096;

                filePtr->isOpenForWriting = true;
                nameMap[name] = filePtr;
                taskMutex.unlock();
            }
            return filePtr;
        }

        /// Чтение из файла
        /// Из большого блока считывается информация в соответствии
        /// с расположением файла в нем - сдвигом и размером
        size_t Read(File *f, char *buff, size_t len) {

            if (f == nullptr || !f->isOpenForReading) {
                return 0;
            }

            streamToBlob.seekg(f->beginning);
            streamToBlob.read(buff, std::min(len, f->actSize));

            return streamToBlob.gcount();
        }

        /// Запись в файл
        /// При записи проверяется, есть ли файл и открыт ли он для записи
        /// Далее закрывается мьютекс, и содержимое буфера записывается с
        /// учетом того, достаточно ли места в участке блока, выделенном под файл
        /// - если нет, то файл переносится в конец блока и его размер увеличивается
        size_t Write(File *f, char *buff, size_t len) {
            if (f == nullptr || !(f->isOpenForWriting)) {
                return 0;
            }
            taskMutex.lock();
            /// если файл помещается в свое оставшееся место
            if (f->actSize + len <= f->maxSize) {
                streamToBlob.seekp(f->beginning);
                streamToBlob.write(buff, len);
                f->actSize += len;
            } else {
                /// иначе он переносится в конец блока
                char buffRead[f->actSize];
                Read(f, buffRead, f->actSize);

                streamToBlob.write(buffRead, f->actSize);
                streamToBlob.write(buff, len);

                f->beginning = blobSize;
                f->maxSize = std::max(len + f->actSize, f->maxSize * 2);
                f->actSize += len;

                char endBuff[f->maxSize - f->actSize] = {0};
                streamToBlob.write(endBuff, f->maxSize - f->actSize);
                streamToBlob.seekg(0, std::ios::end);
                blobSize = streamToBlob.tellg();
            }
            taskMutex.unlock();
            return len;
        }

        /// Закрытие
        /// При закрытии у файла снимаются разрешения на то,
        /// чтобы его читали и писали
        void Close(File *f) {
            if (f == nullptr) {
                return;
            }

            if (f->isOpenForWriting) {
                f->isOpenForWriting = false;
            }

            if (f->isOpenForReading) {
                f->isOpenForReading = false;
            }
        }

        /// Конструктор для интерфейса - в начале создается и заполняется
        /// мапа с файлами, открывается поток к блоку, в котором они хранятся
        IVFS() {
            nameMap = GetMap();
            streamToBlob = std::fstream(pathToBlob, std::fstream::binary | std::fstream::in | std::fstream::out);
            if(!streamToBlob.is_open()){
              std::ofstream(pathToBlob).close();
              streamToBlob = std::fstream(pathToBlob, std::fstream::binary | std::fstream::in | std::fstream::out);
            }
            streamToBlob.seekg(0, std::ios::end);
            blobSize = streamToBlob.tellg();
        }

        /// В конце работы интерфейса нужно записывать в файл
        /// и потом освобождать мапу из указателей
        ~IVFS() {
            streamToBlob.close();
            WriteMap();
            for (auto pair : nameMap) {
                delete pair.second;
            }
        }

    private:
        /// Метод для записи мапы с информацией о файлах в файл,
        /// при этом записываются название файла, индекс его начала
        /// и размеры
        void WriteMap() {
            taskMutex.lock();
            char *buffer;
            std::ofstream stream;
            size_t numberOfFiles = nameMap.size();
            std::cout << numberOfFiles << std::endl;
            if (numberOfFiles == 0) {
                taskMutex.unlock();
                return;
            }
            stream.open(pathToMap);
            stream.write((char *) &numberOfFiles, sizeof(size_t));
            for (auto pairs : nameMap) {
                size_t size = pairs.first.size();
                stream.write((char *) &size, sizeof(size_t));
                stream.write(pairs.first.c_str(), size);
                stream.write((char *) &pairs.second->beginning, sizeof(size_t));
                stream.write((char *) &pairs.second->actSize, sizeof(size_t));
                stream.write((char *) &pairs.second->maxSize, sizeof(size_t));
            }

            taskMutex.unlock();
            stream.close();
        }

        /// Метод для считывания мапы в соответствии с форматом записи
        std::map<std::string, File *> GetMap() {
            std::ifstream stream;
            size_t numberOfFiles = 0;
            stream.open(pathToMap);
            stream.read((char *) &numberOfFiles, sizeof(size_t));
            std::string name;
            size_t stringLen, beginning, actSize, maxSize;

            for (int i = 0; i < numberOfFiles; ++i) {
                stream.read((char *) &stringLen, sizeof(size_t));
                char buf[stringLen+1] = {0};
                stream.read(buf, stringLen);
                name = buf;
                stream.read((char *) &beginning, sizeof(size_t));
                stream.read((char *) &actSize, sizeof(size_t));
                stream.read((char *) &maxSize, sizeof(size_t));

                nameMap[name] = new File(beginning, actSize, maxSize);
            }

            stream.close();
            return nameMap;
        }

        /// Поля, благодаря которым работает интерфейс:
        /// мапа для имен файлов и указателей на их структуры,
        /// пути к файлам - общему хранилищу и записанной мапе,
        /// поток для работы с блоком общего хранилища, переменная
        /// для размера блока и мьютекс для обеспечения
        /// потокобезопасности записи
        std::map<std::string, File *> nameMap;
        std::string pathToBlob = "BigBlobFile.txt";
        std::string pathToMap = "nameMap.txt";
        std::fstream streamToBlob;
        size_t blobSize = 0;
        std::mutex taskMutex;
    };
}// namespace TestTask

int main() {
  std::ifstream file("vfs.cpp");
  char rbuf[20000] = {0};
  file.read(rbuf, 20000);

  TestTask::IVFS fs;
  auto f = fs.Create("test.txt");
  printf("File = {len = %ld, pos = %ld, cap = %ld}\n", f->actSize, f->beginning, f->maxSize);
  fs.Write(f, rbuf, strlen(rbuf));
  printf("File = {len = %ld, pos = %ld, cap = %ld}\n", f->actSize, f->beginning, f->maxSize);
  fs.Close(f);

  auto f2 = fs.Open("test2.txt");
  char rbuf2[1000] = {0};
  fs.Read(f2, rbuf2, 1000);
  fs.Close(f2);
  printf("прочли:\n%s\n", rbuf2);

  return 0;
}
