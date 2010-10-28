#include "Serial_driver.h"

/* definition of functions declared in Serial_driver.h */
static byte *g_MatrixBuf;
int g_BufCursor;


/* receive data from SBUF */
void SerialRecv(void) interrupt 4 using 3
{
    /* memory full, return */
    if (g_BufCursor = SBUF_MAX_LENGTH)
        return;

    if (RI)                         /* if receive interrupt is triggered */
    {
        ES = 0;                     /* close the Serial Port interrupt */
        *(g_MatrixBuf + (g_BufCursor++)) = SBUF;
        RI = 0;                     /* close the receive interrupt */
        ES = 1;
    }
}



byte SerialRead(byte *Buffer, byte BufLen)
{
    /* if the memory not full or the BufLen is too short */
    if (g_BufCursor < SBUF_MAX_LENGTH || BufLen < g_BufCursor)
    {
        return -1;
    }

    Buffer = g_MatrixBuf;
    return SBUF_MAX_LENGTH;
}

/* Send a string to SBUF */
void SerialWrite(byte *pucString, byte ucLen) 
{
    byte ucStrlen= ucLen;
    byte *p= pucString;

    SBUF = *p;
    
    if (TI)                         /* if transmit interrupt is triggered */
    {
        TI = 0;
        while (ucStrlen > p++)
        {
            SBUF = ucChar;
            while (TI == 0)         /* wait for transmit interrupt */  
                ;                   /* do nothing */
        }
    }
}

/* initialization of Serial */
void SerialInit(byte *pucSerialMem, bit isMode0)
{
    g_MatrixBuf= pucSerialMem;
    g_BufCursor = 0;

    if (isMode0)
    {
        SCON = 0x10;
    }
    else
    {
        SCON = 0x50;        // UART is mode 1, 8 bit data, allow receive
        RCAP2 = 0xFFD9;     // Set the original start time
        IE |= 0x90;         // Enable Serial Interrupt
        T2CON = 0x34;       // timer 2 run
        ES = 1;
    }
    TI = 0;
    RI = 0;
}

