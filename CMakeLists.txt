# Require CMake >= 3.8
cmake_minimum_required (VERSION 3.8)

project (ArchDemos)

add_executable (ArchDemos WIN32 src/ArchDemos.cpp src/ArchDemos.h)

set_property(TARGET ArchDemos PROPERTY CXX_STANDARD 11)

set_target_properties(ArchDemos PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
