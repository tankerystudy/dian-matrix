/*
 * MainControler.c
 *
 * 这是主控程序，负责整个程序的控制和执行。main函数也在这个里面。
 *
 * Author:  tankery.chen@gmail.com
 *
 * Copyleft@2010, Coogle@DianGroup
 */
/* 通用驱动源文件放在commondriver文件夹中 */
#include "commondriver/graphics_driver.h"
#include "commondriver/memory_driver.h"
#include "commondriver/keyboard_driver.h"
#include "painter.h"
#include "com_driver.h"
#include "time_manager.h"
#include <其他文件>




/********************************************************************/
/* 全局变量定义 */
enum Mode   {                   /* 模式种类 */
    MODE_STATIC = 0,
    MODE_TIMER,
    MODE_COUNTER,
    MODE_DOWNLOAD,
    MODE_COUNT
};


byte graphData[128];            /* 显存数据 */
enum Mode currentMode;          /* 当前模式 */
bit isEditing;                  /* 是否正在修改状态 */
bit needUpdate;                 /* 标志是否需要更新 */
bit drawingLine;                /* 标志是否正在画线 */

Location blinkTopLeft;          /* 闪烁区域的左上角 */
Location blinkBottomRight;      /* 闪烁区域的右下角 */
Location cursor;                /* 当前光标位置 */
Location preCursor;             /* 先前光标位置 */
Location firstPoint;            /* 直线的第一点 */

structTime editingTime;         /* 正在修改的时间 */

#define TIME_TIMER      1       /* 定义定时器时间 */
/* 初始化定时器为2ms
 * 这样可以使屏幕的刷新时间为2*16= 32ms
 * 约31帧/s
 */
#define TIME_REFRESH    2       /* 在显卡驱动中 *//* 定义行扫描时间为2ms */
#define TIME_KEYSCAN    20      /* 在键盘驱动中 *//* 定义键盘扫描时间间隔20ms */
#define TIME_EFFECTS    400     /* 在绘图模块中 *//* 定义特效改变时间为400ms */
#define TIME_TIMECOUNT  1000    /* 在时间管理模块中 *//* 定义台签时间更新时间为1s，即台签时间精确到秒*/


/********************************************************************/
/* 初始化函数 */

/* 初始化定时器为TIME_TIMER
 */
void InitTimer();

/*
 * 初始化显存
 * 从EE中读取点阵数据到内存
 */
void InitGraphMem();


/********************************************************************/
/* 中断函数 */

/*
 * 定时器中断函数（1ms）
 */
void OnTimer()
{
    static byte reTCount= TIME_REFRESH/TIME_TIMER;      /* 行刷新时间计数*/
    static byte ksTCount= TIME_KEYSCAN/TIME_TIMER;      /* 键盘扫描时间计数 */
    static byte tcTCount= TIME_TIMECOUNT/TIME_TIMER;    /* 台签时间更新时间计数 */
    static byte efTCount= TIME_EFFECTS/TIME_TIMER;      /* 特效更改时间计数 */

    if (--reTCount == 0)
    {
        reTCount= TIME_REFRESH/TIME_TIMER;
        /* 调用显卡驱动的刷新行函数 */
        GDI_Refresh();
    }
    if (--ksTCount == 0)
    {
        ksTCount= TIME_KEYSCAN/TIME_TIMER;
        /* 调用键盘驱动的键盘扫描函数 */
    }
    if (--tcTCount == 0)
    {
        tcTCount= TIME_TIMECOUNT/TIME_TIMER;
        /* 调用时间管理模块的更新时间函数 */
    }
    if (--efTCount == 0)
    {
        efTCount= TIME_EFFECTS/TIME_TIMER;

        /* 编辑状态下的闪烁特效 */
        if (/* 是编辑状态 */)
            /* 调用本模块的闪烁函数 */
    }

}

