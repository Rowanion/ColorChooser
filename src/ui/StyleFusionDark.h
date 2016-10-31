#pragma once

#include <QtCore/QLatin1String>
#include <QtGui/QPalette>
#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>


namespace ui_style{;

void setStyle()
{
	qApp->setStyle( QStyleFactory::create( QLatin1String( "Fusion" ) ) );

	QPalette darkPalette;
	darkPalette.setColor( QPalette::Window, QColor( 53, 53, 53 ) );
	darkPalette.setColor( QPalette::WindowText, Qt::white );
	darkPalette.setColor( QPalette::Base, QColor( 25, 25, 25 ) );
	darkPalette.setColor( QPalette::AlternateBase, QColor( 53, 53, 53 ) );
	darkPalette.setColor( QPalette::ToolTipBase, Qt::white );
	darkPalette.setColor( QPalette::ToolTipText, Qt::white );
	darkPalette.setColor( QPalette::Text, Qt::white );
	darkPalette.setColor( QPalette::Button, QColor( 53, 53, 53 ) );
	darkPalette.setColor( QPalette::ButtonText, Qt::white );
	darkPalette.setColor( QPalette::BrightText, Qt::red );
	darkPalette.setColor( QPalette::Link, QColor( 42, 130, 218 ) );

	darkPalette.setColor( QPalette::Highlight, QColor( 42, 130, 218 ) );
	darkPalette.setColor( QPalette::HighlightedText, Qt::black );

	darkPalette.setColor( QPalette::Disabled, QPalette::Text, Qt::darkGray );
	darkPalette.setColor( QPalette::Disabled, QPalette::ButtonText, Qt::darkGray );
	darkPalette.setColor( QPalette::Disabled, QPalette::Foreground, Qt::darkGray );

	qApp->setPalette( darkPalette );

	qApp->setStyleSheet( QLatin1String( "QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }" ) );

}

} //namespace ui_style

