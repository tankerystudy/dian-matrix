#include <reg52.h>

typedef unsigned char data byte;

sbit ledLineA = P2^4;
sbit ledLineB = P2^3;
sbit ledLineC = P2^2;
sbit ledLineD = P2^1;
sbit ledSwitch = P2^0;

void delay(byte time)
{
    byte i;
    do
    {
        for (i= 0; i < 250; i++)
            ;
    } while (--time != 0);
}

void lighting(byte lineNum)
{
	ledLineA = (lineNum & 1) ? 1 : 0;
    ledLineB = (lineNum & 2) ? 1 : 0;
    ledLineC = (lineNum & 4) ? 1 : 0;
    ledLineD = (lineNum & 8) ? 1 : 0;
}

void main()
{
    byte lightLine = 0;

    ledSwitch = 0;
    
    while (1)
    {
        if (++lightLine == 16)
            lightLine = 0;
        lighting(lightLine);

        delay(1);
    }
}
