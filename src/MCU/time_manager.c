#include <reg52.h>
#include "commondriver/common_def.h"
#include "time_manager.h"

/* 显存和显存大小 */
static idata uchar *g_bGraphMem;
static byte g_bGraphLen;

/* 全局变量，记录系统时间 */
stTime *g_Time;
/* 记录系统状态 */
eMode *g_Mode;

/* 时间模块的初始化 */
void TMI_Init(byte pbGraphMem[], byte bGraphLen, stTime *time, eMode *mode)
{
    /* 1. 获取显存。 */
    g_bGraphMem = pbGraphMem;
    g_bGraphLen = bGraphLen;

    /* 2. 获取系统当前时间 */
    g_Time = time;

    /* 2. 获取系统当前模式 */
    g_Mode= mode;
    
    return;
}

/* 将当前的系统时间刷新到屏幕上 */
void TMI_Refresh(void)
{    
    /* 根据当前系统模式调用不同的刷新函数 */
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

/* 时钟显示，系统时间增加1s */
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

                BEEP = 0;  /* 午夜报时，蜂鸣器响 */
                sleep(1);
                BEEP = 1;
            }
        }
    }

    return;
}

/* 倒计时，系统时间减少1s */
void TMI_GetRemainTime(stTime *time)
{    
    if (TIME_0 == time->second)
    {
        if (TIME_0 == time->minute)
        {
            if (TIME_0 == time->hour)
            {
                BEEP = 0;  /* 倒计时时间到，蜂鸣器响 */
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
