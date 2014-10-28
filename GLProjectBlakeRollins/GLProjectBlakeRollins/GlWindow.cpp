#include "GlWindow.h"

const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VERTICE = sizeof(Neumont::Vertex) / sizeof(float);
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VERTICE * sizeof(float);

GLuint glBufferId;
GLuint glTextureId;

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
	assert(glGetError() == GL_NO_ERROR);

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

bool GlWindow::checkProgramStatus(GLuint programID)
{
	GLint compilationStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &compilationStatus);
	assert(glGetError() == GL_NO_ERROR);

	if (compilationStatus != 0) {
		GLint logLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		char* buffer = new char[logLength];
		GLsizei bitBucket;
		glGetProgramInfoLog(programID, logLength, &bitBucket, buffer);
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

	temp = readShaderCode("Shaders/VertexShaderVertex.glsl");
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
	checkShaderStatus(vertexShaderIDPassThrough);
	checkShaderStatus(fragmentShaderIDPassThrough);
	glAttachShader(programIDPassThrough, vertexShaderIDPassThrough);
	glAttachShader(programIDPassThrough, fragmentShaderIDPassThrough);
	glLinkProgram(programIDPassThrough);
	checkProgramStatus(programIDPassThrough);
	glDeleteShader(vertexShaderIDPassThrough);
	glDeleteShader(fragmentShaderIDPassThrough);


	glCompileShader(vertexShaderIDVertex);
	glCompileShader(fragmentShaderIDVertex);
	checkShaderStatus(vertexShaderIDVertex);
	checkShaderStatus(fragmentShaderIDVertex);
	glAttachShader(programIDVertex, vertexShaderIDVertex);
	glAttachShader(programIDVertex, fragmentShaderIDVertex);
	glLinkProgram(programIDVertex);
	checkProgramStatus(programIDVertex);
	glDeleteShader(vertexShaderIDVertex);
	glDeleteShader(fragmentShaderIDVertex);

	glCompileShader(vertexShaderIDFragment);
	glCompileShader(fragmentShaderIDFragment);
	checkShaderStatus(vertexShaderIDFragment);
	checkShaderStatus(fragmentShaderIDFragment);
	glAttachShader(programIDFragment, vertexShaderIDFragment);
	glAttachShader(programIDFragment, fragmentShaderIDFragment);
	glLinkProgram(programIDFragment);
	checkProgramStatus(programIDFragment);
	glDeleteShader(vertexShaderIDFragment);
	glDeleteShader(fragmentShaderIDFragment);

	glUseProgram(0);
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
	glEnableVertexAttribArray(3); // v_uv
	glBindBuffer(GL_ARRAY_BUFFER, glBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[ARROW_INDEX] + (0 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[ARROW_INDEX] + (3 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[ARROW_INDEX] + (7 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[ARROW_INDEX] + (9 * sizeof(GL_FLOAT))));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBufferId);

	glBindVertexArray(vertexArrayObjectIds[CUBE_INDEX]);
	glEnableVertexAttribArray(0); // v_position
	glEnableVertexAttribArray(1); // v_color
	glEnableVertexAttribArray(2); // v_normal
	glEnableVertexAttribArray(3); // v_uv
	glBindBuffer(GL_ARRAY_BUFFER, glBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[CUBE_INDEX] + (0 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[CUBE_INDEX] + (3 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[CUBE_INDEX] + (7 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[CUBE_INDEX] + (9 * sizeof(GL_FLOAT))));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBufferId);

	glBindVertexArray(vertexArrayObjectIds[SPHERE_INDEX]);
	glEnableVertexAttribArray(0); // v_position
	glEnableVertexAttribArray(1); // v_color
	glEnableVertexAttribArray(2); // v_normal
	glEnableVertexAttribArray(3); // v_uv
	glBindBuffer(GL_ARRAY_BUFFER, glBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[SPHERE_INDEX] + (0 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[SPHERE_INDEX] + (3 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[SPHERE_INDEX] + (7 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[SPHERE_INDEX] + (9 * sizeof(GL_FLOAT))));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBufferId);

	glBindVertexArray(vertexArrayObjectIds[PLANE_INDEX]);
	glEnableVertexAttribArray(0); // v_position
	glEnableVertexAttribArray(1); // v_color
	glEnableVertexAttribArray(2); // v_normal
	glEnableVertexAttribArray(3); // v_uv
	glBindBuffer(GL_ARRAY_BUFFER, glBufferId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[PLANE_INDEX] + (0 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[PLANE_INDEX] + (3 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[PLANE_INDEX] + (7 * sizeof(GL_FLOAT))));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, (void*)(vertexByteOffset[PLANE_INDEX] + (7 * sizeof(GL_FLOAT))));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glBufferId);


	TextureImage myImage = TextureImage("uvtemplate.bmp");

	glGenTextures(1, &glTextureId);
	glBindTexture(GL_TEXTURE_2D, glTextureId);
	glActiveTexture(GL_TEXTURE0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myImage.width, myImage.height, 0, GL_RGB, GL_UNSIGNED_BYTE, myImage.data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

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
	GLuint programIDCurrent = model->programIndex + 1;

	glUseProgram(programIDCurrent);

	GLint modelToProjectionMatrixUniformLocation = glGetUniformLocation(programIDCurrent, "modelToProjectionMatrix");
	GLint ambientLightColorUniformLocation = glGetUniformLocation(programIDCurrent, "ambientLightColor");
	GLint diffuseLightColorUniformLocation = glGetUniformLocation(programIDCurrent, "diffuseLightColor");
	GLint specularLightColorUniformLocation = glGetUniformLocation(programIDCurrent, "specularLightColor");
	GLint specularExponentUniformLocation = glGetUniformLocation(programIDCurrent, "specularExponent");
	GLint lightPositionWorldUniformLocation = glGetUniformLocation(programIDCurrent, "lightPositionWorld");
	GLint cameraPositionWorldUniformLocation = glGetUniformLocation(programIDCurrent, "cameraPositionWorld");
	GLint modelToWorldMatrixUniformLocation = glGetUniformLocation(programIDCurrent, "modelToWorldMatrix");
	GLint addLightingUniformLocation = glGetUniformLocation(programIDCurrent, "addLighting");


	
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	glUniform4fv(ambientLightColorUniformLocation, 1, &model->ambientLightColor[0]);
	glUniform4fv(diffuseLightColorUniformLocation, 1, &model->diffuseLightColor[0]);
	glUniform4fv(specularLightColorUniformLocation, 1, &model->specularLightColor[0]);
	glUniform1f(specularExponentUniformLocation, model->specularExponent);
	glUniform3fv(lightPositionWorldUniformLocation, 1, &model->lightPosition[0]);
	glm::vec3 eyePosition = camera.getPosition();
	glUniform3fv(cameraPositionWorldUniformLocation, 1, &eyePosition[0]);
	glUniform1f(addLightingUniformLocation, (model->addLighting > 0) ? 1.0f : -1.0f);


	glm::mat4 modelToProjectionMatrix;
	glm::mat4 modelToWorldMatrix;
	glm::mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width() / height()), 0.1f, 20.0f);
	glm::mat4 worldToViewMatrix = camera.getWorldToViewMatrix();
	glm::mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;


	// arrow
	glBindVertexArray(vertexArrayObjectIds[ARROW_INDEX]);
	modelToWorldMatrix =
		glm::translate(glm::vec3(2.5f, 3.0f, -3.75f)) *
		glm::rotate(20.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelToProjectionMatrix = worldToProjectionMatrix * modelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionMatrixUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &modelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[ARROW_INDEX], GL_UNSIGNED_SHORT, (void*)indexByteOffset[ARROW_INDEX]);

	// cube
	glBindVertexArray(vertexArrayObjectIds[CUBE_INDEX]);
	modelToWorldMatrix =
		glm::translate(glm::vec3(3.0f, 0.0f, 0.0f)) *
		glm::rotate(26.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelToProjectionMatrix = worldToProjectionMatrix * modelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionMatrixUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &modelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[CUBE_INDEX], GL_UNSIGNED_SHORT, (void*)indexByteOffset[CUBE_INDEX]);

	// sphere
	glBindVertexArray(vertexArrayObjectIds[SPHERE_INDEX]);
	modelToWorldMatrix =
		glm::translate(glm::vec3(0.0f, 1.0f, 0.0f));
	modelToProjectionMatrix = worldToProjectionMatrix * modelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionMatrixUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &modelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[SPHERE_INDEX], GL_UNSIGNED_SHORT, (void*)indexByteOffset[SPHERE_INDEX]);

	// plane
	glBindVertexArray(vertexArrayObjectIds[PLANE_INDEX]);
	modelToWorldMatrix =
		glm::mat4();
	modelToProjectionMatrix = worldToProjectionMatrix * modelToWorldMatrix;
	glUniformMatrix4fv(modelToProjectionMatrixUniformLocation, 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glUniformMatrix4fv(modelToWorldMatrixUniformLocation, 1, GL_FALSE, &modelToWorldMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[PLANE_INDEX], GL_UNSIGNED_SHORT, (void*)indexByteOffset[PLANE_INDEX]);

	// camera
	glUseProgram(programIDPassThrough);
	glBindVertexArray(vertexArrayObjectIds[CUBE_INDEX]);
	modelToWorldMatrix =
		glm::translate(model->lightPosition) *
		glm::scale(0.5f, 0.5f, 0.5f);
	modelToProjectionMatrix = worldToProjectionMatrix * modelToWorldMatrix;
	glUniformMatrix4fv(glGetUniformLocation(programIDPassThrough, "modelToProjectionMatrix"), 1, GL_FALSE, &modelToProjectionMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, numIndices[CUBE_INDEX], GL_UNSIGNED_SHORT, (void*)indexByteOffset[CUBE_INDEX]);
}

GlWindow::~GlWindow()
{
	glDeleteBuffers(1, &glBufferId);
	glUseProgram(0);
	//glDeleteProgram(programID);
}