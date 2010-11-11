#include <reg52.h>
#include "common_def.h"
#include "time_manager.h"


/* �Դ���Դ��С */
static uchar *g_bGraphMem;
static byte g_bGraphLen;

/* ȫ�ֱ�������¼ϵͳʱ�� */
stTime *g_Time;

/* ʱ��ģ��ĳ�ʼ�� */
void TMI_Init(byte pbGraphMem[], byte bGraphLen, stTime *time)
{
    /* 1. ��ȡ�Դ档 */
    g_bGraphMem = pbGraphMem;
    g_bGraphLen = bGraphLen;

    /* 2. ��ȡϵͳ��ǰʱ�� */
    g_Time = time;
    
    return;
}

/* ����ǰ��ϵͳʱ��ˢ�µ���Ļ�� */
void TMI_Refresh(void)
{
    /* 1. ����ǰ��ϵͳʱ��ת��Ϊ�ַ����� */

    /* 2.ͨ�����ڷ������ݵ�74HC595 */
    
    return;
}

/* ʱ����ʾ��ϵͳʱ������1s */
void TMI_GetCurrentTime(stTime *time)
{
    (time->second)++;
    
    if (TIME_60 == time->second)
    {
        time->second = 0;
        time->minite++;

        if (TIME_60 == time->minite)
        {
            time->minite = 0;
            time->hour++;

            if (TIME_24 == time->hour)
            {
                time->second = 0;
                time->minite = 0;
                time->hour = 0;

                BEEP = 0;  /* ��ҹ��ʱ���������� */
            }
        }
    }

    return;
}

/* ����ʱ��ϵͳʱ�����1s */
void TMI_GetRemainTime(stTime *time)
{
    (time->second)--;
    
    if (TIME_0 > time->second)
    {
        time->second = TIME_60 -1;
        time->minite--;

        if (TIME_0 > time->minite)
        {
            time->minite = TIME_60 -1;
            time->hour--;

            if (TIME_0 > time->hour)
            {
                BEEP = 0;  /* ����ʱʱ�䵽���������� */
            }
        }
    }

    return;
}
