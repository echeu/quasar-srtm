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
include Configuration/CMakeFiles/Configuration.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Configuration/CMakeFiles/Configuration.dir/compiler_depend.make

# Include the progress variables for this target.
include Configuration/CMakeFiles/Configuration.dir/progress.make

# Include the compile flags for this target's objects.
include Configuration/CMakeFiles/Configuration.dir/flags.make

Configuration/Configuration.cxx: Configuration/Configuration.xsd
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating Configuration.cxx, Configuration.hxx"
	cd /home/root/quasar/opcua-server/Configuration && xsdcxx cxx-tree --std c++11 --ordered-type-all --generate-serialization --namespace-map http://cern.ch/quasar/Configuration=Configuration --output-dir /home/root/quasar/opcua-server/build/Configuration /home/root/quasar/opcua-server/build/Configuration/Configuration.xsd

Configuration/Configuration.hxx: Configuration/Configuration.cxx
	@$(CMAKE_COMMAND) -E touch_nocreate Configuration/Configuration.hxx

Configuration/Configurator.cpp: ../Design/Design.xml
Configuration/Configurator.cpp: ../Configuration/templates/designToConfigurator.jinja
Configuration/Configurator.cpp: ../quasar.py
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating Configurator.cpp"
	cd /home/root/quasar/opcua-server && python3 quasar.py generate config_cpp --project_binary_dir /home/root/quasar/opcua-server/build

Configuration/ConfigValidator.cpp: ../Design/Design.xml
Configuration/ConfigValidator.cpp: ../Configuration/templates/designToConfigValidator.jinja
Configuration/ConfigValidator.cpp: ../quasar.py
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating ConfigValidator.cpp"
	cd /home/root/quasar/opcua-server && python3 quasar.py generate config_validator --project_binary_dir /home/root/quasar/opcua-server/build

Configuration/Configuration.xsd: ../Design/Design.xml
Configuration/Configuration.xsd: ../Configuration/templates/designToConfigurationXSD.jinja
Configuration/Configuration.xsd: ../quasar.py
Configuration/Configuration.xsd: ../Meta/config/Meta.xsd
Configuration/Configuration.xsd: ../FrameworkInternals/transformDesign.py
Configuration/Configuration.xsd: ../FrameworkInternals/DesignInspector.py
Configuration/Configuration.xsd: ../FrameworkInternals/Oracle.py
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Generating Configuration.xsd"
	cd /home/root/quasar/opcua-server && python3 quasar.py generate config_xsd --project_binary_dir /home/root/quasar/opcua-server/build

Configuration/CMakeFiles/Configuration.dir/Configuration.cxx.o: Configuration/CMakeFiles/Configuration.dir/flags.make
Configuration/CMakeFiles/Configuration.dir/Configuration.cxx.o: Configuration/Configuration.cxx
Configuration/CMakeFiles/Configuration.dir/Configuration.cxx.o: Configuration/CMakeFiles/Configuration.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object Configuration/CMakeFiles/Configuration.dir/Configuration.cxx.o"
	cd /home/root/quasar/opcua-server/build/Configuration && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Configuration/CMakeFiles/Configuration.dir/Configuration.cxx.o -MF CMakeFiles/Configuration.dir/Configuration.cxx.o.d -o CMakeFiles/Configuration.dir/Configuration.cxx.o -c /home/root/quasar/opcua-server/build/Configuration/Configuration.cxx

Configuration/CMakeFiles/Configuration.dir/Configuration.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Configuration.dir/Configuration.cxx.i"
	cd /home/root/quasar/opcua-server/build/Configuration && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/build/Configuration/Configuration.cxx > CMakeFiles/Configuration.dir/Configuration.cxx.i

Configuration/CMakeFiles/Configuration.dir/Configuration.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Configuration.dir/Configuration.cxx.s"
	cd /home/root/quasar/opcua-server/build/Configuration && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/build/Configuration/Configuration.cxx -o CMakeFiles/Configuration.dir/Configuration.cxx.s

