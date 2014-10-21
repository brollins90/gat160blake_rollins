#include "GlWindow.h"

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = sizeof(Neumont::Vertex) / sizeof(float);
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint glBufferId;
GLint fullTransformMatrixUniformLocation;
GLint ambientLightUniformLocation;
GLint lightPositionUniformLocation;
GLint modelToWorldTransformMatrixUniformLocation;

const uint ARROW_INDEX = 0;
const uint CUBE_INDEX = 1;
const uint SPHERE_INDEX = 2;
const uint PLANE_INDEX = 3;
GLuint vertexArrayObjectIds[4];
GLuint numIndices[4];
GLuint vertexByteOffset[4];
GLuint indexByteOffset[4];

ProjectModel* model;

GlWindow::GlWindow(ProjectModel* model_in)
{
	model = model_in;
}

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
	programIDPassThrough = glCreateProgram();
	programIDFragment = glCreateProgram();
	programIDVertex = glCreateProgram();
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

	GLuint vertexShaderIDPassThrough = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderIDPassThrough = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vertexShaderIDFragment = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderIDFragment = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint vertexShaderIDVertex = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderIDVertex = glCreateShader(GL_FRAGMENT_SHADER);

	const char* glslCode[1];
	std::string temp = readShaderCode("Shaders/VertexShaderPassThrough.glsl");
	glslCode[0] = temp.c_str();
	glShaderSource(vertexShaderIDPassThrough, 1, glslCode, 0);
	temp = readShaderCode("Shaders/FragmentShaderPassThrough.glsl").c_str();
	glslCode[0] = temp.c_str();
	glShaderSource(fragmentShaderIDPassThrough, 1, glslCode, 0);

	readShaderCode("Shaders/VertexShaderVertex.glsl");
	glslCode[0] = temp.c_str();
	glShaderSource(vertexShaderIDVertex, 1, glslCode, 0);
	temp = readShaderCode("Shaders/FragmentShaderVertex.glsl").c_str();
	glslCode[0] = temp.c_str();
	glShaderSource(fragmentShaderIDVertex, 1, glslCode, 0);

	temp = readShaderCode("Shaders/VertexShaderFragment.glsl");
	glslCode[0] = temp.c_str();
	glShaderSource(vertexShaderIDFragment, 1, glslCode, 0);
	temp = readShaderCode("Shaders/FragmentShaderFragment.glsl").c_str();
	glslCode[0] = temp.c_str();
	glShaderSource(fragmentShaderIDFragment, 1, glslCode, 0);

	glCompileShader(vertexShaderIDPassThrough);
	glCompileShader(fragmentShaderIDPassThrough);
	glCompileShader(vertexShaderIDVertex);
	glCompileShader(fragmentShaderIDVertex);
	glCompileShader(vertexShaderIDFragment);
	glCompileShader(fragmentShaderIDFragment);

	checkShaderStatus(vertexShaderIDPassThrough);
	checkShaderStatus(fragmentShaderIDPassThrough);
	checkShaderStatus(vertexShaderIDVertex);
	checkShaderStatus(fragmentShaderIDVertex);
	checkShaderStatus(vertexShaderIDFragment);
	checkShaderStatus(fragmentShaderIDFragment);

	glAttachShader(programIDPassThrough, vertexShaderIDPassThrough);
	glAttachShader(programIDPassThrough, fragmentShaderIDPassThrough);
	glAttachShader(programIDVertex, vertexShaderIDVertex);
	glAttachShader(programIDVertex, fragmentShaderIDVertex);
	glAttachShader(programIDFragment, vertexShaderIDFragment);
	glAttachShader(programIDFragment, fragmentShaderIDFragment);

	glLinkProgram(programIDPassThrough);
	glLinkProgram(programIDVertex);
	glLinkProgram(programIDFragment);

	glDeleteShader(vertexShaderIDPassThrough);
	glDeleteShader(fragmentShaderIDPassThrough);
	glDeleteShader(vertexShaderIDVertex);
	glDeleteShader(fragmentShaderIDVertex);
	glDeleteShader(vertexShaderIDFragment);
	glDeleteShader(fragmentShaderIDFragment);

	glUseProgram(programIDPassThrough);
}

