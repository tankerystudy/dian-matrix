#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"

#define DATA_LEN	8

/* serial data array */
byte g_SerialArray[DATA_LEN];


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

