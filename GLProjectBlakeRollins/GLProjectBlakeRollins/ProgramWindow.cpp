#include "ProgramWindow.h"
#include "Qt\qlayout.h"
#include "Qt\qlabel.h"
#include "GlWindow.h"
#include "DebugSlider.h"

DebugSlider* ambientR;
DebugSlider* ambientG;
DebugSlider* ambientB;
DebugSlider* diffuseR;
DebugSlider* diffuseG;
DebugSlider* diffuseB;
DebugSlider* specularR;
DebugSlider* specularG;
DebugSlider* specularB;

ProgramWindow::ProgramWindow()
{
	CreateSliders();
	QVBoxLayout* mainLayout = new QVBoxLayout();
	QVBoxLayout* slidersLayout = new QVBoxLayout();

	setLayout(mainLayout);
	GlWindow* window = new GlWindow();
	mainLayout->addLayout(slidersLayout);
	mainLayout->addWidget(window);

	QHBoxLayout* row1Layout = new QHBoxLayout();
	slidersLayout->addLayout(row1Layout);
	row1Layout->addWidget(new QLabel("Ambient < "));
	row1Layout->addWidget(ambientR);
	row1Layout->addWidget(new QLabel(" , "));
	row1Layout->addWidget(ambientG);
	row1Layout->addWidget(new QLabel(" , "));
	row1Layout->addWidget(ambientB);
	row1Layout->addWidget(new QLabel(" >"));

	QHBoxLayout* row2Layout = new QHBoxLayout();
	slidersLayout->addLayout(row2Layout);
	row2Layout->addWidget(new QLabel("diffuse < "));
	row2Layout->addWidget(diffuseR);
	row2Layout->addWidget(new QLabel(" , "));
	row2Layout->addWidget(diffuseG);
	row2Layout->addWidget(new QLabel(" , "));
	row2Layout->addWidget(diffuseB);
	row2Layout->addWidget(new QLabel(" >"));

	QHBoxLayout* row3Layout = new QHBoxLayout();
	slidersLayout->addLayout(row3Layout);
	row3Layout->addWidget(new QLabel("specular < "));
	row3Layout->addWidget(specularR);
	row3Layout->addWidget(new QLabel(" , "));
	row3Layout->addWidget(specularG);
	row3Layout->addWidget(new QLabel(" , "));
	row3Layout->addWidget(specularB);
	row3Layout->addWidget(new QLabel(" >"));


}

void ProgramWindow::CreateSliders()
{
	ambientR = new DebugSlider();
	ambientG = new DebugSlider();
	ambientB = new DebugSlider();
	diffuseR = new DebugSlider();
	diffuseG = new DebugSlider();
	diffuseB = new DebugSlider();
	specularR = new DebugSlider();
	specularG = new DebugSlider();
	specularB = new DebugSlider();
}

ProgramWindow::~ProgramWindow()
{

}