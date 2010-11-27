#include "Painter.h"

code byte const timeNumLib16[SYMBOL_COUNT][16] = {
    {0x00,0x00,0x00,0x7C,0xEC,0xEE,0xEE,0xC6, 0xC6,0xC6,0xEE,0xEE,0xEC,0x7C,0x00,0x00},   // 0
    {0x00,0x00,0x00,0x18,0x38,0x78,0x78,0x18, 0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00},   // 1
    {0x00,0x00,0x00,0x7C,0xEE,0xE6,0x06,0x0E, 0x1C,0x1C,0x38,0x70,0xE0,0xFE,0x00,0x00},   // 2
    {0x00,0x00,0x00,0x7C,0xEC,0xEE,0x0E,0x0C, 0x3C,0x0E,0x06,0xEE,0xEE,0x7C,0x00,0x00},   // 3
    {0x00,0x00,0x00,0x1C,0x1C,0x3C,0x3C,0x7C, 0xFC,0xDC,0xFE,0x1C,0x1C,0x1C,0x00,0x00},   // 4
    {0x00,0x00,0x00,0x7E,0x60,0xE0,0xE0,0xFC, 0xEE,0x0E,0x06,0xEE,0xEE,0x7C,0x00,0x00},   // 5
    {0x00,0x00,0x00,0x7C,0x6E,0xEE,0xC0,0xFC, 0xEE,0xEE,0xC6,0xEE,0xEE,0x7C,0x00,0x00},   // 6
    {0x00,0x00,0x00,0xFE,0x0E,0x1C,0x1C,0x38, 0x38,0x30,0x70,0x70,0x70,0x70,0x00,0x00},   // 7
    {0x00,0x00,0x00,0x7C,0xEC,0xEE,0xEE,0xEE, 0x7C,0xEE,0xC6,0xC6,0xEE,0x7C,0x00,0x00},   // 8
    {0x00,0x00,0x00,0x7C,0xEC,0xEE,0xC6,0xEE, 0xEE,0x7E,0x06,0xEE,0xEC,0x7C,0x00,0x00},   // 9
    {0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x00, 0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0x00}    // :
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
    byte x= x0 >> 8;    // calculate x using char axis.
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

    for (x = x0, y = y0, i= 0; x < SCRN_LENGTH && i < length; x += 8, i++)
        PainterDrawNumber(num[i], x, y, method);
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
