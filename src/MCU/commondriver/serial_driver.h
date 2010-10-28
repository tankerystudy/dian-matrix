

#define SBUF_MAX_LENGTH     128

/* function declaration */
void SerialWrite(byte *pucString, byte ucLen);
byte SerialRead(byte *Buffer, byte BufLen);
void SerialInit(byte *pucSerialMem, bit isMode0);



