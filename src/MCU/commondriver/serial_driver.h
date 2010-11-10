#ifndef __SERIAL_DRIVER_H__
#define __SERIAL_DRIVER_H__
#include "..\..\commondriver\common_def.h"

/* function declaration */
void SerialWrite(uchar *pucString, byte ucLen);
byte SerialRead(uchar *Buffer, byte BufLen);
void SerialInit(uchar *pucSerialMem, byte ucMemLen, bit isMode0);

#endif  // #ifndef __SERIAL_DRIVER_H__


