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
	QString red(QString::number(rgb[0]));
	QString green(QString::number(rgb[1]));
	QString blue(QString::number(rgb[2]));
	// add trailing 0 decimals
	if(floor(rgb[0]) == ceil(rgb[0])) red.append(QLatin1String(".0"));
	if(floor(rgb[1]) == ceil(rgb[1])) green.append(QLatin1String(".0"));
	if(floor(rgb[2]) == ceil(rgb[2])) blue.append(QLatin1String(".0"));
	QApplication::clipboard()->setText( QString( "%1f, %2f, %3f" ).arg( red ).arg( green ).arg( blue ) );
}

void ColorChooser::onTableClicked( const QModelIndex& pIndex )
{
	m_currentColor = m_model->data( m_model->index( pIndex.row(), 0 ), Qt::UserRole ).toString();
	w_colorEditor->setCurrentColor( QColor( m_currentColor ) );
}

void ColorChooser::on_actionCopy_As_Hex_RGB_triggered()
{
	const QColor color = w_colorEditor->getCurrentColor();
	QApplication::clipboard()->setText( color.name( QColor::HexRgb ) );
}

void ColorChooser::on_actionCopy_As_Hex_ARGB_triggered()
{
	const QColor color = w_colorEditor->getCurrentColor();
	QApplication::clipboard()->setText( color.name( QColor::HexArgb ) );
}

void ColorChooser::on_actionCopy_As_Byte_RGB_triggered()
{
	const QColor color = w_colorEditor->getCurrentColor();
	int rgb[3];
	color.getRgb(rgb, rgb+1, rgb+2);
	QApplication::clipboard()->setText( QString( "%1, %2, %3" ).arg( QString::number(rgb[0]) ).arg( QString::number(rgb[1]) ).arg( QString::number(rgb[2]) ) );
}

void ColorChooser::on_actionCopy_As_Float_RGB_triggered()
{
	const QColor color = w_colorEditor->getCurrentColor();
	qreal rgb[3];
	color.getRgbF(rgb, rgb+1, rgb+2);
	QString red(QString::number(rgb[0]));
	QString green(QString::number(rgb[1]));
	QString blue(QString::number(rgb[2]));
	// add trailing 0 decimals
	if(floor(rgb[0]) == ceil(rgb[0])) red.append(QLatin1String(".0"));
	if(floor(rgb[1]) == ceil(rgb[1])) green.append(QLatin1String(".0"));
	if(floor(rgb[2]) == ceil(rgb[2])) blue.append(QLatin1String(".0"));
	QApplication::clipboard()->setText( QString( "%1f, %2f, %3f" ).arg( red ).arg( green ).arg( blue ) );
}

void ColorChooser::on_actionPaste_From_Float_RGB_triggered()
{
	//TODO:
	QRegExp regex( "\\(?\\s*([0-9\\.]+)[fd]?[\\s,]+([0-9\\.]+)[fd]?[\\s,]+([0-9\\.]+)[fd]?[\\s,]*([0-9\\.]+)?[fd]?\\s*.*\\)?" );
	if( regex.indexIn( QApplication::clipboard()->text() ) != -1 )
	{
		float rgba[4] = {0.0f, 0.0f, 0.0f, 1.0f};
		for( int i=1; i<regex.captureCount(); ++i )
		{
			rgba[i-1] = regex.cap(i).toFloat();
		}
		QColor col;
		col.setRgbF( rgba[0], rgba[1], rgba[2], rgba[3] );
		w_colorEditor->setCurrentColor( col );
	}
}
