cmake_minimum_required (VERSION 3.16.2)

project(argon_game_launcher)

message(OPENGL FOUND VAL ${OPENGL_FOUND})

add_subdirectory(fundamental)
add_subdirectory(data_structures)
add_subdirectory(math)
add_subdirectory(engine_core)
add_subdirectory(third_party/gl)
add_subdirectory(third_party/glfw)
add_subdirectory(render)
add_subdirectory(game_launcher)
