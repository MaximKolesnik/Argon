cmake_minimum_required (VERSION 3.13.3)

project(engine_core)

add_library(${PROJECT_NAME} SHARED)

target_sources(
	${PROJECT_NAME}
	PUBLIC
	include/engine_core/component.hpp
	include/engine_core/core_singleton.hpp
	include/engine_core/core.hpp
	include/engine_core/forward_declarations.hpp
	include/engine_core/system_manager.hpp
	include/engine_core/system.hpp
	include/engine_core/time.hpp
	PRIVATE
	src/component.cpp
	src/core.cpp
	src/system_manager.cpp
	src/system.cpp
	src/time.cpp)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/engine_core")

target_link_libraries(
	${PROJECT_NAME}
	PRIVATE
	Argon::fundamental
	Argon::math
	Argon::render)

add_library(Argon::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
