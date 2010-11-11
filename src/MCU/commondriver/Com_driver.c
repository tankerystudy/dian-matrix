/* This file(20101110) is created to test serial transfer  */
#include <reg52.h>
#include "Serial_driver.h"
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\graphics_driver.h"

#define LIGHT       0

/* definition of functions declared in Serial_driver.h */
static byte *g_SerialBuf;
static byte g_MemLen;
static int g_BufCursor;

sbit greenLED1 = P1 ^ 1;
sbit greenLED2 = P1 ^ 2;
sbit greenLED3 = P1 ^ 3;

idata byte g_SerialArray[LED_LINE][LED_ROW];



/*
    简体汉字点阵
    字体：宋体   2个汉字
    点阵：16×16
    文本：给力
*/
/*
byte Character1[65]= 
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
};*/

void main()
{
    byte ucLen = 0;
    byte szRcvSerialData[16];



    EA = 1;
    IT0 = 1;        // EDGE-TRIGGERED INTERRUPT

    SerialInit(&g_SerialArray[0][0],LED_LINE*LED_ROW,0);

    while (1)
    {
        ucLen = SerialRead(szRcvSerialData, 16);
        if (!ucLen)
        {
            switch((szRcvSerialData[0] - 48))
            {
                case 1:
                {
                    greenLED1 = LIGHT;
                    break;
                }
                case 2:
                {
                    greenLED2 = LIGHT;
                    break;
                }
                case 3:
                {
                    greenLED3 = LIGHT;
                    break;
                }
                default:
                    break;
            }
        }
    }
}



