cmake_minimum_required (VERSION 3.16.2)

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
)

target_compile_options(
	${PROJECT_NAME}
	PRIVATE
	"-Wno-used-but-marked-unused" "-Wno-covered-switch-default"
)

set_target_properties (
	${PROJECT_NAME}
	PROPERTIES
	LINKER_LANGUAGE CXX
)
