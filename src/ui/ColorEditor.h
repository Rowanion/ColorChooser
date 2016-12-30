#pragma once

#include <QtWidgets/QWidget>
#include "ui_ColorEditor.h"

constexpr float g_fraction = 1.0f / 255.0f;

class ColorEditor : public QWidget, public Ui_ColorEditor
{
	Q_OBJECT
public:
	ColorEditor( QWidget* parent = nullptr );
	virtual ~ColorEditor() = default;

private slots:
	void onDialogColorChanged( const QColor &color );
	void onFloatColorChanged( double pValue );
	void onAHTMLChanged( const QString& text );

private:
	class QColorDialog* m_qtDialog = nullptr;
};
