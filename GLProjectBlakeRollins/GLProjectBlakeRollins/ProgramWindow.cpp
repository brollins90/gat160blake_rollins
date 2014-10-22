#include "ProgramWindow.h"
#include "Qt\qlayout.h"
#include "Qt\qlabel.h"
#include "SliderWindow.h"
#include "GlWindow.h"
#include "DebugSlider.h"


ProgramWindow::ProgramWindow()
{
	ProjectModel* model = new ProjectModel();

	QVBoxLayout* mainLayout = new QVBoxLayout();

	setLayout(mainLayout);
	SliderWindow* sliders = new SliderWindow(model);
	sliders->setMinimumHeight(400);
	sliders->setMaximumHeight(400);
	mainLayout->addWidget(sliders);
	GlWindow* window = new GlWindow(model);
	//window->setMinimumWidth(1600);
	//window->setMinimumHeight(500);
	mainLayout->addWidget(window);
}

ProgramWindow::~ProgramWindow()
{

}