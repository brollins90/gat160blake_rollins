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
vec3 triVelocity;
vec3 triDirection;
float triAngle;
const float SPEED = 0.005f;
const float SCALE = 0.1f;
GLint fullTransformMatrixUniformLocation;

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

	// Copy the shader code into an array because the glShaderSource functions require a char**
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

	fullTransformMatrixUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");
}

void GlWindow::initializeGL()
{
	glewInit();
	createProgram();

	triPosition = vec3(0.0f, 0.0f, 0.0f);
	triAngle = (3.141f / 4.0f);
	triVelocity = vec3(0.0f, 0.0f, 0.0f);

	sendDataToHardware();
	compileShaders();
}

void GlWindow::checkKeyState()
{
	if (GetAsyncKeyState(VK_UP)) {
		triVelocity += (/*dt **/ SPEED * triDirection);
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		triVelocity -= (/*dt **/ SPEED * triDirection);
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		triAngle += .75;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		triAngle -= .75;
	}
}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());



	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void GlWindow::sendDataToHardware()
{
	// Define the data
	GLfloat vertices[] =
	{
		+0.00f, +0.30f, +0.00f, +0.8f, +1.0f, +0.1f,
		-0.15f, -0.30f, +0.00f, +1.0f, +0.0f, +0.7f,
		+0.00f, -0.20f, +0.00f, +0.9f, +0.2F, +0.3f,
		+0.15f, -0.30f, +0.00f, +1.0f, +0.0f, +0.7f,
		+0.00f, -0.20f, +0.00f, +0.9f, +0.2F, +0.3f,
/*
		+0.00f, +0.30f, +0.00f, +1.0f, +0.0f, +0.0f,
		-0.30f, -0.30f, +0.00f, +0.0f, +1.0f, +0.0f,
		+0.30f, -0.30f, +0.00f, +0.0f, +0.0F, +1.0f,*/
	};

	// Create a buffer in graphics ram
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
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

	GLushort indices[] = { 0,1,2, 0,3,4};
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

}

// Timer callback
void GlWindow::windowUpdate()
{
	qDebug() << triPosition.x;
	checkKeyState();
	applyTransforms();
	repaint();
}

void GlWindow::applyTransforms()
{
	triVelocity *= 0.9;
	triPosition += triVelocity;

	//mat4 translationMatrix = mat4();
	//translationMatrix[3].x = triPosition.x;
	//translationMatrix[3].y = triPosition.y;

	mat4 translationMatrix = glm::translate(mat4(), triPosition);
	mat4 rotationMatrix = glm::rotate(mat4(), triAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	mat4 scaleMatrix = glm::scale(mat4(), vec3(SCALE));
	///mat4 projectionMatrix = glm::perspective(60.0f, ((float)width()) / height(), 0.1f, 10.0f);

	mat4 fullTransformMatrix = scaleMatrix * translationMatrix * rotationMatrix;

	// Direction is normalized about the Y axis
	triDirection = glm::normalize(vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));



	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
}