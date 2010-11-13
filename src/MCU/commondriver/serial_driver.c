#include <reg52.h>
#include "Serial_driver.h"

/* definition of functions declared in Serial_driver.h */
static byte *g_SerialBuf;
static byte g_MemLen;
static byte g_BufCursor;

/* receive data from SBUF */
void SerialRecv(void) interrupt 4
{
    if (RI)                         /* if receive interrupt is triggered */
    {
        /* close the receive interrupt, must been clear! */
        RI = 0;

        /* memory full, return */
        if (g_BufCursor == g_MemLen)
            return;
        *(g_SerialBuf + (g_BufCursor++)) = SBUF;
    }
}

byte SerialRead(byte *Buffer, byte BufLen)
{
    /* if the memory is not full or the BufLen is too short */
    if (g_BufCursor < g_MemLen || BufLen < g_BufCursor)
    {
        return 0;
    }
 
    Buffer = g_SerialBuf;
    return g_MemLen;
}

/* Send a string to SBUF */
bit SerialWrite(byte *pucString, byte ucLen) 
{
    byte index;
    int  r;

    for (index= 0; index < ucLen; index++)
    {
        SBUF = *(pucString + index);

        // wait to response
        r = SERIAL_OVERTIME;
        while((TI == 0) && (--r != 0))
            ;
        TI = 0;
        if (r == 0)
            return 0;
    }
    return 1;
}

/* initialization of Serial */
void SerialInit(byte *pucSerialMem, byte ucMemLen, bit isMode0)
{
    g_SerialBuf= pucSerialMem;
    g_MemLen = ucMemLen;            
    g_BufCursor = 0;

    if (isMode0)
    {
        SCON = 0x10;        // UART is mode 0
        ES = 0;             // Close Serial Interrupt
        PS = 0;
    }
    else
    {
        SCON = 0x50;        // UART is mode 1, 8 bit data, allow receive
        RCAP2 = 0xFFD9;     // Set the original start time
        IE |= 0x90;         // Enable Serial Interrupt
        T2CON = 0x34;       // timer 2 run
        PS = 1;
    }
}

