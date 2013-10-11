# CMake minimun version required to compile
cmake_minimum_required(VERSION 2.6)

message("====================================")
message("Building Makefile for Quantum")
message("====================================")

message("Source dir: ${CMAKE_SOURCE_DIR}")
message("Binary dir: ${CMAKE_BINARY_DIR}")

add_subdirectory("${CMAKE_SOURCE_DIR}/Libraries/Quantum-Engine")

# =================================================================
# Project properties
# =================================================================
# Project name
project("Quantum")

set(QUANTUM_VERSION "0.1")
set(QUANTUM_VERSION_LONG "0.1 - Build 11-Oct-2013")

# Specify build output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Prevents cmake from following symbolic links using GLOB
cmake_policy(SET CMP0009 NEW)

# =================================================================
# Macros
# =================================================================
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
        SET(dirlist ${dirlist} ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# =================================================================
# Source Files (.cpp files)
# =================================================================
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/Game/*.cpp")
# Add the main file
set(SOURCE_FILES ${SOURCE_FILES} "${CMAKE_SOURCE_DIR}/main.cpp")

message("-- Source Files (.cpp)")
foreach(file ${SOURCE_FILES})
    message("    ${file}")
endforeach()

# =================================================================
# Include directories (containing .h files)
# =================================================================
SUBDIRLIST(INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/Game")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/Game/*.h")

message("-- Header Files (.h)")
foreach(file ${HEADER_FILES})
    message("    ${file}")
endforeach()

# =================================================================
# Libraries
# =================================================================
# Quantum Engine
# Library is already present in the CMake scope from the subdirectory of the library, so we just need to add the path to the include folder
# and add QuantumEngine (the name the library have in the subdir CMakeLists.txt) to the target_link_libraries (at the end of the file)
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} "${CMAKE_SOURCE_DIR}/Libraries/Quantum-Engine/include")

# Artemis-Cpp
find_library(ARTEMIS_LIB NAMES artemis)

# JsonCpp
add_library(JSON_LIB STATIC IMPORTED json)
set_target_properties(JSON_LIB PROPERTIES IMPORTED_LOCATION "/usr/local/lib")

# OpenGL
if(APPLE)
    find_library(OpenGL_LIBRARY OpenGL)
    set(OPENGL_LIBRARY ${OPENGL_LIBRARIES})
    if(NOT OpenGL_LIBRARY)
        message (FATAL_ERROR "Failed to find OpenGL in your Frameworks directory")
    endif(NOT OpenGL_LIBRARY)
endif(APPLE)

# SDL2 Libraries
if(APPLE)
    find_library(SDL2_LIBRARY SDL2)
    if(NOT SDL2_LIBRARY)
        message(FATAL_ERROR "Failed to find SDL2 in your Frameworks directory.")
    endif(NOT SDL2_LIBRARY)

    find_library(SDL2_IMAGE_LIBRARY SDL2_image)
    if(NOT SDL2_IMAGE_LIBRARY)
        message(FATAL_ERROR "Failed to find SDL2_image in your Frameworks directory.")
    endif(NOT SDL2_IMAGE_LIBRARY)

    find_library(SDL2_TTF_LIBRARY SDL2_ttf)
    if(NOT SDL2_TTF_LIBRARY)
        message(FATAL_ERROR "Failed to find SDL2_ttf in your Frameworks directory.")
    endif(NOT SDL2_TTF_LIBRARY)
endif(APPLE)

# Additional step for Mac OS X
if(APPLE)
    mark_as_advanced(OPENGL_LIBRARY SDL2_LIBRARY SDL2_IMAGE_LIBRARY SDL2_TTF_LIBRARY)
endif(APPLE)

# =================================================================
# Executable
# =================================================================
include_directories(${INCLUDE_DIRECTORIES})
message("Directories included in the Header search:")
foreach(dir ${INCLUDE_DIRECTORIES})
    message("    ${dir}")
endforeach(dir)

if (APPLE)
    add_executable(Quantum MACOSX_BUNDLE ${SOURCE_FILES})
    set(MACOSX_BUNDLE true)
    set(MACOSX_BUNDLE_BUNDLE_NAME Quantum)
    set(MACOSX_BUNDLE_INFO_STRING "Quantum ${QUANTUM_VERSION}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${QUANTUM_VERSION_LONG}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${QUANTUM_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${QUANTUM_VERSION}")
    set(MACOSX_BUNDLE_ICON_FILE "${CMAKE_SOURCE_DIR}/../Assets/App Icon.icns")
else (APPLE)
    add_executable(Quantum ${SOURCE_FILES})
endif(APPLE)

target_link_libraries(Quantum QuantumEngine ${ARTEMIS_LIB} ${JSON_LIB} ${OPENGL_LIBRARY} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})

# =================================================================
# Resources
# =================================================================
if(APPLE)
    set_source_files_properties(
        "${CMAKE_SOURCE_DIR}/../Assets/Images/bg.png"
        "${CMAKE_SOURCE_DIR}/../Assets/Images/bg.json"
        "${CMAKE_SOURCE_DIR}/../Assets/Images/zero.png"
        "${CMAKE_SOURCE_DIR}/../Assets/Images/zero.json"
        PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources)
endif(APPLE)

message("====================================")
message("Quantum finished")
message("====================================")