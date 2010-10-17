#include <QtGui>

#include "matrixDlg.h"

int Qtmain(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMatrixDlg *dlg= new QMatrixDlg();
    dlg->show();

    return a.exec();
}
