#include <GL\glew.h>
#include "GlWindow.h"

void GlWindow::initializeGL()
{
	glewInit();

	GLfloat vertices[] =
	{
		+0.0f, +0.8f,   +1.0f, +0.0f, +0.0f,
		-0.8f, -0.8f,   +0.0f, +1.0f, +0.0f,
		+0.8f, -0.8f,   +0.0f, +0.0f, +1.0f,
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

void GlWindow::paintGL()
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
}