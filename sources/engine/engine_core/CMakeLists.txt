cmake_minimum_required (VERSION 3.12.1)

project (engine_core)



#message (${SOURCES})

add_executable (engine_core main.cpp)

#message (${SOURCES})

target_link_libraries (
	engine_core
	PRIVATE
	core_types
	fundamental
	math)
