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

# Include any dependencies generated for this target.
include LogIt/CMakeFiles/LogIt.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include LogIt/CMakeFiles/LogIt.dir/compiler_depend.make

# Include the progress variables for this target.
include LogIt/CMakeFiles/LogIt.dir/progress.make

# Include the compile flags for this target's objects.
include LogIt/CMakeFiles/LogIt.dir/flags.make

LogIt/CMakeFiles/LogIt.dir/src/LogIt.cpp.o: LogIt/CMakeFiles/LogIt.dir/flags.make
LogIt/CMakeFiles/LogIt.dir/src/LogIt.cpp.o: ../LogIt/src/LogIt.cpp
LogIt/CMakeFiles/LogIt.dir/src/LogIt.cpp.o: LogIt/CMakeFiles/LogIt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object LogIt/CMakeFiles/LogIt.dir/src/LogIt.cpp.o"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LogIt/CMakeFiles/LogIt.dir/src/LogIt.cpp.o -MF CMakeFiles/LogIt.dir/src/LogIt.cpp.o.d -o CMakeFiles/LogIt.dir/src/LogIt.cpp.o -c /home/root/quasar/opcua-server/LogIt/src/LogIt.cpp

LogIt/CMakeFiles/LogIt.dir/src/LogIt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogIt.dir/src/LogIt.cpp.i"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/LogIt/src/LogIt.cpp > CMakeFiles/LogIt.dir/src/LogIt.cpp.i

LogIt/CMakeFiles/LogIt.dir/src/LogIt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogIt.dir/src/LogIt.cpp.s"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/LogIt/src/LogIt.cpp -o CMakeFiles/LogIt.dir/src/LogIt.cpp.s

LogIt/CMakeFiles/LogIt.dir/src/LogLevels.cpp.o: LogIt/CMakeFiles/LogIt.dir/flags.make
LogIt/CMakeFiles/LogIt.dir/src/LogLevels.cpp.o: ../LogIt/src/LogLevels.cpp
LogIt/CMakeFiles/LogIt.dir/src/LogLevels.cpp.o: LogIt/CMakeFiles/LogIt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object LogIt/CMakeFiles/LogIt.dir/src/LogLevels.cpp.o"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LogIt/CMakeFiles/LogIt.dir/src/LogLevels.cpp.o -MF CMakeFiles/LogIt.dir/src/LogLevels.cpp.o.d -o CMakeFiles/LogIt.dir/src/LogLevels.cpp.o -c /home/root/quasar/opcua-server/LogIt/src/LogLevels.cpp

LogIt/CMakeFiles/LogIt.dir/src/LogLevels.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogIt.dir/src/LogLevels.cpp.i"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/LogIt/src/LogLevels.cpp > CMakeFiles/LogIt.dir/src/LogLevels.cpp.i

LogIt/CMakeFiles/LogIt.dir/src/LogLevels.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogIt.dir/src/LogLevels.cpp.s"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/LogIt/src/LogLevels.cpp -o CMakeFiles/LogIt.dir/src/LogLevels.cpp.s

LogIt/CMakeFiles/LogIt.dir/src/LogRecord.cpp.o: LogIt/CMakeFiles/LogIt.dir/flags.make
LogIt/CMakeFiles/LogIt.dir/src/LogRecord.cpp.o: ../LogIt/src/LogRecord.cpp
LogIt/CMakeFiles/LogIt.dir/src/LogRecord.cpp.o: LogIt/CMakeFiles/LogIt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object LogIt/CMakeFiles/LogIt.dir/src/LogRecord.cpp.o"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LogIt/CMakeFiles/LogIt.dir/src/LogRecord.cpp.o -MF CMakeFiles/LogIt.dir/src/LogRecord.cpp.o.d -o CMakeFiles/LogIt.dir/src/LogRecord.cpp.o -c /home/root/quasar/opcua-server/LogIt/src/LogRecord.cpp

LogIt/CMakeFiles/LogIt.dir/src/LogRecord.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogIt.dir/src/LogRecord.cpp.i"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/LogIt/src/LogRecord.cpp > CMakeFiles/LogIt.dir/src/LogRecord.cpp.i

LogIt/CMakeFiles/LogIt.dir/src/LogRecord.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogIt.dir/src/LogRecord.cpp.s"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/LogIt/src/LogRecord.cpp -o CMakeFiles/LogIt.dir/src/LogRecord.cpp.s

LogIt/CMakeFiles/LogIt.dir/src/LogSinks.cpp.o: LogIt/CMakeFiles/LogIt.dir/flags.make
LogIt/CMakeFiles/LogIt.dir/src/LogSinks.cpp.o: ../LogIt/src/LogSinks.cpp
LogIt/CMakeFiles/LogIt.dir/src/LogSinks.cpp.o: LogIt/CMakeFiles/LogIt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object LogIt/CMakeFiles/LogIt.dir/src/LogSinks.cpp.o"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LogIt/CMakeFiles/LogIt.dir/src/LogSinks.cpp.o -MF CMakeFiles/LogIt.dir/src/LogSinks.cpp.o.d -o CMakeFiles/LogIt.dir/src/LogSinks.cpp.o -c /home/root/quasar/opcua-server/LogIt/src/LogSinks.cpp

LogIt/CMakeFiles/LogIt.dir/src/LogSinks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogIt.dir/src/LogSinks.cpp.i"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/LogIt/src/LogSinks.cpp > CMakeFiles/LogIt.dir/src/LogSinks.cpp.i

