#include <QtWidgets/QApplication>
#include "ui/ColorChooser.h"

#include "StyleFusionDark.h"

int main( int argc, char* argv[] )
{
	QApplication app(argc, argv);
	ColorChooser chooser;
	ui_style::setStyle();
	chooser.show();

	app.exec();
	return 0;
}
