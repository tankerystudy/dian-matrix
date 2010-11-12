#ifndef DIAN_MATRIX_COMDEF_H
#define DIAN_MATRIX_COMDEF_H
#include <reg52.h>

typedef unsigned char uchar;
typedef uchar byte;

sfr16 RCAP2 = 0xCA;     /* timer2 initial count */
sbit BEEP = P1 ^ 0;

void sleep(int time);

#endif // #ifndef SEED51_COMDEF_H
