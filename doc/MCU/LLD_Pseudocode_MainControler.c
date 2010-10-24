/*
 * MainControler.c
 *
 * �������س��򣬸�����������Ŀ��ƺ�ִ�С�main����Ҳ��������档
 *
 * Author:  tankery.chen@gmail.com
 *
 * Copyleft@2010, Coogle@DianGroup
 */
/* ͨ������Դ�ļ�����commondriver�ļ����� */
#include "commondriver/graphics_driver.h"
#include "commondriver/memory_driver.h"
#include "commondriver/keyboard_driver.h"
#include "painter.h"
#include "com_driver.h"
#include "time_manager.h"
#include <�����ļ�>




/********************************************************************/
/* ȫ�ֱ������� */
enum Mode   {                   /* ģʽ���� */
    MODE_STATIC = 0,
    MODE_TIMER,
    MODE_COUNTER,
    MODE_DOWNLOAD,
    MODE_COUNT
};


byte graphData[128];            /* �Դ����� */
enum Mode currentMode;          /* ��ǰģʽ */
bit isEditing;                  /* �Ƿ������޸�״̬ */
bit needUpdate;                 /* ��־�Ƿ���Ҫ���� */
bit drawingLine;                /* ��־�Ƿ����ڻ��� */

Location blinkTopLeft;          /* ��˸��������Ͻ� */
Location blinkBottomRight;      /* ��˸��������½� */
Location cursor;                /* ��ǰ���λ�� */
Location preCursor;             /* ��ǰ���λ�� */
Location firstPoint;            /* ֱ�ߵĵ�һ�� */

structTime editingTime;         /* �����޸ĵ�ʱ�� */

#define TIME_TIMER      1       /* ���嶨ʱ��ʱ�� */
/* ��ʼ����ʱ��Ϊ2ms
 * ��������ʹ��Ļ��ˢ��ʱ��Ϊ2*16= 32ms
 * Լ31֡/s
 */
#define TIME_REFRESH    2       /* ���Կ������� *//* ������ɨ��ʱ��Ϊ2ms */
#define TIME_KEYSCAN    20      /* �ڼ��������� *//* �������ɨ��ʱ����20ms */
#define TIME_EFFECTS    400     /* �ڻ�ͼģ���� *//* ������Ч�ı�ʱ��Ϊ400ms */
#define TIME_TIMECOUNT  1000    /* ��ʱ�����ģ���� *//* ����̨ǩʱ�����ʱ��Ϊ1s����̨ǩʱ�侫ȷ����*/


/********************************************************************/
/* ��ʼ������ */

/* ��ʼ����ʱ��ΪTIME_TIMER
 */
void InitTimer();

/*
 * ��ʼ���Դ�
 * ��EE�ж�ȡ�������ݵ��ڴ�
 */
void InitGraphMem();


/********************************************************************/
/* �жϺ��� */

/*
 * ��ʱ���жϺ�����1ms��
 */
void OnTimer()
{
    static byte reTCount= TIME_REFRESH/TIME_TIMER;      /* ��ˢ��ʱ�����*/
    static byte ksTCount= TIME_KEYSCAN/TIME_TIMER;      /* ����ɨ��ʱ����� */
    static byte tcTCount= TIME_TIMECOUNT/TIME_TIMER;    /* ̨ǩʱ�����ʱ����� */
    static byte efTCount= TIME_EFFECTS/TIME_TIMER;      /* ��Ч����ʱ����� */

    if (--reTCount == 0)
    {
        reTCount= TIME_REFRESH/TIME_TIMER;
        /* �����Կ�������ˢ���к��� */
        GDI_Refresh();
    }
    if (--ksTCount == 0)
    {
        ksTCount= TIME_KEYSCAN/TIME_TIMER;
        /* ���ü��������ļ���ɨ�躯�� */
    }
    if (--tcTCount == 0)
    {
        tcTCount= TIME_TIMECOUNT/TIME_TIMER;
        /* ����ʱ�����ģ��ĸ���ʱ�亯�� */
    }
    if (--efTCount == 0)
    {
        efTCount= TIME_EFFECTS/TIME_TIMER;

        /* �༭״̬�µ���˸��Ч */
        if (/* �Ǳ༭״̬ */)
            /* ���ñ�ģ�����˸���� */
    }

}

/* �ⲿ�ж�0 */
void ExtInt0()
{
    /* ѭ���л�ģʽ */
    if (++currentMode == MODE_COUNT)
        currentMode= MODE_STATIC;
}


/********************************************************************/
/* ���ܺ��� */


/*
 * �����¼�
 */
