#include <QtGui/QApplication>
#include "ui/ColorChooser.h"

int main( int argc, char* argv[] )
{
	QApplication app(argc, argv);
	ColorChooser chooser;

	app.exec();
	return 0;
}
