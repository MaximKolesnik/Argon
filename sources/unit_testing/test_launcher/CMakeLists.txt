cmake_minimum_required (VERSION 3.13.3)

project (test_launcher)

add_executable (
	${PROJECT_NAME}
	"")

target_sources (
  ${PROJECT_NAME}
	PRIVATE
	main.cpp
)

target_link_libraries (
	${PROJECT_NAME}
	PRIVATE
	data_structures_test
)
