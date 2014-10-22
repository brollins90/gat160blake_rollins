#include "SliderWindow.h"
#include "Qt\qlayout.h"
#include "Qt\qlabel.h"

#include "Qt\qdebug.h"


SliderWindow::SliderWindow(ProjectModel* model_in)
{
	model = model_in;

	CreateSliders();
	QVBoxLayout* slidersLayout = new QVBoxLayout();
	setLayout(slidersLayout);

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

	QHBoxLayout* row4Layout = new QHBoxLayout();
	slidersLayout->addLayout(row4Layout);
	row4Layout->addWidget(new QLabel("position < "));
	row4Layout->addWidget(lightPositionX);
	row4Layout->addWidget(new QLabel(" , "));
	row4Layout->addWidget(lightPositionY);
	row4Layout->addWidget(new QLabel(" , "));
	row4Layout->addWidget(lightPositionZ);
	row4Layout->addWidget(new QLabel(" >"));
}

void SliderWindow::CreateSliders()
{
	ambientR = new DebugSlider(0.0f, 1.0f);
	ambientR->setValue(model->ambientLightColor.r);
	connect(ambientR, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	ambientG = new DebugSlider(0.0f, 1.0f);
	ambientG->setValue(model->ambientLightColor.g);
	connect(ambientG, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	ambientB = new DebugSlider(0.0f, 1.0f);
	ambientB->setValue(model->ambientLightColor.b);
	connect(ambientB, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	
	diffuseR = new DebugSlider(0.0f, 1.0f);
	diffuseR->setValue(model->diffuseLightColor.r);
	connect(diffuseR, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	diffuseG = new DebugSlider(0.0f, 1.0f);
	diffuseG->setValue(model->diffuseLightColor.g);
	connect(diffuseG, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	diffuseB = new DebugSlider(0.0f, 1.0f);
	diffuseB->setValue(model->diffuseLightColor.b);
	connect(diffuseB, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));

	specularR = new DebugSlider(0.0f, 1.0f);
	specularR->setValue(model->specularLightColor.r);
	connect(specularR, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	specularG = new DebugSlider(0.0f, 1.0f);
	specularG->setValue(model->specularLightColor.g);
	connect(specularG, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	specularB = new DebugSlider(0.0f, 1.0f);
	specularB->setValue(model->specularLightColor.b);
	connect(specularB, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));

	lightPositionX = new DebugSlider();
	lightPositionX->setValue(model->lightPosition.x);
	connect(lightPositionX, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	lightPositionY = new DebugSlider();
	lightPositionY->setValue(model->lightPosition.y);
	connect(lightPositionY, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	lightPositionZ = new DebugSlider();
	lightPositionZ->setValue(model->lightPosition.z);
	connect(lightPositionZ, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
}

void SliderWindow::updateModel()
{
//	qDebug() << w << " " << f;
	model->ambientLightColor.r = ambientR->value();
	model->ambientLightColor.g = ambientG->value();
	model->ambientLightColor.b = ambientB->value();

	model->diffuseLightColor.r = diffuseR->value();
	model->diffuseLightColor.g = diffuseG->value();
	model->diffuseLightColor.b = diffuseB->value();
	
	model->specularLightColor.r = specularR->value();
	model->specularLightColor.g = specularG->value();
	model->specularLightColor.b = specularB->value();

	model->lightPosition.x = lightPositionX->value();
	model->lightPosition.y = lightPositionY->value();
	model->lightPosition.z = lightPositionZ->value();
}

SliderWindow::~SliderWindow()
{
}