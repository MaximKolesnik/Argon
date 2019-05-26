cmake_minimum_required (VERSION 3.13.3)

project (engine)

#GLFW
set (GLFW_BUILD_EXAMPLES OFF)
set (GLFW_BUILD_TESTS OFF)
set (GLFW_BUILD_DOCS OFF)
set (GLFW_INSTALL OFF)
set (GLFW_VULKAN_STATIC OFF)
#GLFW end

message (OPENGL FOUND VAL ${OPENGL_FOUND})

set (ARGON_PROJECTS
	engine/core_types
	engine/engine_core
	fundamental
	math
	render
	third_party/gl
	third_party/glfw
)
