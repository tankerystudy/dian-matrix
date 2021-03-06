/*
* This file is part of QSerialDevice, an open-source cross-platform library
* Copyright (C) 2009  Denis Shienkov
*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*
* Contact Denis Shienkov:
*          e-mail: <scapig2@yandex.ru>
*             ICQ: 321789831
*/

#ifndef ABSTRACTSERIAL_H
#define ABSTRACTSERIAL_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QDateTime>

#ifndef QT_NO_DEBUG_STREAM
#include <QtCore/QDebug>
#endif

#include "../qserialdevice_global.h"

#ifndef ABSTRACTSERIAL_READ_CHUNK_SIZE
#define ABSTRACTSERIAL_READ_CHUNK_SIZE Q_INT64_C(4096)
#endif


class AbstractSerialPrivate;
#if defined(QSERIALDEVICE_EXPORT)
class QSERIALDEVICE_EXPORT AbstractSerial : public QObject
#else
class AbstractSerial : public QObject
#endif
{
    Q_OBJECT

Q_SIGNALS:
    void signalStatus(const QString &status, QDateTime current);
    void readyRead();
    void readyWrite();
    void exception();
    void bytesWritten(qint64 bytes);

    void ctsChanged(bool value);
    void dsrChanged(bool value);
    void ringChanged(bool value);

public:

    /*! \~english
        \enum BaudRate
        Profiles opening serial device that supports the class AbstractSerial.
    */
    enum OpenMode {
        NotOpen     = 0,    /*!< \~english Not openly. */
        ReadOnly    = 1,    /*!< \~english Open read-only. */
        WriteOnly   = 2,    /*!< \~english Open for writing only. */
        ReadWrite   = 3     /*!< \~english Open for reading and writing. */
                  };
    /*! \~english
        \enum BaudRate
        Standard types of speed serial device that supports the class AbstractSerial.
    */
    enum BaudRate {
        BaudRateUndefined = -1, /*!< \~english Undefined speed. */

        BaudRate50,       /*!< \~english Speed 50 bauds. */
        BaudRate75,       /*!< \~english Speed 75 bauds. */
        BaudRate110,      /*!< \~english Speed 110 bauds. */
        BaudRate134,      /*!< \~english Speed 134 bauds. */
        BaudRate150,      /*!< \~english Speed 150 bauds. */
        BaudRate200,      /*!< \~english Speed 200 bauds. */
        BaudRate300,      /*!< \~english Speed 300 bauds. */
        BaudRate600,      /*!< \~english Speed 600 bauds. */
        BaudRate1200,     /*!< \~english Speed 1200 bauds. */
        BaudRate1800,     /*!< \~english Speed 1800 bauds. */
        BaudRate2400,     /*!< \~english Speed 2400 bauds. */
        BaudRate4800,     /*!< \~english Speed 4800 bauds. */
        BaudRate9600,     /*!< \~english Speed 9600 bauds. */
        BaudRate14400,    /*!< \~english Speed 14400 bauds. */
        BaudRate19200,    /*!< \~english Speed 19200 bauds. */
        BaudRate38400,    /*!< \~english Speed 38400 bauds. */
        BaudRate56000,    /*!< \~english Speed 56000 bauds. */
        BaudRate57600,    /*!< \~english Speed 57600 bauds. */
        BaudRate76800,    /*!< \~english Speed 76800 bauds. */
        BaudRate115200,   /*!< \~english Speed 115200 bauds. */
        BaudRate128000,   /*!< \~english Speed 128000 bauds. */
        BaudRate230400,   /*!< \~english Speed 230400 bauds. */     //enhanced speed (experimental)
        BaudRate256000,   /*!< \~english Speed 256000 bauds. */
        BaudRate460800,   /*!< \~english Speed 460800 bauds. */     //enhanced speed (experimental)
        BaudRate500000,   /*!< \~english Speed 500000 bauds. */     //enhanced speed (experimental)
        BaudRate576000,   /*!< \~english Speed 576000 bauds. */     //enhanced speed (experimental)
        BaudRate921600,   /*!< \~english Speed 921600 bauds. */     //enhanced speed (experimental)
        BaudRate1000000,  /*!< \~english Speed 1000000 bauds. */    //enhanced speed (experimental)
        BaudRate1152000,  /*!< \~english Speed 1152000 bauds. */    //enhanced speed (experimental)
        BaudRate1500000,  /*!< \~english Speed 1500000 bauds. */    //enhanced speed (experimental)
        BaudRate2000000,  /*!< \~english Speed 2000000 bauds. */    //enhanced speed (experimental)
        BaudRate2500000,  /*!< \~english Speed 2500000 bauds. */    //enhanced speed (experimental)
        BaudRate3000000,  /*!< \~english Speed 3000000 bauds. */    //enhanced speed (experimental)
        BaudRate3500000,  /*!< \~english Speed 3500000 bauds. */    //enhanced speed (experimental)
        BaudRate4000000   /*!< \~english Speed 4000000 bauds. */    //enhanced speed (experimental)
    };
    /*! \~english
        \enum DataBits
        Standard types of data bits serial device that supports the class AbstractSerial.
    */
    enum DataBits {
        DataBitsUndefined = -1, /*!< \~english Undefined data bits. */

        DataBits5,  /*!< \~english 5 data bits. */
        DataBits6,  /*!< \~english 6 data bits. */
        DataBits7,  /*!< \~english 7 data bits. */
        DataBits8   /*!< \~english 8 data bits. */
    };
    /*! \~english
        \enum Parity
        Standard types of parity serial device that supports the class AbstractSerial.
    */
    enum Parity {
        ParityUndefined = -1,   /*!< \~english Parity undefined. */

        ParityNone, /*!< \~english "None" parity. */
        ParityOdd,  /*!< \~english "Odd" parity. */
        ParityEven, /*!< \~english "Even" parity. */
        ParityMark, /*!< \~english "Mark" parity. */
        ParitySpace /*!< \~english "Space" parity. */
    };
    /*! \~english
        \enum StopBits
        Standard types of stop bits serial device that supports the class AbstractSerial.
    */
    enum StopBits {
        StopBitsUndefined = -1, /*!< \~english Undefined stop bit. */

        StopBits1,      /*!< \~english One stop bit. */
        StopBits1_5,    /*!< \~english Half stop bit. */
        StopBits2       /*!< \~english Two stop bit. */
    };
    /*! \~english
        \enum Flow
        Standard types of flow control serial device that supports the class AbstractSerial.
    */
    enum Flow {
        FlowControlUndefined = -1,  /*!< \~english Flow control undefined. */

        FlowControlOff,         /*!< \~english Flow control "Off". */
        FlowControlHardware,    /*!< \~english Flow control "Hardware". */
        FlowControlXonXoff      /*!< \~english Flow control "Xon/Xoff". */
    };
    /*! \~english
        \enum Status
        The statuses of the serial device that supports the class AbstractSerial.
    */
    enum Status {
        /* group of "SUCESS STATES" */
        //all
        ENone                       = 0,    /*!< \~english No errors. */
        ENoneOpen                   = 1,    /*!< \~english Successfully opened. */
        ENoneClose                  = 2,    /*!< \~english Successfully closed. */
        ENoneSetBaudRate            = 3,    /*!< \~english Type speed successfully changed. */
        ENoneSetParity              = 4,    /*!< \~english Type of parity was successfully changed. */
        ENoneSetDataBits            = 5,    /*!< \~english Type of data bits successfully changed. */
        ENoneSetStopBits            = 6,    /*!< \~english Type of stop bits successfully changed. */
        ENoneSetFlow                = 7,    /*!< \~english Type of flow control was successfully changed. */
        ENoneSetCharTimeout         = 8,    /*!< \~english Timeout waiting symbol was successfully changed. */
        ENoneSetDtr                 = 9,    /*!< \~english DTR successfully changed. */
        ENoneSetRts                 = 10,   /*!< \~english RTS successfully changed.  */
        ENoneLineStatus             = 11,   /*!< \~english Status lines successfully get. */
        // 12-14 reserved

        /* Groups of "ERROR STATES" */

        //group of "OPEN"
        EOpen                       = 15,   /*!< \~english Error opening. */
        EDeviceIsNotOpen            = 16,   /*!< \~english Not open. */
        EOpenModeUnsupported        = 17,   /*!< \~english Open mode is not supported. */
        EOpenModeUndefined          = 18,   /*!< \~english Opening mode undefined. */
        EOpenInvalidFD              = 19,   /*!< \~english Not Actual descriptor. */
        EOpenOldSettingsNotSaved    = 20,   /*!< \~english Failed saving the old parameters when opening. */
        EOpenGetCurrentSettings     = 21,   /*!< \~english Failed to get the current settings when you open. */
        EOpenSetDefaultSettings     = 22,   /*!< \~english Error changing the default settings when you open. */
        EDeviceIsOpen               = 23,   /*!< \~english Already open. */

        //group of "CLOSE"
        ECloseSetOldSettings        = 24,   /*!< \~english Failed saving the old settings when closing. */
        ECloseFD                    = 25,   /*!< \~english Error closing descriptor. */
        EClose                      = 26,   /*!< \~english Error closing. */
        // 27-31 reserved

        //group of "SETTINGS"
        ESetBaudRate                = 32,   /*!< \~english Error change the type of speed. */
        ESetDataBits                = 33,   /*!< \~english Error change the type of data bits. */
        ESetParity                  = 34,   /*!< \~english Error changing the type of parity. */
        ESetStopBits                = 35,   /*!< \~english Error changing the type of stop bits. */
        ESetFlowControl             = 36,   /*!< \~english Error changing the type of flow control. */
        ESetCharIntervalTimeout     = 37,   /*!< \~english Error changing the timeout waiting symbol. */
        // 38-39 reserved

        //group of "CONTROL"
        EBytesAvailable             = 40,   /*!< \~english Failed to get number of bytes from the buffer ready for reading. */
        ESetDtr                     = 41,   /*!< \~english Error changing DTR. */
        ESetRts                     = 42,   /*!< \~english Error changing RTS. */
        ELineStatus                 = 43,   /*!< \~english Failed to get status lines. */
        EWaitReadyReadIO            = 44,   /*!< \~english Error I/O waiting to receive data. */
        EWaitReadyReadTimeout       = 45,   /*!< \~english Timeout waiting to receive data. */
        EWaitReadyWriteIO           = 46,   /*!< \~english Error I/O waiting to transfer data.  */
        EWaitReadyWriteTimeout      = 47,   /*!< \~english Timeout waiting to transfer data. */
        EReadDataIO                 = 48,   /*!< \~english Error reading data. */
        EWriteDataIO                = 49,   /*!< \~english Error writing data. */
        EFlush                      = 50,   /*!< \~english Error clearing transmission queue buffer. */
        ESendBreak                  = 51,   /*!< \~english Transmission error of a continuous flow of zero bits. */
        ESetBreak                   = 52    /*!< \~english Error changing signal discontinuity line. */
                                      // 53-55 reserved
                                  };
    /*! \~english
        \enum LineStatusFlag
        Flags of states of the lines: CTS, DSR, DCD, RI, RTS, DTR, ST, SR
        interface serial device (see RS-232 standard, etc.).\n
        To determine the state of the desired line is necessary to impose a mask "and" of the flag line at
        the result of the method: quint16 AbstractSerial::lineStatus().
    */
    enum LineStatusFlag {
        LineLE    = 0x0001, /*!< \~english Line DSR (data set ready/line enable). */
        LineDTR   = 0x0002, /*!< \~english Line DTR (data terminal ready). */
        LineRTS   = 0x0004, /*!< \~english Line RTS (request to send). */
        LineST    = 0x0008, /*!< \~english Line Secondary TXD (transmit). */
        LineSR    = 0x0010, /*!< \~english Line Secondary RXD (receive.) */
        LineCTS   = 0x0020, /*!< \~english Line CTS (clear to send). */
        LineDCD   = 0x0040, /*!< \~english Line DCD (data carrier detect). */
        LineRI    = 0x0080, /*!< \~english Line RNG (ring). */
        LineDSR   = 0x0100, /*!< \~english Line DSR (data set ready). */

        LineErr   = 0x8000  /*!< \~english Error get line status. */
              };
    Q_DECLARE_FLAGS(LineStatus, LineStatusFlag)

    explicit AbstractSerial(QObject *parent = 0);
    virtual ~AbstractSerial();

    void setDeviceName(const QString &deviceName);
    QString deviceName() const;

    void setOpenMode(AbstractSerial::OpenMode mode);
    AbstractSerial::OpenMode openMode() const;

    bool open(AbstractSerial::OpenMode mode);
    bool isOpen() const;

    void close();

    //baud rate
    bool setBaudRate(BaudRate baudRate);
    bool setInputBaudRate(BaudRate baudRate);
    bool setOutputBaudRate(BaudRate baudRate);
    bool setBaudRate(const QString &baudRate);//overload
    bool setInputBaudRate(const QString &baudRate);//overload
    bool setOutputBaudRate(const QString &baudRate);//overload
    QString baudRate() const;
    QString inputBaudRate() const;
    QString outputBaudRate() const;
    QStringList listBaudRate() const;
    QMap<AbstractSerial::BaudRate, QString> baudRateMap() const;
    //data bits
    bool setDataBits(DataBits dataBits);
    bool setDataBits(const QString &dataBits);
    QString dataBits() const;
    QStringList listDataBits() const;
    QMap<AbstractSerial::DataBits, QString> dataBitsMap() const;
    //parity
    bool setParity(Parity parity);
    bool setParity(const QString &parity);
    QString parity() const;
    QStringList listParity() const;
    QMap<AbstractSerial::Parity, QString> parityMap() const;
    //stop bits
    bool setStopBits(StopBits stopBits);
    bool setStopBits(const QString &stopBits);
    QString stopBits() const;
    QStringList listStopBits() const;
    QMap<AbstractSerial::StopBits, QString> stopBitsMap() const;
    //flow
    bool setFlowControl(Flow flow);
    bool setFlowControl(const QString &flow);
    QString flowControl() const;
    QStringList listFlowControl() const;
    QMap<AbstractSerial::Flow, QString> flowControlMap() const;
    //CharIntervalTimeout
    bool setCharIntervalTimeout(int msecs = 10);
    int charIntervalTimeout() const;
    //Lines statuses
    bool setDtr(bool set);
    bool setRts(bool set);
    quint16 lineStatus();
    //Break
    bool sendBreak(int duration);
    bool setBreak(bool set);
    //
    bool flush();
    bool reset();

    qint64 bytesAvailable() const;

    bool waitForReadyRead(int msecs = 5000);
    bool waitForBytesWritten(int msecs = 5000);

    //Turns the emit signal change of status (state) devices see enum Status
    void enableEmitStatus(bool enable);

    //IO
    //read
    qint64 read(char *data, qint64 maxSize);
    QByteArray read(qint64 maxSize);
    QByteArray readAll();
    //write
    qint64 write(const char *data, qint64 maxSize);
    qint64 write(const char *data);
    qint64 write(const QByteArray &byteArray);

protected:
    AbstractSerialPrivate * const d_ptr;

private:
    Q_DECLARE_PRIVATE(AbstractSerial)
    Q_DISABLE_COPY(AbstractSerial)

    void emitStatusString(AbstractSerial::Status status);
    bool canEmitStatusString() const;

    bool isValid() const;
};

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, AbstractSerial::OpenMode value);
QDebug operator<<(QDebug debug, AbstractSerial::BaudRate value);
QDebug operator<<(QDebug debug, AbstractSerial::DataBits value);
QDebug operator<<(QDebug debug, AbstractSerial::Parity value);
QDebug operator<<(QDebug debug, AbstractSerial::StopBits value);
QDebug operator<<(QDebug debug, AbstractSerial::Flow value);
QDebug operator<<(QDebug debug, AbstractSerial::LineStatus value);
#endif

#endif // ABSTRACTSERIAL_H
