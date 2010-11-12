#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"
#include "..\..\commondriver\graphics_driver.h"

#define DATA_LEN    4
#define LINE_LEN    16

int freshTime;
/* serial data array */
idata byte g_SerialArray[LINE_LEN][DATA_LEN];
/*
    简体汉字点阵
    字体：宋体   2个汉字
    点阵：16×16
    文本：给力
*/

byte Character[65]= 
{
    0x10,0x40,
    0x10,0x40,
    0x20,0xA0,
    0x21,0x10,
    0x4A,0x08,
    0xF4,0x06,
    0x13,0xF8,
    0x20,0x00,
    0x40,0x00,
    0xFB,0xF8,
    0x42,0x08,
    0x02,0x08,
    0x1A,0x08,
    0xE2,0x08,
    0x43,0xF8,
    0x02,0x08,
    
    0x02,0x00,
    0x02,0x00,
    0x02,0x00,
    0x02,0x00,
    0x7F,0xF8,
    0x02,0x08,
    0x02,0x08,
    0x02,0x08,
    0x02,0x08,
    0x04,0x08,
    0x04,0x08,
    0x08,0x08,
    0x08,0x08,
    0x10,0x88,
    0x20,0x50,
    0x40,0x20,
};


sbit isWorking = P1^4;

sfr  T2MOD = 0xC9;
sbit ledLineA = P2^4;
sbit ledLineB = P2^3;
sbit ledLineC = P2^2;
sbit ledLineD = P2^1;
sbit ledSwitch = P2^0;

sbit SCLK = P3^3;

void delay(int time)
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
    freshTime = 700;
}

void main()
{
    unsigned char x, y;
    byte lightLine = 0;
    ledSwitch = 0;
    isWorking = 1;
    SCLK = 0;

    freshTime = 1;

    EA = 1;
    EX0 = 1;        // ENABLE EXTERN INTERRUPT 0
    IT0 = 1;        // EDGE-TRIGGERED INTERRUPT

    SerialInit(&g_SerialArray[0][0],LINE_LEN*DATA_LEN,1);

    for (y=0; y<LINE_LEN; y++)
       for (x=0; x<DATA_LEN; x++)
        {
            CY= 0;

    		g_SerialArray[y][x] = Character[y * LINE_LEN + x];
            GDI_ByteMapping(&g_SerialArray[y][x]);
        }

    while (1)
    {
        ledSwitch = 1;
        if (++lightLine == 16)
            lightLine = 0;

        SerialWrite(&g_SerialArray[lightLine%LINE_LEN][0], DATA_LEN);
        isWorking = 0;
        SCLK = 1;
        isWorking = 1;
        SCLK = 0;

        lighting(lightLine);
        ledSwitch = 0;

        delay(freshTime);
    }
}

