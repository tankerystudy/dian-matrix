#include <reg52.h>
#include "commondriver/common_def.h"
#include "time_manager.h"

/* �Դ���Դ��С */
static idata uchar *g_bGraphMem;
static byte g_bGraphLen;

/* ȫ�ֱ�������¼ϵͳʱ�� */
stTime *g_Time;
/* ��¼ϵͳ״̬ */
eMode *g_Mode;

/* ʱ��ģ��ĳ�ʼ�� */
void TMI_Init(byte pbGraphMem[], byte bGraphLen, stTime *time, eMode *mode)
{
    /* 1. ��ȡ�Դ档 */
    g_bGraphMem = pbGraphMem;
    g_bGraphLen = bGraphLen;

    /* 2. ��ȡϵͳ��ǰʱ�� */
    g_Time = time;

    /* 2. ��ȡϵͳ��ǰģʽ */
    g_Mode= mode;
    
    return;
}

/* ����ǰ��ϵͳʱ��ˢ�µ���Ļ�� */
void TMI_Refresh(void)
{    
    /* ���ݵ�ǰϵͳģʽ���ò�ͬ��ˢ�º��� */
    if (g_Mode == MODE_TIMER)
    {
        TMI_GetCurrentTime(g_Time);
    }
    else
    {
        TMI_GetRemainTime(g_Time);

    }
    
    return;
}

/* ʱ����ʾ��ϵͳʱ������1s */
void TMI_GetCurrentTime(stTime *time)
{
    (time->second)++;
    
    if (TIME_60 == time->second)
    {
        time->second = 0;
        time->minute++;

        if (TIME_60 == time->minute)
        {
            time->minute = 0;
            time->hour++;

            if (TIME_24 == time->hour)
            {
                time->second = 0;
                time->minute = 0;
                time->hour = 0;

                BEEP = 0;  /* ��ҹ��ʱ���������� */
                sleep(1);
                BEEP = 1;
            }
        }
    }

    return;
}

/* ����ʱ��ϵͳʱ�����1s */
void TMI_GetRemainTime(stTime *time)
{    
    if (TIME_0 == time->second)
    {
        if (TIME_0 == time->minute)
        {
            if (TIME_0 == time->hour)
            {
                BEEP = 0;  /* ����ʱʱ�䵽���������� */
                sleep(1);
                BEEP =1;
            }
            (time->hour)--;
            time->minute = TIME_60 -1;
        }
        (time->minute)--;
        time->second = TIME_60 -1;
    }
    (time->second)--;

    return;
}
