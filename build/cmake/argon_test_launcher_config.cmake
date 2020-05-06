cmake_minimum_required (VERSION 3.16.2)

project (argon_unit_testing)

add_subdirectory(fundamental)
add_subdirectory(data_structures)
add_subdirectory(math)
add_subdirectory(third_party/gl)
add_subdirectory(third_party/glfw)
add_subdirectory(third_party/gtest)
add_subdirectory(third_party/sole)
add_subdirectory(unit_testing/data_structures_test)
add_subdirectory(unit_testing/test_launcher)
