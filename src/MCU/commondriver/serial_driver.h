#include __SERIAL_DRIVER_H__
#include __SERIAL_DRIVER_H__

/* function declaration */
void SerialWrite(byte *pucString, byte ucLen);
byte SerialRead(byte *Buffer, byte BufLen);
void SerialInit(byte *pucSerialMem, bit isMode0);

#endif


