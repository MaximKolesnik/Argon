cmake_minimum_required(VERSION 3.16.2)

project(data_structures CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(
	${PROJECT_NAME}
	INTERFACE
	Argon::fundamental)

add_library(Argon::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
