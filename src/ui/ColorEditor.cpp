#include "ColorEditor.h"

#include <QtWidgets/QColorDialog>
#include <QtGui/QClipboard>

ColorEditor::ColorEditor( QWidget* parent )
: QWidget( parent )
, m_qtDialog( new QColorDialog(this) )
{
	setupUi( this );

	m_qtDialog->setOptions( QColorDialog::ShowAlphaChannel | QColorDialog::NoButtons | QColorDialog::DontUseNativeDialog );
	stackedWidget->addWidget( m_qtDialog );

	onDialogColorChanged( m_qtDialog->currentColor() );
	connect( m_qtDialog, &QColorDialog::currentColorChanged, this, &ColorEditor::onDialogColorChanged );

	connect( cbb_actions, SIGNAL( currentIndexChanged(int) ), this, SLOT( onActionSelected(int) ) );

	connect( dsb_red, SIGNAL(valueChanged(double) ), SLOT(onFloatColorChanged(double) ) );
	connect( dsb_green, SIGNAL( valueChanged( double ) ), SLOT( onFloatColorChanged( double ) ) );
	connect( dsb_blue, SIGNAL( valueChanged( double ) ), SLOT( onFloatColorChanged( double ) ) );
	connect( dsb_alpha, SIGNAL( valueChanged( double ) ), SLOT( onFloatColorChanged( double ) ) );

	connect( dsb_hueHSL, SIGNAL( valueChanged( double ) ), SLOT( onHSLChanged( double ) ) );
	connect( dsb_satHSL, SIGNAL( valueChanged( double ) ), SLOT( onHSLChanged( double ) ) );
	connect( dsb_ligHSL, SIGNAL( valueChanged( double ) ), SLOT( onHSLChanged( double ) ) );

	connect( le_htmlA, &QLineEdit::textChanged, this, &ColorEditor::onAHTMLChanged);
	setCurrentColor( QColor(0, 0, 0, 255) );
}

void ColorEditor::onDialogColorChanged( const QColor &color )
{
	dsb_red->blockSignals( true );
	dsb_green->blockSignals( true );
	dsb_blue->blockSignals( true );
	dsb_alpha->blockSignals( true );
	le_htmlA->blockSignals( true );
	dsb_hueHSL->blockSignals( true );
	dsb_satHSL->blockSignals( true );
	dsb_ligHSL->blockSignals( true );

	dsb_red->setValue( color.redF() );
	dsb_green->setValue( color.greenF() );
	dsb_blue->setValue( color.blueF() );
	dsb_alpha->setValue( color.alphaF() );
	le_htmlA->setText( color.name( QColor::HexArgb ) );
	dsb_hueHSL->setValue( color.hslHueF() * 359.0 );
	dsb_satHSL->setValue( color.hslSaturationF() );
	dsb_ligHSL->setValue( color.lightnessF() );

	dsb_red->blockSignals( false );
	dsb_green->blockSignals( false );
	dsb_blue->blockSignals( false );
	dsb_alpha->blockSignals( false );
	le_htmlA->blockSignals( false );
	dsb_hueHSL->blockSignals( false );
	dsb_satHSL->blockSignals( false );
	dsb_ligHSL->blockSignals( false );
}

void ColorEditor::onFloatColorChanged( double pValue )
{
	QColor col( m_qtDialog->currentColor() );
	QDoubleSpinBox* dsb = qobject_cast<QDoubleSpinBox*>( sender() );
	if( !dsb ) return;

	if( dsb == dsb_red )
	{
		col.setRedF( dsb->value() );
	}
	else if( dsb == dsb_green )
	{
		col.setGreenF( dsb->value() );
	}
	else if( dsb == dsb_blue )
	{
		col.setBlueF( dsb->value() );
	}
	else if( dsb == dsb_alpha )
	{
		col.setAlphaF( dsb->value() );
	}

	m_qtDialog->setCurrentColor( col );
}

void ColorEditor::onAHTMLChanged( const QString& text )
{
	QColor col;
	col.setNamedColor( text );
	if( col.isValid() )
	{
		m_qtDialog->setCurrentColor( col );
	}
}

void ColorEditor::setCurrentColor( const QColor& pColor )
{
	m_qtDialog->setCurrentColor( pColor );
}

void ColorEditor::onHSLChanged( double pValue )
{
	QColor col(m_qtDialog->currentColor());
	col.setHslF( dsb_hueHSL->value() / 359.0, dsb_satHSL->value(), dsb_ligHSL->value() );
	m_qtDialog->setCurrentColor( col );
}

void ColorEditor::onActionSelected( int pIdx )
{
	switch(pIdx)
	{
		case 0:	// copy as #RRGGBB
			QApplication::clipboard()->setText( m_qtDialog->currentColor().name( QColor::HexRgb ) );
		break;
		case 1: // copy as #AARRGGBB
			QApplication::clipboard()->setText( m_qtDialog->currentColor().name( QColor::HexArgb ) );
		break;
		case 2: // copy as Byte (r, g, b)
		{
			QColor color( m_qtDialog->currentColor() );
			int rgb[3];
			color.getRgb(rgb, rgb+1, rgb+2);
			QApplication::clipboard()->setText( QString( "%1, %2, %3" ).arg( QString::number(rgb[0]) ).arg( QString::number(rgb[1]) ).arg( QString::number(rgb[2]) ) );
		}
		break;
		case 3: // copy as Float (r, g, b)
		{
			QColor color( m_qtDialog->currentColor() );
			qreal rgb[3];
			color.getRgbF(rgb, rgb+1, rgb+2);
			QApplication::clipboard()->setText( QString( "%1f, %2f, %3f" ).arg( QString::number(rgb[0]) ).arg( QString::number(rgb[1]) ).arg( QString::number(rgb[2]) ) );
		}
		break;
	}
	
	cbb_actions->blockSignals( true );
	cbb_actions->setCurrentIndex( -1 );
	cbb_actions->blockSignals( false );
}