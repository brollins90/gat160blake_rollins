#pragma once
#include <GL\glew.h>
#include <QtOpenGl\qglwidget>
#include <Qt\qtimer.h>

class GlWindow : public QGLWidget
{
private:
	GLuint bufferID;
	GLuint programID;
	QTimer windowTimer;

	void createProgram();
	void compileShaders();
	void sendDataToHardware();
	void windowUpdate();
protected:
	void initializeGL();
	void paintGL();
public:
};
