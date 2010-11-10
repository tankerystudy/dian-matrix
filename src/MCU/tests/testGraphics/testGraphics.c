#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"
#include "..\..\commondriver\graphics_driver.h"

/* serial data array */
idata byte g_SerialArray[LED_LINE][LED_ROW];


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
            g_SerialArray[y][x] = ~(y+1);
        }

    led_drv_DisInit(&g_SerialArray[0][0], LED_LINE*LED_ROW);
    led_drv_DisFormat();

    while (1)
    {
        led_drv_Refresh();

        delay(1);
    }
}

