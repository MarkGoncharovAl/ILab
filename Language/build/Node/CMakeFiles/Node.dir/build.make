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
CMAKE_SOURCE_DIR = /home/mark/VS_prog/ILab/Language

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mark/VS_prog/ILab/Language/build

# Include any dependencies generated for this target.
include Node/CMakeFiles/Node.dir/depend.make

# Include the progress variables for this target.
include Node/CMakeFiles/Node.dir/progress.make

# Include the compile flags for this target's objects.
include Node/CMakeFiles/Node.dir/flags.make

Node/CMakeFiles/Node.dir/Node.cpp.o: Node/CMakeFiles/Node.dir/flags.make
Node/CMakeFiles/Node.dir/Node.cpp.o: ../Node/Node.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mark/VS_prog/ILab/Language/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Node/CMakeFiles/Node.dir/Node.cpp.o"
	cd /home/mark/VS_prog/ILab/Language/build/Node && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Node.dir/Node.cpp.o -c /home/mark/VS_prog/ILab/Language/Node/Node.cpp

Node/CMakeFiles/Node.dir/Node.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Node.dir/Node.cpp.i"
	cd /home/mark/VS_prog/ILab/Language/build/Node && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mark/VS_prog/ILab/Language/Node/Node.cpp > CMakeFiles/Node.dir/Node.cpp.i

Node/CMakeFiles/Node.dir/Node.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Node.dir/Node.cpp.s"
	cd /home/mark/VS_prog/ILab/Language/build/Node && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mark/VS_prog/ILab/Language/Node/Node.cpp -o CMakeFiles/Node.dir/Node.cpp.s

# Object files for target Node
Node_OBJECTS = \
"CMakeFiles/Node.dir/Node.cpp.o"

# External object files for target Node
Node_EXTERNAL_OBJECTS =

Node/libNode.a: Node/CMakeFiles/Node.dir/Node.cpp.o
Node/libNode.a: Node/CMakeFiles/Node.dir/build.make
Node/libNode.a: Node/CMakeFiles/Node.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mark/VS_prog/ILab/Language/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libNode.a"
	cd /home/mark/VS_prog/ILab/Language/build/Node && $(CMAKE_COMMAND) -P CMakeFiles/Node.dir/cmake_clean_target.cmake
	cd /home/mark/VS_prog/ILab/Language/build/Node && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Node.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Node/CMakeFiles/Node.dir/build: Node/libNode.a

.PHONY : Node/CMakeFiles/Node.dir/build

Node/CMakeFiles/Node.dir/clean:
	cd /home/mark/VS_prog/ILab/Language/build/Node && $(CMAKE_COMMAND) -P CMakeFiles/Node.dir/cmake_clean.cmake
.PHONY : Node/CMakeFiles/Node.dir/clean

Node/CMakeFiles/Node.dir/depend:
	cd /home/mark/VS_prog/ILab/Language/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mark/VS_prog/ILab/Language /home/mark/VS_prog/ILab/Language/Node /home/mark/VS_prog/ILab/Language/build /home/mark/VS_prog/ILab/Language/build/Node /home/mark/VS_prog/ILab/Language/build/Node/CMakeFiles/Node.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Node/CMakeFiles/Node.dir/depend

