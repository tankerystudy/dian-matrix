#include <reg52.h>
#define DATA_LEN	8
typedef unsigned char data byte;
 
/* serial */
static byte *g_SerialBuf;
static byte g_MemLen;
int g_BufCursor;
byte g_SerialArray[DATA_LEN];

void SerialWrite(byte *pucString, byte ucLen);
byte SerialRead(byte *Buffer, byte BufLen);
void SerialInit(byte *pucSerialMem, byte ucMemLen, bit isMode0);
/* serial */


sfr  T2MOD = 0xC9;
sbit ledLineA = P2^4;
sbit ledLineB = P2^3;
sbit ledLineC = P2^2;
sbit ledLineD = P2^1;
sbit ledSwitch = P2^0;

void delay(byte time)
{
    byte i;
    do
    {
        for (i= 0; i < 250; i++)
            ;
    } while (--time != 0);
}

void lighting(byte lineNum)
{
	ledLineA = (lineNum & 1) ? 1 : 0;
    ledLineB = (lineNum & 2) ? 1 : 0;
    ledLineC = (lineNum & 4) ? 1 : 0;
    ledLineD = (lineNum & 8) ? 1 : 0;
}

void main()
{
    int i;
    byte lightLine = 0;
    ledSwitch = 0;
	
	for (i=0; i<DATA_LEN; i++)
    {
		g_SerialArray[i] = 0xaa;
	}
	SerialInit(g_SerialArray,8,1);
	SerialWrite(g_SerialArray, 8);
    while (1)
    {
        if (++lightLine == 16)
            lightLine = 0;
        lighting(lightLine);

        delay(1);
    }
}

/* serial */
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
        RCAP2H = 0xFF;     // Set the original start time
        RCAP2L = 0xd9;
        IE |= 0x90;         // Enable Serial Interrupt
        T2CON = 0x34;       // timer 2 run
        ES = 1;
    }
    TI = 0;
    RI = 0;
}

/* Send a string to SBUF */
void SerialWrite(byte *pucString, byte ucLen) 
{
    byte ucStrlen= ucLen;
    byte *p= pucString;
    int i = 0;
	while (ucStrlen > i++)
	{
		SBUF = *p++;
		while (TI == 0)         /* wait for transmit interrupt */  
			;                   /* do nothing */
	}
}


/* serial */