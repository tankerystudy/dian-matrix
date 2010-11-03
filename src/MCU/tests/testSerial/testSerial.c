#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"
#include "..\..\commondriver\graphics_driver.h"

#define DATA_LEN	8

/* serial data array */
byte g_SerialArray[DATA_LEN];

sbit isWorking = P1^4;

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

void ext_int0() interrupt 0
{
    int i;
	for (i=0; i<DATA_LEN; i++)
    {
        CY= 0;
		g_SerialArray[i] = 0x80>>i;
        led_drv_InterfaceMap(&g_SerialArray[i]);
	}
	SerialWrite(g_SerialArray, 1);
}

void main()
{
    int i;
    byte lightLine = 0;
    ledSwitch = 0;
    isWorking = 1;
	
    EA = 1;
    EX0 = 1;        // ENABLE EXTERN INTERRUPT 0
    IT0 = 1;        // EDGE-TRIGGERED INTERRUPT

	for (i=0; i<DATA_LEN; i++)
    {
        CY= 0;
		g_SerialArray[i] = 0x80>>i;
	}
	SerialInit(g_SerialArray,DATA_LEN,1);
	SerialWrite(g_SerialArray, DATA_LEN);
    while (1)
    {
        if (++lightLine == 16)
            lightLine = 0;
        lighting(lightLine);

        delay(1);
    }
}

