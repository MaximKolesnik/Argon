cmake_minimum_required (VERSION 3.13.3)

project (render)

add_library (
	${PROJECT_NAME}
	SHARED
	shader_loader.cpp
	shader_loader.hpp
	simple_app.cpp
	simple_app.hpp
	)

target_link_libraries (
	${PROJECT_NAME}
	PRIVATE
	fundamental
	math
	gl
	glfw
)

set_target_properties (
	${PROJECT_NAME}
	PROPERTIES
	LINKER_LANGUAGE CXX
	)
