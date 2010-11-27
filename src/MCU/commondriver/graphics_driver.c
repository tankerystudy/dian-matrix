#include <reg52.h>
#include "common_def.h"
#include "graphics_driver.h"
#include "serial_driver.h"

/* ȫ�ֱ������� */
/* ��ʾ���IO�ڶ��� */
sbit LINE_A = P2^4;
sbit LINE_B = P2^3;
sbit LINE_C = P2^2;
sbit LINE_D = P2^1;
sbit LINE_EN = P2^0;

/* �Ĵ���ʹ�ܣ��ߵ�ƽ���� */
sbit RES_EN = P1^4;
/* �Ĵ���ʱ���߿��� */
sbit RES_CLK = P3^3;

/* �Դ���Դ��С */
static idata uchar *g_bGraphMem;
static byte g_bGraphLen;

//static void LineRefresh(byte *bData, uchar iCurrentLine);
static void SetCurrentLine(uchar iCurrentLine);
//static void ByteMapping(byte *bData);


/* �Կ����� */

/*******************************************************************************
    Func Name: GDI_Init
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
  2010-11-10  Tankery          ��ӼĴ������Ƽ��Ĵ���ʱ���߿���

*******************************************************************************/
void GDI_Init(byte idata *pbGraphMem, byte bGraphLen)
{
    /* 1.ʹ��74HC38�� */
    LINE_EN = 0;    /* �͵�ƽʹ�� */

    /* 2.���üĴ��� */
    RES_EN = 1;     /* �ߵ�ƽʹ�� */
    RES_CLK = 0;    /* �ͷ�ʱ���߿���Ȩ */

    /* 3.��ȡ�Դ档 */
    g_bGraphMem = pbGraphMem;
    g_bGraphLen = bGraphLen;

    /* 4.���ô���ģʽ0�����ڴ������ݡ� */
    /* ���ô��������Ľӿ� */
    SerialInit(g_bGraphMem, g_bGraphLen, 1);
	LED3 = 0;
		while(1);

  return;
}

/*******************************************************************************
    Func Name: GDI_ByteMapping
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
  Description: ����ByteMapping��������ʽ�������Դ�����
        Input: none
       Output: none
       Return: void
      Caution: ��
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
    /* ��ǰ��ʾ������ */
    static byte currentLine = 0;
    /* ˢ��һ�� */
    LineRefresh((g_bGraphMem + currentLine * LED_ROW), currentLine);

    /* ���µ�ǰ�� */
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
  2010-11-10  Tankery          ˢ��ǰ�ر���ʾ�����������
  2010-11-10  Tankery          ����һ��ʱ�����ڣ�ʹ��ȷ��������

*******************************************************************************/
void LineRefresh(byte *bData, uchar iCurrentLine)
{
    /* �ر����� */
    LINE_EN = 1;

    /* ͨ�����ڷ������ݵ�74HC595 */
    SerialWrite(bData, LED_ROW);
    /* ����Ӳ�����ʧ�󣬲���һ��ʱ�����ڣ�ʹ��ȷ�������� */
    RES_EN = 0;
    RES_CLK = 1;
    RES_EN = 1;
    RES_CLK = 0;

    /* ��ѡ */
    SetCurrentLine(iCurrentLine);
    
    /* ������ */
    LINE_EN = 0;

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
static void SetCurrentLine(uchar iCurrentLine)
{
    LINE_A = (iCurrentLine & 1) ? 1 : 0;
    LINE_B = (iCurrentLine & 2) ? 1 : 0;
    LINE_C = (iCurrentLine & 4) ? 1 : 0;
    LINE_D = (iCurrentLine & 8) ? 1 : 0;

    return;
}

