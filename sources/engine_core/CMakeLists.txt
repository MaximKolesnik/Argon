cmake_minimum_required (VERSION 3.13.3)

project (engine_core)

add_library (
	${PROJECT_NAME}
	SHARED
	component.cpp
	component.hpp
	game_object.cpp
	game_object.hpp
	main.cpp
	time.cpp
	time.hpp
)

target_link_libraries (
	${PROJECT_NAME}
	PRIVATE
	fundamental
	math
	render
)
