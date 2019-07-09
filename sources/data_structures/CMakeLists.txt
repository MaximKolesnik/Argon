cmake_minimum_required (VERSION 3.13.3)

project (data_structures)

add_library (
	${PROJECT_NAME}
  SHARED
	slot_map.hpp
	)

target_link_libraries(
	${PROJECT_NAME}
	PUBLIC
	fundamental
	)

set_target_properties (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
