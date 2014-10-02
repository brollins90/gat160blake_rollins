#pragma once
#include <QtOpenGl\qglwidget>

class GlWindow : public QGLWidget
{
private:
	void sendDataToHardware();
	void compileShaders();
protected:
	void initializeGL();
	void paintGL();
public:
};
