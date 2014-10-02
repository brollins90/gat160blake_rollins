#include <GL\glew.h>
#include "GlWindow.h"

char* vertexShaderCode =
"#version 400\r\n"
""
"in layout(location=0) vec2 v_position;"
"in layout(location=1) vec3 v_color;"
""
"out vec3 f_color;"
""
"void main()"
"{"
"	gl_Position = v_position;"
"	f_color = v_color;"
"}"
"";

char* fragmentShaderCode =
"#version 400\r\n"
""
"int vec3 f_color;"
"out vec3 final_color;"
""
"void main()"
"{"
"	final_color = f_color;"
"}"
"";

void GlWindow::initializeGL()
{
	glewInit();
	sendDataToHardware();
	compileShaders();
}

void GlWindow::sendDataToHardware()
{
	GLfloat vertices[] =
	{
		+0.0f, +0.8f, +1.0f, +0.0f, +0.0f,
		-0.8f, -0.8f, +0.0f, +1.0f, +0.0f,
		+0.8f, -0.8f, +0.0f, +0.0f, +1.0f,
	};

	GLuint bufferID;
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
}

void GlWindow::compileShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	char* glslCode[1];
	glslCode[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, (const GLchar **)&glslCode, 0);
	glslCode[0] = fragmentShaderCode;
	glShaderSource(fragmentShaderID, 1, (const GLchar **)&glslCode, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	glUseProgram(programID);
}

void GlWindow::paintGL()
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
}