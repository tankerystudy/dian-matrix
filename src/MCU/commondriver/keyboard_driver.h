#ifndef __KEYBOARD_DRIVER_H__
#define __KEYBOARD_DRIVER_H__

#ifdef __cplusplus
    extern "C"{
#endif

#include "common_def.h"

/* define the key value */
#define KEY_NULL   0
#define KEY_LEFT   1
#define KEY_OK     2
#define KEY_RIGHT  3
#define KEY_YES    4
#define KEY_DOWN   5
#define KEY_UP     6
#define KEY_NO     7

typedef enum EnumKeyEventKind
{
    SingleClick,   
    DoubleClick,
    LongPress,
    Nothing
}EKeyEventKind;

typedef struct StructKeyEvent 
{
    EKeyEventKind eEventKind;
    unsigned char ucKeyNum;
}SKeyEvent;

void KDI_Init(void);
void KDI_Scan(void);
void KDI_GetCurrentKey(uchar *CurrentKey, uchar *KeyEvent);


#ifdef __cplusplus
}
#endif

#endif
