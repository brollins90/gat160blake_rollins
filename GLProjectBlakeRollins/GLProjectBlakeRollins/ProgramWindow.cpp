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
	GlWindow* window = new GlWindow(model);
	window->setMinimumWidth(1600);
	window->setMinimumHeight(500);
	mainLayout->addWidget(sliders);
	mainLayout->addWidget(window);
}

ProgramWindow::~ProgramWindow()
{

}