cmake_minimum_required (VERSION 3.13.3)

project (data_structures_test)

add_library (
	${PROJECT_NAME}
  slot_map_test.cpp
)

target_link_libraries (
	${PROJECT_NAME}
	PUBLIC
	data_structures
	gtest
	pthread
)

set_target_properties (
	${PROJECT_NAME}
	PROPERTIES
	LINKER_LANGUAGE CXX
)
