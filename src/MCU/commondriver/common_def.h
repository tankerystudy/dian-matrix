#ifndef DIAN_MATRIX_COMDEF_H
#define DIAN_MATRIX_COMDEF_H
#include <reg52.h>

typedef unsigned char uchar;
typedef uchar byte;

sfr16   RCAP2   = 0xCA;     /* timer2 initial count */

sbit    BEEP    = P1^0;
sbit    LED1    = P1^3;
sbit    LED2    = P1^2;
sbit    LED3    = P1^1;

/* for dian matrix & 51 board common test.
sbit    RED_LED = P2^0;
sbit    LED0    = P1^0;
sbit    LED1    = P1^3;
sbit    LED2    = P1^2;
sbit    LED3    = P1^1;
*/
/* for 51 board
sbit    RED_LED = P0^6;
sbit    LED0    = P1^0;
sbit    LED1    = P1^1;
sbit    LED2    = P1^2;
sbit    LED3    = P1^3;
*/

void sleep(int time);

#endif // #ifndef SEED51_COMDEF_H
