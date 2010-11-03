#include <reg52.h>
#include "Serial_driver.h"

/* definition of functions declared in Serial_driver.h */
static byte *g_SerialBuf;
static byte g_MemLen;
static int g_BufCursor;

/* receive data from SBUF */
void SerialRecv(void) interrupt 4 using 3
{
    /* memory full, return */
    if (g_BufCursor = g_MemLen)
        return;

    if (RI)                         /* if receive interrupt is triggered */
    {
        ES = 0;                     /* close the Serial Port interrupt */
        *(g_SerialBuf + (g_BufCursor++)) = SBUF;
        RI = 0;                     /* close the receive interrupt */
        ES = 1;
    }
}

byte SerialRead(byte *Buffer, byte BufLen)
{
    /* if the memory is not full or the BufLen is too short */
    if (g_BufCursor < g_MemLen || BufLen < g_BufCursor)
    {
        return -1;
    }

    Buffer = g_SerialBuf;
    return g_MemLen;
}

/* Send a string to SBUF */
void SerialWrite(byte *pucString, byte ucLen) 
{
    byte ucStrlen= ucLen;
    byte i;

	for (i= 0; i < ucLen; i++)
	{
		SBUF = *(pucString + i);
		while (TI == 0)         /* wait for transmit interrupt */  
			;                   /* do nothing */
        TI= 1;
	}
}

/* initialization of Serial */
void SerialInit(byte *pucSerialMem, byte ucMemLen, bit isMode0)
{
    g_SerialBuf= pucSerialMem;
	g_MemLen = ucMemLen;			
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
}

