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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/thomas/Documents/COMP371

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/thomas/Documents/COMP371/cmake-build-default

# Include any dependencies generated for this target.
include lib/glad/CMakeFiles/glad.dir/depend.make

# Include the progress variables for this target.
include lib/glad/CMakeFiles/glad.dir/progress.make

# Include the compile flags for this target's objects.
include lib/glad/CMakeFiles/glad.dir/flags.make

lib/glad/src/glad.c:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating GLAD"
	cd /Users/thomas/Documents/COMP371/lib/glad && /usr/local/Frameworks/Python.framework/Versions/3.7/bin/python3.7 -m glad --profile=compatibility --out-path=/Users/thomas/Documents/COMP371/cmake-build-default/lib/glad --api= --generator=c --extensions= --spec=gl

lib/glad/include/glad/glad.h: lib/glad/src/glad.c
	@$(CMAKE_COMMAND) -E touch_nocreate lib/glad/include/glad/glad.h

lib/glad/CMakeFiles/glad.dir/src/glad.c.o: lib/glad/CMakeFiles/glad.dir/flags.make
lib/glad/CMakeFiles/glad.dir/src/glad.c.o: lib/glad/src/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object lib/glad/CMakeFiles/glad.dir/src/glad.c.o"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/glad.dir/src/glad.c.o   -c /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad/src/glad.c

lib/glad/CMakeFiles/glad.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/glad.dir/src/glad.c.i"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad/src/glad.c > CMakeFiles/glad.dir/src/glad.c.i

lib/glad/CMakeFiles/glad.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/glad.dir/src/glad.c.s"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad/src/glad.c -o CMakeFiles/glad.dir/src/glad.c.s

# Object files for target glad
glad_OBJECTS = \
"CMakeFiles/glad.dir/src/glad.c.o"

# External object files for target glad
glad_EXTERNAL_OBJECTS =

lib/glad/libglad.a: lib/glad/CMakeFiles/glad.dir/src/glad.c.o
lib/glad/libglad.a: lib/glad/CMakeFiles/glad.dir/build.make
lib/glad/libglad.a: lib/glad/CMakeFiles/glad.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library libglad.a"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad && $(CMAKE_COMMAND) -P CMakeFiles/glad.dir/cmake_clean_target.cmake
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glad.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/glad/CMakeFiles/glad.dir/build: lib/glad/libglad.a

.PHONY : lib/glad/CMakeFiles/glad.dir/build

lib/glad/CMakeFiles/glad.dir/clean:
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad && $(CMAKE_COMMAND) -P CMakeFiles/glad.dir/cmake_clean.cmake
.PHONY : lib/glad/CMakeFiles/glad.dir/clean

lib/glad/CMakeFiles/glad.dir/depend: lib/glad/src/glad.c
lib/glad/CMakeFiles/glad.dir/depend: lib/glad/include/glad/glad.h
	cd /Users/thomas/Documents/COMP371/cmake-build-default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/thomas/Documents/COMP371 /Users/thomas/Documents/COMP371/lib/glad /Users/thomas/Documents/COMP371/cmake-build-default /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad /Users/thomas/Documents/COMP371/cmake-build-default/lib/glad/CMakeFiles/glad.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/glad/CMakeFiles/glad.dir/depend

