#ifndef __GRAPHICS_DRIVER_H__
#define __GRAPHICS_DRIVER_H__

#ifdef __cplusplus
    extern "C"{
#endif

#include "common_def.h"

#define LED_LINE  16
#define LED_ROW   4
#define LED_MEM   (LED_LINE * LED_ROW)


void GDI_Init(uchar pbGraphMem[], byte bGraphLen);
void GDI_ByteMapping(byte *bData);
void GDI_DisFormat(void);
void GDI_Refresh(void);


#ifdef __cplusplus
}
#endif

#endif  // #ifndef __GRAPHICS_DRIVER_H__
