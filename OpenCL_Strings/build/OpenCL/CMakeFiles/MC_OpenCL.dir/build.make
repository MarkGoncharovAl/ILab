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
CMAKE_SOURCE_DIR = /home/mark/VS_prog/ILab/OpenCL_Strings

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mark/VS_prog/ILab/OpenCL_Strings/build

# Include any dependencies generated for this target.
include OpenCL/CMakeFiles/MC_OpenCL.dir/depend.make

# Include the progress variables for this target.
include OpenCL/CMakeFiles/MC_OpenCL.dir/progress.make

# Include the compile flags for this target's objects.
include OpenCL/CMakeFiles/MC_OpenCL.dir/flags.make

OpenCL/CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.o: OpenCL/CMakeFiles/MC_OpenCL.dir/flags.make
OpenCL/CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.o: ../OpenCL/MC_OpenCL.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mark/VS_prog/ILab/OpenCL_Strings/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object OpenCL/CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.o"
	cd /home/mark/VS_prog/ILab/OpenCL_Strings/build/OpenCL && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.o -c /home/mark/VS_prog/ILab/OpenCL_Strings/OpenCL/MC_OpenCL.cpp

OpenCL/CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.i"
	cd /home/mark/VS_prog/ILab/OpenCL_Strings/build/OpenCL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mark/VS_prog/ILab/OpenCL_Strings/OpenCL/MC_OpenCL.cpp > CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.i

OpenCL/CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.s"
	cd /home/mark/VS_prog/ILab/OpenCL_Strings/build/OpenCL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mark/VS_prog/ILab/OpenCL_Strings/OpenCL/MC_OpenCL.cpp -o CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.s

# Object files for target MC_OpenCL
MC_OpenCL_OBJECTS = \
"CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.o"

# External object files for target MC_OpenCL
MC_OpenCL_EXTERNAL_OBJECTS =

OpenCL/libMC_OpenCL.a: OpenCL/CMakeFiles/MC_OpenCL.dir/MC_OpenCL.cpp.o
OpenCL/libMC_OpenCL.a: OpenCL/CMakeFiles/MC_OpenCL.dir/build.make
OpenCL/libMC_OpenCL.a: OpenCL/CMakeFiles/MC_OpenCL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mark/VS_prog/ILab/OpenCL_Strings/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libMC_OpenCL.a"
	cd /home/mark/VS_prog/ILab/OpenCL_Strings/build/OpenCL && $(CMAKE_COMMAND) -P CMakeFiles/MC_OpenCL.dir/cmake_clean_target.cmake
	cd /home/mark/VS_prog/ILab/OpenCL_Strings/build/OpenCL && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MC_OpenCL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
OpenCL/CMakeFiles/MC_OpenCL.dir/build: OpenCL/libMC_OpenCL.a

.PHONY : OpenCL/CMakeFiles/MC_OpenCL.dir/build

OpenCL/CMakeFiles/MC_OpenCL.dir/clean:
	cd /home/mark/VS_prog/ILab/OpenCL_Strings/build/OpenCL && $(CMAKE_COMMAND) -P CMakeFiles/MC_OpenCL.dir/cmake_clean.cmake
.PHONY : OpenCL/CMakeFiles/MC_OpenCL.dir/clean

OpenCL/CMakeFiles/MC_OpenCL.dir/depend:
	cd /home/mark/VS_prog/ILab/OpenCL_Strings/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mark/VS_prog/ILab/OpenCL_Strings /home/mark/VS_prog/ILab/OpenCL_Strings/OpenCL /home/mark/VS_prog/ILab/OpenCL_Strings/build /home/mark/VS_prog/ILab/OpenCL_Strings/build/OpenCL /home/mark/VS_prog/ILab/OpenCL_Strings/build/OpenCL/CMakeFiles/MC_OpenCL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : OpenCL/CMakeFiles/MC_OpenCL.dir/depend

