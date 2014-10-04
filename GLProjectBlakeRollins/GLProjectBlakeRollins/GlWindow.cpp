#include "GlWindow.h"

using glm::vec2;
using glm::vec3;

char* vertexShaderCode =
"#version 400\r\n"
""
"in layout(location=0) vec2 v_position;"
"in layout(location=1) vec3 v_color;"
""
"uniform vec3 triPosition;"
//"uniform vec3 dominating_color;"
//"uniform float yFlip;"
""
"out vec3 frag_color;"
""
"void main()"
"{"
"    gl_Position = vec4(v_position.x + triPosition.x, v_position.y + triPosition.y, 0.0f, 1.0f);"
//"    gl_Position.x += triPosition.x;"
//"    gl_Position.y += triPosition.y;"
//"    gl_Position.z += triPosition.z;"
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

	sendDataToHardware();
	compileShaders();
}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	//GLint dominatingColorUniformLocation = glGetUniformLocation(programID, "dominating_color");
	//GLint yFlipUniformLocation = glGetUniformLocation(programID, "yFlip");
	GLint triPositionUniformLocation = glGetUniformLocation(programID, "triPosition");
	//vec3 dominatingColor(1.0f, 0.0f, 0.0f);

	//glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform1f(yFlipUniformLocation, 1.0f);
	
	glUniform3fv(triPositionUniformLocation, 1, &triPosition[0]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//dominatingColor.r = 0;
	//dominatingColor.b = 1;
	//glUniform3fv(dominatingColorUniformLocation, 1, &dominatingColor[0]);
	//glUniform1f(yFlipUniformLocation, -1.0f);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GlWindow::sendDataToHardware()
{
	// Define the data
	GLfloat vertices[] =
	{
		+0.00f, +0.25f,		+0.8f, +1.0f, +0.1f,
		-0.25f, -0.25f,		+0.0f, +1.0f, +1.0f,
		+0.25f, -0.25f,		+1.0f, +0.2f, +0.0f,
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
	glVertexAttribPointer(vPositionLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), 0);

	GLuint vColorLocation = 1;
	glEnableVertexAttribArray(vColorLocation);
	glVertexAttribPointer(vColorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
}

// Timer callback
void GlWindow::windowUpdate()
{
	qDebug() << "Update: " << triPosition.x;
	vec2 velocity(0.005f, 0.005f);
	triPosition.x += velocity.x;
	triPosition.y += velocity.y;

	repaint();
}

