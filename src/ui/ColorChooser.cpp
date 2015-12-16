#include "ColorChooser.h"

ColorChooser::ColorChooser(QWidget* parent)
: QMainWindow(parent)
{
	setupUi(this);

	QStringList names( QColor::colorNames() );
	foreach( const QString& name, names)
	{
		
	}
}

ColorChooser::~ColorChooser()
{

}
