#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"
#include "..\..\commondriver\graphics_driver.h"



bit downloading;

/* serial data array */
idata byte g_SerialArray[LED_MEM];



void ext_int0() interrupt  0
{
    downloading = !downloading;

    if (downloading)
        SerialInit(g_SerialArray, LED_MEM, 0);
    else
        GDI_Init(g_SerialArray, LED_MEM);
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
            g_SerialArray[y*LED_ROW + x] = ~(y*LED_ROW + x + 1);
        }

    downloading = 0;
    GDI_Init(g_SerialArray, LED_MEM);
    GDI_DisFormat();

    while (1)
    {
        if (downloading)
        {
            if (SerialRead(g_SerialArray, LED_MEM) > 0)
            {
                downloading = 0;
                GDI_Init(g_SerialArray, LED_MEM);
                GDI_DisFormat();
            }
        }
        else
            GDI_Refresh();

        sleep(1);
    }
}

