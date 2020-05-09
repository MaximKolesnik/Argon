#pragma once

#include <GL/galogen.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#include <GLFW/glfw3.h>
#pragma clang diagnostic pop

namespace argon::render
{
struct ShaderInfo
{
	GLenum m_type;
	GLuint m_shader;
	const char *m_filename;
};

GLuint LoadShaders(ShaderInfo* info);
} // namespace argon::render
