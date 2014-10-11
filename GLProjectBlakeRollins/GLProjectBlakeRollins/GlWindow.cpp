#include "GlWindow.h"
#include <ShapeGenerator.h>
#include <ShapeData.h>

glm::vec3 cubePosition, cubeVelocity, cubeDirection, cubeAngles;
glm::vec3 camPosition;
const float SPEED = 0.005f;
const float SCALE = 0.45f;
GLint fullTransformMatrixUniformLocation;
GLint MVPLocation;
Camera camera;

GLfloat verts[] =
{
	-1.0f, +1.0f, +1.0f, // 0
	+1.0f, +0.0f, +0.0f, // Color
	+1.0f, +1.0f, +1.0f, // 1
	+0.0f, +1.0f, +0.0f, // Color
	+1.0f, +1.0f, -1.0f, // 2
	+0.0f, +0.0f, +1.0f, // Color
	-1.0f, +1.0f, -1.0f, // 3
	+1.0f, +1.0f, +1.0f, // Color

	-1.0f, +1.0f, -1.0f, // 4
	+1.0f, +0.0f, +1.0f, // Color
	+1.0f, +1.0f, -1.0f, // 5
	+0.0f, +0.5f, +0.2f, // Color
	+1.0f, -1.0f, -1.0f, // 6
	+0.8f, +0.6f, +0.4f, // Color
	-1.0f, -1.0f, -1.0f, // 7
	+0.3f, +1.0f, +0.5f, // Color

	+1.0f, +1.0f, -1.0f, // 8
	+0.2f, +0.5f, +0.2f, // Color
	+1.0f, +1.0f, +1.0f, // 9
	+0.9f, +0.3f, +0.7f, // Color
	+1.0f, -1.0f, +1.0f, // 10
	+0.3f, +0.7f, +0.5f, // Color
	+1.0f, -1.0f, -1.0f, // 11
	+0.5f, +0.7f, +0.5f, // Color

	-1.0f, +1.0f, +1.0f, // 12
	+0.7f, +0.8f, +0.2f, // Color
	-1.0f, +1.0f, -1.0f, // 13
	+0.5f, +0.7f, +0.3f, // Color
	-1.0f, -1.0f, -1.0f, // 14
	+0.4f, +0.7f, +0.7f, // Color
	-1.0f, -1.0f, +1.0f, // 15
	+0.2f, +0.5f, +1.0f, // Color

	+1.0f, +1.0f, +1.0f, // 16
	+0.6f, +1.0f, +0.7f, // Color
	-1.0f, +1.0f, +1.0f, // 17
	+0.6f, +0.4f, +0.8f, // Color
	-1.0f, -1.0f, +1.0f, // 18
	+0.2f, +0.8f, +0.7f, // Color
	+1.0f, -1.0f, +1.0f, // 19
	+0.2f, +0.7f, +1.0f, // Color

	+1.0f, -1.0f, -1.0f, // 20
	+0.8f, +0.3f, +0.7f, // Color
	-1.0f, -1.0f, -1.0f, // 21
	+0.8f, +0.9f, +0.5f, // Color
	-1.0f, -1.0f, +1.0f, // 22
	+0.5f, +0.8f, +0.5f, // Color
	+1.0f, -1.0f, +1.0f, // 23
	+0.9f, +1.0f, +0.2f, // Color
};

GLushort indices[] = {
	0, 1, 2, 0, 2, 3, // Top
	4, 5, 6, 4, 6, 7, // Front
	8, 9, 10, 8, 10, 11, // Right 
	12, 13, 14, 12, 14, 15, // Left
	16, 17, 18, 16, 18, 19, // Back
	20, 22, 21, 20, 23, 22, // Bottom
};

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = 6;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

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

std::string readShaderCode(const char* fileName)
{
	std::ifstream myFile(fileName);
	if (! myFile.good()) {
		std::cout << "File failed to load: " << fileName << std::endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(myFile),
		std::istreambuf_iterator<char>());

}

void GlWindow::compileShaders()
{
	glEnable(GL_DEPTH_TEST);

	// Create some shaders
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Copy the shader code into an array because the glShaderSource functions require a char**
	const char* glslCode[1];
	std::string temp = readShaderCode("Shaders/VertexShaderCode.glsl");
	glslCode[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, glslCode, 0);
	temp = readShaderCode("Shaders/FragmentShaderCode.glsl").c_str();
	glslCode[0] = temp.c_str();
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

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	// Tell the hardware to use our stuff and not the default shaders
	glUseProgram(programID);

	fullTransformMatrixUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");
	MVPLocation = glGetUniformLocation(programID, "MVP");
}

void GlWindow::initializeGL()
{
	glewInit();
	createProgram();
	// Register the timer callback
	connect(&windowTimer, SIGNAL(timeout()), this, SLOT(windowUpdate()));
	windowTimer.start(0);


	cubePosition = glm::vec3(+0.0f, 0.0f, -3.0f);
	cubeAngles = glm::vec3(0.0f, 0.0f, 0.0f);
	cubeVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

	camPosition = glm::vec3(0.0f, -5.0f, -10.0f);

	sendDataToHardware();
	compileShaders();
}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)(0 * sizeof(GLushort)), 2);
}

void GlWindow::sendDataToHardware()
{
	//Neumont::ShapeData shape = Neumont::ShapeGenerator::makeCube();

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // v_position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glEnableVertexAttribArray(1); // v_color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(3 * sizeof(GL_FLOAT)));

	GLuint indexArrayBufferID;
	glGenBuffers(1, &indexArrayBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexArrayBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint transformationMatrixBufferID;
	glGenBuffers(1, &transformationMatrixBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, transformationMatrixBufferID);

	glm::mat4 projectionMatrix = glm::perspective(60.0f, ((float)width() / height()), 0.1f, 10.0f);
	glm::mat4 fullTransforms[] =
	{
		projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(glm::vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(36.0f, glm::vec3(1.0f, 0.0f, 0.0f)),
		projectionMatrix * camera.getWorldToViewMatrix() * glm::translate(glm::vec3(1.0f, 0.0f, -3.75f)) * glm::rotate(126.0f, glm::vec3(0.0f, 1.0f, 0.0f)),
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(fullTransforms), fullTransforms, GL_STATIC_DRAW);
	// attribs can only be a max of size 4 so a mat4 needs 4 seperate attribs
	for (int i = 0; i < 4; i++) {
		glVertexAttribPointer(i + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(GL_FLOAT) * (i * 4)));
		glEnableVertexAttribArray(i + 2);
		glVertexAttribDivisor(i + 2, 1);
	}
}

// Timer callback
void GlWindow::windowUpdate()
{
	cubeAngles.x += .1f;
	cubeAngles.y += .1f;
	cubeAngles.z += .1f;
	repaint();
}

GlWindow::~GlWindow()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}