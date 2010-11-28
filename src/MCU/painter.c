#include "Painter.h"

code byte const timeNumLib16[SYMBOL_COUNT][SCRN_HEIGHT] = 
{
    {0xff,0xff,0xff,0x83,0x13,0x11,0x11,0x39,0x39,0x39,0x11,0x11,0x13,0x83,0xff,0xff},   // 0
    {0xff,0xff,0xff,0xe7,0xc7,0x87,0x87,0xe7,0xe7,0xe7,0xe7,0xe7,0xe7,0xe7,0xff,0xff},   // 1
    {0xff,0xff,0xff,0x83,0x11,0x19,0xf9,0xf1,0xe3,0xe3,0xc7,0x8f,0x1f,0x01,0xff,0xff},   // 2
    {0xff,0xff,0xff,0x83,0x13,0x11,0xf1,0xf3,0xc3,0xf1,0xf9,0x11,0x11,0x83,0xff,0xff},   // 3
    {0xff,0xff,0xff,0xe3,0xe3,0xc3,0xc3,0x83,0x03,0x23,0x01,0xe3,0xe3,0xe3,0xff,0xff},   // 4
    {0xff,0xff,0xff,0x81,0x9f,0x1f,0x1f,0x03,0x11,0xf1,0xf9,0x11,0x11,0x83,0xff,0xff},   // 5
    {0xff,0xff,0xff,0x83,0x91,0x11,0x3f,0x03,0x11,0x11,0x39,0x11,0x11,0x83,0xff,0xff},   // 6
    {0xff,0xff,0xff,0x01,0xf1,0xe3,0xe3,0xc7,0xc7,0xcf,0x8f,0x8f,0x8f,0x8f,0xff,0xff},   // 7
    {0xff,0xff,0xff,0x83,0x13,0x11,0x11,0x11,0x83,0x11,0x39,0x39,0x11,0x83,0xff,0xff},   // 8
    {0xff,0xff,0xff,0x83,0x13,0x11,0x39,0x11,0x11,0x81,0xf9,0x11,0x13,0x83,0xff,0xff},   // 9
    {0xff,0xff,0xff,0xff,0xc3,0xc3,0xc3,0xff,0xff,0xc3,0xc3,0xc3,0xff,0xff,0xff,0xff}    // :
};

// graphics memory pointer.
static byte *g_pGraphMem;
static byte g_dataWidth;

// must initial with graphics memory pointer.
void PainterInit(byte *mem, byte const width)
{
    g_pGraphMem = mem;
    g_dataWidth = width;
}

// clear the screen, make all led off.
void PainterClearScreen()
{
    ;
}

// draw point using method.
void PainterDrawPoint(byte const x, byte const y, enum PaintMethod method)
{
    byte *pcurChar= 0;
    byte chCount= x >> 3;       // switch to the x-index of char, x >> 3 == x/8
    byte restBit= x & 0x07;     // get low 3 bit, that is x % 8

    // get current char g_pGraphMem[chChount][y]
    pcurChar = g_pGraphMem + (g_dataWidth*y + chCount);

    if (method == PM_COPY)
    {
        *pcurChar |= (0x80 >> restBit);
    }
    else
    {
        *pcurChar ^= (0x80 >> restBit);
    }
}

// using the paint method to draw a rect/line at top-left, button-right area.
void PainterDrawRect(byte const x0, byte const y0, byte const x1, byte const y1,
                     enum PaintMethod method)
{
    byte x= x1, y= y1;

    do
    {
        do
        {
            PainterDrawPoint(x, y, method);
        } while (--x != x0);
    } while (--y != y0);
}

void PainterDrawLine(byte const x0, byte const y0, byte const x1, byte const y1,
                     enum PaintMethod method)
{
    // notes: the dx & dy will be negative sometimes.
    byte dx= x1 - x0;
    byte dy= y1 - y0;
    byte ds= dx > dy ? dx : dy;  // get max of dx & dy
    byte i= ds;

    do
    {
        // the point should be calculate every time to reduce warp
        PainterDrawPoint((x0*ds + dx*i)/ds, (y0*ds + dy*i)/ds, method);
    } while (--i != 0);
}

// draw a number place it's top-left conner on specific location.
// only support draw on char location now.
void PainterDrawNumber(byte const num, byte const x0, byte const y0,
                            enum PaintMethod method)
{
    byte y;
    byte x= x0 >> 3;    // calculate x using char axis.
    //byte x = x0;
    for (y = y0; y < 16 && y < SCRN_HEIGHT; y++)
    {
        unsigned char curChar = timeNumLib16[num][y];
        if (method == PM_XOR)
            *(g_pGraphMem + (g_dataWidth*y + x))^= curChar;
        else
            *(g_pGraphMem + (g_dataWidth*y + x)) = curChar;
    }
}

// draw a number(time) string on specific location (top-left).
void PainterDrawString(byte const num[], byte length, byte const x0, byte const y0,
                       enum PaintMethod method)
{
    byte x, y;
    byte i;

    //for (x = x0, y = y0, i= 0; x < SCRN_LENGTH && i < length; x++, i++)
    for (x = x0, y = y0, i= 0; x < SCRN_LENGTH && i < length; x += 8, i++)
    {
        PainterDrawNumber(num[i], x, y, method);
    }
}

// draw a cursor on specific location using NOT method.
void PainterDrawCursor(byte const xc, byte const yc, byte size
                            , enum PaintMethod method)
{
    // draw horizontal line
    PainterDrawLine(xc - size, yc, xc + size, yc, method);
    // draw vertical line
    PainterDrawLine(xc, yc - size, xc, yc + size, method);
}
