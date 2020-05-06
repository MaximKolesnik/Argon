cmake_minimum_required(VERSION 3.16.2)

if (NOT ARGON_TARGET)
	message(FATAL_ERROR "ARGON_TARGET is not set")
endif()

project(${ARGON_TARGET} CXX)

function(add_compile_options_config CONFIG)
	foreach(opt ${ARGN})
		add_compile_options("$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CONFIG:${CONFIG}>>:${opt}>")
	endforeach()
endfunction()

# TODO reconsider this. probably most or all of the compile options should be set
# in all targets
add_compile_options_config(
	DEBUG
	"-Weverything" "-Werror" "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic"
	"-Wno-float-equal" "-Wno-global-constructors" "-Wno-comma" "-Wno-c++98-compat-pedantic"
	"-Wno-weak-vtables" "-Wno-exit-time-destructors"
	"-std=c++17" "-fms-extensions" "-fno-exceptions" "-Wno-undef"
	"-fvisibility=hidden" "-mavx")

if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
	message (FATAL_ERROR "Only 64-bit build is supported")
endif ()

add_compile_definitions(AR_CLANG)

if (UNIX)
	add_compile_definitions(AR_UNIX)
elseif (WIN32)
	add_compile_definitions(AR_WINDOWS)
else ()
	message(FATAL_ERROR "Unsupported platform")
endif ()

set(BUILD_SHARED_LIBS ON)

set(ARGON_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component (CMAKE_BINARY_DIR ${ARGON_SOURCE_DIR}/../binaries ABSOLUTE)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE STRING "OUTPUT DIR" FORCE)
set(CMAKE_MODULE_PATH "${ARGON_SOURCE_DIR}/../build/cmake")
set(ARGON_PLUGINS "${CMAKE_BINARY_DIR}/bin/plugins")
include(${ARGON_TARGET}_config)
