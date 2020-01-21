cmake_minimum_required (VERSION 3.13.3)

project (gl C)

find_package(OpenGL REQUIRED)

add_library(${PROJECT_NAME} SHARED)

target_sources(
	${PROJECT_NAME}
	INTERFACE
	include/GL/galogen.h
	PRIVATE
	src/galogen.c)

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	${OPENGL_LIBRARIES})

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include/GL/)
