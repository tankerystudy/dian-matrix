/* Writer
*
* This application is part of the examples on the use of the library QSerialDevice.
*
* writer - a test console application to write data to the port
*
* Copyright (C) 2009  Denis Shienkov
*
* Contact Denis Shienkov:
*          e-mail: <scapig2@yandex.ru>
*             ICQ: 321789831
*/

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
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
    if (port->open(AbstractSerial::WriteOnly)) {

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

        //Here, the new set parameters (for example)
        qDebug() << "= New parameters =";
        qDebug() << "Device name            : " << port->deviceName();
        qDebug() << "Baud rate              : " << port->baudRate();
        qDebug() << "Data bits              : " << port->dataBits();
        qDebug() << "Parity                 : " << port->parity();
        qDebug() << "Stop bits              : " << port->stopBits();
        qDebug() << "Flow                   : " << port->flowControl();
        qDebug() << "Char timeout, msec     : " << port->charIntervalTimeout();

        QByteArray ba; //data to send
        qint64 bw = 0; //bytes really writed

        /* 5. Fifth - you can now read / write device, or further modify its settings, etc.
        */
        while (1) {
            bw = 0;
            cout << "Please enter count bytes for wtitten : ";
            cin >> bw;

            qDebug() << "Starting writting " << bw << " bytes in time : " << QTime::currentTime();

            ba.clear();
            ba.resize(bw);

            while (bw--) //filling data array
                ba[(int)bw] = bw;

            bw = port->write(ba);
            qDebug() << "Writed is : " << bw << " bytes";
        }
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
