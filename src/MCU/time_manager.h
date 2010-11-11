/* ʱ��ת��
   ÿ��1s����func()����ʱ�� 
*/
#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

#ifdef __cpluscplus
    extern "C" {
#endif

/* ʱ��Ľ������� */
#define TIME_0   0
#define TIME_60  60
#define TIME_24  24

typedef struct Time
{
    byte hour;
    byte minite;
    byte second;
}stTime;


void TMI_Init(byte pbGraphMem[], byte bGraphLen, stTime *time);
void TMI_Refresh(void);
void TMI_GetCurrentTime(stTime *time);
void TMI_GetRemainTime(stTime *time);


#ifdef __cplusplus
}
#endif

#endif
