/* Reader
*
* This application is part of the examples on the use of the library QSerialDevice.
*
* reader - a test console application to read data from the port using the method of expectations waitForReadyRead()
*
* Copyright (C) 2009  Denis Shienkov
*
* Contact Denis Shienkov:
*          e-mail: <scapig2@yandex.ru>
*             ICQ: 321789831
*/

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <abstractserial.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    /* 1. First - create an instance of an object.
    */
    AbstractSerial *port = new AbstractSerial();

    char dn[50]; //device name
    cout << "Please enter serial device name, specific by OS, \n example: in Windows -> COMn, in GNU/Linux -> /dev/ttyXYZn: ";
    cin >> dn;

    /* 2. Second - set the device name.
    */
    port->setDeviceName(dn);

    /* 3. Third - open the device.
    */
    if (port->open(AbstractSerial::ReadOnly)) {
        qDebug() << "Serial device " << port->deviceName() << " open in " << port->openMode();

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
            goto label;
        };

        if (!port->setDataBits(AbstractSerial::DataBits8)) {
            qDebug() << "Set data bits " <<  AbstractSerial::DataBits8 << " error.";
            goto label;
        }

        if (!port->setParity(AbstractSerial::ParityNone)) {
            qDebug() << "Set parity " <<  AbstractSerial::ParityNone << " error.";
            goto label;
        }

        if (!port->setStopBits(AbstractSerial::StopBits1)) {
            qDebug() << "Set stop bits " <<  AbstractSerial::StopBits1 << " error.";
            goto label;
        }

        if (!port->setFlowControl(AbstractSerial::FlowControlOff)) {
            qDebug() << "Set flow " <<  AbstractSerial::FlowControlOff << " error.";
            goto label;
        }

        /*
            ...
            here you can set other parameters.
            ...
        */
        if (!port->setCharIntervalTimeout(50)) {
            qDebug() << "Set char timeout " << " error.";
            goto label;
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

        int rrto = 0; //timeout for ready read
        cout << "Please enter wait timeout for ready read, msec : ";
        cin >> rrto;

        int len = 0; //len data for read
        cout << "Please enter len data for read, bytes : ";
        cin >> len;

        qDebug() << "Enter is " << rrto << " msecs, " << len << " bytes";
        qDebug() << "Starting waiting ready read in time : " << QTime::currentTime();

        QByteArray ba; //received data

        /* 5. Fifth - you can now read / write device, or further modify its settings, etc.
        */
        while (1) {
            if (port->waitForReadyRead(rrto)) {
                ba.clear();
                ba = port->read(len);
                qDebug() << "Readed is : " << ba.size() << " bytes";
            }
            else {
                qDebug() << "Timeout read data in time : " << QTime::currentTime();
            }
        }//while
    }
    else {
        qDebug() << "Error opened serial device " << port->deviceName();
    }

label:

    port->close();
    qDebug() << "Serial device " << port->deviceName() << " is closed";
    delete port;
    port = 0;
    return app.exec();
}
