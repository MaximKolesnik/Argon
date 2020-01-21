cmake_minimum_required(VERSION 3.16.2)

project(math CXX)

add_library(${PROJECT_NAME} SHARED)

target_sources(
	${PROJECT_NAME}
	INTERFACE
	include/math/constants.hpp
	include/math/matrix3.hpp
	include/math/matrix4.hpp
	include/math/quaternion.hpp
	include/math/simd.hpp
	include/math/utils.hpp
	include/math/vector2.hpp
	include/math/vector3.hpp
	include/math/vector4.hpp
	PRIVATE
	src/matrix3.cpp
	src/matrix4.cpp
	src/quaternion.cpp
	src/utils.cpp
	src/vector2.cpp
	src/vector3.cpp
	src/vector4.cpp
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/math")

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	Argon::fundamental)

target_compile_options(${PROJECT_NAME} PRIVATE -fPIC)
target_link_options(${PROJECT_NAME} PRIVATE -fPIC)

add_library(Argon::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
