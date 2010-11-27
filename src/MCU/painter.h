#ifndef __DIAN_MATRIX_PAINTER_H
#define __DIAN_MATRIX_PAINTER_H
#include "commondriver\common_def.h"

// the encoding array for time number (0 ~ 9, :)
enum {
    LIB_NUM_0= 0,
    LIB_NUM_1,
    LIB_NUM_2,
    LIB_NUM_3,
    LIB_NUM_4,
    LIB_NUM_5,
    LIB_NUM_6,
    LIB_NUM_7,
    LIB_NUM_8,
    LIB_NUM_9,
    LIB_COLON,

    SYMBOL_COUNT
};

// define paint method.
enum PaintMethod {
    PM_COPY = 0,    // paint using direct drawing method
    PM_XOR  = 1     // paint using not method
};
enum {
    SCRN_LENGTH = 64,
    SCRN_HEIGHT = 16
};

// must initial with graphics memory pointer.
void PainterInit(byte *pmem, byte const);

// clear the screen, make all led off.
void PainterClearScreen();
// draw point using method.
void PainterDrawPoint(byte const, byte const, enum PaintMethod);
// using the paint method to draw a rect/line at top-left, button-right area.
void PainterDrawRect(byte const, byte const, byte const, byte const, enum PaintMethod);
void PainterDrawLine(byte const, byte const, byte const, byte const, enum PaintMethod);
// draw a number place it's top-left conner on specific location.
void PainterDrawNumber(byte const num, byte const, byte const, enum PaintMethod);
// draw a number(time) string on specific location (top-left).
void PainterDrawString(byte const numstr[], byte const length, byte const, byte const, enum PaintMethod);
// draw a cursor on specific location using NOT method.
void PainterDrawCursor(byte const, byte const, byte size, enum PaintMethod);

#endif // #ifndef __DIAN_MATRIX_PAINTER_H

