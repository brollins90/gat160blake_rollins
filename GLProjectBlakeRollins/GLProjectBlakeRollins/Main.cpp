#include <Qt\qapplication.h>
#include "ProgramWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	ProgramWindow* mainWindow = new ProgramWindow();
	mainWindow->setFixedWidth(1280);
	mainWindow->setFixedHeight(800);
	mainWindow->show();
	return app.exec();
}