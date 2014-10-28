#pragma once
#include <Qt\qwidget.h>
#include "DebugSlider.h"
#include "ProjectModel.h"
#include "Qt\qlayout.h"
#include "Qt\qlabel.h"
#include "Qt\qcombobox.h"
#include "Qt\qdebug.h"
#include "Qt\qcheckbox.h"

class SliderWindow : public QWidget
{
private:
	Q_OBJECT
	ProjectModel* model;

	DebugSlider* ambientR;
	DebugSlider* ambientG;
	DebugSlider* ambientB;
	DebugSlider* diffuseR;
	DebugSlider* diffuseG;
	DebugSlider* diffuseB;
	DebugSlider* specularR;
	DebugSlider* specularG;
	DebugSlider* specularB;
	DebugSlider* specularExp;
	DebugSlider* lightPositionX;
	DebugSlider* lightPositionY;
	DebugSlider* lightPositionZ;
	QComboBox* programCombo;
	QCheckBox* lightingCheck;

private slots:
void updateModel();

protected:
	void CreateSliders();
public:
	SliderWindow(ProjectModel* model);
	~SliderWindow();
};

