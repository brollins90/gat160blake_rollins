#include "GlWindow.h"

glm::vec3 cubePosition, cubeVelocity, cubeDirection, cubeAngles;
glm::vec3 camPosition;
const float SPEED = 0.005f;
const float SCALE = 0.45f;
GLint fullTransformMatrixUniformLocation;
GLint MVPLocation;


// Define the data
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
	MVPLocation = glGetUniformLocation(programID, "MVP");
}

void GlWindow::initializeGL()
{
	glewInit();
	createProgram();

	cubePosition = glm::vec3(+0.0f, 0.0f, -1.0f);
	cubeAngles = glm::vec3(0.0f, 0.0f, 0.0f);
	cubeVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

	camPosition = glm::vec3(0.0f, -5.0f, -10.0f);

	sendDataToHardware();
	compileShaders();
}

void GlWindow::paintGL()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glViewport(0, 0, width(), height());
	
	// model
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(), glm::vec3(SCALE)); // scale by our scale value
	glm::mat4 rotationMatrix = glm::mat4(); 
	rotationMatrix = glm::rotate(rotationMatrix, cubeAngles.x, glm::vec3(1.0f, 0.0f, 0.0f)); // rotate x
	rotationMatrix = glm::rotate(rotationMatrix, cubeAngles.y, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate y
	rotationMatrix = glm::rotate(rotationMatrix, cubeAngles.z, glm::vec3(0.0f, 0.0f, 1.0f)); // rotate z
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), cubePosition); // translate
	glm::mat4 model = translationMatrix * rotationMatrix * scaleMatrix * glm::mat4(1.0f); // combine

	// Camera - view
	glm::mat4 view = glm::lookAt(
		camPosition, // camera location in world space
		glm::vec3(0.0f, 0.0f, 0.0f), // Looking at the origin
		glm::vec3(0.0f, 1.0f, 0.0f)); // y is up

	// projection 
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	glm::mat4 MVP = projection * view * model;
	glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)(0 * sizeof(GLushort)));
}

void GlWindow::sendDataToHardware()
{

	// Create a buffer in graphics ram
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

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
	cubeAngles.x += .1f;
	cubeAngles.y += .1f;
	cubeAngles.z += .1f;
	repaint();
}

