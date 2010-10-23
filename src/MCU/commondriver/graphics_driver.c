#include <reg52.h>
#include "graphics_driver.h"

/* 全局变量定义 */
/* 显示相关IO口定义 */
sbit LINE_A = P2^5;
sbit LINE_B = P2^4;
sbit LINE_C = P2^3;
sbit LINE_D = P2^2;
sbit LINE_EN = P2^1;

/* 显存和显存大小 */
static byte *g_bGraphMem;
static byte g_bGraphLen;


/* 当前显示的行数 */
uchar g_ucCurrentLine;
uchar g_ucCurrentRowLed;


/* 显卡驱动 */

/*******************************************************************************
    Func Name: led_drv_DisInit
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

*******************************************************************************/
void led_drv_DisInit(byte *pbGraphMem, byte bGraphLen)
{
    /* 1.使能74HC38。 */
    LINE_EN = 0;    /* 低电平使能 */

    /* 2.初始化全局变量。 */
    g_ucCurrentLine = 0;

    /* 3.获取显存。 */
    g_bGraphMem = pbGraphMem;
    g_bGraphLen = bGraphLen;

    /* 4.设置串口模式0，用于传送数据。 */
    /* 调用串口驱动的接口 */

    return;
}

/*******************************************************************************
    Func Name: led_drv_InterfaceMap
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
void led_drv_InterfaceMap(INOUT byte *bData)
{
    bit temp[8];
    bit tempdata[8];
    uchar i = 0;
    uchar data = 1;

    while (8 > i)
    {
        tempdata[i] = (bit) (((*bData) & data) >> i);
        data <<= 1;
        i++;
    }

    /* 按照接口关系映射表进行映射 */
    temp[0] = tempdata[1];
    temp[1] = tempdata[3];
    temp[2] = tempdata[0];
    temp[3] = tempdata[4];
    temp[4] = tempdata[6];
    temp[5] = tempdata[7];
    temp[6] = tempdata[5];
    temp[7] = tempdata[2];

    /* 将8bits转换成1byte */
    i = 0;
    while (8 > i)
    {
        *bData = ((*bData) << i) & temp[7 - i];
        i++;
    }

    return;
}

/*******************************************************************************
    Func Name: led_drv_Refresh
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
void led_drv_Refresh(void)
{
    /* 刷新一行 */
    led_drv_LineRefresh(g_bGraphMem[g_ucCurrentLine], g_ucCurrentLine);

    /* 更新当前行 */
    g_ucCurrentLine++;
    if (LED_LINE <= g_ucCurrentLine)
    {
        g_ucCurrentLine = 0;
    }
    
    return;
}

/*******************************************************************************
    Func Name: led_drv_LineRefresh
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

*******************************************************************************/
void led_drv_LineRefresh(IN byte *bData,IN uchar iCurrentLine)
{
    /* 行选 */
    SetCurrentLine(iCurrentLine);

    /* 通过串口发送数据到74HC595 */
    SerialSendStr(bData);

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
void SetCurrentLine(IN uchar iCurrentLine)
{
    LINE_D= iCurrentLine / 8;
    LINE_C= (iCurrentLine % 8) / 4;
    LINE_B= (iCurrentLine % 4) / 2;
    LINE_A= iCurrentLine % 2; 

    return;
}

