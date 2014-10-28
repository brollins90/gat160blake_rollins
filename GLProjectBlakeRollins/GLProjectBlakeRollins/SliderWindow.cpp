#include "SliderWindow.h"


SliderWindow::SliderWindow(ProjectModel* model_in)
{
	model = model_in;

	CreateSliders();
	QVBoxLayout* slidersLayout = new QVBoxLayout();
	setLayout(slidersLayout);

	QHBoxLayout* row0Layout = new QHBoxLayout();
	slidersLayout->addLayout(row0Layout);
	row0Layout->addWidget(new QLabel("Blake's GL Project Lighting "));
	row0Layout->addWidget(programCombo);
	row0Layout->addWidget(new QLabel("Lighting"));
	row0Layout->addWidget(lightingCheck);
	
	QHBoxLayout* row1Layout = new QHBoxLayout();
	slidersLayout->addLayout(row1Layout);

	QHBoxLayout* row11Layout = new QHBoxLayout();
	row1Layout->addLayout(row11Layout);
	row11Layout->addWidget(new QLabel("Ambient < "));
	row11Layout->addWidget(ambientR);
	row11Layout->addWidget(new QLabel(" , "));
	row11Layout->addWidget(ambientG);
	row11Layout->addWidget(new QLabel(" , "));
	row11Layout->addWidget(ambientB);
	row11Layout->addWidget(new QLabel(" >"));

	QHBoxLayout* row12Layout = new QHBoxLayout();
	row1Layout->addLayout(row12Layout);
	row12Layout->addWidget(new QLabel("Diffuse < "));
	row12Layout->addWidget(diffuseR);
	row12Layout->addWidget(new QLabel(" , "));
	row12Layout->addWidget(diffuseG);
	row12Layout->addWidget(new QLabel(" , "));
	row12Layout->addWidget(diffuseB);
	row12Layout->addWidget(new QLabel(" >"));


	QHBoxLayout* row2Layout = new QHBoxLayout();
	slidersLayout->addLayout(row2Layout);

	QHBoxLayout* row21Layout = new QHBoxLayout();
	row2Layout->addLayout(row21Layout);
	row21Layout->addWidget(new QLabel("Specular < "));
	row21Layout->addWidget(specularR);
	row21Layout->addWidget(new QLabel(" , "));
	row21Layout->addWidget(specularG);
	row21Layout->addWidget(new QLabel(" , "));
	row21Layout->addWidget(specularB);
	row21Layout->addWidget(new QLabel(" >"));

	QHBoxLayout* row22Layout = new QHBoxLayout();
	row2Layout->addLayout(row22Layout);
	row22Layout->addWidget(new QLabel("Specular Exponent < "));
	row22Layout->addWidget(specularExp);
	row22Layout->addWidget(new QLabel(" >"));

	QHBoxLayout* row3Layout = new QHBoxLayout();
	slidersLayout->addLayout(row3Layout);
	row3Layout->addWidget(new QLabel("position < "));
	row3Layout->addWidget(lightPositionX);
	row3Layout->addWidget(new QLabel(" , "));
	row3Layout->addWidget(lightPositionY);
	row3Layout->addWidget(new QLabel(" , "));
	row3Layout->addWidget(lightPositionZ);
	row3Layout->addWidget(new QLabel(" >"));
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
	specularExp = new DebugSlider(1.0f, 50.0f);
	specularExp->setValue(model->specularExponent);
	connect(specularExp, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));

	

	lightPositionX = new DebugSlider();
	lightPositionX->setValue(model->lightPosition.x);
	connect(lightPositionX, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	lightPositionY = new DebugSlider();
	lightPositionY->setValue(model->lightPosition.y);
	connect(lightPositionY, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));
	lightPositionZ = new DebugSlider();
	lightPositionZ->setValue(model->lightPosition.z);
	connect(lightPositionZ, SIGNAL(valueChanged(float)), this, SLOT(updateModel()));


	programCombo = new QComboBox();
	programCombo->addItem("Pass Through", 0);
	programCombo->addItem("Fragment", 1);
	programCombo->addItem("Vertex", 2);
	programCombo->setCurrentIndex(model->programIndex);
	connect(programCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateModel()));

	lightingCheck = new QCheckBox();
	connect(lightingCheck, SIGNAL(stateChanged(int)), this, SLOT(updateModel()));
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

	model->specularExponent = specularExp->value();

	model->lightPosition.x = lightPositionX->value();
	model->lightPosition.y = lightPositionY->value();
	model->lightPosition.z = lightPositionZ->value();

	model->programIndex = programCombo->currentIndex();

	if (lightingCheck->isChecked())
	{
		model->addLighting = 1;
	}
	else
	{
		model->addLighting = 0;
	}
}

SliderWindow::~SliderWindow()
{
}
