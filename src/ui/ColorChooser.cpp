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
#include <QtCore/QString>
#include <QtCore/QStringBuilder>

ColorChooser::ColorChooser(QWidget* parent)
   : QMainWindow(parent)
{

   // force dot as comma for QDoubleSpinBox and float conversion
   setLocale(QLocale(QLatin1String("C")));

   setupUi(this);

   //TODO: implement
   l_filter->setVisible(false);
   le_filter->setVisible(false);

   w_colorEditor->setVisible(false);
   line->setVisible(false);

   m_clipboardContent = new QLabel();
   statusbar->addWidget(m_clipboardContent);
   connect(&m_clipboardTimer, &QTimer::timeout, this, &ColorChooser::onUpdateClipboardContents);
   m_clipboardTimer.setInterval(1000);
   m_clipboardTimer.setSingleShot(false);
   m_clipboardTimer.start();
   onUpdateClipboardContents();

   // setup the table
   m_model = new QStandardItemModel(this);
   tableView->setModel(m_model);
   tableView->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(tableView, &QTableView::customContextMenuRequested, this, &ColorChooser::onCustomContextMenu);
   QHeaderView* verticalHeader = tableView->verticalHeader();
   verticalHeader->setContextMenuPolicy(Qt::CustomContextMenu);
   connect(verticalHeader, &QHeaderView::customContextMenuRequested, this, &ColorChooser::onCustomContextMenu);
   connect(cb_showEditor, &QCheckBox::toggled, w_colorEditor, &ColorEditor::setVisible);
   connect(cb_showEditor, &QCheckBox::toggled, line, &QFrame::setVisible);
   connect(tableView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onTableClicked(const QModelIndex&)));

   QStringList names(QColor::colorNames());
   m_model->setVerticalHeaderLabels(names);

   // fill the table
   QStandardItem* item{ nullptr };
   int i = 0;
   foreach(const QString & name, names)
   {
      const QColor theColor(name);
      item = new QStandardItem(QLatin1String{ " " });
      item->setData(name, Qt::UserRole);
      item->setBackground(QBrush(QColor(theColor)));
      m_model->setItem(i++, item);
   }

   tableView->resizeColumnsToContents();
   tableView->resizeRowsToContents();
}

ColorChooser::~ColorChooser()
{

}

void ColorChooser::onCustomContextMenu(const QPoint& point)
{
   QModelIndex index = tableView->indexAt(point);
   if (!index.isValid()) return;

   // get data
   m_currentColor = m_model->data(index, Qt::UserRole).toString();

   QPixmap pixmap(100, 100);
   pixmap.fill(QColor(m_currentColor));
   QIcon anIcon(pixmap);

   //set context menu
   QMenu contextMenu(tableView);
   contextMenu.addAction(anIcon, QLatin1String("copy color name"), this, SLOT(copyName()));
   contextMenu.addAction(anIcon, QLatin1String("copy as #RRGGBB"), this, SLOT(copyHexRGBColor()));
   contextMenu.addAction(anIcon, QLatin1String("copy as #AARRGGBB"), this, SLOT(copyHexARGBColor()));
   contextMenu.addAction(anIcon, QLatin1String("copy as Byte (r, g, b)"), this, SLOT(copyByteRGBColor()));
   contextMenu.addAction(anIcon, QLatin1String("copy as Float (r, g, b)"), this, SLOT(copyFloatRGBColor()));

   contextMenu.exec(QCursor::pos());
}

void ColorChooser::copyName()
{
   QApplication::clipboard()->setText(m_currentColor);
}

void ColorChooser::copyHexRGBColor()
{
   QColor color(m_currentColor);
   QApplication::clipboard()->setText(color.name(QColor::HexRgb));
}

void ColorChooser::copyHexARGBColor()
{
   QColor color(m_currentColor);
   QApplication::clipboard()->setText(color.name(QColor::HexArgb));
}

void ColorChooser::copyByteRGBColor()
{
   QApplication::clipboard()->setText(colorToByteString(m_currentColor));
}

