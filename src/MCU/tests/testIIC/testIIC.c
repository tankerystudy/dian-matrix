#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"
#include "..\..\commondriver\graphics_driver.h"
#include "..\..\commondriver\iic_driver.h"

idata byte g_SerialArray[LED_MEM] =
{
    0x10,0x40,  0x02,0x00,
    0x10,0x40,  0x02,0x00,
    0x20,0xA0,  0x02,0x00,
    0x21,0x10,  0x02,0x00,
    0x4A,0x08,  0x7F,0xF8,
    0xF4,0x06,  0x02,0x08,
    0x13,0xF8,  0x02,0x08,
    0x20,0x00,  0x02,0x08,

    0x40,0x00,  0x02,0x08,  
    0xFB,0xF8,  0x04,0x08,
    0x42,0x08,  0x04,0x08,
    0x02,0x08,  0x08,0x08,
    0x1A,0x08,  0x08,0x08,
    0xE2,0x08,  0x10,0x88,
    0x43,0xF8,  0x20,0x50,
    0x02,0x08,  0x40,0x20
};

void main()
{
    unsigned char x, y;
    byte const startAddr = 0x00;

    LED1= LED2= LED3= 0;    // switch on 3 LEDs

    for (y=0; y < LED_LINE; y++)
    {
       for (x=0; x < LED_ROW; x++)
        {
    		g_SerialArray[y*LED_ROW + x] = ~g_SerialArray[y*LED_ROW + x];
    	}
    }

    // writing IIC
    if (I2COpen(startAddr, I2CDEFAULT))
    {
        x = I2CWrite(g_SerialArray, LED_MEM);
        I2CClose();

        switch (x)
        {
        case 0:
            LED1 = LED3 = 1;

            // clear array.
            for (y=0; y < LED_LINE; y++)
            {
               for (x=0; x < LED_ROW; x++)
                {
            		g_SerialArray[y*LED_ROW + x] = 0x00;
            	}
            }

            // reading IIC
            if (I2COpen(startAddr, I2CREAD))
            {
                I2CReadOneTime(g_SerialArray, LED_MEM);
                I2CClose();

                // start show
            	GDI_Init(g_SerialArray, LED_MEM);
                GDI_DisFormat();
            }
            else
            {
                LED2 = LED3 = 1;
            }
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
    }
    else
    {
        LED1 = LED2 = 1;
    }



    while (1)
    {
    	GDI_Refresh();

        sleep(1);
    }
}