void GlWindow::initializeGL()
{
	glewInit();
	glEnable(GL_DEPTH_TEST);
	setMouseTracking(true);
	createProgram();

	connect(&windowTimer, SIGNAL(timeout()), this, SLOT(windowUpdate()));
	windowTimer.start(0);

	sendDataToHardware();
	compileShaders();

}

void GlWindow::sendDataToHardware()
{
	Neumont::ShapeData arrow = Neumont::ShapeGenerator::makeArrow();
	Neumont::ShapeData cube = Neumont::ShapeGenerator::makeCube();
	Neumont::ShapeData sphere = Neumont::ShapeGenerator::makeSphere(9);
	Neumont::ShapeData plane = Neumont::ShapeGenerator::makePlane(10);


	glGenBuffers(1, &glBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, glBufferId);
	glBufferData(GL_ARRAY_BUFFER,
		(arrow.vertexBufferSize() + arrow.indexBufferSize() +
		cube.vertexBufferSize() + cube.indexBufferSize() +
		sphere.vertexBufferSize() + sphere.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize()),
		0, GL_STATIC_DRAW);
	GLsizeiptr currentOffset = 0;
	vertexByteOffset[ARROW_INDEX] = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.vertexBufferSize(), arrow.verts);
	currentOffset += arrow.vertexBufferSize();
	indexByteOffset[ARROW_INDEX] = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, arrow.indexBufferSize(), arrow.indices);
	currentOffset += arrow.indexBufferSize();
	vertexByteOffset[CUBE_INDEX] = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.vertexBufferSize(), cube.verts);
	currentOffset += cube.vertexBufferSize();
	indexByteOffset[CUBE_INDEX] = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, cube.indexBufferSize(), cube.indices);
	currentOffset += cube.indexBufferSize();
	vertexByteOffset[SPHERE_INDEX] = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.vertexBufferSize(), sphere.verts);
	currentOffset += sphere.vertexBufferSize();
	indexByteOffset[SPHERE_INDEX] = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, sphere.indexBufferSize(), sphere.indices);
	currentOffset += sphere.indexBufferSize();
	vertexByteOffset[PLANE_INDEX] = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.vertexBufferSize(), plane.verts);
	currentOffset += plane.vertexBufferSize();
	indexByteOffset[PLANE_INDEX] = currentOffset;
	glBufferSubData(GL_ARRAY_BUFFER, currentOffset, plane.indexBufferSize(), plane.indices);

	numIndices[ARROW_INDEX] = arrow.numIndices;
	numIndices[CUBE_INDEX] = cube.numIndices;
	numIndices[SPHERE_INDEX] = sphere.numIndices;
	numIndices[PLANE_INDEX] = plane.numIndices;

	glGenVertexArrays(1, &vertexArrayObjectIds[ARROW_INDEX]);
	glGenVertexArrays(1, &vertexArrayObjectIds[CUBE_INDEX]);
	glGenVertexArrays(1, &vertexArrayObjectIds[SPHERE_INDEX]);
	glGenVertexArrays(1, &vertexArrayObjectIds[PLANE_INDEX]);

	glBindVertexArray(vertexArrayObjectIds[ARROW_INDEX]);
	glEnableVertexAttribArray(0); // v_position
	glEnableVertexAttribArray(1); // v_color
	glEnableVertexAttribArray(2); // v_normal
	glBindBuffer(GL_ARRAY_BUFFER, glBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[ARROW_INDEX] + (0 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[ARROW_INDEX] + (3 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[ARROW_INDEX] + (7 * sizeof(GL_FLOAT))));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBufferId);

	glBindVertexArray(vertexArrayObjectIds[CUBE_INDEX]);
	glEnableVertexAttribArray(0); // v_position
	glEnableVertexAttribArray(1); // v_color
	glEnableVertexAttribArray(2); // v_normal
	glBindBuffer(GL_ARRAY_BUFFER, glBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[CUBE_INDEX] + (0 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[CUBE_INDEX] + (3 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[CUBE_INDEX] + (7 * sizeof(GL_FLOAT))));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBufferId);

	glBindVertexArray(vertexArrayObjectIds[SPHERE_INDEX]);
	glEnableVertexAttribArray(0); // v_position
	glEnableVertexAttribArray(1); // v_color
	glEnableVertexAttribArray(2); // v_normal
	glBindBuffer(GL_ARRAY_BUFFER, glBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[SPHERE_INDEX] + (0 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[SPHERE_INDEX] + (3 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[SPHERE_INDEX] + (7 * sizeof(GL_FLOAT))));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBufferId);

	glBindVertexArray(vertexArrayObjectIds[PLANE_INDEX]);
	glEnableVertexAttribArray(0); // v_position
	glEnableVertexAttribArray(1); // v_color
	glEnableVertexAttribArray(2); // v_normal
	glBindBuffer(GL_ARRAY_BUFFER, glBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[PLANE_INDEX] + (0 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[PLANE_INDEX] + (3 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[PLANE_INDEX] + (7 * sizeof(GL_FLOAT))));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBufferId);

}

