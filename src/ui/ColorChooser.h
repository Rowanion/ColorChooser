#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCore/QTimer>
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
	void on_actionPaste_From_Byte_RGB_triggered();
	void onUpdateClipboardContents();
	void on_actionByteToFloat_triggered();
	void on_actionFloatToByte_triggered();
	void on_actionByteToHex_triggered();
	void on_actionHexToByte_triggered();
	void on_actionFloatToHex_triggered();
	void on_actionHexToFloat_triggered();

private:
	class QStandardItemModel* m_model{ nullptr };
	QString m_currentColor;
	QTimer m_clipboardTimer;
	class QLabel* m_clipboardContent = nullptr;
	QColor floatStringToColor( const QString& pString );
	QColor byteStringToColor( const QString& pString );
	const QString colorToFloatString( const QColor& pColor );
	const QString colorToByteString( const QColor& pColor );
};
