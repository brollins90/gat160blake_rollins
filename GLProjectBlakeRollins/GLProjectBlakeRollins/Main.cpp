#include <Qt\qapplication.h>
#include "ProgramWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	ProgramWindow* mainWindow = new ProgramWindow();
	mainWindow->setFixedWidth(1600);
	mainWindow->setFixedHeight(900);
	mainWindow->show();
	return app.exec();
}