void KeyEvent(byte curKey, enumKeyEvent event)
{
    switch (curKey)
    {
        case KEY_SAVE:
            if (isEditing)
                isEditing= 0;       /* �Դ�����ʵʱ���£�ֱ�Ӹı�״̬���� */
            else
            {
                /* ����ǰ�Դ�д��EE��Ϊȡ��������׼�� */
                isEditing= 1;
            }
            break;
        case KEY_CANCEL:
            if (isEditing)
            {
                /* ȡ��д��EE���Դ����� */
                isEditing= 0;
            }
            break;
        case KEY_OK:
            if (currentMode == MODE_STATIC)
            {
                if (drawingLine)
                {
                    drawingLine = 0;
                    /* ֱ�ӻ��� */
                }
                else
                    drawingLine = 1;
            }
            else
                SwitchTimeUnit(/* ���� */true);
            break;
        case KEY_UP:
            currentMode == MODE_STATIC ?
                CurcorMove(/* ���ϵ�λʸ�� */) : AddCurUnitTime(/* ����ʱ�� */true);
            break;
        case KEY_DOWN:
            currentMode == MODE_STATIC ?
                CurcorMove(/* ���µ�λʸ�� */) : AddCurUnitTime(/* ����ʱ�� */false);
            break;
        case KEY_LEFT:
            currentMode == MODE_STATIC ?
                CurcorMove(/* ����λʸ�� */) : SwitchTimeUnit(/* ���� */false);
            break;
        case KEY_RIGHT:
            currentMode == MODE_STATIC ?
                CurcorMove(/* ���ҵ�λʸ�� */) : SwitchTimeUnit(/* ���� */true);
            break;
        default:
            break;
    }
}

/*
 * ����״̬����
 */
void ChangeState(bit saving)
{
    if (isEditing)
    {
        if (saving)
            ;
        else if (/* û��ȷ��ȡ�� */)
            return;

        /* �л�״̬ */
    }
    else
        /* �л����༭״̬ */
}

/*
 * ����ͼ��
 * ����Ǳ༭ģʽ�����ƹ�꣬����ʱ����ֱ��
 */
void UpdatePaint()
{
    if (isEditing)
    {
        if (/* �ڻ�ֱ�� */)
        {
            /* ʹ��ȡ�����ǰһ���� */
            /* ʹ��ȡ��ģʽ�����ڵ��� */
            preCursor= cursor;
        }
        else
        {
            /* ʹ��ȡ�����ǰһ��� */
            /* ʹ��ȡ�����Ƶ�ǰ��� */
            preCursor= cursor;
        }
    }
}

/*
 * ����ʱ�亯��
 * ����ǷǱ༭״̬�����Ƶ�ǰ/ʣ��ʱ��
 * ����Ǳ༭״̬���������ڸ��ĵ�ʱ��
 */
void UpdateTime()
{
    DisChar disArray[8];

    if (!isEditing)
    {
        if (currentMode == MODE_TIMER)
            TMI_GetCurrentTime(&editingTime);
        else
            TMI_GetRemainTime(&editingTime);
    }
    disArray[0] = (byte)editingTime.hour/10 + DIS_NUM0;
    disArray[1] = (byte)editingTime.hour%10 + DIS_NUM0;
    disArray[2] = DIS_COLON;
    disArray[3] = (byte)editingTime.minute/10 + DIS_NUM0;
    disArray[4] = (byte)editingTime.minute%10 + DIS_NUM0;
    disArray[5] = DIS_COLON;
    disArray[6] = (byte)editingTime.second/10 + DIS_NUM0;
    disArray[7] = (byte)editingTime.second%10 + DIS_NUM0;

    GDI_DrawCharArray(disArray);
}

void UpdateCom()
{
    /* �����ȡ������Ϣ�ɹ� */
    if (CDI_SerialRead(graphData) > 0)
    {
        /* �ر�ͨѶ */
        /* ��ʼ���Կ� */
        /* ����ģʽΪ��̬��ʾ */
    }
}

/*
 * ��������
 * ��Ҫ����ʱ���ô˺�������������Ҫ��ģʽ
 */
void UpdateData()
{
    switch (currentMode)
    {
        case MODE_STATIC:
            /* ���»�ͼ */
            UpdatePaint();
            break;
        case MODE_TIMER:    /* ��counterʹ��ͬһ�����º��� */
        case MODE_COUNTER:
            /* ���ø���ʱ�亯�� */
            UpdateTime();
            break;
        case MODE_DOWNLOAD:
            UpdateCom();
            break;
        default:
            /* ������ */
            break;
    }
}

void main()
{
    /* ���ñ�ģ���ʼ������ */
    /* ��ʼ���Կ����� */
    void GDI_Init(graphData);
    /* ��ʼ��ʱ�����ģ�� */

    while (1)
    {
        /* ���ü��������Ķ�ȡ��ǰ������currentKey */
        KDI_GetCurrentKey(&currentKey, &event);
        if (/* �кϷ����� */)
            KeyEvent(currentKey, event);
    }
}


