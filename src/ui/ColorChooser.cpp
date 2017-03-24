#include "ColorChooser.h"

#include <QtGui/QStandardItemModel>
#include <QtGui/QStandardItem>
#include <QtGui/QBrush>
#include <QtGui/QClipboard>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QApplication>
#include <QtGui/QPainter>
#include <QtWidgets/QColorDialog>
#include "ColorEditor.h"
#include <QtWidgets/QCheckBox>

ColorChooser::ColorChooser(QWidget* parent)
: QMainWindow(parent)
{

	// force dot as comma for QDoubleSpinBox and float conversion
	setLocale( QLocale(QLatin1String("C")) );

	setupUi(this);

	//TODO: implement
	l_filter->setVisible( false );
	le_filter->setVisible( false );

	w_colorEditor->setVisible( false );
	line->setVisible( false );

	// setup the table
	m_model = new QStandardItemModel(this);
	tableView->setModel( m_model );
	tableView->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( tableView, &QTableView::customContextMenuRequested, this, &ColorChooser::onCustomContextMenu );
	QHeaderView* verticalHeader = tableView->verticalHeader();
	verticalHeader->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( verticalHeader, &QHeaderView::customContextMenuRequested, this, &ColorChooser::onCustomContextMenu );
	connect( cb_showEditor, &QCheckBox::toggled, w_colorEditor, &ColorEditor::setVisible );
	connect( cb_showEditor, &QCheckBox::toggled, line, &QFrame::setVisible );
	connect( tableView, SIGNAL( clicked( const QModelIndex & ) ), this, SLOT( onTableClicked( const QModelIndex & ) ) );
	
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
	QMenu contextMenu( tableView );
	contextMenu.addAction( anIcon, QLatin1String( "copy color name" ), this, SLOT( copyName() ) );
	contextMenu.addAction( anIcon, QLatin1String( "copy as #RRGGBB" ), this, SLOT( copyHexRGBColor() ) );
	contextMenu.addAction( anIcon, QLatin1String( "copy as #AARRGGBB" ), this, SLOT( copyHexARGBColor() ) );
	contextMenu.addAction( anIcon, QLatin1String( "copy as Byte (r, g, b)" ), this, SLOT( copyByteRGBColor() ) );
	contextMenu.addAction( anIcon, QLatin1String( "copy as Float (r, g, b)" ), this, SLOT( copyFloatRGBColor() ) );

	contextMenu.exec( QCursor::pos() );
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

void ColorChooser::copyByteRGBColor()
{
	QColor color( m_currentColor );
	int rgb[3];
	color.getRgb(rgb, rgb+1, rgb+2);
	QApplication::clipboard()->setText( QString( "%1, %2, %3" ).arg( QString::number(rgb[0]) ).arg( QString::number(rgb[1]) ).arg( QString::number(rgb[2]) ) );
}

void ColorChooser::copyFloatRGBColor()
{
	QColor color( m_currentColor );
	qreal rgb[3];
	color.getRgbF(rgb, rgb+1, rgb+2);
	QApplication::clipboard()->setText( QString( "%1f, %2f, %3f" ).arg( QString::number(rgb[0]) ).arg( QString::number(rgb[1]) ).arg( QString::number(rgb[2]) ) );
}

void ColorChooser::onTableClicked( const QModelIndex& pIndex )
{
	m_currentColor = m_model->data( m_model->index( pIndex.row(), 0 ), Qt::UserRole ).toString();
	w_colorEditor->setCurrentColor( QColor( m_currentColor ) );
}