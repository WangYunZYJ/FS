# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /home/wy/下载/clion-2018.2.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/wy/下载/clion-2018.2.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wy/FS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wy/FS/cmake-build-debug

# Include any dependencies generated for this target.
include test/CMakeFiles/FS.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/FS.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/FS.dir/flags.make

test/CMakeFiles/FS.dir/io/io_test.cpp.o: test/CMakeFiles/FS.dir/flags.make
test/CMakeFiles/FS.dir/io/io_test.cpp.o: ../test/io/io_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wy/FS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/FS.dir/io/io_test.cpp.o"
	cd /home/wy/FS/cmake-build-debug/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FS.dir/io/io_test.cpp.o -c /home/wy/FS/test/io/io_test.cpp

test/CMakeFiles/FS.dir/io/io_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FS.dir/io/io_test.cpp.i"
	cd /home/wy/FS/cmake-build-debug/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wy/FS/test/io/io_test.cpp > CMakeFiles/FS.dir/io/io_test.cpp.i

test/CMakeFiles/FS.dir/io/io_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FS.dir/io/io_test.cpp.s"
	cd /home/wy/FS/cmake-build-debug/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wy/FS/test/io/io_test.cpp -o CMakeFiles/FS.dir/io/io_test.cpp.s

test/CMakeFiles/FS.dir/test.cpp.o: test/CMakeFiles/FS.dir/flags.make
test/CMakeFiles/FS.dir/test.cpp.o: ../test/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wy/FS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object test/CMakeFiles/FS.dir/test.cpp.o"
	cd /home/wy/FS/cmake-build-debug/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FS.dir/test.cpp.o -c /home/wy/FS/test/test.cpp

test/CMakeFiles/FS.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FS.dir/test.cpp.i"
	cd /home/wy/FS/cmake-build-debug/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wy/FS/test/test.cpp > CMakeFiles/FS.dir/test.cpp.i

test/CMakeFiles/FS.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FS.dir/test.cpp.s"
	cd /home/wy/FS/cmake-build-debug/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wy/FS/test/test.cpp -o CMakeFiles/FS.dir/test.cpp.s

test/CMakeFiles/FS.dir/volume/free_stack_test.cpp.o: test/CMakeFiles/FS.dir/flags.make
test/CMakeFiles/FS.dir/volume/free_stack_test.cpp.o: ../test/volume/free_stack_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wy/FS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object test/CMakeFiles/FS.dir/volume/free_stack_test.cpp.o"
	cd /home/wy/FS/cmake-build-debug/test && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FS.dir/volume/free_stack_test.cpp.o -c /home/wy/FS/test/volume/free_stack_test.cpp

test/CMakeFiles/FS.dir/volume/free_stack_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FS.dir/volume/free_stack_test.cpp.i"
	cd /home/wy/FS/cmake-build-debug/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wy/FS/test/volume/free_stack_test.cpp > CMakeFiles/FS.dir/volume/free_stack_test.cpp.i

test/CMakeFiles/FS.dir/volume/free_stack_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FS.dir/volume/free_stack_test.cpp.s"
	cd /home/wy/FS/cmake-build-debug/test && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wy/FS/test/volume/free_stack_test.cpp -o CMakeFiles/FS.dir/volume/free_stack_test.cpp.s

# Object files for target FS
FS_OBJECTS = \
"CMakeFiles/FS.dir/io/io_test.cpp.o" \
"CMakeFiles/FS.dir/test.cpp.o" \
"CMakeFiles/FS.dir/volume/free_stack_test.cpp.o"

# External object files for target FS
FS_EXTERNAL_OBJECTS =

../build/bin/FS: test/CMakeFiles/FS.dir/io/io_test.cpp.o
../build/bin/FS: test/CMakeFiles/FS.dir/test.cpp.o
../build/bin/FS: test/CMakeFiles/FS.dir/volume/free_stack_test.cpp.o
../build/bin/FS: test/CMakeFiles/FS.dir/build.make
../build/bin/FS: test/CMakeFiles/FS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wy/FS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../build/bin/FS"
	cd /home/wy/FS/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/FS.dir/build: ../build/bin/FS

.PHONY : test/CMakeFiles/FS.dir/build

test/CMakeFiles/FS.dir/clean:
	cd /home/wy/FS/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/FS.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/FS.dir/clean

test/CMakeFiles/FS.dir/depend:
	cd /home/wy/FS/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wy/FS /home/wy/FS/test /home/wy/FS/cmake-build-debug /home/wy/FS/cmake-build-debug/test /home/wy/FS/cmake-build-debug/test/CMakeFiles/FS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/FS.dir/depend

