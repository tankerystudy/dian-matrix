

#define SBUF_MAX_LENGTH     128
#define byte               unsigned char

#define SERIAL_MODE_DISP        0
#define SERIAL_MODE_DOWNLOAD    1

static byte *MatrixBuf;
int g_BufCursor = 0;


/* function declaration */
void SerialSendChar(byte ucChar);
void SerialSendStr(byte *pucString);
byte SerialRead(byte *Buffer, byte BufLen);
byte SerialWrite(byte BufLen);
void SerialModeSelect(bit Mode);
void Timer2Init();
void SerialInit();



