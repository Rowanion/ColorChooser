#include <QtWidgets/QApplication>
#include "ui/ColorChooser.h"

int main( int argc, char* argv[] )
{
	QApplication app(argc, argv);
	ColorChooser chooser;
	chooser.show();

	app.exec();
	return 0;
}
