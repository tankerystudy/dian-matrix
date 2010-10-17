#include <QtGui>

#include "LEDScrn.h"
#include "DataManager.h"

QLEDScrn::QLEDScrn(DataManager *data, QWidget *parent/*= 0*/)
: QWidget(parent)
, matrixData(data)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

QLEDScrn::~QLEDScrn(void)
{
}

void QLEDScrn::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    float xSpace= (float)width()/LED_MATRIX_X_COUNT;
    float ySpace= (float)height()/LED_MATRIX_Y_COUNT;
    float const scale= 6.0;
    QRect rect(xSpace/scale, ySpace/scale, xSpace-xSpace/scale, ySpace-ySpace/scale);

    // draw black background
    painter.setBrush(Qt::SolidPattern);
    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

    for (int x= 0; x < LED_MATRIX_X_COUNT; x++)
    {
        for (int y= 0; y < LED_MATRIX_Y_COUNT; y++)
        {
            painter.save();
            painter.translate(x*xSpace, y*ySpace);

            if (matrixData->readBool(x, y))
            {
                painter.setPen(Qt::red);
                painter.setBrush(Qt::red);
            }
            else
            {
                painter.setPen(QPen(Qt::red, 0, Qt::DotLine));
            }

            painter.drawEllipse(rect);      // draw ellipse use specific style

            painter.restore();
        }
    }
}
