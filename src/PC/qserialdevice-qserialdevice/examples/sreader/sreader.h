#ifndef SREADER_H
#define SREADER_H

#include <QtCore/QDebug>
#include <QtCore/QByteArray>

#include <abstractserial.h>
#include <iostream>

using namespace std;

class Reader : public QObject
{
    Q_OBJECT

public:
    Reader(QObject *parent = 0)
        : QObject(parent) {

        /*1. First - create an instance of an object.
        */
        port = new AbstractSerial();
        //1.1. Connect to ...
        connect(port, SIGNAL(readyRead()), this, SLOT(slotRead()));

        /*2. Second - set the device name.
        */

        char dn[50]; //device name
        cout << "Please enter serial device name, specific by OS, \n example: in Windows -> COMn, in GNU/Linux -> /dev/ttyXYZn: ";
        cin >> dn;
        //port->setDeviceName("COM1");
        port->setDeviceName(dn);

        /*
        ...
        Here code for set name.
        Warning: I'm not going to set the name -
        so the device will open with the default name: COM1 (Windows) or /dev/ttyS0 (Linux), etc.
        ...
        I'm too lazy to write code. :)
        */

        /* 3. Third - open the device.
        */
        if (!port->open(AbstractSerial::ReadOnly)) {
            qDebug() << "Serial device by default: " << port->deviceName() << " open fail.";
            return;
        }

        //Here, the default current parameters (for example)
        qDebug() << "= Default parameters =";
        qDebug() << "Device name            : " << port->deviceName();
        qDebug() << "Baud rate              : " << port->baudRate();
        qDebug() << "Data bits              : " << port->dataBits();
        qDebug() << "Parity                 : " << port->parity();
        qDebug() << "Stop bits              : " << port->stopBits();
        qDebug() << "Flow                   : " << port->flowControl();
        qDebug() << "Char timeout, msec     : " << port->charIntervalTimeout();

        /* 4. Fourth - now you can set the parameters. (after successfully opened port)
        */

        //Here example set baud rate 115200 bit/sec (baud)
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



        /*
            ...
            here you can set other parameters.
            ...
        */
        if (!port->setCharIntervalTimeout(50)) {
            qDebug() << "Set char interval timeout error.";
            return;
        }


        //Here, the new set parameters (for example)
        qDebug() << "= New parameters =";
        qDebug() << "Device name            : " << port->deviceName();
        qDebug() << "Baud rate              : " << port->baudRate();
        qDebug() << "Data bits              : " << port->dataBits();
        qDebug() << "Parity                 : " << port->parity();
        qDebug() << "Stop bits              : " << port->stopBits();
        qDebug() << "Flow                   : " << port->flowControl();
        qDebug() << "Char timeout, msec     : " << port->charIntervalTimeout();
    }
    ~Reader() {
        port->close();
        delete port;
        port = 0;
    }
private slots:
    void slotRead() {
        QByteArray ba = port->readAll();
        qDebug() << "Readed is : " << ba.size() << " bytes";
    }
private:
    AbstractSerial *port;
    int readBytes;
};

#endif
