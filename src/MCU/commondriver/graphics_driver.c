#include <reg52.h>
#include "common_def.h"
#include "graphics_driver.h"
#include "serial_driver.h"

/* ȫ�ֱ������� */
/* ��ʾ���IO�ڶ��� */
sbit LINE_A = P2^5;
sbit LINE_B = P2^4;
sbit LINE_C = P2^3;
sbit LINE_D = P2^2;
sbit LINE_EN = P2^1;

/* �Դ���Դ��С */
static byte *g_bGraphMem;
static byte g_bGraphLen;


/* ��ǰ��ʾ������ */
uchar g_ucCurrentLine;
uchar g_ucCurrentRowLed;

void led_drv_LineRefresh(byte *bData, uchar iCurrentLine);
void SetCurrentLine(uchar iCurrentLine);


/* �Կ����� */

/*******************************************************************************
    Func Name: led_drv_DisInit
 Date Created: 2010-10-14
       Author: lihaitao
  Description: ��ʾ��ʼ����
               1.ʹ��74HC38��
               2.��ʼ��ȫ�ֱ�����
               3.���ô���ģʽ0�����ڴ������ݡ�
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
void led_drv_DisInit(byte pbGraphMem[], byte bGraphLen)
{
    /* 1.ʹ��74HC38�� */
    LINE_EN = 0;    /* �͵�ƽʹ�� */

    /* 2.��ʼ��ȫ�ֱ����� */
    g_ucCurrentLine = 0;

    /* 3.��ȡ�Դ档 */
    g_bGraphMem = pbGraphMem;
    g_bGraphLen = bGraphLen;

    /* 4.���ô���ģʽ0�����ڴ������ݡ� */
    /* ���ô��������Ľӿ� */
    SerialInit(g_bGraphMem, g_bGraphLen, 1);

    return;
}

/*******************************************************************************
    Func Name: led_drv_InterfaceMap
 Date Created: 2010-10-14
       Author: lihaitao
  Description: ���ӿ�ӳ���ϵ��ʽ���Դ档
               74HC595�Ľӿ�ӳ���ϵ��:
               QH -- 2
               QG -- 4
               QF -- 1
               QE -- 5
               QD -- 7
               QC -- 8
               QB -- 6
               QA -- 3
        Input: INOUT bit *bData, ԭʼ����(8bits)
       Output: INOUT bit *bData, ӳ��������(8bits)
       Return: void
      Caution: ��
--------------------------------------------------------------------------------
  Modification History
  DATE        NAME             DESCRIPTION
--------------------------------------------------------------------------------
  YYYY-MM-DD

*******************************************************************************/
void led_drv_InterfaceMap(byte *bData)
{
    uchar i = 8;
    byte temp = 0;
    byte shiftdata = 0x01;
//  byte maptable[8] = {1, 3, 0, 4, 6, 7, 5, 2};
    byte maptable[8] = {6, 4, 7, 3, 1, 0, 2, 5};

    do 
    {
        temp = (temp << 1) + ((*bData & (shiftdata << maptable[i-1])) >> maptable[i-1]);
    } while (0 != --i);
    
    *bData = temp;
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
    /* ˢ��һ�� */
    led_drv_LineRefresh((g_bGraphMem + g_ucCurrentLine * 8), g_ucCurrentLine);

    /* ���µ�ǰ�� */
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
  Description: ˢ��һ�е����ݡ�������һ��LED������(8�ֽ�)ͨ������ģʽ0���ã�
               ���������ϡ�
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
void led_drv_LineRefresh(byte *bData, uchar iCurrentLine)
{
    /* ��ѡ */
    SetCurrentLine(iCurrentLine);

    /* ͨ�����ڷ������ݵ�74HC595 */
    SerialWrite(bData, LED_ROW);

    return;
}

/*******************************************************************************
    Func Name: SetCurrentLine
 Date Created: 2010-10-15
       Author: lihaitao
  Description: ���ݵ�ǰ�У�����74HC138����ѡ�źš�
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
void SetCurrentLine(uchar iCurrentLine)
{
    LINE_D= iCurrentLine / 8;
    LINE_C= (iCurrentLine % 8) / 4;
    LINE_B= (iCurrentLine % 4) / 2;
    LINE_A= iCurrentLine % 2; 

/*  
    LINE_A = (iCurrentLine & 1) ? 1 : 0;
    LINE_B = (iCurrentLine & 2) ? 1 : 0;
    LINE_C = (iCurrentLine & 4) ? 1 : 0;
    LINE_D = (iCurrentLine & 8) ? 1 : 0;
*/

    return;
}

