#include <reg52.h>
#include "common_def.h"
#include "time_manager.h"


/* 显存和显存大小 */
static uchar *g_bGraphMem;
static byte g_bGraphLen;

/* 全局变量，记录系统时间 */
stTime *g_Time;

/* 时间模块的初始化 */
void TMI_Init(byte pbGraphMem[], byte bGraphLen, stTime *time)
{
    /* 1. 获取显存。 */
    g_bGraphMem = pbGraphMem;
    g_bGraphLen = bGraphLen;

    /* 2. 获取系统当前时间 */
    g_Time = time;
    
    return;
}

/* 将当前的系统时间刷新到屏幕上 */
void TMI_Refresh(void)
{
    /* 1. 将当前的系统时间转换为字符编码 */

    /* 2.通过串口发送数据到74HC595 */
    
    return;
}

/* 时钟显示，系统时间增加1s */
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

                BEEP = 0;  /* 午夜报时，蜂鸣器响 */
            }
        }
    }

    return;
}

/* 倒计时，系统时间减少1s */
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
                BEEP = 0;  /* 倒计时时间到，蜂鸣器响 */
            }
        }
    }

    return;
}
