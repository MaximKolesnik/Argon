cmake_minimum_required (VERSION 3.16.2)

project(engine_core)

add_library(${PROJECT_NAME} SHARED)

target_sources(
	${PROJECT_NAME}
	PUBLIC
	include/engine_core/engine_state.hpp
	include/engine_core/engine.hpp
	include/engine_core/entity_manager.hpp
	include/engine_core/entity.hpp
	include/engine_core/filesystem.hpp
	include/engine_core/forward_declarations.hpp
	include/engine_core/reflection.hpp
	include/engine_core/service.hpp
	include/engine_core/space.hpp
	include/engine_core/system_manager.hpp
	include/engine_core/system.hpp
	include/engine_core/time.hpp
	PRIVATE
	private/private/plugin/plugin_manager.hpp
	private/private/construction_data_impl.hpp
	private/private/entity_manager_data_provider.hpp
	private/private/service_manager.hpp
	private/private/system_manager_data_provider.hpp

	src/private/plugin/plugin_manager.cpp
	src/private/entity_manager_data_provider.cpp
	src/private/system_manager_data_provider.cpp

	src/engine_state.cpp
	src/engine.cpp
	src/entity_manager.cpp
	src/entity.cpp
	src/filesystem.cpp
	src/service.cpp
	src/space.cpp
	src/system_manager.cpp
	src/system.cpp
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
	thirdparty::sole)

add_library(Argon::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
