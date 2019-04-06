cmake_minimum_required (VERSION 3.13.3)

project (engine_core)

add_executable (engine_core "")

target_sources (${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries (
	engine_core
	PRIVATE
	#core_types
	fundamental
	math
	render
)
