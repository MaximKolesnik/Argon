cmake_minimum_required (VERSION 3.16.2)

project (render CXX)

add_library (${PROJECT_NAME} SHARED)

target_sources(
	${PROJECT_NAME}
	INTERFACE
	include/render/shader_loader.hpp
	include/render/simple_app.hpp
	PRIVATE
	src/shader_loader.cpp
	src/simple_app.cpp)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/render")

target_link_libraries (
	${PROJECT_NAME}
	PRIVATE
	Argon::fundamental
	Argon::math
	Argon::engine_core
	gl
	glfw)

add_library(Argon::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
