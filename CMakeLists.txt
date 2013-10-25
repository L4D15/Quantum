# CMake minimun version required to compile
cmake_minimum_required(VERSION 2.6)

message("====================================")
message("Building Makefile for Quantum")
message("====================================")

message("Source dir: ${CMAKE_SOURCE_DIR}")
message("Binary dir: ${CMAKE_BINARY_DIR}")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/lib/Quantum-Engine")

# =================================================================
# Project properties
# =================================================================
# Project name
project("Quantum")

set(QUANTUM_VERSION "0.1")
set(QUANTUM_VERSION_LONG "0.1 - Build 14-Oct-2013")

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
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/Game/*.cpp")
# Add the main file
set(SOURCE_FILES ${SOURCE_FILES} "${CMAKE_SOURCE_DIR}/src/main.cpp")

# =================================================================
# Include directories (containing .h files)
# =================================================================
SUBDIRLIST(INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/src/Game")
file(GLOB_RECURSE HEADER_FILES "${CMAKE_SOURCE_DIR}/src/Game/*.h")
# =================================================================
# Libraries
# =================================================================
# Quantum Engine
# Library is already present in the CMake scope from the subdirectory of the library, so we just need to add the path to the include folder
# and add QuantumEngine (the name the library have in the subdir CMakeLists.txt) to the target_link_libraries (at the end of the file)
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} "${CMAKE_SOURCE_DIR}/src/lib/Quantum-Engine/include")
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} "${CMAKE_SOURCE_DIR}/src/lib/Quantum-Engine/lib/Artemis-Cpp/include")
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} "${CMAKE_SOURCE_DIR}/src/lib/Quantum-Engine/lib/JsonCpp/include")

# =================================================================
# Resources
# =================================================================
file(GLOB_RECURSE RESOURCE_FILES_IMAGES "${CMAKE_SOURCE_DIR}/../assets/images/*")
message("Images: ${RESOURCE_FILES_IMAGES}")

if(APPLE)
    set_source_files_properties(
        ${RESOURCE_FILES_IMAGES}
        PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources/assets/images")
endif(APPLE)

# =================================================================
# Executable
# =================================================================
include_directories(${INCLUDE_DIRECTORIES})
message("Directories included in the Header search:")
foreach(dir ${INCLUDE_DIRECTORIES})
    message("    ${dir}")
endforeach(dir)

if(APPLE)
    set(MACOSX_BUNDLE true)
    set(MACOSX_BUNDLE_BUNDLE_NAME Quantum)
    set(MACOSX_BUNDLE_INFO_STRING "Quantum ${QUANTUM_VERSION}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${QUANTUM_VERSION_LONG}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${QUANTUM_VERSION}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${QUANTUM_VERSION}")
    set(MACOSX_BUNDLE_ICON_FILE "${CMAKE_SOURCE_DIR}/assets/App Icon.icns")
    add_executable(Quantum MACOSX_BUNDLE ${SOURCE_FILES} ${RESOURCE_FILES_IMAGES})
else(APPLE)
    add_executable(Quantum ${SOURCE_FILES})
endif(APPLE)

target_link_libraries(Quantum QuantumEngine)

# =================================================================
# Resources
# =================================================================
#add_custom_command(TARGET Quantum PRE_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_directory
#                       "${CMAKE_CURRENT_SOURCE_DIR}/Assets" "${CMAKE_CURRENT_BINARY_DIR}/Assets")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/res/bin" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

message("====================================")
message("Quantum finished")
message("====================================")
