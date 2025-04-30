#   ----------------------------------------------------------------------------------
#   This is the very top level CMakeLists.txt file for the Engine.
#   This file is responsible for setting up the project and finding the dependencies.
#   It also includes the subdirectories for the dependencies and the source code.
#   This file is the entry point for the CMake build system.

#   Start here to understand the build process.
#   ----------------------------------------------------------------------------------
#   Created by Shaun Matthews on 15/07/24
#   ----------------------------------------------------------------------------------
#   Current Version of this file: 1.2
#   Please add changes to the version history below, with date and author.
#   ----------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.28)
project(Engine)
set(PROJECT_NAME Engine)

# Set the C++ standard you want to use.
set(CMAKE_CXX_STANDARD 17)


# Find the OpenGL package
find_package(OpenGL REQUIRED)

# Include directories for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/stb)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Entt)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/ImGUI)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLM/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/assimp/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/lua/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/Sol2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/JSON)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/lib/Release/x64)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/assimp/lib/Release)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/lua)
# Add subdirectories for dependencies
add_subdirectory(Dependencies/GLEW)
add_subdirectory(Dependencies/GLFW)
add_subdirectory(Dependencies/assimp)
add_subdirectory(Dependencies/ImGUI)
add_subdirectory(Dependencies/lua)
add_subdirectory(Dependencies/Sol2)
add_subdirectory(Dependencies/JSON)
add_subdirectory(source)

