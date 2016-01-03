#include "ColorChooser.h"

#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtGui/QBrush>
#include <QtGui/QClipboard>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QApplication>
#include <QtGui/QPainter>

ColorChooser::ColorChooser(QWidget* parent)
: QMainWindow(parent)
{
	setupUi(this);

	// setup the table
	m_model = new QStandardItemModel(this);
	tableView->setModel( m_model );
	tableView->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( tableView, &QTableView::customContextMenuRequested, this, &ColorChooser::onCustomContextMenu );


	QStringList names( QColor::colorNames() );
	m_model->setVerticalHeaderLabels(names);

	// fill the table
	QStandardItem* item{ nullptr };
	int i = 0;
	foreach( const QString& name, names)
	{
		const QColor theColor( name );
		item = new QStandardItem( QLatin1String{ " " } );
		item->setData( name, Qt::UserRole );
		item->setBackground( QBrush( QColor( theColor ) ) );
		m_model->setItem( i++, item );
	}

	tableView->resizeColumnsToContents();
	tableView->resizeRowsToContents();
}

ColorChooser::~ColorChooser()
{

}

void ColorChooser::onCustomContextMenu( const QPoint & point )
{
	QModelIndex index = tableView->indexAt( point );
	if (!index.isValid()) return;

	// get data
	m_currentColor = m_model->data( index, Qt::UserRole ).toString();

	QPixmap pixmap( 100, 100 );
	pixmap.fill( QColor( m_currentColor ) );
	QIcon anIcon( pixmap );

	//set context menu
	QMenu conextMenu( tableView );
	conextMenu.addAction( anIcon, QLatin1String( "copy color name" ), this, SLOT( copyName() ) );
	conextMenu.addAction( anIcon, QLatin1String( "copy as #RRGGBB" ), this, SLOT( copyHexRGBColor() ) );
	conextMenu.addAction( anIcon, QLatin1String( "copy as ##AARRGGBB" ), this, SLOT( copyHexARGBColor() ) );

	conextMenu.exec( QCursor::pos() );
}

void ColorChooser::copyName()
{
	QApplication::clipboard()->setText( m_currentColor );
}

void ColorChooser::copyHexRGBColor()
{
	QColor color( m_currentColor );
	QApplication::clipboard()->setText( color.name( QColor::HexRgb ) );
}

void ColorChooser::copyHexARGBColor()
{
	QColor color( m_currentColor );
	QApplication::clipboard()->setText( color.name( QColor::HexArgb ) );
}
