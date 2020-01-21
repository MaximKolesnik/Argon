#include <fstream>

#include <fundamental/debug.hpp>

#include "shader_loader.hpp"

namespace Argon
{
namespace Render
{
namespace
{
const GLchar* ReadShader(const char *filename)
{
	std::ifstream shaderFile;
	shaderFile.open(filename);

	if (!shaderFile.is_open())
	{
		Debug::errorMsg("Shader file ", filename, " cannot be opened");
		return nullptr;
	}

	shaderFile.seekg(0, shaderFile.end);
	const size_t fileSize = static_cast<size_t>(shaderFile.tellg());
	shaderFile.seekg(0, shaderFile.beg);

	GLchar *source = new GLchar[fileSize + 1];
	shaderFile.read(source, static_cast<std::streamsize>(fileSize));
	shaderFile.close();

	source[fileSize] = 0;

	return const_cast<const GLchar*>(source);
}
} // namespace

GLuint LoadShaders(ShaderInfo *info)
{
	if (!info)
		return 0;

	GLuint program = glCreateProgram();
	ShaderInfo *entry = info;

	while (entry->m_type != GL_NONE)
	{
		GLuint shader = glCreateShader(entry->m_type);

		entry->m_shader = shader;

		const GLchar *source = ReadShader(entry->m_filename);
		if (source == nullptr)
		{
			for (entry = info; entry->m_type != GL_NONE; ++entry)
			{
				glDeleteShader(entry->m_shader);
				entry->m_shader = 0;
			}

			return 0;
		}

		glShaderSource(shader, 1, &source, nullptr);
		delete [] source;

		glCompileShader( shader );

		GLint compiled;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			GLsizei len;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

			GLchar *log = new GLchar[static_cast<size_t>(len + 1)];
			glGetShaderInfoLog(shader, len, &len, log);
			Debug::errorMsg("Shader compilation failed: ", log);

			delete []log;
			return 0;
		}

		glAttachShader( program, shader );
		++entry;
	}

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

		GLchar *log = new GLchar[static_cast<size_t>(len + 1)];
		glGetProgramInfoLog(program, len, &len, log);
		Debug::errorMsg("Shader linking failed: ", log);
		delete []log;

		for (entry = info; entry->m_type != GL_NONE; ++entry)
		{
			glDeleteShader(entry->m_shader);
			entry->m_shader = 0;
		}

		return 0;
	}

	return program;
}
} // namespace Render
} // namespace Argon