LogIt/CMakeFiles/LogIt.dir/src/LogSinks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogIt.dir/src/LogSinks.cpp.s"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/LogIt/src/LogSinks.cpp -o CMakeFiles/LogIt.dir/src/LogSinks.cpp.s

LogIt/CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.o: LogIt/CMakeFiles/LogIt.dir/flags.make
LogIt/CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.o: ../LogIt/src/ComponentAttributes.cpp
LogIt/CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.o: LogIt/CMakeFiles/LogIt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object LogIt/CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.o"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LogIt/CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.o -MF CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.o.d -o CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.o -c /home/root/quasar/opcua-server/LogIt/src/ComponentAttributes.cpp

LogIt/CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.i"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/LogIt/src/ComponentAttributes.cpp > CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.i

LogIt/CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.s"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/LogIt/src/ComponentAttributes.cpp -o CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.s

LogIt/CMakeFiles/LogIt.dir/src/LogItInstance.cpp.o: LogIt/CMakeFiles/LogIt.dir/flags.make
LogIt/CMakeFiles/LogIt.dir/src/LogItInstance.cpp.o: ../LogIt/src/LogItInstance.cpp
LogIt/CMakeFiles/LogIt.dir/src/LogItInstance.cpp.o: LogIt/CMakeFiles/LogIt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object LogIt/CMakeFiles/LogIt.dir/src/LogItInstance.cpp.o"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LogIt/CMakeFiles/LogIt.dir/src/LogItInstance.cpp.o -MF CMakeFiles/LogIt.dir/src/LogItInstance.cpp.o.d -o CMakeFiles/LogIt.dir/src/LogItInstance.cpp.o -c /home/root/quasar/opcua-server/LogIt/src/LogItInstance.cpp

LogIt/CMakeFiles/LogIt.dir/src/LogItInstance.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogIt.dir/src/LogItInstance.cpp.i"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/LogIt/src/LogItInstance.cpp > CMakeFiles/LogIt.dir/src/LogItInstance.cpp.i

LogIt/CMakeFiles/LogIt.dir/src/LogItInstance.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogIt.dir/src/LogItInstance.cpp.s"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/LogIt/src/LogItInstance.cpp -o CMakeFiles/LogIt.dir/src/LogItInstance.cpp.s

LogIt/CMakeFiles/LogIt.dir/src/StdOutLog.cpp.o: LogIt/CMakeFiles/LogIt.dir/flags.make
LogIt/CMakeFiles/LogIt.dir/src/StdOutLog.cpp.o: ../LogIt/src/StdOutLog.cpp
LogIt/CMakeFiles/LogIt.dir/src/StdOutLog.cpp.o: LogIt/CMakeFiles/LogIt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object LogIt/CMakeFiles/LogIt.dir/src/StdOutLog.cpp.o"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT LogIt/CMakeFiles/LogIt.dir/src/StdOutLog.cpp.o -MF CMakeFiles/LogIt.dir/src/StdOutLog.cpp.o.d -o CMakeFiles/LogIt.dir/src/StdOutLog.cpp.o -c /home/root/quasar/opcua-server/LogIt/src/StdOutLog.cpp

LogIt/CMakeFiles/LogIt.dir/src/StdOutLog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LogIt.dir/src/StdOutLog.cpp.i"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/LogIt/src/StdOutLog.cpp > CMakeFiles/LogIt.dir/src/StdOutLog.cpp.i

LogIt/CMakeFiles/LogIt.dir/src/StdOutLog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LogIt.dir/src/StdOutLog.cpp.s"
	cd /home/root/quasar/opcua-server/build/LogIt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/LogIt/src/StdOutLog.cpp -o CMakeFiles/LogIt.dir/src/StdOutLog.cpp.s

LogIt: LogIt/CMakeFiles/LogIt.dir/src/LogIt.cpp.o
LogIt: LogIt/CMakeFiles/LogIt.dir/src/LogLevels.cpp.o
LogIt: LogIt/CMakeFiles/LogIt.dir/src/LogRecord.cpp.o
LogIt: LogIt/CMakeFiles/LogIt.dir/src/LogSinks.cpp.o
LogIt: LogIt/CMakeFiles/LogIt.dir/src/ComponentAttributes.cpp.o
LogIt: LogIt/CMakeFiles/LogIt.dir/src/LogItInstance.cpp.o
LogIt: LogIt/CMakeFiles/LogIt.dir/src/StdOutLog.cpp.o
LogIt: LogIt/CMakeFiles/LogIt.dir/build.make
.PHONY : LogIt

# Rule to build all files generated by this target.
LogIt/CMakeFiles/LogIt.dir/build: LogIt
.PHONY : LogIt/CMakeFiles/LogIt.dir/build

LogIt/CMakeFiles/LogIt.dir/clean:
	cd /home/root/quasar/opcua-server/build/LogIt && $(CMAKE_COMMAND) -P CMakeFiles/LogIt.dir/cmake_clean.cmake
.PHONY : LogIt/CMakeFiles/LogIt.dir/clean

LogIt/CMakeFiles/LogIt.dir/depend:
	cd /home/root/quasar/opcua-server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/root/quasar/opcua-server /home/root/quasar/opcua-server/LogIt /home/root/quasar/opcua-server/build /home/root/quasar/opcua-server/build/LogIt /home/root/quasar/opcua-server/build/LogIt/CMakeFiles/LogIt.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : LogIt/CMakeFiles/LogIt.dir/depend
