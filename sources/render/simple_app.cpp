#include "shader_loader.hpp"
#include "simple_app.hpp"

#define BUFFER_OFFSET(a) (static_cast<void*>(a))

namespace Argon
{
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

static GLuint VAOs[NumVAOs];
static GLuint Buffers[NumBuffers];

const GLuint  NumVertices = 6;

void SimpleApp::run()
{
	glfwInit();

	GLFWwindow *window = glfwCreateWindow(640, 480, "Triangles", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glGenVertexArrays( NumVAOs, VAOs );
	glBindVertexArray( VAOs[Triangles] );

	GLfloat vertices[NumVertices][2] = {
			{ -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
			{  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f }   // Triangle 2
	};

	glCreateBuffers( NumBuffers, Buffers );
	glBindBuffer( GL_ARRAY_BUFFER, Buffers[ArrayBuffer] );
	glBufferStorage( GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);

	Render::ShaderInfo  shaders[] =
	{
			{ GL_VERTEX_SHADER, 0, "shaders/triangles/triangles.vert" },
			{ GL_FRAGMENT_SHADER, 0, "shaders/triangles/triangles.frag" },
			{ GL_NONE, 0, nullptr }
	};

	GLuint program = LoadShaders( shaders );
	glUseProgram( program );

	glVertexAttribPointer( vPosition, 2, GL_FLOAT,
												 GL_FALSE, 0, BUFFER_OFFSET(nullptr) );
	glEnableVertexAttribArray( vPosition );

	while (!glfwWindowShouldClose(window))
	{
			static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

			glClearBufferfv(GL_COLOR, 0, black);

			glBindVertexArray( VAOs[Triangles] );
			glDrawArrays( GL_TRIANGLES, 0, NumVertices );
			glfwSwapBuffers(window);
			glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
}
}
