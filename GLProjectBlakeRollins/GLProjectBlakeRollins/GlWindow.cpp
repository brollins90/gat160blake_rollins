#include "GlWindow.h"

glm::vec3 triPosition1, triPosition2;
glm::vec3 triVelocity1, triVelocity2;
glm::vec3 triDirection1, triDirection2;
float triAngle1, triAngle2;
float triDepth1, triDepth2;
const float SPEED = 0.005f;
const float SCALE = 0.45f;
GLint fullTransformMatrixUniformLocation;
GLint uniformColorUniformLocation;
GLint vertexDepthUniformLocation;
float triScore1 = 0;
float triScore2 = 0;


// Define the data
GLfloat vertices[] =
{
	// triangle indices
	+0.00f, +0.30f, +0.00f, +0.8f, +1.0f, +0.1f, // 0
	-0.15f, -0.30f, +0.00f, +1.0f, +0.0f, +0.7f, // 1
	+0.00f, -0.20f, +0.00f, +0.9f, +0.2F, +0.3f, // 2
	+0.15f, -0.30f, +0.00f, +1.0f, +0.0f, +0.7f, // 3
	+0.00f, -0.20f, +0.00f, +0.9f, +0.2F, +0.3f, // 4

	// Hill indicis
	+0.2f, +0.2f, +0.0f, +0.0f, 0.0f, 0.1f, // 5
	-0.2f, +0.2f, +0.0f, +0.0f, 0.0f, 0.1f, // 6
	-0.2f, -0.2f, +0.0f, +0.0f, 0.0f, 0.1f, // 7
	+0.2f, -0.2f, +0.0f, +0.0f, 0.0f, 0.1f, // 8

	/*
	+0.00f, +0.30f, +0.00f, +1.0f, +0.0f, +0.0f,
	-0.30f, -0.30f, +0.00f, +0.0f, +1.0f, +0.0f,
	+0.30f, -0.30f, +0.00f, +0.0f, +0.0F, +1.0f,*/
};

GLushort indices[] = { 0,1,2, 0,3,4, // tri
						5,6,7, 5,7,8 }; // hill

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
	std::string temp = readShaderCode("VertexShaderCode.glsl");
	glslCode[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, glslCode, 0);
	temp = readShaderCode("FragmentShaderCode.glsl").c_str();
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

	// Tell the hardware to use our stuff and not the default shaders
	glUseProgram(programID);

	fullTransformMatrixUniformLocation = glGetUniformLocation(programID, "fullTransformMatrix");
	uniformColorUniformLocation = glGetUniformLocation(programID, "uniformColor");
	vertexDepthUniformLocation = glGetUniformLocation(programID, "vertexDepth");
}

void GlWindow::initializeGL()
{
	glewInit();
	createProgram();

	triPosition1 = glm::vec3(-0.5f, 0.0f, 0.0f);
	triAngle1 = (3.141f / 4.0f);
	triVelocity1 = glm::vec3(0.0f, 0.0f, 0.0f);

	triPosition2 = glm::vec3(0.5f, 0.0f, 0.0f);
	triAngle2 = (3.141f / 4.0f);
	triVelocity2 = glm::vec3(0.0f, 0.0f, 0.0f);

	sendDataToHardware();
	compileShaders();
}

