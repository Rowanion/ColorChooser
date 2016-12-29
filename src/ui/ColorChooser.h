#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ColorChooser.h"

class ColorChooser : public QMainWindow, public Ui_ColorChooser
{
	Q_OBJECT
	
public:
	ColorChooser(QWidget* parent = nullptr);
	~ColorChooser();

private slots:
	void onCustomContextMenu( const QPoint & point );
	void copyName();
	void copyHexRGBColor();
	void copyHexARGBColor();
	void copyByteRGBColor();
	void copyFloatRGBColor();

private:
	class QStandardItemModel* m_model{ nullptr };
	QString m_currentColor;
	class ColorEditor* m_colorEditor = nullptr;
};
