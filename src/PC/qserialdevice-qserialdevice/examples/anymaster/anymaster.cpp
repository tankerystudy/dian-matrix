
#include <QtCore/QTimer>
#include <QtCore/QDebug>

#include <abstractserial.h>
#include "anymaster.h"

#include <iostream>

using namespace std;


AnyMaster::AnyMaster(QObject *parent)
    : QObject(parent), responseTimeout(500), m_queryLen(MinQueryLen)
{
    port = new AbstractSerial(this);

    char dn[50]; //device name
    cout << "Please enter serial device name, specific by OS, \n example: in Windows -> COMn, in GNU/Linux -> /dev/ttyXYZn: ";
    cin >> dn;

    port->setDeviceName(dn);

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

    timer = new QTimer(this);
    timer->setInterval(100); //polling interval
    connect(timer, SIGNAL(timeout()), this, SLOT(transaction()));
}

AnyMaster::~AnyMaster()
{
    start(false);
    port->close();
}

void AnyMaster::start(bool enable)
{
    if (enable)
        timer->start();
    else
        timer->stop();
}

void AnyMaster::transaction()
{
    start(false);

    QByteArray data = generateRequest();

    quint64 r = port->write(data);

    if (r == quint64(data.size())) {
        qDebug() << "Writed: " << r << " bytes";
        if (!port->waitForReadyRead(responseTimeout)) {
            qDebug() << "Response timeout.";
        }
        else {
            data = port->readAll();
            qDebug() << "Readed: " << data.size() << " bytes";
        }
    }
    else qDebug() << "Bytes writed small: " << r;

    start(true);
}

QByteArray AnyMaster::generateRequest()
{
    QByteArray request;

    if (m_queryLen == AnyMaster::MaxQueryLen)
        m_queryLen = MinQueryLen;

    //create example request ( filling random values )
    for (int i = 0; i < m_queryLen; i++)
        request.append(qrand());

    m_queryLen += MinQueryLen;
    return request;
}








