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
include src/CMakeFiles/FS_lib_static.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/FS_lib_static.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/FS_lib_static.dir/flags.make

src/CMakeFiles/FS_lib_static.dir/inode/inode.cpp.o: src/CMakeFiles/FS_lib_static.dir/flags.make
src/CMakeFiles/FS_lib_static.dir/inode/inode.cpp.o: ../src/inode/inode.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wy/FS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/FS_lib_static.dir/inode/inode.cpp.o"
	cd /home/wy/FS/cmake-build-debug/src && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FS_lib_static.dir/inode/inode.cpp.o -c /home/wy/FS/src/inode/inode.cpp

src/CMakeFiles/FS_lib_static.dir/inode/inode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FS_lib_static.dir/inode/inode.cpp.i"
	cd /home/wy/FS/cmake-build-debug/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wy/FS/src/inode/inode.cpp > CMakeFiles/FS_lib_static.dir/inode/inode.cpp.i

src/CMakeFiles/FS_lib_static.dir/inode/inode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FS_lib_static.dir/inode/inode.cpp.s"
	cd /home/wy/FS/cmake-build-debug/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wy/FS/src/inode/inode.cpp -o CMakeFiles/FS_lib_static.dir/inode/inode.cpp.s

src/CMakeFiles/FS_lib_static.dir/io/io.cpp.o: src/CMakeFiles/FS_lib_static.dir/flags.make
src/CMakeFiles/FS_lib_static.dir/io/io.cpp.o: ../src/io/io.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wy/FS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/FS_lib_static.dir/io/io.cpp.o"
	cd /home/wy/FS/cmake-build-debug/src && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FS_lib_static.dir/io/io.cpp.o -c /home/wy/FS/src/io/io.cpp

src/CMakeFiles/FS_lib_static.dir/io/io.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FS_lib_static.dir/io/io.cpp.i"
	cd /home/wy/FS/cmake-build-debug/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wy/FS/src/io/io.cpp > CMakeFiles/FS_lib_static.dir/io/io.cpp.i

src/CMakeFiles/FS_lib_static.dir/io/io.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FS_lib_static.dir/io/io.cpp.s"
	cd /home/wy/FS/cmake-build-debug/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wy/FS/src/io/io.cpp -o CMakeFiles/FS_lib_static.dir/io/io.cpp.s

src/CMakeFiles/FS_lib_static.dir/volume/volume.cpp.o: src/CMakeFiles/FS_lib_static.dir/flags.make
src/CMakeFiles/FS_lib_static.dir/volume/volume.cpp.o: ../src/volume/volume.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wy/FS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/FS_lib_static.dir/volume/volume.cpp.o"
	cd /home/wy/FS/cmake-build-debug/src && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FS_lib_static.dir/volume/volume.cpp.o -c /home/wy/FS/src/volume/volume.cpp

src/CMakeFiles/FS_lib_static.dir/volume/volume.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FS_lib_static.dir/volume/volume.cpp.i"
	cd /home/wy/FS/cmake-build-debug/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wy/FS/src/volume/volume.cpp > CMakeFiles/FS_lib_static.dir/volume/volume.cpp.i

src/CMakeFiles/FS_lib_static.dir/volume/volume.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FS_lib_static.dir/volume/volume.cpp.s"
	cd /home/wy/FS/cmake-build-debug/src && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wy/FS/src/volume/volume.cpp -o CMakeFiles/FS_lib_static.dir/volume/volume.cpp.s

# Object files for target FS_lib_static
FS_lib_static_OBJECTS = \
"CMakeFiles/FS_lib_static.dir/inode/inode.cpp.o" \
"CMakeFiles/FS_lib_static.dir/io/io.cpp.o" \
"CMakeFiles/FS_lib_static.dir/volume/volume.cpp.o"

# External object files for target FS_lib_static
FS_lib_static_EXTERNAL_OBJECTS =

../build/lib/libwyfs.a: src/CMakeFiles/FS_lib_static.dir/inode/inode.cpp.o
../build/lib/libwyfs.a: src/CMakeFiles/FS_lib_static.dir/io/io.cpp.o
../build/lib/libwyfs.a: src/CMakeFiles/FS_lib_static.dir/volume/volume.cpp.o
../build/lib/libwyfs.a: src/CMakeFiles/FS_lib_static.dir/build.make
../build/lib/libwyfs.a: src/CMakeFiles/FS_lib_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wy/FS/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library ../../build/lib/libwyfs.a"
	cd /home/wy/FS/cmake-build-debug/src && $(CMAKE_COMMAND) -P CMakeFiles/FS_lib_static.dir/cmake_clean_target.cmake
	cd /home/wy/FS/cmake-build-debug/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FS_lib_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/FS_lib_static.dir/build: ../build/lib/libwyfs.a

.PHONY : src/CMakeFiles/FS_lib_static.dir/build

src/CMakeFiles/FS_lib_static.dir/clean:
	cd /home/wy/FS/cmake-build-debug/src && $(CMAKE_COMMAND) -P CMakeFiles/FS_lib_static.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/FS_lib_static.dir/clean

src/CMakeFiles/FS_lib_static.dir/depend:
	cd /home/wy/FS/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wy/FS /home/wy/FS/src /home/wy/FS/cmake-build-debug /home/wy/FS/cmake-build-debug/src /home/wy/FS/cmake-build-debug/src/CMakeFiles/FS_lib_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/FS_lib_static.dir/depend
