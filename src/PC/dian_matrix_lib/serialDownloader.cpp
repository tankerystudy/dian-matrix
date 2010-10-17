/*
 * serialDownloader.cpp
 *  The QSerialDownloader class implementation file.
 *
 * QSerialDownloader is a class for data sending using serial serialPort.
 * 
 * Copyleft @2010, tankery.chen@gmail.com
 */
#include <QByteArray>
#include "abstractserial.h"
#include "serialDownloader.h"

QSerialDownloader::QSerialDownloader()
{
    serialPort = new AbstractSerial();
}

QSerialDownloader::~QSerialDownloader()
{
    close();

    if (serialPort)
    {
        delete serialPort;
        serialPort= 0;
    }
}

bool QSerialDownloader::initPort()
{
    if (serialPort->isOpen())
    {
        if (!serialPort->setBaudRate(AbstractSerial::BaudRate9600)) {
            return false;
        };

        if (!serialPort->setDataBits(AbstractSerial::DataBits8)) {
            return false;
        }

        if (!serialPort->setParity(AbstractSerial::ParityNone)) {
            return false;
        }

        if (!serialPort->setStopBits(AbstractSerial::StopBits1)) {
            return false;
        }

        if (!serialPort->setFlowControl(AbstractSerial::FlowControlOff)) {
            return false;
        }

    }   // if (serialPort->isOpen())
    else
        return false;

    return true;
}

bool QSerialDownloader::openDevice(const QString &dn)
{
    serialPort->setDeviceName(dn);
    if (serialPort->open(AbstractSerial::WriteOnly))
        return true;

    return false;
}

void QSerialDownloader::close()
{
    if (serialPort->isOpen())
        serialPort->close();
}

bool QSerialDownloader::sendBytes(QByteArray ba)
{
    if (ba.length() == serialPort->write(ba))
        return true;
    
    return false;
}