void ColorChooser::copyFloatRGBColor()
{
   QApplication::clipboard()->setText(colorToFloatString(m_currentColor));
}

void ColorChooser::onTableClicked(const QModelIndex& pIndex)
{
   m_currentColor = m_model->data(m_model->index(pIndex.row(), 0), Qt::UserRole).toString();
   w_colorEditor->setCurrentColor(QColor(m_currentColor));
}

void ColorChooser::on_actionCopy_As_Hex_RGB_triggered()
{
   const QColor color = w_colorEditor->getCurrentColor();
   QApplication::clipboard()->setText(color.name(QColor::HexRgb));
}

void ColorChooser::on_actionCopy_As_Hex_ARGB_triggered()
{
   const QColor color = w_colorEditor->getCurrentColor();
   QApplication::clipboard()->setText(color.name(QColor::HexArgb));
}

void ColorChooser::on_actionCopy_As_Byte_RGB_triggered()
{
   QApplication::clipboard()->setText(colorToByteString(w_colorEditor->getCurrentColor()));
}

void ColorChooser::on_actionCopy_As_Float_RGB_triggered()
{
   QApplication::clipboard()->setText(colorToFloatString(w_colorEditor->getCurrentColor()));
}

void ColorChooser::on_actionCopy_As_UE4_Float_triggered()
{
   QApplication::clipboard()->setText(colorToUE4FloatString(w_colorEditor->getCurrentColor()));
}

void ColorChooser::on_actionPaste_From_Float_RGB_triggered()
{
   w_colorEditor->setCurrentColor(floatStringToColor(QApplication::clipboard()->text()));
}

void ColorChooser::on_actionPaste_From_Byte_RGB_triggered()
{
   w_colorEditor->setCurrentColor(byteStringToColor(QApplication::clipboard()->text()));
}

void ColorChooser::on_actionPaste_From_Color_Name_triggered()
{
   w_colorEditor->setCurrentColor(std::move(QApplication::clipboard()->text()));
}

void ColorChooser::on_actionPaste_From_UE4_Float_triggered()
{
   w_colorEditor->setCurrentColor(ue4StringToColor(QApplication::clipboard()->text()));
}

void ColorChooser::onUpdateClipboardContents()
{
   QString cb = QApplication::clipboard()->text().remove(QLatin1String("\n"));
   if (cb.size() > 50)
   {
      cb = cb.left(50) % QLatin1String("...");
   }
   m_clipboardContent->setText(QString(QLatin1String("@Clipboard: \"%1\"")).arg(cb));
}

void ColorChooser::on_actionByteToFloat_triggered()
{
   QColor color = byteStringToColor(QApplication::clipboard()->text());
   QApplication::clipboard()->setText(colorToFloatString(color));
}

void ColorChooser::on_actionFloatToByte_triggered()
{
   QColor color = floatStringToColor(QApplication::clipboard()->text());
   QApplication::clipboard()->setText(colorToByteString(color));
}

void ColorChooser::on_actionByteToHex_triggered()
{
   QColor color = byteStringToColor(QApplication::clipboard()->text());
   QApplication::clipboard()->setText(color.name(QColor::HexRgb));
}

void ColorChooser::on_actionFloatToHex_triggered()
{
   QColor color = floatStringToColor(QApplication::clipboard()->text());
   QApplication::clipboard()->setText(color.name(QColor::HexRgb));
}

void ColorChooser::on_actionHexToByte_triggered()
{
   QColor color;
   color.setNamedColor(QApplication::clipboard()->text());
   QApplication::clipboard()->setText(colorToByteString(color));
}

void ColorChooser::on_actionHexToFloat_triggered()
{
   QColor color;
   color.setNamedColor(QApplication::clipboard()->text());
   QApplication::clipboard()->setText(colorToFloatString(color));
}

