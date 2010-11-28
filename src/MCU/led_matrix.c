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
//#include "commondriver/iic_driver.h"
#include "commondriver/keyboard_driver.h"
#include "commondriver/serial_driver.h"
#include "painter.h"
//#include "com_driver.h"
#include "time_manager.h"

/********************************************************************/
/* ȫ�ֱ������� */
idata byte graphData[LED_LINE][LED_ROW];  /* �Դ����� */
eMode currentMode;          /* ��ǰģʽ */
bit isEditing;                  /* �Ƿ������޸�״̬ */
bit needUpdate;                 /* ��־�Ƿ���Ҫ���� */
bit drawingLine;                /* ��־�Ƿ����ڻ��� */

uchar blinkTopLeft;          /* ��˸��������Ͻ� */
uchar blinkBottomRight;      /* ��˸��������½� */
uchar cursor;                /* ��ǰ���λ�� */
uchar preCursor;             /* ��ǰ���λ�� */
uchar firstPoint;            /* ֱ�ߵĵ�һ�� */

stTime editingTime;          /* �����޸ĵ�ʱ�� */
e_TimeUnit g_CurrentTimeUnit;      /* ���ڱ༭��ʱ�䵥Ԫ */

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

/* ��ʼ��ȫ�ֱ��� */
void InitGlobal(void)
{
    currentMode = MODE_STATIC;
    LED1 = 0;
    LED2 = 1;
    LED3 = 1;
    
    isEditing = 0;
    needUpdate = 0;
    drawingLine = 0;

    /* ʱ�� */
    editingTime.hour = 0;
    editingTime.minute = 0;
    editingTime.second = 0;

    g_CurrentTimeUnit = HOUR;

    return;
}

/* ��ʼ����ʱ��ΪTIME_TIMER
 */
void InitTimer(void)
{
    TMOD = 0x01;     /* T0/T1,mode1 */ 
    TH0 = 0xfc;      /* T0������ֵ����ʱ2ms */
    TL0 = 0x18;      /* T0����ˢ�¼��� */

    return;
}

void InitInte(void)
{
    EA = 1;          /* ���ж� */
    ET0 = 1;         /* ����T0�ж� */
    EX0 = 1;         /* �����ⲿ�ж�0 */

    return;
}


/*
 * ��ʼ���Դ�
 * ��EE�ж�ȡ�������ݵ��ڴ�
 */
void InitGraphMem(void)
{    
    byte x,y;

    for (y=0; y < LED_LINE; y++)
    {
        for (x=0; x < LED_ROW; x++)
        {
            graphData[y][x] = 0xff;
        }
    }
    /* ��EE�ж�ȡ�������ݵ��ڴ� */
    /* graphData = ; */
    
    return;
}

/* ȫ�ֳ�ʼ������ */
void InitMain(void)
{    
    /* ��ʼ��ȫ�ֱ��� */
    InitGlobal();
    
    /* ��ʼ��ȫ�ֶ�ʱ�� */
    InitTimer();

    /* �жϳ�ʼ�� */
    InitInte();    
    
    /* ��ʼ���Դ� */
    InitGraphMem();
    
    /* ��ʼ���������� */
    KDI_Init();
    
    /* ��ʼ���Կ����� */
    GDI_Init(graphData, LED_MEM);   

    /* ��ʼ�� */
    PainterInit(graphData, LED_ROW);
    
    /* ��ʼ��ʱ�����ģ�� */
    TMI_Init(graphData, LED_MEM, &editingTime, &currentMode);

    return;
}

void Blink(void)
{
    switch (currentMode)
    {
        case MODE_STATIC:
            /* ʹ�����˸ */
            break;
        case MODE_TIMER:    /* ��counterʹ��ͬһ�����º��� */
        case MODE_COUNTER:
            /* ʹʱ����˸ */
            break;
        default:
            /* ����˸ */
            break;
    }

    return;
}

/********************************************************************/
/* �жϺ��� */

/*
 * ��ʱ���жϺ�����1ms��
 */
