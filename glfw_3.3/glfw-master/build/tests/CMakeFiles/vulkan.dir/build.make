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
include tests/CMakeFiles/vulkan.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/vulkan.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/vulkan.dir/flags.make

tests/CMakeFiles/vulkan.dir/vulkan.c.o: tests/CMakeFiles/vulkan.dir/flags.make
tests/CMakeFiles/vulkan.dir/vulkan.c.o: ../tests/vulkan.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/vulkan.dir/vulkan.c.o"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vulkan.dir/vulkan.c.o   -c /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/tests/vulkan.c

tests/CMakeFiles/vulkan.dir/vulkan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vulkan.dir/vulkan.c.i"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/tests/vulkan.c > CMakeFiles/vulkan.dir/vulkan.c.i

tests/CMakeFiles/vulkan.dir/vulkan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vulkan.dir/vulkan.c.s"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/tests/vulkan.c -o CMakeFiles/vulkan.dir/vulkan.c.s

tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o: tests/CMakeFiles/vulkan.dir/flags.make
tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o: ../deps/glad_vulkan.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o   -c /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/deps/glad_vulkan.c

tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.i"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/deps/glad_vulkan.c > CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.i

tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.s"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && /usr/bin/clang $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/deps/glad_vulkan.c -o CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.s

# Object files for target vulkan
vulkan_OBJECTS = \
"CMakeFiles/vulkan.dir/vulkan.c.o" \
"CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o"

# External object files for target vulkan
vulkan_EXTERNAL_OBJECTS =

tests/vulkan: tests/CMakeFiles/vulkan.dir/vulkan.c.o
tests/vulkan: tests/CMakeFiles/vulkan.dir/__/deps/glad_vulkan.c.o
tests/vulkan: tests/CMakeFiles/vulkan.dir/build.make
tests/vulkan: src/libglfw.3.4.dylib
tests/vulkan: tests/CMakeFiles/vulkan.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable vulkan"
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vulkan.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/vulkan.dir/build: tests/vulkan

.PHONY : tests/CMakeFiles/vulkan.dir/build

tests/CMakeFiles/vulkan.dir/clean:
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/vulkan.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/vulkan.dir/clean

tests/CMakeFiles/vulkan.dir/depend:
	cd /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/tests /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests /Users/zhujunkai/Desktop/GL/glfw_3.3/glfw-master/build/tests/CMakeFiles/vulkan.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/vulkan.dir/depend

