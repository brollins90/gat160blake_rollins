#include "GlWindow.h"
#include <glm\gtx\transform.hpp>

using glm::vec2;
using glm::vec3;
using glm::mat4;

char* vertexShaderCode =
"#version 430\r\n"
""
"in layout(location=0) vec3 v_position;"
"in layout(location=1) vec3 v_color;"
""
"uniform mat4 fullTransformMatrix;"
""
"out vec3 frag_color;"
""
"void main()"
"{"
"    vec4 v = vec4(v_position, 1.0f);"
"    gl_Position = fullTransformMatrix * v;"
"    frag_color = v_color;"
"}"
"";

char* fragmentShaderCode =
"#version 400\r\n"
""
"in vec3 frag_color;"
"out vec4 out_color;"
""
"void main()"
"{"
"    out_color = vec4(frag_color, 1.0f);"
"}"
"";

vec3 triPosition;
float triAngle;
const float SPEED = 0.005f;

bool GlWindow::checkShaderStatus(GLuint shaderID) 
{
	GLint compilationStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus != 0) {
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* buffer = new char[logLength];
		GLsizei bitBucket;
		glGetShaderInfoLog(shaderID, logLength, &bitBucket, buffer);
		return false;
	}
	return true;
}

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
	checkShaderStatus(vertexShaderID);
	checkShaderStatus(fragmentShaderID);

	// attach the shaders
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	// Link
	glLinkProgram(programID);

	// Tell the hardware to use our stuff and not the default shaders
	glUseProgram(programID);
}

void GlWindow::initializeGL()
{
	glewInit();
	createProgram();

	triPosition = vec3(0.0f, 0.0f, 0.0f);
	triAngle = 0.0f;

	sendDataToHardware();
	compileShaders();
}

void GlWindow::checkKeyState()
{
	if (GetAsyncKeyState(VK_UP)) {
		triPosition.y += SPEED;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		triPosition.y -= SPEED;
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		triPosition.x -= SPEED;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		//triPosition.x += SPEED;
		triAngle += .5;
	}
}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	mat4 translationMatrix = glm::translate(mat4(), vec3(triPosition.x, triPosition.y, -3.0f));
	mat4 rotationMatrix = glm::rotate(mat4(), triAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	mat4 projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 10.0f);

	mat4 fullTransformMatrix = projectionMatrix * translationMatrix * rotationMatrix;

	GLint fullTransformMatrixUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GlWindow::sendDataToHardware()
{
	// Define the data
	GLfloat vertices[] =
	{
		+0.00f, +0.50f, +0.00f, +0.8f, +1.0f, +0.1f,
		-0.15f, -0.10f, +0.00f, +1.0f, +0.0f, +0.7f,
		+0.00f, +0.00f, +0.00f, +0.9f, +0.2F, +0.3f,

		+0.00f, +0.50f, +0.00f, +0.8f, +1.0f, +0.1f,
		+0.15f, -0.10f, +0.00f, +1.0f, +0.0f, +0.7f,
		+0.00f, +0.00f, +0.00f, +0.9f, +0.2F, +0.3f,
	};

	// Create a buffer in graphics ram
	glGenBuffers(1, &bufferID);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	// Copy the vertices to the graphics ram
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Register the timer callback
	connect(&windowTimer, SIGNAL(timeout()), this, SLOT(windowUpdate()));
	windowTimer.start(0);

	// define the data attributes so we can reference the data later
	GLuint vPositionLocation = 0;
	glEnableVertexAttribArray(vPositionLocation);
	glVertexAttribPointer(vPositionLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), 0);

	GLuint vColorLocation = 1;
	glEnableVertexAttribArray(vColorLocation);
	glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
}

// Timer callback
void GlWindow::windowUpdate()
{
	qDebug() << triPosition.x;
	checkKeyState();
	repaint();
}
