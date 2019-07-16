cmake_minimum_required (VERSION 3.13.3)

set (CMAKE_CONFIGURATION_TYPES "Debug;Optimized;Shipping" CACHE STRING "TEST" FORCE)

enable_language (CXX)

if (NOT ARGON_TARGET)
	message(FATAL_ERROR "ARGON_TARGET is not set")
endif()

project (${ARGON_TARGET})

set (CMAKE_CXX_STANDARD 17)
set (
	CMAKE_CXX_FLAGS
	"-Weverything -Werror -Wno-c++98-compat -Wno-c++98-compat-pedantic \
	-Wno-float-equal -Wno-global-constructors -Wno-comma -Wno-c++98-compat-pedantic \
	-Wno-weak-vtables -Wno-exit-time-destructors \
	-std=c++17 -fms-extensions -fno-exceptions -Wno-undef \
	-fvisibility=hidden -mavx")

set_property (GLOBAL PROPERTY CXX_STANDARD 17)

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
	message (FATAL_ERROR "Only 64-bit build is supported")
endif ()

add_compile_definitions (AR_CLANG)

if (UNIX)
	add_compile_definitions (AR_UNIX)
elseif (WIN32)
	add_compile_definitions (AR_WINDOWS)
else ()
	message (FATAL_ERROR "Unsupported platform")
endif ()

set (BUILD_SHARED_LIBS ON)

set (ARGON_SOURCE_DIR ${PROJECT_SOURCE_DIR})
set (CMAKE_BINARY_DIR ${ARGON_SOURCE_DIR}/../binaries)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_MODULE_PATH "${ARGON_SOURCE_DIR}/../build/cmake")

message (STATUS "${CMAKE_MODULE_PATH}")

include (${ARGON_TARGET}_config)

# add projects
list (LENGTH ARGON_PROJECTS _projectLength)
set (_iterator 0)

message (STATUS ${_projectLength})

while (_iterator LESS _projectLength)
	list (GET ARGON_PROJECTS ${_iterator} projPath)
	math (EXPR _iterator "${_iterator} + 1")
	add_subdirectory (${projPath})
	message (STATUS ${projPath})
endwhile ()

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
	message(STATUS "dir='${dir}'")
endforeach()
