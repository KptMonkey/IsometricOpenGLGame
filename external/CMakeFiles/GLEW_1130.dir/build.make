# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/cmake-gui

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/balu/CPP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/balu/CPP

# Include any dependencies generated for this target.
include external/CMakeFiles/GLEW_1130.dir/depend.make

# Include the progress variables for this target.
include external/CMakeFiles/GLEW_1130.dir/progress.make

# Include the compile flags for this target's objects.
include external/CMakeFiles/GLEW_1130.dir/flags.make

external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o: external/CMakeFiles/GLEW_1130.dir/flags.make
external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o: external/glew/src/glew.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/balu/CPP/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o"
	cd /home/balu/CPP/external && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o   -c /home/balu/CPP/external/glew/src/glew.c

external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/GLEW_1130.dir/glew/src/glew.c.i"
	cd /home/balu/CPP/external && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/balu/CPP/external/glew/src/glew.c > CMakeFiles/GLEW_1130.dir/glew/src/glew.c.i

external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/GLEW_1130.dir/glew/src/glew.c.s"
	cd /home/balu/CPP/external && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/balu/CPP/external/glew/src/glew.c -o CMakeFiles/GLEW_1130.dir/glew/src/glew.c.s

external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o.requires:
.PHONY : external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o.requires

external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o.provides: external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o.requires
	$(MAKE) -f external/CMakeFiles/GLEW_1130.dir/build.make external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o.provides.build
.PHONY : external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o.provides

external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o.provides.build: external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o

# Object files for target GLEW_1130
GLEW_1130_OBJECTS = \
"CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o"

# External object files for target GLEW_1130
GLEW_1130_EXTERNAL_OBJECTS =

bin/libGLEW_1130.a: external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o
bin/libGLEW_1130.a: external/CMakeFiles/GLEW_1130.dir/build.make
bin/libGLEW_1130.a: external/CMakeFiles/GLEW_1130.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library ../bin/libGLEW_1130.a"
	cd /home/balu/CPP/external && $(CMAKE_COMMAND) -P CMakeFiles/GLEW_1130.dir/cmake_clean_target.cmake
	cd /home/balu/CPP/external && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GLEW_1130.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
external/CMakeFiles/GLEW_1130.dir/build: bin/libGLEW_1130.a
.PHONY : external/CMakeFiles/GLEW_1130.dir/build

external/CMakeFiles/GLEW_1130.dir/requires: external/CMakeFiles/GLEW_1130.dir/glew/src/glew.c.o.requires
.PHONY : external/CMakeFiles/GLEW_1130.dir/requires

external/CMakeFiles/GLEW_1130.dir/clean:
	cd /home/balu/CPP/external && $(CMAKE_COMMAND) -P CMakeFiles/GLEW_1130.dir/cmake_clean.cmake
.PHONY : external/CMakeFiles/GLEW_1130.dir/clean

external/CMakeFiles/GLEW_1130.dir/depend:
	cd /home/balu/CPP && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/balu/CPP /home/balu/CPP/external /home/balu/CPP /home/balu/CPP/external /home/balu/CPP/external/CMakeFiles/GLEW_1130.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : external/CMakeFiles/GLEW_1130.dir/depend

