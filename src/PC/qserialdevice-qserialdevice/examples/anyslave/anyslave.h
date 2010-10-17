#ifndef SAnySlave_H
#define SAnySlave_H

#include <QtCore>
#include <QDebug>
#include <QByteArray>



class AbstractSerial;
class QTimer;
class AnySlave : public QObject
{
    Q_OBJECT

public:
    AnySlave(QObject *parent = 0);
    virtual ~AnySlave();
    
private slots:
    void transaction();
    
private:
    AbstractSerial *port;
};

#endif
