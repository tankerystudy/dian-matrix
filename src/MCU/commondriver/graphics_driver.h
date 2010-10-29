#ifndef __GRAPHICS_DRIVER_H__
#define __GRAPHICS_DRIVER_H__

#ifdef __cplusplus
    extern "C"{
#endif

#include "common_def.h"

#define LED_LINE  16
#define LED_ROW   8


void led_drv_DisInit(byte pbGraphMem[]);
void led_drv_InterfaceMap(byte *bData);
void led_drv_Refresh(void);

#ifdef __cplusplus
}
#endif

#endif