QColor ColorChooser::floatStringToColor(const QString& pString)
{
   //TODO:
   QColor color;
   QRegExp regex("\\(?\\s*([0-9\\.]+)[fd]?[\\s,]+([0-9\\.]+)[fd]?[\\s,]+([0-9\\.]+)[fd]?[\\s,]*([0-9\\.]+)?[fd]?\\s*.*\\)?");
   if (regex.indexIn(pString) != -1)
   {
      float rgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
      for (int i = 1; i <= regex.captureCount(); ++i)
      {
         rgba[i - 1] = regex.cap(i).toFloat();
      }
      color.setRgbF(rgba[0], rgba[1], rgba[2], rgba[3]);
   }
   return color;
}

QColor ColorChooser::ue4StringToColor(const QString& pString)
{
   // (SpecifiedColor=(R=1.000000,G=1.000000,B=1.000000,A=1.000000),ColorUseRule=UseColor_Specified)
   QColor color;
   QRegExp regex("R=([0-9\\.]+),G=([0-9\\.]+),B=([0-9\\.]+),A=([0-9\\.]+)\\)");
   if (regex.indexIn(pString) != -1)
   {
      float rgba[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
      for (int i = 1; i <= regex.captureCount(); ++i)
      {
         rgba[i - 1] = regex.cap(i).toFloat();
      }
      color.setRgbF(rgba[0], rgba[1], rgba[2], rgba[3]);
   }
   return color;
}

QColor ColorChooser::byteStringToColor(const QString& pString)
{
   QColor color;
   QRegExp regex("\\(?\\s*([0-9]+)[\\s,]+([0-9]+)[\\s,]+([0-9]+)[\\s,]*([0-9]+)?\\s*.*\\)?");
   if (regex.indexIn(pString) != -1)
   {
      unsigned char rgba[4] = { 0, 0, 0, 255 };
      for (int i = 1; i <= regex.captureCount(); ++i)
      {
         rgba[i - 1] = regex.cap(i).toUInt();
      }
      color.setRgb(rgba[0], rgba[1], rgba[2], rgba[3]);
   }
   return color;
}

const QString ColorChooser::colorToFloatString(const QColor& pColor)
{
   qreal rgb[3];
   pColor.getRgbF(rgb, rgb + 1, rgb + 2);
   QString red(QString::number(rgb[0]));
   QString green(QString::number(rgb[1]));
   QString blue(QString::number(rgb[2]));
   // add trailing 0 decimals
   if (floor(rgb[0]) == ceil(rgb[0])) red.append(QLatin1String(".0"));
   if (floor(rgb[1]) == ceil(rgb[1])) green.append(QLatin1String(".0"));
   if (floor(rgb[2]) == ceil(rgb[2])) blue.append(QLatin1String(".0"));
   return QString("%1f, %2f, %3f").arg(red).arg(green).arg(blue);
}

const QString ColorChooser::colorToUE4FloatString(const QColor& pColor)
{
   qreal rgb[4];
   pColor.getRgbF(rgb, rgb + 1, rgb + 2, rgb + 3);
   QString red(QString::number(rgb[0]));
   QString green(QString::number(rgb[1]));
   QString blue(QString::number(rgb[2]));
   QString alpha(QString::number(rgb[3]));
   // add trailing 0 decimals
   if (floor(rgb[0]) == ceil(rgb[0])) red.append(QLatin1String(".0"));
   if (floor(rgb[1]) == ceil(rgb[1])) green.append(QLatin1String(".0"));
   if (floor(rgb[2]) == ceil(rgb[2])) blue.append(QLatin1String(".0"));
   if (floor(rgb[3]) == ceil(rgb[3])) alpha.append(QLatin1String(".0"));
   return QString("(SpecifiedColor=(R=%1,G=%2,B=%3,A=%4),ColorUseRule=UseColor_Specified)")
      .arg(red)
      .arg(green)
      .arg(blue)
      .arg(alpha);
}

const QString ColorChooser::colorToByteString(const QColor& pColor)
{
   int rgb[3];
   pColor.getRgb(rgb, rgb + 1, rgb + 2);
   return QString("%1, %2, %3").arg(QString::number(rgb[0])).arg(QString::number(rgb[1])).arg(QString::number(rgb[2]));
}
