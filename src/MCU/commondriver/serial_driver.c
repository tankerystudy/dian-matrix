#include "Serial_driver.h"

/* definition of functions declared in Serial_driver.h */


/* send a charactor to SBUF */
void SerialSendChar(byte ucChar)
{
    TI = 0;
    SBUF = ucChar;
    while (TI == 0)                 /* wait for transmit interrupt */  
        ;                           /* do nothing */
    TI = 0;                         /* close the transmit interrupt */
}

/* Send a string to SBUF */
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

/* receive data from SBUF */
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
    if (g_BufCursor < BufLen)
    {
        return -1;
    }
    else
    {
        Buffer = MatrixBuf;
    }   
}


byte SerialWrite(byte BufLen)
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

/* select mode for serial */
void SerialModeSelect(bit Mode)
{
    if (SERIAL_MODE_DISP == Mode)
    {
        SCON = 0x10;
	}
    else
    {
        SCON = 0x50;
    }
	TI = 0;
	RI = 0;
	ES = 1;
}

/* initialization of Timer2 */
void Timer2Init()
{
	PCON = 0x00;
    T2MOD = 0X00;
    T2CON = 0x30;
   
    RCAP2H = 0xff;
    RCAP2L = 0xd9;
    
    TH2 = 0xff;
    TL2 = 0xd9;
}

/* initialization of Serial */
void SerialInit()

{
	SerialModeSelect(SERIAL_MODE_DOWNLOAD);
	Timer2Init();
}

