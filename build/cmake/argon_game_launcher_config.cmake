cmake_minimum_required (VERSION 3.13.3)

project (argon_game_launcher)

#GLFW
set (GLFW_BUILD_EXAMPLES OFF)
set (GLFW_BUILD_TESTS OFF)
set (GLFW_BUILD_DOCS OFF)
set (GLFW_INSTALL OFF)
set (GLFW_VULKAN_STATIC OFF)
#GLFW end

message (OPENGL FOUND VAL ${OPENGL_FOUND})

set (ARGON_PROJECTS
	data_structures
	engine_core
	fundamental
	game_launcher
	math
	render
	third_party/gl
	third_party/glfw
)
