cmake_minimum_required (VERSION 3.13.3)

project (test_launcher)

find_package (GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

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
  gtest
  gtest_main
  pthread
)

