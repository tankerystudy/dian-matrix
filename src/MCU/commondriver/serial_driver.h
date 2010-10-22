#define SBUF_MAX_LENGTH     128
#define byte               unsigned char

#define SERIAL_MODE_DISP        0
#define SERIAL_MODE_DOWNLOAD    1

static byte *MatrixBuf;
int BufCursor = 0;


void SerialSendChar(byte ucChar)
{
    TI = 0;
    SBUF = ucChar;
    while (TI == 0)                 /* wait for transmit interrupt */  
        ;                           /* do nothing */
    TI = 0;                         /* close the transmit interrupt */
}


void SerialSendStr(byte *pucString) 
{
    byte ucStrlen;
    byte *p;
    p=&pucString;
    ucStrlen = strlen(pucString);

    SBUF = *p;
    
    if (TI)                         /* if transmit interrupt is triggered */
    {
        TI = 0;
        if (ucStrlen > p++)
        {
            SBUF = *p;
        }
    }
}


void SerialRecv(void) interrupt 4 using 3
{
    if (RI)                         /* if receive interrupt is triggered */
    {
        ES = 0;                     /* close the Serial Port interrupt */
//      MatrixBuf[BufCursor/16][BufCursor%16] = SBUF;           /* copy the SBUF content to our own buff */
        MatrixBuf[BufCursor++] = SBUF;
        RI = 0;                     /* close the receive interrupt */
        ES = 1;
    }
}


byte SerialRead(byte *Buffer, byte BufLen)
{
    if (BufCursor < BufLen)
    {
        return -1;
    }
    else
    {
        Buffer = MatrixBuf;
    }   
}


byte SerialWrite(byte *BufCursor, byte BufLen)
{
    byte *p;
    p=&pucString;

    SBUF = *p;
    
    if (TI)                         /* if transmit interrupt is triggered */
    {
        TI = 0;
        if (BufLen> p++)
        {
            SBUF = *p;
        }
    }    
}


void SerialModeSelect(bit Mode)
{
    if (SERIAL_MODE_DISP== Mode)
    {
        SCON = 0x10;
        TI = 0;
        RI = 0;
        ES = 1;
    }
    else
    {
        SCON = 0x50;
        TI = 0;
        RI = 0;
        ES = 1;
    }
}

