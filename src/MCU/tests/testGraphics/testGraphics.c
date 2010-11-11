#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"
#include "..\..\commondriver\graphics_driver.h"

/* serial data array */
idata byte g_SerialArray[LED_LINE][LED_ROW];
/*
    简体汉字点阵
    字体：宋体   2个汉字
    点阵：16×16
    文本：给力


byte Character[64]= 
{
    0x10,0x40, 0x40,0x00,  
    0x10,0x40, 0xFB,0xF8,  
    0x20,0xA0, 0x42,0x08,  
    0x21,0x10, 0x02,0x08,  
    0x4A,0x08, 0x1A,0x08,  
    0xF4,0x06, 0xE2,0x08,  
    0x13,0xF8, 0x43,0xF8,  
    0x20,0x00, 0x02,0x08,
    
    0x02,0x00, 0x02,0x08,
    0x02,0x00, 0x04,0x08,
    0x02,0x00, 0x04,0x08,
    0x02,0x00, 0x08,0x08,
    0x7F,0xF8, 0x08,0x08,
    0x02,0x08, 0x10,0x88,
    0x02,0x08, 0x20,0x50,
    0x02,0x08, 0x40,0x20
};
*/

byte Character2[LED_LINE*LED_ROW] = 
{
    1,1,1,1,
    2,2,2,2,
    3,3,3,3,
    4,4,4,4,
    5,5,5,5,
    6,6,6,6,
    7,7,7,7,
    8,8,8,8,
    9,9,9,9,
    10,10,10,10,
    11,11,11,11,
    12,12,12,12,
    13,13,13,13,
    14,14,14,14,
    15,15,15,15,
    16,16,16,16
};

void delay(int time)
{
    byte i;
    do
    {
        for (i= 0; i < 250; i++)
            ;
    } while (--time != 0);
}

void main()
{
    unsigned char x, y;

    EA = 1;
    EX0 = 1;        // ENABLE EXTERN INTERRUPT 0
    IT0 = 1;        // EDGE-TRIGGERED INTERRUPT

   for (y=0; y < LED_LINE; y++)
	   for (x=0; x < LED_ROW; x++)
        {
    		//g_SerialArray[y][x] = ~(y+1);
    		g_SerialArray[y][x] = Character2[LED_ROW * y + x];
            //GDI_ByteMapping(&g_SerialArray[y][x]);
    	}

	GDI_Init(&g_SerialArray[0][0], LED_LINE*LED_ROW);
    GDI_DisFormat();

    while (1)
    {
    	GDI_Refresh();

        delay(1);
    }
}