void GlWindow::windowUpdate()
{
	//lightPosition.x += 0.1f;
	//qDebug() << lightPosition.x;
	checkKeyState();
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
		camera.moveLeft();
	}
	if (GetAsyncKeyState('D')) {
		camera.moveRight();
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

void GlWindow::mouseMoveEvent(QMouseEvent* ev)
{
	camera.mouseUpdate(glm::vec2(ev->x(), ev->y()));
	repaint();
}

void GlWindow::paintGL()
{
	GLuint programIDCurrent = programIDFragment;
	glUseProgram(programIDCurrent);
	ambientLightUniformLocation = glGetUniformLocation(programIDCurrent, "ambientLight");
	lightPositionUniformLocation = glGetUniformLocation(programIDCurrent, "lightPosition");
	fullTransformMatrixUniformLocation = glGetUniformLocation(programIDCurrent, "fullTransformMatrix");
	modelToWorldTransformMatrixUniformLocation = glGetUniformLocation(programIDCurrent, "modelToWorldTransformMatrix");

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glUniform3fv(ambientLightUniformLocation, 1, &model->ambientLightColor[0]);
	glUniform3fv(lightPositionUniformLocation, 1, &model->lightPosition[0]);
	

	glm::mat4 fullTransformMatrix;
	glm::mat4 modelToWorldTransformMatrix;
	glm::mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width() / height()), 0.1f, 20.0f);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;


	// arrow
	glBindVertexArray(vertexArrayObjectIds[ARROW_INDEX]);
	modelToWorldTransformMatrix =
		glm::translate(glm::vec3(2.5f, 3.0f, -3.75f)) *
		glm::rotate(20.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	fullTransformMatrix = worldToProjectionMatrix *modelToWorldTransformMatrix;
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &modelToWorldTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[ARROW_INDEX], GL_UNSIGNED_SHORT, (void*)indexByteOffset[ARROW_INDEX]);

	// cube
	glBindVertexArray(vertexArrayObjectIds[CUBE_INDEX]);
	modelToWorldTransformMatrix =
		glm::translate(glm::vec3(3.0f, 0.0f, 0.0f)) *
		glm::rotate(26.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix *modelToWorldTransformMatrix;
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &modelToWorldTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[CUBE_INDEX], GL_UNSIGNED_SHORT, (void*)indexByteOffset[CUBE_INDEX]);

	// sphere
	glBindVertexArray(vertexArrayObjectIds[SPHERE_INDEX]);
	modelToWorldTransformMatrix =
		glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
	fullTransformMatrix = worldToProjectionMatrix *modelToWorldTransformMatrix;
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &modelToWorldTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[SPHERE_INDEX], GL_UNSIGNED_SHORT, (void*)indexByteOffset[SPHERE_INDEX]);

	// plane
	glBindVertexArray(vertexArrayObjectIds[PLANE_INDEX]);
	modelToWorldTransformMatrix =
		glm::mat4();
	fullTransformMatrix = worldToProjectionMatrix * modelToWorldTransformMatrix;
	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldTransformMatrixUniformLocation, 1, GL_FALSE, &modelToWorldTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[PLANE_INDEX], GL_UNSIGNED_SHORT, (void*)indexByteOffset[PLANE_INDEX]);
}

GlWindow::~GlWindow()
{
	glDeleteBuffers(1, &glBufferId);
	glUseProgram(0);
	//glDeleteProgram(programID);
}