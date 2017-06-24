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
	void onTableClicked( const QModelIndex& pIndex );
	void on_actionCopy_As_Hex_RGB_triggered();
	void on_actionCopy_As_Hex_ARGB_triggered();
	void on_actionCopy_As_Byte_RGB_triggered();
	void on_actionCopy_As_Float_RGB_triggered();
	void on_actionPaste_From_Float_RGB_triggered();

private:
	class QStandardItemModel* m_model{ nullptr };
	QString m_currentColor;
};
