cmake_minimum_required (VERSION 3.13.3)

project (math)

add_library (
	${PROJECT_NAME}
	SHARED
	constants.hpp
	matrix3.cpp
	matrix3.hpp
	matrix4.cpp
	matrix4.hpp
	quaternion.cpp
	quaternion.hpp
	simd.hpp
	utils.cpp
	utils.hpp
	vector2.cpp
	vector2.hpp
	vector3.cpp
	vector3.hpp
	vector4.cpp
	vector4.hpp
	)

target_link_libraries (
	${PROJECT_NAME}
	PUBLIC
	fundamental
	)

target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
target_link_options(${PROJECT_NAME} PRIVATE -fPIC)

set_target_properties (
	${PROJECT_NAME}
	PROPERTIES
	LINKER_LANGUAGE CXX)
