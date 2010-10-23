#ifndef __GRAPHICS_DRIVER_H__
#define __GRAPHICS_DRIVER_H__

#ifdef __cplusplus
    extern "C"{
#endif

#define IN 
#define INOUT
#define OUT

#define LED_LINE  16
#define LED_ROW   8

typedef unsigned char uchar;
typedef uchar byte;


void led_drv_DisInit(byte *pbGraphMem, byte bGraphLen);
void led_drv_InterfaceMap(INOUT byte *bData);
void led_drv_Refresh(void);


#ifdef __cplusplus
}
#endif

#endif
