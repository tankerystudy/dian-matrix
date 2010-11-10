#ifndef __GRAPHICS_DRIVER_H__
#define __GRAPHICS_DRIVER_H__

#ifdef __cplusplus
    extern "C"{
#endif

#include "common_def.h"

#define LED_LINE  16
#define LED_ROW   4


void led_drv_DisInit(uchar pbGraphMem[], byte bGraphLen);
void led_drv_InterfaceMap(uchar *bData);
void led_drv_DisFormat(void);
void led_drv_Refresh(void);

#ifdef __cplusplus
}
#endif

#endif  // #ifndef __GRAPHICS_DRIVER_H__
