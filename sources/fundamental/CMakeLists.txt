cmake_minimum_required (VERSION 3.13.3)

project (fundamental)

add_library (
	${PROJECT_NAME}
	compiler_macros.hpp
	debug.hpp
	helper_macros.hpp
	types.hpp
	)

target_include_directories (
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../
	)

set_target_properties (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
