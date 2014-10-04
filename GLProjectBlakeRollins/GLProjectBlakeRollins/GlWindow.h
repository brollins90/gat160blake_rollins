#pragma once
#include <GL\glew.h>
#include <QtOpenGl\qglwidget>
#include <Qt\qtimer.h>
#include <glm\glm.hpp>
#include <Qt\qdebug.h>

class GlWindow : public QGLWidget
{
private:

	Q_OBJECT

	GLuint bufferID;
	GLuint programID;
	QTimer windowTimer;

	bool checkShaderStatus(GLuint shaderID);
	void createProgram();
	void compileShaders();
	void sendDataToHardware();
private slots:
	void windowUpdate();
protected:
	void initializeGL();
	void paintGL();
public:
};
