cmake_minimum_required(VERSION 3.16.2)

project(plugin_test)

add_library(${PROJECT_NAME} SHARED)

target_sources(
	${PROJECT_NAME}
	PRIVATE
	plugin.cpp
	plugin.hpp
)

target_link_libraries (
	${PROJECT_NAME}
	PUBLIC
	thirdparty::rttr
	Argon::engine_core
	Argon::fundamental
	Argon::render
)

set_target_properties(
	${PROJECT_NAME}
	PROPERTIES
	LIBRARY_OUTPUT_DIRECTORY ${ARGON_PLUGINS}
)
