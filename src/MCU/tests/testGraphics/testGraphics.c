#include <reg52.h>
#include "..\..\commondriver\common_def.h"
#include "..\..\commondriver\serial_driver.h"
#include "..\..\commondriver\graphics_driver.h"

#define TIME_TIMER      1       /* 定义定时器时间 */
/* 初始化定时器为2ms
 * 这样可以使屏幕的刷新时间为2*16= 32ms
 * 约31帧/s
 */
#define TIME_REFRESH    200       /* 在显卡驱动中 *//* 定义行扫描时间为2ms */

uchar g_NumOfChar;
static byte reTCount;

byte idata g_SerialArray[LED_LINE][LED_ROW] =
{    
    //1.LOVE
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0x74, 0xff, 0x8f, 0x7a, 0x74, 0xa5, 0x4, 0x20, 
    0x7d, 0xff, 0xb7, 0x7d, 0x7d, 0xb7, 0x7d, 0xaf, 
    0x7d, 0xff, 0xf5, 0xf5, 0xf5, 0xf5, 0x7d, 0xaf, 
    0x7d, 0xff, 0x7d, 0xb7, 0xf5, 0xf5, 0x7d, 0xff, 
    0x7d, 0xff, 0x7d, 0xb7, 0xb7, 0x7d, 0x7d, 0x7d, 
    0x7d, 0xff, 0x7d, 0xb7, 0xb7, 0x7d, 0x5, 0x78, 
    0x7d, 0xff, 0x7d, 0xb7, 0xaf, 0x7e, 0x7d, 0x7d, 
    0x7d, 0xff, 0x7d,0xb7, 0xaf, 0x7e, 0x7d, 0xff, 
    0x7d, 0xb7, 0xf5, 0xf5, 0xcf, 0xfa, 0x7d, 0xaf, 
    0x7d, 0xb7, 0xb7, 0x7d, 0xcf, 0xfa, 0x7d, 0xaf, 
    0x4, 0x30, 0x8f, 0x7a, 0xdf, 0xfb, 0x4, 0x20, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

/* 点阵结束 */


/* 映射 */
/*
void main()
{
    unsigned char x, y;

    EA = 1;
    EX0 = 1;        // ENABLE EXTERN INTERRUPT 0
    IT0 = 1;        // EDGE-TRIGGERED INTERRUPT

   for (y=0; y < LED_LINE; y++)
   {
	   for (x=0; x < LED_ROW; x++)
        {
    		//g_SerialArray[y*LED_ROW + x] = ~(y*LED_ROW + x + 1);
			g_SerialArray[y*LED_ROW + x] = ~g_SerialArray[y*LED_ROW + x];
    	}
   }

	GDI_Init(g_SerialArray, LED_MEM);
    GDI_DisFormat();

    while (1)
    {
    	GDI_Refresh();

        sleep(1);
    }
}*/

/* 初始化定时器为TIME_TIMER
 */
void InitTimer(void)
{
    TMOD = 0x01;     /* T0/T1,mode1 */ 
    TH0 = 0xfc;      /* T0计数初值，定时1ms */
    TL0 = 0x18;      /* T0用于刷新键盘 */

	reTCount= TIME_REFRESH/TIME_TIMER;      /* 行刷新时间计数*/

    return;
}

/*
void ext_int0() interrupt 0
{
    if (++g_NumOfChar == NUMOFCHAR)
    {
        g_NumOfChar = 0;
    }

    return;
}*/

/*
void OnTimer(void) interrupt 1 using 0
{
    if (--reTCount == 0)
    {
        reTCount= TIME_REFRESH/TIME_TIMER;      /* 行刷新时间计数

        if (++g_NumOfChar == NUMOFCHAR)
        {
            g_NumOfChar = 0;
        }
    }

    return;
}*/

void main()
{
    uchar i;
    uchar CurrentLine = 0;
    byte DisMem[LED_ROW];

    g_NumOfChar = 0;

    ES = 0;
	EA = 1;
    EX0 = 1;        // ENABLE EXTERN INTERRUPT 0
    ET0 = 1;         /* 允许T0中断 */
    IT0 = 1;        // EDGE-TRIGGERED INTERRUPT

   	LED1 = 1;
	LED2 = 1;
	LED3 = 1;	

//	sleep(800);		// sleep 500ms to make LED blink (if possible) clear
//	LED1 = 0;
//	while(1);

    //InitTimer();

	//GDI_Init(&g_SerialArray[0][0], LED_MEM);
	GDI_Init(&g_SerialArray[0][0], LED_MEM);
//	LED1 = 0;
//	while(1);

    while (1)
    {
        for (i = 0; i < LED_ROW; i++)
        {
//			if( CurrentLine == 8 )
//				DisMem[i] = 0xaa;
//			else
            	DisMem[i] = g_SerialArray[CurrentLine][i];
            //GDI_ByteMapping(&DisMem[i]);
//			LED1 = 0;
        }
        
    	LineRefresh(DisMem, CurrentLine);
//	LED2 = 0;
     	if (LED_LINE == ++CurrentLine)
        {
            CurrentLine = 0;
        }
//		LED3 = 0;
    }
}

