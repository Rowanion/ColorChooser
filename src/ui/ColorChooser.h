#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ColorChooser.h"

class ColorChooser : public QMainWindow, public Ui_ColorChooser
{
	Q_OBJECT
	
public:
	ColorChooser(QWidget* parent = nullptr);
	~ColorChooser();
};
