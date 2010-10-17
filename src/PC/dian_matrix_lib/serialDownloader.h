/*
 * serialDownloader.h
 *  The QSerialDownloader class definition file.
 *
 * QSerialDownloader is a class for data sending using serial port.
 * 
 * Copyleft @2010, tankery.chen@gmail.com
 */
#ifndef SERIAL_DOWNLOADER_H
#define SERIAL_DOWNLOADER_H

class AbstractSerial;
class QByteArray;

class QSerialDownloader
{
private:
    AbstractSerial *serialPort;

    bool initPort(void);

public:
    QSerialDownloader();
    ~QSerialDownloader();

    bool openDevice(const QString &dn);
    bool sendBytes(QByteArray ba);
    void close(void);
};  // class QSerialDownloader

#endif // #ifndef SERIAL_DOWNLOADER_H

