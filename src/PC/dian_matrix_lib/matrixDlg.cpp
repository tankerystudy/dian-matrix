#include <QtGui>
#include "matrixDlg.h"
#include "DataManager.h"
#include "LEDScrn.h"
#include "matrixCreator.h"
#include "serialDownloader.h"

QMatrixDlg::QMatrixDlg(QWidget *parent/*= 0*/, Qt::WFlags flags/* = 0*/)
    : QDialog(parent, flags)
    , screen(0)
    , LEDmatrixData(0)
    , serialName("")
{
    setupUi(this);

    // make the link of author informations available.
    informations->setOpenExternalLinks(true);

    LEDmatrixData = new DataManager(8, 16);

    // add LED screen panel to the dialog
    screen = new QLEDScrn(LEDmatrixData, this);
    QVBoxLayout *layout= new QVBoxLayout(matrixPanel);
    layout->addWidget(screen);

    // add serial device name to the combo box.
    QString commonName=
#ifdef _WINDOWS
        "COM";
#else
        "/dev/ttyS";
#endif

    for (int i= 0; i < 4; i++)
    {
        QString fullName= commonName +
#ifdef _WINDOWS
            QString::number(i+1);
#else
            QString::number(i);
#endif
        serialDevice->addItem(fullName);
    }
    changeSerial();     // update the serial port device name

    QObject::connect(comitBtn, SIGNAL(clicked()), this, SLOT(comitContents()));
    QObject::connect(downloadBtn, SIGNAL(clicked()), this, SLOT(startDownload()));
    QObject::connect(serialDevice, SIGNAL(activated(int)), this, SLOT(changeSerial()));
}

QMatrixDlg::~QMatrixDlg()
{
    if (screen)
        delete screen;
    if (LEDmatrixData)
        delete LEDmatrixData;

    screen= 0;
    LEDmatrixData= 0;   // clear to make memory safe.
}

void QMatrixDlg::comitContents()
{
    QString contStr= contents->text();
    if (contStr.length() > 4)
    {
        QMessageBox::information(this, tr("Notes"), tr("Please input words not longer than 4..."));
        return;
    }

    QString libPath= QDir::toNativeSeparators(
        QCoreApplication::applicationDirPath() + tr("/HZK16"));

    if (!QFile::exists(libPath))
    {
        QMessageBox::critical(this, tr("Error!"),
            tr("I'm sorry we got an problem,\n") +
            tr("please make sure that if there is an \"HZK16\" file at application directory..."),
            QMessageBox::Ok);
        return;
    }

    // update the LED matrix data by contents
    MatrixCreator *newMatrix= new MatrixCreator(libPath.toStdString());
    QTextCodec *chinese=QTextCodec::codecForName("GB18030");
    if (!newMatrix->makeStrMatrix16(chinese->fromUnicode(contStr).data(), contStr.length(), LEDmatrixData))
    {
        QMessageBox::critical(this, tr("Error!"),
            tr("I'm sorry we got an internal problem,\n") +
            tr("please contact the developers..."),
            QMessageBox::Ok);
    }
    delete newMatrix;

    screen->update();
}

void QMatrixDlg::startDownload()
{
    // make the contents for sending
    QByteArray ba;

    for (int y= 0; y < 16; y++)
        for (int x= 0; x < 8; x++)
        {
            ba.push_back(LEDmatrixData->readChar(x, y));
        }
    QSerialDownloader *downloader= new QSerialDownloader();

    if (downloader->openDevice(serialName))
    {
        if (!downloader->sendBytes(ba))
        {
            downloader->close();
            QMessageBox::critical(this, tr("Error!"),
                tr("I'm sorry the sending of data failed,\n") +
                tr("please contect the developer..."),
                QMessageBox::Ok);
        }
        else
        {
            downloader->close();
            QMessageBox::information(this, tr("Congratulations!"),
                tr("Congratulations!,\n") +
                tr("The data sending success..."),
                QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error!"),
            tr("I'm sorry we got an problem,\n") +
            tr("please make sure that the \'") + serialName + tr("\' port is available..."),
            QMessageBox::Ok);
    }

    if (downloader)
    {
        delete downloader;
        downloader = 0;
    }
}

void QMatrixDlg::changeSerial()
{
    serialName= serialDevice->itemText(
        serialDevice->currentIndex());
}
