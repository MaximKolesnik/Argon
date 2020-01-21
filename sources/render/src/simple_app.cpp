#include "shader_loader.hpp"
#include "simple_app.hpp"

#define BUFFER_OFFSET(a) (reinterpret_cast<void*>(a))

namespace Argon
{
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColor = 1 };

static GLuint VAOs[NumVAOs];
static GLuint Buffers[NumBuffers];

const GLuint  NumVertices = 6;

struct VertexData
{
	GLubyte color[4];
	GLfloat position[2];
};

static GLFWwindow *window = nullptr;

void SimpleApp::prepare()
{
	glfwInit();
	window = glfwCreateWindow(640, 480, "Triangles", nullptr, nullptr);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

	glGenVertexArrays( NumVAOs, VAOs );
	glBindVertexArray( VAOs[Triangles] );

	VertexData vertices[NumVertices]= {
		{{ 255, 0, 0, 255 }, { -0.90f, -0.90f }}, // Triangle 1
		{{0, 255, 0, 255 }, { 0.85f, -0.90f }},
		{{0, 0, 255, 255 }, { -0.90f, 0.85f }},
		{{ 10, 10, 10, 255 }, { 0.90f, -0.85f }}, // Triangle 2
		{{ 100, 100, 100, 255 }, { 0.90f, 0.90f }},
		{{ 255, 255, 255, 255 }, { -0.85f, 0.90f }}
	};

	glCreateBuffers( NumBuffers, Buffers );
	glBindBuffer( GL_ARRAY_BUFFER, Buffers[ArrayBuffer] );
	glBufferData( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	Render::ShaderInfo  shaders[] =
	{
		{ GL_VERTEX_SHADER, 0, "shaders/gouraud/gouraud.vert" },
		{ GL_FRAGMENT_SHADER, 0, "shaders/gouraud/gouraud.frag" },
		{ GL_NONE, 0, nullptr }
	};

	GLuint program = LoadShaders( shaders );
	glUseProgram( program );

	glVertexAttribPointer( vColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexData),
		BUFFER_OFFSET(0));
	glVertexAttribPointer( vPosition, 2, GL_FLOAT,
		GL_FALSE, sizeof(VertexData), BUFFER_OFFSET(sizeof(vertices[0].color)));
	glEnableVertexAttribArray( vColor );
	glEnableVertexAttribArray( vPosition );
}

bool SimpleApp::shouldClose()
{
	return glfwWindowShouldClose(window);
}

void SimpleApp::tick()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear( GL_COLOR_BUFFER_BIT );

	glBindVertexArray( VAOs[Triangles] );
	glDrawArrays( GL_TRIANGLES, 0, NumVertices );
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void SimpleApp::close()
{
	glfwDestroyWindow(window);

	glfwTerminate();
}
}
