cmake_minimum_required (VERSION 3.13.3)

project (gl)

find_package(OpenGL REQUIRED)

add_library (
	${PROJECT_NAME}
	SHARED
	src/galogen.c
	include/GL/galogen.h
	)

target_link_libraries (
	${PROJECT_NAME}
	PRIVATE
	${OPENGL_LIBRARIES}
	)

target_include_directories (
	${PROJECT_NAME}
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include/
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include/GL/
	)

set_target_properties (
	${PROJECT_NAME}
	PROPERTIES
	LINKER_LANGUAGE CXX
	)
