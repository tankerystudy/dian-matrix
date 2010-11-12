#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"
#include "..\..\commondriver\graphics_driver.h"
#include "..\..\commondriver\iic_driver.h"

idata byte g_SerialArray[LED_LINE][LED_ROW];

void main()
{
    unsigned char x, y;
    byte const startAddr = 0x01;

    LED1= LED2= LED3= 0;    // switch on 3 LEDs

    for (y=0; y < LED_LINE; y++)
    {
       for (x=0; x < LED_ROW; x++)
        {
    		g_SerialArray[y][x] = ~(y*LED_ROW + x + 1);
    	}
    }

    // writing IIC
    if (I2COpen(startAddr, I2CDEFAULT))
    {
        x = I2CWrite(&g_SerialArray[0][0], LED_MEM);

        switch (x)
        {
        case 0:
            LED1 = LED3 = 1;
            break;
        case 1:
            LED1 = 1;
            break;
        case 2:
            LED2 = 1;
            break;
        case 3:
            LED3 = 1;
            break;
        default:
            break;
        }

        I2CClose();
    }
    else
    {
        LED1 = LED2 = 1;
    }

    // clear array.
    for (y=0; y < LED_LINE; y++)
    {
       for (x=0; x < LED_ROW; x++)
        {
    		g_SerialArray[y][x] = 0x00;
    	}
    }

    // reading IIC
    if (I2COpen(startAddr, I2CREAD))
    {
        I2CReadOneTime(&g_SerialArray[0][0], LED_MEM);
        I2CClose();
    }
    else
    {
        LED2 = LED3 = 1;
    }


    // start show
	GDI_Init(&g_SerialArray[0][0], LED_MEM);
    GDI_DisFormat();

    while (1)
    {
    	GDI_Refresh();

        sleep(1);
    }
}

