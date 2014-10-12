#pragma once
#include <GL\glew.h>
#include <QtOpenGl\qglwidget>
#include <iostream>
#include <fstream>
#include <glm\gtx\transform.hpp>
#include <Qt\qtimer.h>
#include <glm\glm.hpp>
#include <Qt\qdebug.h>
#include <QtGui\QKeyEvent>
#include <QtGui\qmouseevent>
#include "Camera.h"
//#include "cube.h"


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
//	bool checkProgramStatus(GLuint programID);
	void createProgram();
	void compileShaders();
	void hillMath();
	void sendDataToHardware();
private slots:
	void windowUpdate();
protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
public:
	~GlWindow();
};
