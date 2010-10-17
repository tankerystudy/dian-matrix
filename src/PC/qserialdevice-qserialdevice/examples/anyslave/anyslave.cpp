
#include <QtCore/QTimer>
#include <QtCore/QDebug>

#include <abstractserial.h>
#include "anyslave.h"

#include <iostream>

using namespace std;


AnySlave::AnySlave(QObject *parent)
    : QObject(parent)
{
    port = new AbstractSerial(this);

    char dn[50]; //device name
    cout << "Please enter serial device name, specific by OS, \n example: in Windows -> COMn, in GNU/Linux -> /dev/ttyXYZn: ";
    cin >> dn;

    port->setDeviceName(dn);

    connect(port, SIGNAL(readyRead()), this, SLOT(transaction()));

    if ( !port->open(AbstractSerial::ReadWrite) ) {
        qDebug() << "Serial device by default: " << port->deviceName() << " open fail.";
        return;
    }
    if (!port->setBaudRate(AbstractSerial::BaudRate115200)) {
        qDebug() << "Set baud rate " <<  AbstractSerial::BaudRate115200 << " error.";
        return;
    };
    if (!port->setDataBits(AbstractSerial::DataBits8)) {
        qDebug() << "Set data bits " <<  AbstractSerial::DataBits8 << " error.";
        return;
    }
    if (!port->setParity(AbstractSerial::ParityNone)) {
        qDebug() << "Set parity " <<  AbstractSerial::ParityNone << " error.";
        return;
    }
    if (!port->setStopBits(AbstractSerial::StopBits1)) {
        qDebug() << "Set stop bits " <<  AbstractSerial::StopBits1 << " error.";
        return;
    }
    if (!port->setFlowControl(AbstractSerial::FlowControlOff)) {
        qDebug() << "Set flow " <<  AbstractSerial::FlowControlOff << " error.";
        return;
    }
    if (!port->setCharIntervalTimeout(50)) {
        qDebug() << "Set char interval timeout error.";
        return;
    }

    qDebug() << "= New parameters =";
    qDebug() << "Device name            : " << port->deviceName();
    qDebug() << "Baud rate              : " << port->baudRate();
    qDebug() << "Data bits              : " << port->dataBits();
    qDebug() << "Parity                 : " << port->parity();
    qDebug() << "Stop bits              : " << port->stopBits();
    qDebug() << "Flow                   : " << port->flowControl();
    qDebug() << "Char timeout, msec     : " << port->charIntervalTimeout();
}

AnySlave::~AnySlave()
{
    port->close();
}

void AnySlave::transaction()
{
    QByteArray data = port->readAll();
    qDebug() << "Readed: " << data.size() << " bytes";
    quint64 len = port->write(data);

    if (len == quint64(data.size())) {
        qDebug() << "Writed: " << len << " bytes";
    }
    else qDebug() << "Bytes writed small: " << len;
}

