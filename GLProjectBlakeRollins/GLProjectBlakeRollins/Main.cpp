#include <Qt\qapplication.h>
#include "GlWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	GlWindow window;
	window.show();

	return app.exec();
}