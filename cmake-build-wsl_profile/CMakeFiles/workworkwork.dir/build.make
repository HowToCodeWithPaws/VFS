# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/cmake-build-wsl_profile

# Include any dependencies generated for this target.
include CMakeFiles/workworkwork.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/workworkwork.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/workworkwork.dir/flags.make

CMakeFiles/workworkwork.dir/main.cpp.o: CMakeFiles/workworkwork.dir/flags.make
CMakeFiles/workworkwork.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/cmake-build-wsl_profile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/workworkwork.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/workworkwork.dir/main.cpp.o -c /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/main.cpp

CMakeFiles/workworkwork.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/workworkwork.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/main.cpp > CMakeFiles/workworkwork.dir/main.cpp.i

CMakeFiles/workworkwork.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/workworkwork.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/main.cpp -o CMakeFiles/workworkwork.dir/main.cpp.s

# Object files for target workworkwork
workworkwork_OBJECTS = \
"CMakeFiles/workworkwork.dir/main.cpp.o"

# External object files for target workworkwork
workworkwork_EXTERNAL_OBJECTS =

workworkwork: CMakeFiles/workworkwork.dir/main.cpp.o
workworkwork: CMakeFiles/workworkwork.dir/build.make
workworkwork: CMakeFiles/workworkwork.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/cmake-build-wsl_profile/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable workworkwork"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/workworkwork.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/workworkwork.dir/build: workworkwork

.PHONY : CMakeFiles/workworkwork.dir/build

CMakeFiles/workworkwork.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/workworkwork.dir/cmake_clean.cmake
.PHONY : CMakeFiles/workworkwork.dir/clean

CMakeFiles/workworkwork.dir/depend:
	cd /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/cmake-build-wsl_profile && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/cmake-build-wsl_profile /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/cmake-build-wsl_profile /mnt/c/Users/Natalya/Desktop/TheCodeSoFar/workworkwork/cmake-build-wsl_profile/CMakeFiles/workworkwork.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/workworkwork.dir/depend

