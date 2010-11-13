#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"


#define LED_MEM     8

bit downloading;

/* serial data array */
idata byte g_SerialArray[LED_MEM];



void ext_int0() interrupt  0
{
    downloading = 1;
}

void main()
{
    unsigned int  blink;

    EA = 1;
    EX0 = 1;        // ENABLE EXTERN INTERRUPT 0
    IT0 = 1;        // EDGE-TRIGGERED INTERRUPT

    SerialInit(g_SerialArray, LED_MEM, 0);

    downloading = 0;
    LED0= LED1= LED2= LED3= 1;

    while (1)
    {
        if (downloading)
        {
            if (SerialRead(g_SerialArray, LED_MEM) > 0)
            {
                downloading = 0;

                SerialWrite(g_SerialArray, LED_MEM);

                LED0 = (0x08 & g_SerialArray[0])? 0:1;
                LED1 = (0x04 & g_SerialArray[0])? 0:1;
                LED2 = (0x02 & g_SerialArray[0])? 0:1;
                LED3 = (0x01 & g_SerialArray[0])? 0:1;
            }
        }

        if (++blink == 500)
        {
            blink = 0;
       //     RED_LED = ~RED_LED;
        }
        sleep(1);
    }
}

