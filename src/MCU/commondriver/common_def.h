#ifndef DIAN_MATRIX_COMDEF_H
#define DIAN_MATRIX_COMDEF_H

typedef unsigned char uchar;
typedef uchar byte;

sfr16 RCAP2 = 0xCA;     /* timer2 initial count */
sbit BEEP = P1 ^ 0;

void sleep(int time)
{
    byte i;
    do
    {
        for (i= 0; i < 250; i++)
            ;
    } while (--time != 0);
}

#endif // #ifndef SEED51_COMDEF_H
