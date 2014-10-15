#include "GlWindow.h"
#include <ShapeGenerator.h>
#include <ShapeData.h>

Camera camera;

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = sizeof(Neumont::Vertex) / sizeof(float);
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

const uint CUBE_INDEX = 0;
const uint ARROW_INDEX = 1;
GLuint vertexBufferIds[2];
GLuint indexBufferIds[2];
GLuint vertexArrayObjectIds[2];
GLuint numIndices[2];
GLuint fullTransformUniformLocation;

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

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const char* glslCode[1];
	std::string temp = readShaderCode("Shaders/VertexShaderCode.glsl");
	glslCode[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, glslCode, 0);
	temp = readShaderCode("Shaders/FragmentShaderCode.glsl").c_str();
	glslCode[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, glslCode, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	checkShaderStatus(vertexShaderID);
	checkShaderStatus(fragmentShaderID);

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void GlWindow::initializeGL()
{
	glewInit();
	setMouseTracking(true);
	createProgram();

	connect(&windowTimer, SIGNAL(timeout()), this, SLOT(windowUpdate()));
	windowTimer.start(0);

	sendDataToHardware();
	setupVertexArrays();
	compileShaders();

	fullTransformUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");
}

void GlWindow::setupVertexArrays()
{
	glGenVertexArrays(1, &vertexArrayObjectIds[CUBE_INDEX]);

	glBindVertexArray(vertexArrayObjectIds[CUBE_INDEX]);
	glEnableVertexAttribArray(0); // v_position
	glEnableVertexAttribArray(1); // v_color
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[CUBE_INDEX]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(GL_FLOAT) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferIds[CUBE_INDEX]);
	// attribs can only be a max of size 4 so a mat4 needs 4 seperate attribs
	for (int i = 0; i < 4; i++) {
		glVertexAttribPointer(i + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(GL_FLOAT) * (i * 4)));
		glEnableVertexAttribArray(i + 2);
		glVertexAttribDivisor(i + 2, 1);
	}
	
	glGenVertexArrays(2, &vertexArrayObjectIds[ARROW_INDEX]);

	glBindVertexArray(vertexArrayObjectIds[ARROW_INDEX]);
	glEnableVertexAttribArray(0); // v_position
	glEnableVertexAttribArray(1); // v_color
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[ARROW_INDEX]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(sizeof(GL_FLOAT) * 3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferIds[ARROW_INDEX]);
	// attribs can only be a max of size 4 so a mat4 needs 4 seperate attribs
	for (int i = 0; i < 4; i++) {
		glVertexAttribPointer(i + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(GL_FLOAT) * (i * 4)));
		glEnableVertexAttribArray(i + 2);
		glVertexAttribDivisor(i + 2, 1);
	}

}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glm::mat4 fullTransformMatrix;
	glm::mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width() / height()), 0.1f, 100.0f);
	glm::mat4 worldToViewMatrix = glm::lookAt(glm::vec3(0,0,-1.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));// camera.getWorldToViewMatrix();
	glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;
	
	// Cube 
	glBindVertexArray(vertexArrayObjectIds[CUBE_INDEX]);
	glm::mat4 cube1ModelToWorldMatrix =
		glm::translate(glm::vec3(-1.0f, 0.0f, 3.0f)) *
		glm::rotate(36.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * cube1ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[CUBE_INDEX], GL_UNSIGNED_SHORT, (void*)(sizeof(GLushort) * 0));
	
	/*glm::mat4 cube2ModelToWorldMatrix =
		glm::translate(glm::vec3(1.0f, 0.0f, -3.75f)) *
		glm::rotate(126.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix * cube2ModelToWorldMatrix;
	glUniformMatrix4fv(fullTransformUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[CUBE_INDEX], GL_UNSIGNED_SHORT, (void*)(sizeof(GLushort) * 0));
	*/
}

void GlWindow::sendDataToHardware()
{
	Neumont::ShapeData shape = Neumont::ShapeGenerator::makeCube();

	glGenBuffers(1, &vertexBufferIds[CUBE_INDEX]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[CUBE_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.verts, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferIds[CUBE_INDEX]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferIds[CUBE_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);

	numIndices[CUBE_INDEX] = shape.numIndices;


	shape = Neumont::ShapeGenerator::makeArrow();

	glGenBuffers(1, &vertexBufferIds[ARROW_INDEX]);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIds[ARROW_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, shape.vertexBufferSize(), shape.verts, GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferIds[ARROW_INDEX]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferIds[ARROW_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indexBufferSize(), shape.indices, GL_STATIC_DRAW);

	numIndices[ARROW_INDEX] = shape.numIndices;

}

// Timer callback
void GlWindow::windowUpdate()
{
	checkKeyState();
	repaint();
}

GlWindow::~GlWindow()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}

void GlWindow::mouseMoveEvent(QMouseEvent* e)
{
	camera.mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void GlWindow::checkKeyState()
{
	if (GetAsyncKeyState('W')) {
		camera.moveForward();
	}
	if (GetAsyncKeyState('S')) {
		camera.moveBackward();
	}
	if (GetAsyncKeyState('A')) {
		camera.strafeLeft();
	}
	if (GetAsyncKeyState('D')) {
		camera.strafeRight();
	}
	if (GetAsyncKeyState('R')) {
		camera.moveUp();
	}
	if (GetAsyncKeyState('F')) {
		camera.moveDown();
	}
	if (GetAsyncKeyState(VK_ESCAPE)) {
		exit(0);
	}
}