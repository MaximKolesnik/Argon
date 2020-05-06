cmake_minimum_required(VERSION 3.16.2)

project(sole)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(
	${PROJECT_NAME}
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_library(thirdparty::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
