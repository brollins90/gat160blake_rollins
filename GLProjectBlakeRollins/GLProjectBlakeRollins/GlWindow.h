#pragma once
#include <GL\glew.h>
#include <QtOpenGl\qglwidget>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <iostream>
#include <fstream>
#include <Qt\qdebug.h>
#include <Qt\qtimer.h>
#include <QtGui\QKeyEvent>
#include "Camera.h"

class GlWindow : public QGLWidget
{
private:
	Q_OBJECT
	GLuint programID;
	QTimer windowTimer;
	Camera camera;

	void createProgram();
	bool checkShaderStatus(GLuint shaderID);
	void compileShaders();
	void sendDataToHardware();
private slots:
	void windowUpdate();
protected:
	void initializeGL();
	void checkKeyState();
	void mouseMoveEvent(QMouseEvent* ev);
	void paintGL();
public:
	~GlWindow();
};
