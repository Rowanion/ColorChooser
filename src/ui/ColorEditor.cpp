#include "ColorEditor.h"

#include <QtWidgets/QColorDialog>

ColorEditor::ColorEditor( QWidget* parent )
: QWidget( parent )
, m_qtDialog( new QColorDialog(this) )
{
	setupUi( this );

	m_qtDialog->setOptions( QColorDialog::ShowAlphaChannel | QColorDialog::NoButtons | QColorDialog::DontUseNativeDialog );
	stackedWidget->addWidget( m_qtDialog );

	onDialogColorChanged( m_qtDialog->currentColor() );
	connect( m_qtDialog, &QColorDialog::currentColorChanged, this, &ColorEditor::onDialogColorChanged );

	connect( dsb_red, SIGNAL(valueChanged(double) ), SLOT(onFloatColorChanged(double) ) );
	connect( dsb_green, SIGNAL( valueChanged( double ) ), SLOT( onFloatColorChanged( double ) ) );
	connect( dsb_blue, SIGNAL( valueChanged( double ) ), SLOT( onFloatColorChanged( double ) ) );
	connect( dsb_alpha, SIGNAL( valueChanged( double ) ), SLOT( onFloatColorChanged( double ) ) );

	connect( le_htmlA, &QLineEdit::textChanged, this, &ColorEditor::onAHTMLChanged);
}

void ColorEditor::onDialogColorChanged( const QColor &color )
{
	dsb_red->blockSignals( true );
	dsb_green->blockSignals( true );
	dsb_blue->blockSignals( true );
	dsb_alpha->blockSignals( true );
	le_htmlA->blockSignals( true );

	dsb_red->setValue( color.redF() );
	dsb_green->setValue( color.greenF() );
	dsb_blue->setValue( color.blueF() );
	dsb_alpha->setValue( color.alphaF() );
	le_htmlA->setText( color.name( QColor::HexArgb ) );

	dsb_red->blockSignals( false );
	dsb_green->blockSignals( false );
	dsb_blue->blockSignals( false );
	dsb_alpha->blockSignals( false );
	le_htmlA->blockSignals( false );
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
