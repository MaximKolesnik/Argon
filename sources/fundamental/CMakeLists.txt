cmake_minimum_required(VERSION 3.16.2)

project(fundamental CXX)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories (
	${PROJECT_NAME}
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(Argon::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
