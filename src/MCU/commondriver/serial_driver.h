#ifndef __SERIAL_DRIVER_H__
#define __SERIAL_DRIVER_H__
#include "..\..\commondriver\common_def.h"

/* function declaration */
void SerialWrite(byte *pucString, byte ucLen);
byte SerialRead(byte *Buffer, byte BufLen);
void SerialInit(byte *pucSerialMem, byte ucMemLen, bit isMode0);

#endif  // #ifndef __SERIAL_DRIVER_H__


