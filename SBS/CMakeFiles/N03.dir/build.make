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
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/losada/g4work/Ecal

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/losada/g4work/SBS

# Utility rule file for N03.

# Include the progress variables for this target.
include CMakeFiles/N03.dir/progress.make

CMakeFiles/N03: SBS

N03: CMakeFiles/N03
N03: CMakeFiles/N03.dir/build.make
.PHONY : N03

# Rule to build all files generated by this target.
CMakeFiles/N03.dir/build: N03
.PHONY : CMakeFiles/N03.dir/build

CMakeFiles/N03.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/N03.dir/cmake_clean.cmake
.PHONY : CMakeFiles/N03.dir/clean

CMakeFiles/N03.dir/depend:
	cd /home/losada/g4work/SBS && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/losada/g4work/Ecal /home/losada/g4work/Ecal /home/losada/g4work/SBS /home/losada/g4work/SBS /home/losada/g4work/SBS/CMakeFiles/N03.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/N03.dir/depend

