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
include lib/glfw/examples/CMakeFiles/simple.dir/depend.make

# Include the progress variables for this target.
include lib/glfw/examples/CMakeFiles/simple.dir/progress.make

# Include the compile flags for this target's objects.
include lib/glfw/examples/CMakeFiles/simple.dir/flags.make

lib/glfw/examples/simple.app/Contents/Resources/glfw.icns: ../lib/glfw/examples/glfw.icns
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Copying OS X content lib/glfw/examples/simple.app/Contents/Resources/glfw.icns"
	$(CMAKE_COMMAND) -E copy /Users/thomas/Documents/COMP371/lib/glfw/examples/glfw.icns lib/glfw/examples/simple.app/Contents/Resources/glfw.icns

lib/glfw/examples/CMakeFiles/simple.dir/simple.c.o: lib/glfw/examples/CMakeFiles/simple.dir/flags.make
lib/glfw/examples/CMakeFiles/simple.dir/simple.c.o: ../lib/glfw/examples/simple.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object lib/glfw/examples/CMakeFiles/simple.dir/simple.c.o"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/simple.dir/simple.c.o   -c /Users/thomas/Documents/COMP371/lib/glfw/examples/simple.c

lib/glfw/examples/CMakeFiles/simple.dir/simple.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple.dir/simple.c.i"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/thomas/Documents/COMP371/lib/glfw/examples/simple.c > CMakeFiles/simple.dir/simple.c.i

lib/glfw/examples/CMakeFiles/simple.dir/simple.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple.dir/simple.c.s"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/thomas/Documents/COMP371/lib/glfw/examples/simple.c -o CMakeFiles/simple.dir/simple.c.s

lib/glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o: lib/glfw/examples/CMakeFiles/simple.dir/flags.make
lib/glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o: ../lib/glfw/deps/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object lib/glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/simple.dir/__/deps/glad.c.o   -c /Users/thomas/Documents/COMP371/lib/glfw/deps/glad.c

lib/glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/simple.dir/__/deps/glad.c.i"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/thomas/Documents/COMP371/lib/glfw/deps/glad.c > CMakeFiles/simple.dir/__/deps/glad.c.i

lib/glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/simple.dir/__/deps/glad.c.s"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/thomas/Documents/COMP371/lib/glfw/deps/glad.c -o CMakeFiles/simple.dir/__/deps/glad.c.s

# Object files for target simple
simple_OBJECTS = \
"CMakeFiles/simple.dir/simple.c.o" \
"CMakeFiles/simple.dir/__/deps/glad.c.o"

# External object files for target simple
simple_EXTERNAL_OBJECTS =

lib/glfw/examples/simple.app/Contents/MacOS/simple: lib/glfw/examples/CMakeFiles/simple.dir/simple.c.o
lib/glfw/examples/simple.app/Contents/MacOS/simple: lib/glfw/examples/CMakeFiles/simple.dir/__/deps/glad.c.o
lib/glfw/examples/simple.app/Contents/MacOS/simple: lib/glfw/examples/CMakeFiles/simple.dir/build.make
lib/glfw/examples/simple.app/Contents/MacOS/simple: lib/glfw/src/libglfw3.a
lib/glfw/examples/simple.app/Contents/MacOS/simple: lib/glfw/examples/CMakeFiles/simple.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/thomas/Documents/COMP371/cmake-build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable simple.app/Contents/MacOS/simple"
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simple.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/glfw/examples/CMakeFiles/simple.dir/build: lib/glfw/examples/simple.app/Contents/MacOS/simple
lib/glfw/examples/CMakeFiles/simple.dir/build: lib/glfw/examples/simple.app/Contents/Resources/glfw.icns

.PHONY : lib/glfw/examples/CMakeFiles/simple.dir/build

lib/glfw/examples/CMakeFiles/simple.dir/clean:
	cd /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples && $(CMAKE_COMMAND) -P CMakeFiles/simple.dir/cmake_clean.cmake
.PHONY : lib/glfw/examples/CMakeFiles/simple.dir/clean

lib/glfw/examples/CMakeFiles/simple.dir/depend:
	cd /Users/thomas/Documents/COMP371/cmake-build-default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/thomas/Documents/COMP371 /Users/thomas/Documents/COMP371/lib/glfw/examples /Users/thomas/Documents/COMP371/cmake-build-default /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples /Users/thomas/Documents/COMP371/cmake-build-default/lib/glfw/examples/CMakeFiles/simple.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/glfw/examples/CMakeFiles/simple.dir/depend