Configuration/CMakeFiles/Configuration.dir/Configurator.cpp.o: Configuration/CMakeFiles/Configuration.dir/flags.make
Configuration/CMakeFiles/Configuration.dir/Configurator.cpp.o: Configuration/Configurator.cpp
Configuration/CMakeFiles/Configuration.dir/Configurator.cpp.o: Configuration/CMakeFiles/Configuration.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object Configuration/CMakeFiles/Configuration.dir/Configurator.cpp.o"
	cd /home/root/quasar/opcua-server/build/Configuration && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Configuration/CMakeFiles/Configuration.dir/Configurator.cpp.o -MF CMakeFiles/Configuration.dir/Configurator.cpp.o.d -o CMakeFiles/Configuration.dir/Configurator.cpp.o -c /home/root/quasar/opcua-server/build/Configuration/Configurator.cpp

Configuration/CMakeFiles/Configuration.dir/Configurator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Configuration.dir/Configurator.cpp.i"
	cd /home/root/quasar/opcua-server/build/Configuration && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/build/Configuration/Configurator.cpp > CMakeFiles/Configuration.dir/Configurator.cpp.i

Configuration/CMakeFiles/Configuration.dir/Configurator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Configuration.dir/Configurator.cpp.s"
	cd /home/root/quasar/opcua-server/build/Configuration && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/build/Configuration/Configurator.cpp -o CMakeFiles/Configuration.dir/Configurator.cpp.s

Configuration/CMakeFiles/Configuration.dir/ConfigValidator.cpp.o: Configuration/CMakeFiles/Configuration.dir/flags.make
Configuration/CMakeFiles/Configuration.dir/ConfigValidator.cpp.o: Configuration/ConfigValidator.cpp
Configuration/CMakeFiles/Configuration.dir/ConfigValidator.cpp.o: Configuration/CMakeFiles/Configuration.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/root/quasar/opcua-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object Configuration/CMakeFiles/Configuration.dir/ConfigValidator.cpp.o"
	cd /home/root/quasar/opcua-server/build/Configuration && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Configuration/CMakeFiles/Configuration.dir/ConfigValidator.cpp.o -MF CMakeFiles/Configuration.dir/ConfigValidator.cpp.o.d -o CMakeFiles/Configuration.dir/ConfigValidator.cpp.o -c /home/root/quasar/opcua-server/build/Configuration/ConfigValidator.cpp

Configuration/CMakeFiles/Configuration.dir/ConfigValidator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Configuration.dir/ConfigValidator.cpp.i"
	cd /home/root/quasar/opcua-server/build/Configuration && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/root/quasar/opcua-server/build/Configuration/ConfigValidator.cpp > CMakeFiles/Configuration.dir/ConfigValidator.cpp.i

Configuration/CMakeFiles/Configuration.dir/ConfigValidator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Configuration.dir/ConfigValidator.cpp.s"
	cd /home/root/quasar/opcua-server/build/Configuration && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/root/quasar/opcua-server/build/Configuration/ConfigValidator.cpp -o CMakeFiles/Configuration.dir/ConfigValidator.cpp.s

Configuration: Configuration/CMakeFiles/Configuration.dir/Configuration.cxx.o
Configuration: Configuration/CMakeFiles/Configuration.dir/Configurator.cpp.o
Configuration: Configuration/CMakeFiles/Configuration.dir/ConfigValidator.cpp.o
Configuration: Configuration/CMakeFiles/Configuration.dir/build.make
.PHONY : Configuration

# Rule to build all files generated by this target.
Configuration/CMakeFiles/Configuration.dir/build: Configuration
.PHONY : Configuration/CMakeFiles/Configuration.dir/build

Configuration/CMakeFiles/Configuration.dir/clean:
	cd /home/root/quasar/opcua-server/build/Configuration && $(CMAKE_COMMAND) -P CMakeFiles/Configuration.dir/cmake_clean.cmake
.PHONY : Configuration/CMakeFiles/Configuration.dir/clean

Configuration/CMakeFiles/Configuration.dir/depend: Configuration/ConfigValidator.cpp
Configuration/CMakeFiles/Configuration.dir/depend: Configuration/Configuration.cxx
Configuration/CMakeFiles/Configuration.dir/depend: Configuration/Configuration.hxx
Configuration/CMakeFiles/Configuration.dir/depend: Configuration/Configuration.xsd
Configuration/CMakeFiles/Configuration.dir/depend: Configuration/Configurator.cpp
	cd /home/root/quasar/opcua-server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/root/quasar/opcua-server /home/root/quasar/opcua-server/Configuration /home/root/quasar/opcua-server/build /home/root/quasar/opcua-server/build/Configuration /home/root/quasar/opcua-server/build/Configuration/CMakeFiles/Configuration.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Configuration/CMakeFiles/Configuration.dir/depend