void OnTimer(void) interrupt 1
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
        KDI_Scan();
    }
    if (--tcTCount == 0)
    {
        tcTCount= TIME_TIMECOUNT/TIME_TIMER;
        /* ����ʱ�����ģ��ĸ���ʱ�亯�� */
        TMI_Refresh();
    }
    if (--efTCount == 0)
    {
        efTCount= TIME_EFFECTS/TIME_TIMER;

        /* �Ǳ༭״̬, ��༭״̬�µ���˸��Ч */
        if (isEditing)
        {
            /* ���ñ�ģ�����˸���� */
			Blink();
        }
    }

    return;
}

/* ��LED��ָʾ��ǰ����״̬ */
static void ModeState(eMode mode)
{
    switch (mode)
    {
        case MODE_STATIC:
            LED1 = 0;
            LED2 = 1;
            LED3 = 1;
            break;
        case MODE_TIMER:
            LED1 = 1;
            LED2 = 0;
            LED3 = 1;
            break;
        case MODE_COUNTER:
            LED1 = 1;
            LED2 = 1;
            LED3 = 0;
            break;
        case MODE_DOWNLOAD:
            LED1 = 0;
            LED1 = 0;
            LED1 = 0;
            break;
        default:
            break;
    }
    return;
}

/* �ⲿ�ж�0 */
void ExtInt0(void) interrupt 0
{
    /* ѭ���л�ģʽ */
    if (++currentMode == MODE_COUNT)
    {
        currentMode= MODE_STATIC;
    }
    ModeState(currentMode);

    return;
}


/********************************************************************/
/* ���ܺ��� */
void SwitchTimeUnit(bit goRight)
{
    if (goRight)
    {
        g_CurrentTimeUnit++;
        if (UNITCOUNT == g_CurrentTimeUnit)
        {
            g_CurrentTimeUnit = HOUR;
        }
    }
    else
    {
        if (HOUR == g_CurrentTimeUnit)
        {

            g_CurrentTimeUnit = UNITCOUNT;
        }
        g_CurrentTimeUnit--;
    }    

    return;
}

void AddCurUnitTime(bit incCurUnit)
{
    if (incCurUnit)
    {
        switch (g_CurrentTimeUnit)
        {
            case HOUR:
                editingTime.hour++;
                if (TIME_24 == editingTime.hour)
                {
                    editingTime.hour= 0;
                }
                break;
            case MINUTE:
                editingTime.minute++;
                if (TIME_60 == editingTime.minute)
                {
                    editingTime.minute = 0;
                }
                break;
            case SECOND:
                editingTime.second++;
                if (TIME_60 == editingTime.second)
                {
                    editingTime.second = 0;
                }
                break;
            default:
                break;
        }
    }
    else
    {
        switch (g_CurrentTimeUnit)
        {
            case HOUR:
                if (TIME_0 == editingTime.hour)
                {
                    editingTime.hour= TIME_24 - 1;
                }
                editingTime.hour--;
                break;
            case MINUTE:
                if (TIME_0 == editingTime.minute)
                {
                    editingTime.minute= TIME_60 - 1;
                }
                editingTime.minute--;
                break;
            case SECOND:
                if (TIME_0 == editingTime.second)
                {
                    editingTime.second = TIME_60 - 1;
                }
                editingTime.second--;
                break;
            default:
                break;
        }
    }

    return;
}

/*
 * �����¼�
 */
