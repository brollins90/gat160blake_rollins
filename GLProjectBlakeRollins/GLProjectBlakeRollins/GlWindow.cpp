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

void GlWindow::createProgram()
{
	programID = glCreateProgram();
}

void GlWindow::compileShaders()
{
	// Create some shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Copy the shaderCode into an array because the glShaderSource functions require a char**
	const char* glslCode[1];
	glslCode[0] = vertexShaderCode;
	glShaderSource(vertexShaderID, 1, glslCode, 0);
	glslCode[0] = fragmentShaderCode;
	glShaderSource(fragmentShaderID, 1, glslCode, 0);

	// Compile
	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	// Check for errors in compilation
	GLint compilationStatus;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus != 0) {
		GLint logLength;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* buffer = new char[logLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(vertexShaderID, logLength, &bitBucket, buffer);
	}

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus != 0) {
		GLint logLength;
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* buffer = new char[logLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(fragmentShaderID, logLength, &bitBucket, buffer);
	}

	// attach the shaders
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	// Link
	glLinkProgram(programID);

	// Tell the hardware to use our stuff and not the default shaders
	glUseProgram(programID);
}

void GlWindow::sendDataToHardware()
{
	// Define the data
	GLfloat vertices[] =
	{
		+0.0f, +0.8f, +1.0f, +1.0f, +1.0f,
		-0.8f, -0.8f, +0.0f, +1.0f, +0.0f,
		+0.8f, -0.8f, +0.0f, +0.0f, +1.0f,
	};

	// Create a buffer in graphics ram
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	// Copy the vertices to the graphics ram
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	connect(&windowTimer, SIGNAL(timeout()), this, SLOT(windowUpdate()));
	windowTimer.start(0);

	// define the data attributes so we can reference the data later
	GLuint vPositionLocation = 0;
	glEnableVertexAttribArray(vPositionLocation);
	glVertexAttribPointer(vPositionLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);

	GLuint vColorLocation = 3;
	glEnableVertexAttribArray(vColorLocation);
	glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
}

void GlWindow::windowUpdate()
{

}

void GlWindow::initializeGL()
{
	glewInit();
	createProgram();
	sendDataToHardware();
	compileShaders();
}

void GlWindow::paintGL()
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
}