cmake_minimum_required(VERSION 3.16.2)

project(data_structures CXX)

add_library(${PROJECT_NAME} SHARED)

target_sources(
	${PROJECT_NAME}
	PUBLIC
	include/data_structures/slot_map.hpp
	include/data_structures/sparse_storage.hpp
	PRIVATE
	src/sparse_storage.cpp
)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/data_structures"
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	Argon::fundamental
)

add_library(Argon::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
