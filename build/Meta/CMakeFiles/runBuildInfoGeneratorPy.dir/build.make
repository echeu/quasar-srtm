# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/root/quasar/opcua-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/root/quasar/opcua-server/build

# Utility rule file for runBuildInfoGeneratorPy.

# Include any custom commands dependencies for this target.
include Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/compiler_depend.make

# Include the progress variables for this target.
include Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/progress.make

Meta/CMakeFiles/runBuildInfoGeneratorPy: Meta/alwaysRunBuildInfoGeneratorPy

Meta/alwaysRunBuildInfoGeneratorPy: ../FrameworkInternals/meta_build_info.py
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "generating meta build info definitions header using python script"
	python3 /home/root/quasar/opcua-server/Meta/../FrameworkInternals/meta_build_info.py --project_root_dir /home/root/quasar/opcua-server --target_generation_dir /home/root/quasar/opcua-server/build/generated --toolkit_libs _lrt_lpthread

runBuildInfoGeneratorPy: Meta/CMakeFiles/runBuildInfoGeneratorPy
runBuildInfoGeneratorPy: Meta/alwaysRunBuildInfoGeneratorPy
runBuildInfoGeneratorPy: Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/build.make
.PHONY : runBuildInfoGeneratorPy

# Rule to build all files generated by this target.
Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/build: runBuildInfoGeneratorPy
.PHONY : Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/build

Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/clean:
	cd /home/root/quasar/opcua-server/build/Meta && $(CMAKE_COMMAND) -P CMakeFiles/runBuildInfoGeneratorPy.dir/cmake_clean.cmake
.PHONY : Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/clean

Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/depend:
	cd /home/root/quasar/opcua-server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/root/quasar/opcua-server /home/root/quasar/opcua-server/Meta /home/root/quasar/opcua-server/build /home/root/quasar/opcua-server/build/Meta /home/root/quasar/opcua-server/build/Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Meta/CMakeFiles/runBuildInfoGeneratorPy.dir/depend