void GlWindow::checkKeyState()
{
	if (GetAsyncKeyState('W')) {
		triVelocity1 += (/*dt **/ SPEED * triDirection1);
	}
	if (GetAsyncKeyState('S')) {
		triVelocity1 -= (/*dt **/ SPEED * triDirection1);
	}
	if (GetAsyncKeyState('A')) {
		triAngle1 += .75;
	}
	if (GetAsyncKeyState('D')) {
		triAngle1 -= .75;
	}

	if (GetAsyncKeyState(VK_UP)) {
		triVelocity2 += (/*dt **/ SPEED * triDirection2);
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		triVelocity2 -= (/*dt **/ SPEED * triDirection2);
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		triAngle2 += .75;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		triAngle2 -= .75;
	}
}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width(), height());

	// Object 1

	triVelocity1 *= 0.9;
	triPosition1 += triVelocity1;

	glm::mat4 translationMatrix = glm::translate(glm::mat4(), triPosition1);
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(), triAngle1, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(SCALE));
	glm::mat4 fullTransformMatrix = scaleMatrix * translationMatrix * rotationMatrix;

	// Direction is normalized about the Y axis
	triDirection1 = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
	glm::vec3 uniformColor = glm::vec3(1.0f, 0.0f, 0.0f);

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniform3fv(uniformColorUniformLocation, 1, &uniformColor[0]);
	glUniform1f(vertexDepthUniformLocation, triDepth1);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(0 * sizeof(GLushort)));


	// Object 2

	triVelocity2 *= 0.9;
	triPosition2 += triVelocity2;

	translationMatrix = glm::translate(glm::mat4(), triPosition2);
	rotationMatrix = glm::rotate(glm::mat4(), triAngle2, glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(SCALE));
	fullTransformMatrix = scaleMatrix * translationMatrix * rotationMatrix;

	// Direction is normalized about the Y axis
	triDirection2 = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));
	uniformColor = glm::vec3(0.0f, 1.0f, 0.0f);

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniform3fv(uniformColorUniformLocation, 1, &uniformColor[0]);
	glUniform1f(vertexDepthUniformLocation, triDepth2);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(0 * sizeof(GLushort)));


	// Hill

	translationMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, 0));
	rotationMatrix = glm::rotate(glm::mat4(), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	scaleMatrix = glm::scale(glm::mat4(), glm::vec3(SCALE));
	fullTransformMatrix = scaleMatrix * translationMatrix * rotationMatrix;

	uniformColor = glm::vec3(0.0f, 0.0f, 1.0f);

	glUniformMatrix4fv(fullTransformMatrixUniformLocation, 1, GL_FALSE, &fullTransformMatrix[0][0]);
	glUniform3fv(uniformColorUniformLocation, 1, &uniformColor[0]);
	glUniform1f(vertexDepthUniformLocation, 0.9f);


	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)(6 * sizeof(GLushort)));
}

void GlWindow::sendDataToHardware()
{

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
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

// Timer callback
void GlWindow::windowUpdate()
{
	qDebug() << triScore1 << " " << triScore2;
	hillMath();
	checkKeyState();
	repaint();
}


float GlWindow::distFromTriangle(glm::vec3 pos) {

	return glm::distance(pos, glm::vec3(0, 0, 0));

	//float distMin = 1;
	//mat4 triTrans = glm::translate(mat4(), pos);
	//mat4 hillTrans = glm::translate(mat4(), vec3(0, 0, 0));
	//for (int i = 0; i < 6; i++) {

	//	vec3 trianglepoint = vec3(vertices[(indices[i] * 6) + 0], vertices[(indices[i] * 6) + 1], vertices[(indices[i] * 6) + 2]);

	//	//qDebug() << trianglepoint.x; 
	//	trianglepoint = vec3(triTrans * glm::vec4(trianglepoint, 0));

	//	//qDebug() << trianglepoint.x;

	//	for (int j = 6; j < 12; j++) {
	//		vec3 hillpoint = vec3(vertices[(indices[j] * 6) + 0], vertices[(indices[j] * 6) + 1], vertices[(indices[j] * 6) + 2]);
	//		hillpoint = vec3(hillTrans * glm::vec4(hillpoint, 0));

	//		float dist = glm::distance(hillpoint, trianglepoint);
	//		if (dist < distMin) {
	//			distMin = dist;
	//		}
	//	}
	//}
	////qDebug() << distMin;
	//return distMin;
}

void GlWindow::hillMath()
{
	if (distFromTriangle(triPosition1) < .3) {
		triScore1 += 0.01f;
	}
	if (distFromTriangle(triPosition2) < .3) {
		triScore2 += 0.01f;
	}

	if (triScore1 > triScore2) {
		triDepth1 = -0.7f;
		triDepth2 = +0.7f;
	}
	else {
		triDepth1 = +0.7f;
		triDepth2 = -0.7f;
	}
}