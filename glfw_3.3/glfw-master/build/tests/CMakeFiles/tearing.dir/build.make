# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.13.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.13.4/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/tearing.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/tearing.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/tearing.dir/flags.make

tests/CMakeFiles/tearing.dir/tearing.c.o: tests/CMakeFiles/tearing.dir/flags.make
tests/CMakeFiles/tearing.dir/tearing.c.o: ../tests/tearing.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/tearing.dir/tearing.c.o"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tearing.dir/tearing.c.o   -c /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/tests/tearing.c

tests/CMakeFiles/tearing.dir/tearing.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tearing.dir/tearing.c.i"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/tests/tearing.c > CMakeFiles/tearing.dir/tearing.c.i

tests/CMakeFiles/tearing.dir/tearing.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tearing.dir/tearing.c.s"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/tests/tearing.c -o CMakeFiles/tearing.dir/tearing.c.s

tests/CMakeFiles/tearing.dir/__/deps/glad_gl.c.o: tests/CMakeFiles/tearing.dir/flags.make
tests/CMakeFiles/tearing.dir/__/deps/glad_gl.c.o: ../deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/CMakeFiles/tearing.dir/__/deps/glad_gl.c.o"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/tearing.dir/__/deps/glad_gl.c.o   -c /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/deps/glad_gl.c

tests/CMakeFiles/tearing.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/tearing.dir/__/deps/glad_gl.c.i"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/deps/glad_gl.c > CMakeFiles/tearing.dir/__/deps/glad_gl.c.i

tests/CMakeFiles/tearing.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/tearing.dir/__/deps/glad_gl.c.s"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/deps/glad_gl.c -o CMakeFiles/tearing.dir/__/deps/glad_gl.c.s

# Object files for target tearing
tearing_OBJECTS = \
"CMakeFiles/tearing.dir/tearing.c.o" \
"CMakeFiles/tearing.dir/__/deps/glad_gl.c.o"

# External object files for target tearing
tearing_EXTERNAL_OBJECTS =

tests/tearing.app/Contents/MacOS/tearing: tests/CMakeFiles/tearing.dir/tearing.c.o
tests/tearing.app/Contents/MacOS/tearing: tests/CMakeFiles/tearing.dir/__/deps/glad_gl.c.o
tests/tearing.app/Contents/MacOS/tearing: tests/CMakeFiles/tearing.dir/build.make
tests/tearing.app/Contents/MacOS/tearing: src/libglfw.3.4.dylib
tests/tearing.app/Contents/MacOS/tearing: tests/CMakeFiles/tearing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable tearing.app/Contents/MacOS/tearing"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tearing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/tearing.dir/build: tests/tearing.app/Contents/MacOS/tearing

.PHONY : tests/CMakeFiles/tearing.dir/build

tests/CMakeFiles/tearing.dir/clean:
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/tearing.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/tearing.dir/clean

tests/CMakeFiles/tearing.dir/depend:
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/tests /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests/CMakeFiles/tearing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/tearing.dir/depend

