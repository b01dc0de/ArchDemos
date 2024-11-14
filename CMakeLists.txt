# Require CMake >= 3.8
cmake_minimum_required (VERSION 3.8)

project(ArchDemos)
set(SRC_DIR src)
set(SRC_FILES ${SRC_DIR}/ArchDemos.cpp)
add_executable (ArchDemos WIN32 ${SRC_FILES})

target_include_directories(ArchDemos PRIVATE libs/glfw/include)
target_include_directories(ArchDemos PRIVATE libs/glad/include)

set_property(TARGET ArchDemos PROPERTY CXX_STANDARD 11)
if(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
	#set_target_properties(ArchDemos PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
endif() # MSVC

# Build and link with GLFW
add_subdirectory(libs/glfw)
target_link_libraries(ArchDemos glfw)

# Add OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(ArchDemos OpenGL::GL)
