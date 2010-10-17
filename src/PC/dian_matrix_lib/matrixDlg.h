#ifndef MATRIX_DLG_H
#define MATRIX_DLG_H

#include <QtGui/QDialog>
#include "ui_MatrixDlg.h"

class QLEDScrn;
class DataManager;

class QMatrixDlg : public QDialog, public Ui::matrixDlg
{
    Q_OBJECT

private:
    QLEDScrn *screen;           // the 64x16 LED matrix screen.
    DataManager *LEDmatrixData; // store the LED matrix data
    QString serialName;         // store the serial port device name.

public:
    QMatrixDlg(QWidget *parent= 0, Qt::WFlags flags = 0);
    ~QMatrixDlg();

private slots:
    void comitContents();
    void startDownload();
    void changeSerial();
};  // class QMatrixDlg

#endif  // #ifndef MATRIX_DLG_H
