# CMake minimun version required to compile
cmake_minimum_required(VERSION 2.6)

message("Building Makefile for Quantum")

message("Source dir: ${CMAKE_SOURCE_DIR}")
message("Binary dir: ${CMAKE_BINARY_DIR}")

# =================================================================
# Project properties
# =================================================================
# Project name
project("Quantum")

# Specify build output directories
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Prevents cmake from following symbolic links using GLOB
cmake_policy( SET CMP0009 NEW)

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
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/*.cpp")

message("Source Files (.cpp)")
foreach(file ${SOURCE_FILES})
    message("    ${file}")
endforeach()

# =================================================================
# Include directories (containing .h files)
# =================================================================
SUBDIRLIST(INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/*.h")

message("Header Files (.h)")
foreach(file ${HEADER_FILES})
    message("    ${file}")
endforeach()

# =================================================================
# Libraries
# =================================================================
# Quantum Engine
find_library(QUANTUM_ENGINE_LIB NAMES QuantumEngine HINTS "${CMAKE_SOURCE_DIR}/../Libraries/Quantum-Engine/lib")
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} "${CMAKE_SOURCE_DIR}/../Libraries/Quantum-Engine/include")

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

if (APPLE)
    add_executable(Quantum MACOSX_BUNDLE ${SOURCE_FILES})
else (APPLE)
    add_executable(Quantum ${SOURCE_FILES})
endif(APPLE)

target_link_libraries(Quantum ${QUANTUM_ENGINE_LIB} ${ARTEMIS_LIB} ${JSON_LIB} ${OPENGL_LIBRARY} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})