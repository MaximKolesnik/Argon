cmake_minimum_required (VERSION 3.16.2)

include(ExternalProject)
ExternalProject_Add(
	rttr
	PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/rttr096
	URL ${CMAKE_CURRENT_SOURCE_DIR}/rttr-master.zip
	UPDATE_COMMAND ""
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DCMAKE_INSTALL_LIBDIR=${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -DBUILD_EXAMPLES=OFF -DBUILD_DOCUMENTATION=OFF -DBUILD_UNIT_TESTS=OFF -DBUILD_PACKAGE=OFF -DBUILD_STATIC=OFF
	BUILD_COMMAND ninja)

ExternalProject_Get_Property(rttr install_dir)
file(MAKE_DIRECTORY "${install_dir}/include")

if(NOT EXISTS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/librttr_core.so")
		file(WRITE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/librttr_core.so" "dummy file to be replaced by build")
endif()

add_library(RTTR::Core_Lib SHARED IMPORTED GLOBAL)
add_dependencies(RTTR::Core_Lib rttr)

set_target_properties(
		RTTR::Core_Lib
		PROPERTIES
		IMPORTED_LOCATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/librttr_core.so"
		INTERFACE_INCLUDE_DIRECTORIES "${install_dir}/include")

add_library(thirdparty::rttr ALIAS RTTR::Core_Lib)
