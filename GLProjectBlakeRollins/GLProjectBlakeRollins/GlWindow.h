#pragma once
#include <QtOpenGl\qglwidget>

class GlWindow : public QGLWidget
{
private:
protected:
	void initializeGL();
	void paintGL();
public:
};
