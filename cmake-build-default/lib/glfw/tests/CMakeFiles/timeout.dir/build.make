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
include lib/glfw/tests/CMakeFiles/timeout.dir/depend.make

# Include the progress variables for this target.
include lib/glfw/tests/CMakeFiles/timeout.dir/progress.make

# Include the compile flags for this target's objects.
include lib/glfw/tests/CMakeFiles/timeout.dir/flags.make

lib/glfw/tests/CMakeFiles/timeout.dir/timeout.c.o: lib/glfw/tests/CMakeFiles/timeout.dir/flags.make
lib/glfw/tests/CMakeFiles/timeout.dir/timeout.c.o: ../lib/glfw/tests/timeout.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/glfw/tests/CMakeFiles/timeout.dir/timeout.c.o"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/timeout.dir/timeout.c.o   -c /Users/thomas/Documents/COMP371/lib/glfw/tests/timeout.c

lib/glfw/tests/CMakeFiles/timeout.dir/timeout.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/timeout.dir/timeout.c.i"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/thomas/Documents/COMP371/lib/glfw/tests/timeout.c > CMakeFiles/timeout.dir/timeout.c.i

lib/glfw/tests/CMakeFiles/timeout.dir/timeout.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/timeout.dir/timeout.c.s"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/thomas/Documents/COMP371/lib/glfw/tests/timeout.c -o CMakeFiles/timeout.dir/timeout.c.s

lib/glfw/tests/CMakeFiles/timeout.dir/__/deps/glad.c.o: lib/glfw/tests/CMakeFiles/timeout.dir/flags.make
lib/glfw/tests/CMakeFiles/timeout.dir/__/deps/glad.c.o: ../lib/glfw/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object lib/glfw/tests/CMakeFiles/timeout.dir/__/deps/glad.c.o"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/timeout.dir/__/deps/glad.c.o   -c /Users/thomas/Documents/COMP371/lib/glfw/deps/glad.c

lib/glfw/tests/CMakeFiles/timeout.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/timeout.dir/__/deps/glad.c.i"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/thomas/Documents/COMP371/lib/glfw/deps/glad.c > CMakeFiles/timeout.dir/__/deps/glad.c.i

lib/glfw/tests/CMakeFiles/timeout.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/timeout.dir/__/deps/glad.c.s"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/thomas/Documents/COMP371/lib/glfw/deps/glad.c -o CMakeFiles/timeout.dir/__/deps/glad.c.s

# Object files for target timeout
timeout_OBJECTS = \
"CMakeFiles/timeout.dir/timeout.c.o" \
"CMakeFiles/timeout.dir/__/deps/glad.c.o"

# External object files for target timeout
timeout_EXTERNAL_OBJECTS =

lib/glfw/tests/timeout.app/Contents/MacOS/timeout: lib/glfw/tests/CMakeFiles/timeout.dir/timeout.c.o
lib/glfw/tests/timeout.app/Contents/MacOS/timeout: lib/glfw/tests/CMakeFiles/timeout.dir/__/deps/glad.c.o
lib/glfw/tests/timeout.app/Contents/MacOS/timeout: lib/glfw/tests/CMakeFiles/timeout.dir/build.make
lib/glfw/tests/timeout.app/Contents/MacOS/timeout: lib/glfw/src/libglfw3.a
lib/glfw/tests/timeout.app/Contents/MacOS/timeout: lib/glfw/tests/CMakeFiles/timeout.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable timeout.app/Contents/MacOS/timeout"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/timeout.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/glfw/tests/CMakeFiles/timeout.dir/build: lib/glfw/tests/timeout.app/Contents/MacOS/timeout

.PHONY : lib/glfw/tests/CMakeFiles/timeout.dir/build

lib/glfw/tests/CMakeFiles/timeout.dir/clean:
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests && $(CMAKE_COMMAND) -P CMakeFiles/timeout.dir/cmake_clean.cmake
.PHONY : lib/glfw/tests/CMakeFiles/timeout.dir/clean

lib/glfw/tests/CMakeFiles/timeout.dir/depend:
	cd /Users/thomas/Documents/COMP371/cmake-build-default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/thomas/Documents/COMP371 /Users/thomas/Documents/COMP371/lib/glfw/tests /Users/thomas/Documents/COMP371/cmake-build-default /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/tests/CMakeFiles/timeout.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/glfw/tests/CMakeFiles/timeout.dir/depend

