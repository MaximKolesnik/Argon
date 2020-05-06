cmake_minimum_required (VERSION 3.16.2)

project(engine_core)

add_library(${PROJECT_NAME} SHARED)

target_sources(
	${PROJECT_NAME}
	PUBLIC
	include/engine_core/component.hpp
	include/engine_core/engine.hpp
	include/engine_core/entity_manager.hpp
	include/engine_core/entity.hpp
	include/engine_core/filesystem.hpp
	include/engine_core/forward_declarations.hpp
	include/engine_core/reflection.hpp
	include/engine_core/service.hpp
	include/engine_core/system_manager.hpp
	include/engine_core/system.hpp
	include/engine_core/time.hpp
	PRIVATE
	private/private/plugin/plugin_manager.hpp

	src/private/plugin/plugin_manager.cpp

	src/component.cpp
	src/engine.cpp
	src/entity_manager.cpp
	src/entity.cpp
	src/filesystem.cpp
	src/system_manager.cpp
	src/time.cpp)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/engine_core"
	"${CMAKE_CURRENT_SOURCE_DIR}/private"
)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	Argon::data_structures
	Argon::fundamental
	thirdparty::rttr
	PRIVATE
	Argon::data_structures
	Argon::fundamental
	Argon::math
	Argon::render
	thirdparty::sole)

add_library(Argon::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
