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
include lib/glfw/tests/CMakeFiles/opacity.dir/depend.make

# Include the progress variables for this target.
include lib/glfw/tests/CMakeFiles/opacity.dir/progress.make

# Include the compile flags for this target's objects.
include lib/glfw/tests/CMakeFiles/opacity.dir/flags.make

lib/glfw/tests/CMakeFiles/opacity.dir/opacity.c.o: lib/glfw/tests/CMakeFiles/opacity.dir/flags.make
lib/glfw/tests/CMakeFiles/opacity.dir/opacity.c.o: ../lib/glfw/tests/opacity.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/glfw/tests/CMakeFiles/opacity.dir/opacity.c.o"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/opacity.dir/opacity.c.o   -c /Users/thomas/Documents/COMP371/lib/glfw/tests/opacity.c

lib/glfw/tests/CMakeFiles/opacity.dir/opacity.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/opacity.dir/opacity.c.i"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/thomas/Documents/COMP371/lib/glfw/tests/opacity.c > CMakeFiles/opacity.dir/opacity.c.i

lib/glfw/tests/CMakeFiles/opacity.dir/opacity.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/opacity.dir/opacity.c.s"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/thomas/Documents/COMP371/lib/glfw/tests/opacity.c -o CMakeFiles/opacity.dir/opacity.c.s

lib/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad.c.o: lib/glfw/tests/CMakeFiles/opacity.dir/flags.make
lib/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad.c.o: ../lib/glfw/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object lib/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad.c.o"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/opacity.dir/__/deps/glad.c.o   -c /Users/thomas/Documents/COMP371/lib/glfw/deps/glad.c

lib/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/opacity.dir/__/deps/glad.c.i"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/thomas/Documents/COMP371/lib/glfw/deps/glad.c > CMakeFiles/opacity.dir/__/deps/glad.c.i

lib/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/opacity.dir/__/deps/glad.c.s"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/thomas/Documents/COMP371/lib/glfw/deps/glad.c -o CMakeFiles/opacity.dir/__/deps/glad.c.s

# Object files for target opacity
opacity_OBJECTS = \
"CMakeFiles/opacity.dir/opacity.c.o" \
"CMakeFiles/opacity.dir/__/deps/glad.c.o"

# External object files for target opacity
opacity_EXTERNAL_OBJECTS =

lib/glfw/tests/opacity.app/Contents/MacOS/opacity: lib/glfw/tests/CMakeFiles/opacity.dir/opacity.c.o
lib/glfw/tests/opacity.app/Contents/MacOS/opacity: lib/glfw/tests/CMakeFiles/opacity.dir/__/deps/glad.c.o
lib/glfw/tests/opacity.app/Contents/MacOS/opacity: lib/glfw/tests/CMakeFiles/opacity.dir/build.make
lib/glfw/tests/opacity.app/Contents/MacOS/opacity: lib/glfw/src/libglfw3.a
lib/glfw/tests/opacity.app/Contents/MacOS/opacity: lib/glfw/tests/CMakeFiles/opacity.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable opacity.app/Contents/MacOS/opacity"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opacity.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/glfw/tests/CMakeFiles/opacity.dir/build: lib/glfw/tests/opacity.app/Contents/MacOS/opacity

.PHONY : lib/glfw/tests/CMakeFiles/opacity.dir/build

lib/glfw/tests/CMakeFiles/opacity.dir/clean:
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/opacity.dir/cmake_clean.cmake
.PHONY : lib/glfw/tests/CMakeFiles/opacity.dir/clean

lib/glfw/tests/CMakeFiles/opacity.dir/depend:
	cd /Users/thomas/Documents/COMP371/cmake-build-default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/thomas/Documents/COMP371 /Users/thomas/Documents/COMP371/lib/glfw/tests /Users/thomas/Documents/COMP371/cmake-build-default /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests/CMakeFiles/opacity.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/glfw/tests/CMakeFiles/opacity.dir/depend

