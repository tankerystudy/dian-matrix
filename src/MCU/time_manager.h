/* 时间转换
   每隔1s调用func()增加时间 
*/
#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#ifdef __cpluscplus
    extern "C" {
#endif

#include "commondriver/common_def.h"

/* 时间的进制设置 */
#define TIME_0   0
#define TIME_60  60
#define TIME_12  12
#define TIME_24  24

typedef struct Time
{
    byte hour;
    byte minute;
    byte second;
}stTime;

typedef enum timeunit
{
    HOUR = 0,
    MINUTE,
    SECOND,
    UNITCOUNT
}e_TimeUnit;


void TMI_Init(byte pbGraphMem[], byte bGraphLen, stTime *time, eMode *mode);
void TMI_Refresh(void);
void TMI_GetCurrentTime(stTime *time);
void TMI_GetRemainTime(stTime *time);


#ifdef __cplusplus
}
#endif

#endif
