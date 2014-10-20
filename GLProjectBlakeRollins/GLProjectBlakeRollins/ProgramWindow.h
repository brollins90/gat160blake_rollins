#pragma once
#include <GL\glew.h>
#include <Qt\qwidget.h>
#include "ProjectModel.h"

class ProgramWindow : public QWidget
{
private:
	void CreateSliders();
public:
	ProgramWindow();
	~ProgramWindow();
};