void KeyEvent(byte curKey, EKeyEventKind event)
{
    switch (curKey)
    {
        case KEY_YES:  //KEY_SAVE
            if (isEditing)
            {
                isEditing= 0;       /* �Դ�����ʵʱ���£�ֱ�Ӹı�״̬���� */
            }
            else
            {
                /* ����ǰ�Դ�д��EE��Ϊȡ��������׼�� */
                isEditing= 1;
            }
            break;
        case KEY_NO:  //KEY_CANCEL
            if (isEditing)
            {
                /* ȡ��д��EE���Դ����� */
                isEditing= 0;
            }
            break;
        case KEY_OK:
            if (MODE_STATIC == currentMode)
            {
                if (drawingLine)
                {
                    drawingLine = 0;
                    /* ֱ�ӻ��� */
                }
                else
                {
                    drawingLine = 1;
                }
            }
            else
            {
                //SwitchTimeUnit(/* ���� */true);
                isEditing = 0;  /* �˳��༭״̬ */
            }
            break;
        case KEY_UP:
            if (MODE_STATIC == currentMode)
            {
                //CurcorMove(/* ���ϵ�λʸ�� */) : AddCurUnitTime(/* ����ʱ�� */true);
                AddCurUnitTime(TURE);
            }
            break;
        case KEY_DOWN:
            if (MODE_STATIC == currentMode)
            {
                //CurcorMove(/* ���µ�λʸ�� */) : AddCurUnitTime(/* ����ʱ�� */false);
                AddCurUnitTime(FALSE);
            }
            break;
        case KEY_LEFT:            
            if (MODE_STATIC == currentMode)
            {
                //CurcorMove(/* ����λʸ�� */) : SwitchTimeUnit(/* ���� */false);
                SwitchTimeUnit(FALSE);
            }
            break;
        case KEY_RIGHT:
            if (MODE_STATIC == currentMode)
            {
                //CurcorMove(/* ���ҵ�λʸ�� */) : SwitchTimeUnit(/* ���� */true);
                SwitchTimeUnit(TURE);
            }
            break;
        default:
            break;
    }

    return;
}


/*
 * ����ͼ��
 * ����Ǳ༭ģʽ�����ƹ�꣬����ʱ����ֱ��
 */
void UpdatePaint(void)
{
    if (isEditing)
    {
        if (drawingLine)
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

    return;
}

/*
 * ����ʱ�亯��
 * ����ǷǱ༭״̬�����Ƶ�ǰ/ʣ��ʱ��
 * ����Ǳ༭״̬���������ڸ��ĵ�ʱ��
 */
void UpdateTime()
{
    byte disArray[LED_ROW];
	
	/* ת��ʱ��Ϊ�ַ����� */
    disArray[0] = (byte)editingTime.hour/10 + LIB_NUM_0;
    disArray[1] = (byte)editingTime.hour%10 + LIB_NUM_0;
    disArray[2] = LIB_COLON;
    disArray[3] = (byte)editingTime.minute/10 + LIB_NUM_0;
    disArray[4] = (byte)editingTime.minute%10 + LIB_NUM_0;
    disArray[5] = LIB_COLON;
    disArray[6] = (byte)editingTime.second/10 + LIB_NUM_0;
    disArray[7] = (byte)editingTime.second%10 + LIB_NUM_0;

	/* ��ʱ������д���Դ� */
    PainterDrawString(disArray, LED_ROW, 0, 0, PM_COPY);

    return;
}

void UpdateCom()
{
    byte length;

    /* ��ʼ���򿪴������� */
    SerialInit(graphData, LED_MEM, FALSE);
    
    /* �����ȡ������Ϣ�ɹ� */
    length = SerialRead(graphData, LED_MEM);
    if (length > 0)
    {
        /* �ر�ͨѶ */
        /* ��ʼ���Կ� */
        /* ����ģʽΪ��̬��ʾ */
        currentMode = MODE_STATIC;

        /* ����EE�Ľӿں��������ݴ浽EE */
    }

    /* ����Ϊ����ģʽ0���򿪴��ڴ������� */
    SerialInit(graphData, LED_MEM, TURE);

    return;
}

/*
 * ��������
 * ��Ҫ����ʱ���ô˺�������������Ҫ��ģʽ
 */
void UpdateData(void)
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

    /* ���нӿ�ӳ�䣬Ϊ��ʾ��׼�� */
    GDI_DisFormat();

    return;
}

void main()
{
    uchar ucCurrentKey = KEY_NULL;
    EKeyEventKind eKeyEvent = Nothing;
    
    /* ���ñ�ģ���ʼ������ */
    InitMain();

    while (1)
    {
        /* ���ü��������Ķ�ȡ��ǰ������currentKey */
        KDI_GetCurrentKey(&ucCurrentKey, &eKeyEvent);
        if (KEY_NULL != ucCurrentKey)
        {
            KeyEvent(ucCurrentKey, eKeyEvent);
        }

        UpdateData();
    }

    return;
}


