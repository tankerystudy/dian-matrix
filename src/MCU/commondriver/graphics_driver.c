#include <reg52.h>
#include "common_def.h"
#include "graphics_driver.h"
#include "serial_driver.h"

/* 全局变量定义 */
/* 显示相关IO口定义 */
sbit LINE_A = P2^4;
sbit LINE_B = P2^3;
sbit LINE_C = P2^2;
sbit LINE_D = P2^1;
sbit LINE_EN = P2^0;

/* 寄存器使能，高电平开启 */
sbit RES_EN = P1^4;
/* 寄存器时钟线控制 */
sbit RES_CLK = P3^3;

/* 显存和显存大小 */
static idata uchar *g_bGraphMem;
static byte g_bGraphLen;

//static void LineRefresh(byte *bData, uchar iCurrentLine);
static void SetCurrentLine(uchar iCurrentLine);
//static void ByteMapping(byte *bData);


/* 显卡驱动 */

/*******************************************************************************
    Func Name: GDI_Init
 Date Created: 2010-10-14
       Author: lihaitao
  Description: 显示初始化。
               1.使能74HC38。
               2.初始化全局变量。
               3.设置串口模式0，用于传送数据。
        Input: 
       Output: 
       Return: 
      Caution: 
--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD
  2010-11-10  Tankery          添加寄存器控制及寄存器时钟线控制

*******************************************************************************/
void GDI_Init(byte idata *pbGraphMem, byte bGraphLen)
{
    /* 1.使能74HC38。 */
    LINE_EN = 0;    /* 低电平使能 */

    /* 2.设置寄存器 */
    RES_EN = 1;     /* 高电平使能 */
    RES_CLK = 0;    /* 释放时钟线控制权 */

    /* 3.获取显存。 */
    g_bGraphMem = pbGraphMem;
    g_bGraphLen = bGraphLen;

    /* 4.设置串口模式0，用于传送数据。 */
    /* 调用串口驱动的接口 */
    SerialInit(g_bGraphMem, g_bGraphLen, 1);
	LED3 = 0;
		while(1);

  return;
}

/*******************************************************************************
    Func Name: GDI_ByteMapping
 Date Created: 2010-10-14
       Author: lihaitao
  Description: 按接口映射关系格式化显存。
               74HC595的接口映射关系表:
               QH -- 2
               QG -- 4
               QF -- 1
               QE -- 5
               QD -- 7
               QC -- 8
               QB -- 6
               QA -- 3
        Input: INOUT bit *bData, 原始数据(8bits)
       Output: INOUT bit *bData, 映射后的数据(8bits)
       Return: void
      Caution: 无
--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/
void GDI_ByteMapping(byte *bData)
{
    uchar i = 8;
    byte temp = 0;
    byte shiftdata = 0x01;
    /* maptable[i] represent the order of the element in the LEDmatrixArray order */
    /* {QH, QG, QF, QE, QD, QC, QB, QA} */
    byte maptable[8] = {6, 4, 7, 3, 1, 0, 2, 5};

    do 
    {
        CY = 0;
        temp = (temp << 1) + 
               ((*bData & (shiftdata << maptable[i-1])) >> maptable[i-1]);
    } while (0 != --i);
    
    *bData = temp;

    return;
}

/*******************************************************************************
    Func Name: GDI_DisFormat
 Date Created: 2010-11-10
       Author: Tankery
  Description: 调用ByteMapping函数，格式化所有显存数据
        Input: none
       Output: none
       Return: void
      Caution: 无
--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/
void GDI_DisFormat(void)
{
    int i;

    for (i=0; i < g_bGraphLen; i++)
    {
        GDI_ByteMapping(g_bGraphMem + i);
	}

    return;
}


/*******************************************************************************
    Func Name: GDI_Refresh
 Date Created: 2010-10-15
       Author: lihaitao
  Description: 
        Input: 
       Output: 
       Return: 
      Caution: 
--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/
void GDI_Refresh(void)
{
    /* 当前显示的行数 */
    static byte currentLine = 0;
    /* 刷新一行 */
    LineRefresh((g_bGraphMem + currentLine * LED_ROW), currentLine);

    /* 更新当前行 */
    if (++currentLine == LED_LINE)
    {
        currentLine = 0;
    }
    
    return;
}

/*******************************************************************************
    Func Name: LineRefresh
 Date Created: 2010-10-14
       Author: lihaitao
  Description: 刷新一行的数据。将控制一行LED的数据(8字节)通过串口模式0复用，
               传到列线上。
        Input: 
       Output: 
       Return: 
      Caution: 
--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD
  2010-11-10  Tankery          刷新前关闭显示屏，减少余光
  2010-11-10  Tankery          补偿一个时钟周期，使正确数据锁存

*******************************************************************************/
void LineRefresh(byte *bData, uchar iCurrentLine)
{
    /* 关闭行显 */
    LINE_EN = 1;

    /* 通过串口发送数据到74HC595 */
    SerialWrite(bData, LED_ROW);
    /* 由于硬件设计失误，补偿一个时钟周期，使正确数据锁存 */
    RES_EN = 0;
    RES_CLK = 1;
    RES_EN = 1;
    RES_CLK = 0;

    /* 行选 */
    SetCurrentLine(iCurrentLine);
    
    /* 打开行显 */
    LINE_EN = 0;

    return;
}

/*******************************************************************************
    Func Name: SetCurrentLine
 Date Created: 2010-10-15
       Author: lihaitao
  Description: 根据当前行，设置74HC138的行选信号。
        Input: 
       Output: 
       Return: 
      Caution: 
--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/
static void SetCurrentLine(uchar iCurrentLine)
{
    LINE_A = (iCurrentLine & 1) ? 1 : 0;
    LINE_B = (iCurrentLine & 2) ? 1 : 0;
    LINE_C = (iCurrentLine & 4) ? 1 : 0;
    LINE_D = (iCurrentLine & 8) ? 1 : 0;

    return;
}

