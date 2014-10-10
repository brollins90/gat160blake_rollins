#pragma once
#include <GL\glew.h>
#include <QtOpenGl\qglwidget>
#include <Qt\qtimer.h>
#include <glm\glm.hpp>
#include <Qt\qdebug.h>
#include <QtGui\QKeyEvent>
#include <iostream>
#include <fstream>
#include <glm\gtx\transform.hpp>


class GlWindow : public QGLWidget
{
private:

	Q_OBJECT

	GLuint vertexBufferID;
	GLuint indexBufferID;
	GLuint programID;
	QTimer windowTimer;

	float distFromTriangle(glm::vec3 pos);
	void checkKeyState();
	bool checkShaderStatus(GLuint shaderID);
	void createProgram();
	void compileShaders();
	void hillMath();
	void sendDataToHardware();
private slots:
	void windowUpdate();
protected:
	void initializeGL();
	void paintGL();
public:
};
