# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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
CMAKE_SOURCE_DIR = /home/chibisaincrada/itseez-ss-2016-practice

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chibisaincrada/itseez-ss-2016-practice

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running interactive CMake command-line interface..."
	/usr/bin/cmake -i .
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/chibisaincrada/itseez-ss-2016-practice/CMakeFiles /home/chibisaincrada/itseez-ss-2016-practice/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/chibisaincrada/itseez-ss-2016-practice/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named gtest

# Build rule for target.
gtest: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 gtest
.PHONY : gtest

# fast build rule for target.
gtest/fast:
	$(MAKE) -f 3rdparty/gtest/CMakeFiles/gtest.dir/build.make 3rdparty/gtest/CMakeFiles/gtest.dir/build
.PHONY : gtest/fast

#=============================================================================
# Target rules for targets named summer_school_2016_lib

# Build rule for target.
summer_school_2016_lib: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 summer_school_2016_lib
.PHONY : summer_school_2016_lib

# fast build rule for target.
summer_school_2016_lib/fast:
	$(MAKE) -f src/CMakeFiles/summer_school_2016_lib.dir/build.make src/CMakeFiles/summer_school_2016_lib.dir/build
.PHONY : summer_school_2016_lib/fast

#=============================================================================
# Target rules for targets named test_summer_school_2016

# Build rule for target.
test_summer_school_2016: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 test_summer_school_2016
.PHONY : test_summer_school_2016

# fast build rule for target.
test_summer_school_2016/fast:
	$(MAKE) -f test/CMakeFiles/test_summer_school_2016.dir/build.make test/CMakeFiles/test_summer_school_2016.dir/build
.PHONY : test_summer_school_2016/fast

#=============================================================================
# Target rules for targets named devtools_demo

# Build rule for target.
devtools_demo: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 devtools_demo
.PHONY : devtools_demo

# fast build rule for target.
devtools_demo/fast:
	$(MAKE) -f samples/CMakeFiles/devtools_demo.dir/build.make samples/CMakeFiles/devtools_demo.dir/build
.PHONY : devtools_demo/fast

#=============================================================================
# Target rules for targets named template_demo

# Build rule for target.
template_demo: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 template_demo
.PHONY : template_demo

# fast build rule for target.
template_demo/fast:
	$(MAKE) -f samples/CMakeFiles/template_demo.dir/build.make samples/CMakeFiles/template_demo.dir/build
.PHONY : template_demo/fast

#=============================================================================
# Target rules for targets named template_demo_max_Monday

# Build rule for target.
template_demo_max_Monday: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 template_demo_max_Monday
.PHONY : template_demo_max_Monday

# fast build rule for target.
template_demo_max_Monday/fast:
	$(MAKE) -f samples/CMakeFiles/template_demo_max_Monday.dir/build.make samples/CMakeFiles/template_demo_max_Monday.dir/build
.PHONY : template_demo_max_Monday/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... gtest"
	@echo "... summer_school_2016_lib"
	@echo "... test_summer_school_2016"
	@echo "... devtools_demo"
	@echo "... template_demo"
	@echo "... template_demo_max_Monday"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

