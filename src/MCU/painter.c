#include "ledPainter.h"
#include "DataManager.h"

// must initial with graphics memory pointer.
LedPainter::LedPainter(DataManager *mem)
: graphMem(mem)
{
}

LedPainter::~LedPainter()
{
}

// clear the screen, make all led off.
void LedPainter::ClearScreen()
{
    graphMem->clearAll();
}

// draw point using method.
inline void LedPainter::DrawPoint(GraphVector const point, enum PaintMethod method)
{
    DrawPoint(point.x, point.y, method);
}
inline void LedPainter::DrawPoint(int const x, int const y, enum PaintMethod method)
{
    bool curBit= (method == PM_COPY)? true : !graphMem->readBool(x, y);
    graphMem->writeBool(x, y, curBit);
}

// using the paint method to draw a rect/line at top-left, button-right area.
void LedPainter::DrawRect(GraphVector const topLeft, GraphVector const buttonRight
                           , enum PaintMethod method)
{
    for (int x= topLeft.x; x < buttonRight.x; x++)
    for (int y= topLeft.y; y < buttonRight.y; y++)
    {
        DrawPoint(x, y, method);
    }
}

void LedPainter::DrawLine(GraphVector const topLeft, GraphVector const buttonRight
                           , enum PaintMethod method)
{
    // notes: the dx & dy will be negative sometimes.
    int dx= buttonRight.x - topLeft.x;
    int dy= buttonRight.y - topLeft.y;
    int ds= dx > dy ? dx : dy;  // get max of dx & dy

    for (int i= 0; i <= ds; i++)
    {
        // the point should be calculate every time to reduce warp
        DrawPoint(int((topLeft.x*ds + dx*i)/ds), int((topLeft.y*ds + dy*i)/ds), method);
    }
}

// draw a number place it's top-left conner on specific location.
// only support draw on char location now.
void LedPainter::DrawNumber(GraphVector const topLeft, byte const num
                            , enum PaintMethod method/* = PM_COPY*/)
{
    int x= int(topLeft.x/8);    // calculate x using char axis.
    for (int y = 0; y < 16 && y < SCRN_HEIGHT; y++)
    {
        unsigned char curChar = timeNumLib16[num][y];
        if (method == PM_XOR)
            curChar^= graphMem->readChar(x, y);
        graphMem->writeChar(x, y, curChar);
    }
}

// draw a number(time) string on specific location (top-left).
void LedPainter::DrawString(GraphVector const topLeft, byte const num[], byte length
                            , enum PaintMethod method/* = PM_COPY*/)
{
    GraphVector tl;
    int i;

    for (tl.x = topLeft.x, tl.y = topLeft.y, i= 0;
        tl.x < SCRN_LENGTH && i < length; tl.x += 8, i++)
        DrawNumber(tl, num[i], method);
}

// draw a cursor on specific location using NOT method.
void LedPainter::DrawCursor(GraphVector const center, byte size/* = 3*/
                            , enum PaintMethod method/* = PM_XOR*/)
{
    GraphVector tl, br;

    // draw horizontal line
    tl.x = center.x - size;
    br.x = center.x + size;
    tl.y = center.y;
    br.y = center.y;
    DrawLine(tl, br, method);
    // draw vertical line
    tl.x = center.x;
    br.x = center.x;
    tl.y = center.y - size;
    br.y = center.y + size;
    DrawLine(tl, br, method);
}
