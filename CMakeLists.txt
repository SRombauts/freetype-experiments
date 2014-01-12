# Copyright (c) 2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT) 

cmake_minimum_required(VERSION 2.6)
project(freetype-experiments)

# Define useful variables to handle OS/Compiler differences
if (MSVC)
    set(SYSTEM_LIBRARIES        opengl32.lib)
    # disable Visual Studio usual warnings
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
elseif (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(SYSTEM_LIBRARIES        GL Xrandr Xi)
endif ()

## Core source code ##

# adding a new file require explicittly modifing the CMakeLists.txt
# so that CMake knows that it should rebuild the project (it is best practice)

# add sources for the executable :
add_executable(freetype-experiments
 src/Main.cpp
)


## Libraries ##

# add the subdirectory of the glfw static library (but disable any other build)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
set(GLFW_DOCUMENT_INTERNALS OFF CACHE BOOL "Include internals in documentation")
add_subdirectory(glfw)
include_directories("${PROJECT_SOURCE_DIR}/glfw/include")

# add the subdirectory of the freetype2 library
add_subdirectory(freetype2)
include_directories("${PROJECT_SOURCE_DIR}/freetype2/include")

# add the subdirectory of the gltext library (telling it where to find freetype)
set(FREETYPE_LIBRARY "${PROJECT_SOURCE_DIR}/freetype2")
set(FREETYPE_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/freetype2/include")
add_subdirectory(gltext)
include_directories("${PROJECT_SOURCE_DIR}/gltext")

## Linking ##

# link the executable with all required libraries
target_link_libraries(freetype-experiments freetype gltext glfw ${SYSTEM_LIBRARIES})