/* 外部中断0 */
void ExtInt0()
{
    /* 循环切换模式 */
    if (++currentMode == MODE_COUNT)
        currentMode= MODE_STATIC;
}


/********************************************************************/
/* 功能函数 */


/*
 * 按键事件
 */
void KeyEvent(byte curKey, enumKeyEvent event)
{
    switch (curKey)
    {
        case KEY_SAVE:
            if (isEditing)
                isEditing= 0;       /* 显存数据实时更新，直接改变状态即可 */
            else
            {
                /* 将当前显存写入EE，为取消操作做准备 */
                isEditing= 1;
            }
            break;
        case KEY_CANCEL:
            if (isEditing)
            {
                /* 取回写入EE的显存数据 */
                isEditing= 0;
            }
            break;
        case KEY_OK:
            if (currentMode == MODE_STATIC)
            {
                if (drawingLine)
                {
                    drawingLine = 0;
                    /* 直接画线 */
                }
                else
                    drawingLine = 1;
            }
            else
                SwitchTimeUnit(/* 右移 */true);
            break;
        case KEY_UP:
            currentMode == MODE_STATIC ?
                CurcorMove(/* 向上单位矢量 */) : AddCurUnitTime(/* 增加时间 */true);
            break;
        case KEY_DOWN:
            currentMode == MODE_STATIC ?
                CurcorMove(/* 向下单位矢量 */) : AddCurUnitTime(/* 减少时间 */false);
            break;
        case KEY_LEFT:
            currentMode == MODE_STATIC ?
                CurcorMove(/* 向左单位矢量 */) : SwitchTimeUnit(/* 左移 */false);
            break;
        case KEY_RIGHT:
            currentMode == MODE_STATIC ?
                CurcorMove(/* 向右单位矢量 */) : SwitchTimeUnit(/* 右移 */true);
            break;
        default:
            break;
    }
}

/*
 * 更改状态函数
 */
void ChangeState(bit saving)
{
    if (isEditing)
    {
        if (saving)
            ;
        else if (/* 没有确定取消 */)
            return;

        /* 切换状态 */
    }
    else
        /* 切换到编辑状态 */
}

/*
 * 更新图像
 * 如果是编辑模式，绘制光标，画线时绘制直线
 */
void UpdatePaint()
{
    if (isEditing)
    {
        if (/* 在画直线 */)
        {
            /* 使用取反清除前一条线 */
            /* 使用取反模式画现在的线 */
            preCursor= cursor;
        }
        else
        {
            /* 使用取反清除前一光标 */
            /* 使用取反绘制当前光标 */
            preCursor= cursor;
        }
    }
}

/*
 * 更新时间函数
 * 如果是非编辑状态，绘制当前/剩余时间
 * 如果是编辑状态，绘制正在更改的时间
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
    /* 如果读取下载信息成功 */
    if (CDI_SerialRead(graphData) > 0)
    {
        /* 关闭通讯 */
        /* 初始化显卡 */
        /* 设置模式为静态显示 */
    }
}

/*
 * 更新所有
 * 需要更新时调用此函数，将更新需要的模式
 */
void UpdateData()
{
    switch (currentMode)
    {
        case MODE_STATIC:
            /* 更新绘图 */
            UpdatePaint();
            break;
        case MODE_TIMER:    /* 与counter使用同一个更新函数 */
        case MODE_COUNTER:
            /* 调用更新时间函数 */
            UpdateTime();
            break;
        case MODE_DOWNLOAD:
            UpdateCom();
            break;
        default:
            /* 错误处理 */
            break;
    }
}

void main()
{
    /* 调用本模块初始化函数 */
    /* 初始化显卡驱动 */
    void GDI_Init(graphData);
    /* 初始化时间管理模块 */

    while (1)
    {
        /* 调用键盘驱动的读取当前按键到currentKey */
        KDI_GetCurrentKey(&currentKey, &event);
        if (/* 有合法按键 */)
            KeyEvent(currentKey, event);
    }
}


