# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/robot2/Project/ros_workspace/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/robot2/Project/ros_workspace/build

# Include any dependencies generated for this target.
include move_auto/CMakeFiles/move_auto.dir/depend.make

# Include the progress variables for this target.
include move_auto/CMakeFiles/move_auto.dir/progress.make

# Include the compile flags for this target's objects.
include move_auto/CMakeFiles/move_auto.dir/flags.make

move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o: move_auto/CMakeFiles/move_auto.dir/flags.make
move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o: /home/robot2/Project/ros_workspace/src/move_auto/move-auto.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/robot2/Project/ros_workspace/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o"
	cd /home/robot2/Project/ros_workspace/build/move_auto && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/move_auto.dir/move-auto.cpp.o -c /home/robot2/Project/ros_workspace/src/move_auto/move-auto.cpp

move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/move_auto.dir/move-auto.cpp.i"
	cd /home/robot2/Project/ros_workspace/build/move_auto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/robot2/Project/ros_workspace/src/move_auto/move-auto.cpp > CMakeFiles/move_auto.dir/move-auto.cpp.i

move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/move_auto.dir/move-auto.cpp.s"
	cd /home/robot2/Project/ros_workspace/build/move_auto && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/robot2/Project/ros_workspace/src/move_auto/move-auto.cpp -o CMakeFiles/move_auto.dir/move-auto.cpp.s

move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o.requires:

.PHONY : move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o.requires

move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o.provides: move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o.requires
	$(MAKE) -f move_auto/CMakeFiles/move_auto.dir/build.make move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o.provides.build
.PHONY : move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o.provides

move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o.provides.build: move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o


# Object files for target move_auto
move_auto_OBJECTS = \
"CMakeFiles/move_auto.dir/move-auto.cpp.o"

# External object files for target move_auto
move_auto_EXTERNAL_OBJECTS =

/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: move_auto/CMakeFiles/move_auto.dir/build.make
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/libroscpp.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_filesystem.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_signals.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/librosconsole.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/librosconsole_log4cxx.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/librosconsole_backend_interface.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/liblog4cxx.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_regex.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/libroscpp_serialization.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/libxmlrpcpp.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/librostime.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/libcpp_common.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_system.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_thread.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_chrono.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_date_time.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_atomic.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libpthread.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libconsole_bridge.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/libwiringPi.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/libroscpp.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_filesystem.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_signals.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/librosconsole.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/librosconsole_log4cxx.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/librosconsole_backend_interface.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/liblog4cxx.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_regex.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/libroscpp_serialization.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/libxmlrpcpp.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/librostime.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /opt/ros/kinetic/lib/libcpp_common.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_system.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_thread.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_chrono.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_date_time.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libboost_atomic.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libpthread.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/arm-linux-gnueabihf/libconsole_bridge.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: /usr/lib/libwiringPi.so
/home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto: move_auto/CMakeFiles/move_auto.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/robot2/Project/ros_workspace/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto"
	cd /home/robot2/Project/ros_workspace/build/move_auto && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/move_auto.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
move_auto/CMakeFiles/move_auto.dir/build: /home/robot2/Project/ros_workspace/devel/lib/move_auto/move_auto

.PHONY : move_auto/CMakeFiles/move_auto.dir/build

move_auto/CMakeFiles/move_auto.dir/requires: move_auto/CMakeFiles/move_auto.dir/move-auto.cpp.o.requires

.PHONY : move_auto/CMakeFiles/move_auto.dir/requires

move_auto/CMakeFiles/move_auto.dir/clean:
	cd /home/robot2/Project/ros_workspace/build/move_auto && $(CMAKE_COMMAND) -P CMakeFiles/move_auto.dir/cmake_clean.cmake
.PHONY : move_auto/CMakeFiles/move_auto.dir/clean

move_auto/CMakeFiles/move_auto.dir/depend:
	cd /home/robot2/Project/ros_workspace/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/robot2/Project/ros_workspace/src /home/robot2/Project/ros_workspace/src/move_auto /home/robot2/Project/ros_workspace/build /home/robot2/Project/ros_workspace/build/move_auto /home/robot2/Project/ros_workspace/build/move_auto/CMakeFiles/move_auto.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : move_auto/CMakeFiles/move_auto.dir/depend
