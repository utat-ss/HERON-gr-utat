# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/ryan/Dev/comms/gr-utat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ryan/Dev/comms/gr-utat/build

# Include any dependencies generated for this target.
include lib/CMakeFiles/test-utat.dir/depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/test-utat.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/test-utat.dir/flags.make

lib/CMakeFiles/test-utat.dir/test_utat.cc.o: lib/CMakeFiles/test-utat.dir/flags.make
lib/CMakeFiles/test-utat.dir/test_utat.cc.o: ../lib/test_utat.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ryan/Dev/comms/gr-utat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/CMakeFiles/test-utat.dir/test_utat.cc.o"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-utat.dir/test_utat.cc.o -c /home/ryan/Dev/comms/gr-utat/lib/test_utat.cc

lib/CMakeFiles/test-utat.dir/test_utat.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-utat.dir/test_utat.cc.i"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ryan/Dev/comms/gr-utat/lib/test_utat.cc > CMakeFiles/test-utat.dir/test_utat.cc.i

lib/CMakeFiles/test-utat.dir/test_utat.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-utat.dir/test_utat.cc.s"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ryan/Dev/comms/gr-utat/lib/test_utat.cc -o CMakeFiles/test-utat.dir/test_utat.cc.s

lib/CMakeFiles/test-utat.dir/test_utat.cc.o.requires:

.PHONY : lib/CMakeFiles/test-utat.dir/test_utat.cc.o.requires

lib/CMakeFiles/test-utat.dir/test_utat.cc.o.provides: lib/CMakeFiles/test-utat.dir/test_utat.cc.o.requires
	$(MAKE) -f lib/CMakeFiles/test-utat.dir/build.make lib/CMakeFiles/test-utat.dir/test_utat.cc.o.provides.build
.PHONY : lib/CMakeFiles/test-utat.dir/test_utat.cc.o.provides

lib/CMakeFiles/test-utat.dir/test_utat.cc.o.provides.build: lib/CMakeFiles/test-utat.dir/test_utat.cc.o


lib/CMakeFiles/test-utat.dir/qa_utat.cc.o: lib/CMakeFiles/test-utat.dir/flags.make
lib/CMakeFiles/test-utat.dir/qa_utat.cc.o: ../lib/qa_utat.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ryan/Dev/comms/gr-utat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object lib/CMakeFiles/test-utat.dir/qa_utat.cc.o"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-utat.dir/qa_utat.cc.o -c /home/ryan/Dev/comms/gr-utat/lib/qa_utat.cc

lib/CMakeFiles/test-utat.dir/qa_utat.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-utat.dir/qa_utat.cc.i"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ryan/Dev/comms/gr-utat/lib/qa_utat.cc > CMakeFiles/test-utat.dir/qa_utat.cc.i

lib/CMakeFiles/test-utat.dir/qa_utat.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-utat.dir/qa_utat.cc.s"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ryan/Dev/comms/gr-utat/lib/qa_utat.cc -o CMakeFiles/test-utat.dir/qa_utat.cc.s

lib/CMakeFiles/test-utat.dir/qa_utat.cc.o.requires:

.PHONY : lib/CMakeFiles/test-utat.dir/qa_utat.cc.o.requires

lib/CMakeFiles/test-utat.dir/qa_utat.cc.o.provides: lib/CMakeFiles/test-utat.dir/qa_utat.cc.o.requires
	$(MAKE) -f lib/CMakeFiles/test-utat.dir/build.make lib/CMakeFiles/test-utat.dir/qa_utat.cc.o.provides.build
.PHONY : lib/CMakeFiles/test-utat.dir/qa_utat.cc.o.provides

lib/CMakeFiles/test-utat.dir/qa_utat.cc.o.provides.build: lib/CMakeFiles/test-utat.dir/qa_utat.cc.o


lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o: lib/CMakeFiles/test-utat.dir/flags.make
lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o: ../lib/qa_endurosat_frame_sync_bb.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ryan/Dev/comms/gr-utat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o -c /home/ryan/Dev/comms/gr-utat/lib/qa_endurosat_frame_sync_bb.cc

lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.i"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ryan/Dev/comms/gr-utat/lib/qa_endurosat_frame_sync_bb.cc > CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.i

lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.s"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ryan/Dev/comms/gr-utat/lib/qa_endurosat_frame_sync_bb.cc -o CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.s

lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o.requires:

.PHONY : lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o.requires

lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o.provides: lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o.requires
	$(MAKE) -f lib/CMakeFiles/test-utat.dir/build.make lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o.provides.build
.PHONY : lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o.provides

lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o.provides.build: lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o


# Object files for target test-utat
test__utat_OBJECTS = \
"CMakeFiles/test-utat.dir/test_utat.cc.o" \
"CMakeFiles/test-utat.dir/qa_utat.cc.o" \
"CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o"

# External object files for target test-utat
test__utat_EXTERNAL_OBJECTS =

lib/test-utat: lib/CMakeFiles/test-utat.dir/test_utat.cc.o
lib/test-utat: lib/CMakeFiles/test-utat.dir/qa_utat.cc.o
lib/test-utat: lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o
lib/test-utat: lib/CMakeFiles/test-utat.dir/build.make
lib/test-utat: /usr/lib/x86_64-linux-gnu/libgnuradio-runtime.so
lib/test-utat: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
lib/test-utat: /usr/lib/x86_64-linux-gnu/libboost_system.so
lib/test-utat: /usr/lib/x86_64-linux-gnu/libcppunit.so
lib/test-utat: lib/libgnuradio-utat-1.0.0git.so.0.0.0
lib/test-utat: /usr/lib/x86_64-linux-gnu/libgnuradio-runtime.so
lib/test-utat: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
lib/test-utat: /usr/lib/x86_64-linux-gnu/libboost_system.so
lib/test-utat: lib/CMakeFiles/test-utat.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ryan/Dev/comms/gr-utat/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable test-utat"
	cd /home/ryan/Dev/comms/gr-utat/build/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-utat.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/CMakeFiles/test-utat.dir/build: lib/test-utat

.PHONY : lib/CMakeFiles/test-utat.dir/build

lib/CMakeFiles/test-utat.dir/requires: lib/CMakeFiles/test-utat.dir/test_utat.cc.o.requires
lib/CMakeFiles/test-utat.dir/requires: lib/CMakeFiles/test-utat.dir/qa_utat.cc.o.requires
lib/CMakeFiles/test-utat.dir/requires: lib/CMakeFiles/test-utat.dir/qa_endurosat_frame_sync_bb.cc.o.requires

.PHONY : lib/CMakeFiles/test-utat.dir/requires

lib/CMakeFiles/test-utat.dir/clean:
	cd /home/ryan/Dev/comms/gr-utat/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/test-utat.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/test-utat.dir/clean

lib/CMakeFiles/test-utat.dir/depend:
	cd /home/ryan/Dev/comms/gr-utat/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ryan/Dev/comms/gr-utat /home/ryan/Dev/comms/gr-utat/lib /home/ryan/Dev/comms/gr-utat/build /home/ryan/Dev/comms/gr-utat/build/lib /home/ryan/Dev/comms/gr-utat/build/lib/CMakeFiles/test-utat.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/CMakeFiles/test-utat.dir/depend

