#ifndef DIAN_MATRIX_LED_SCREEN_H
#define DIAN_MATRIX_LED_SCREEN_H

#include <QWidget>

class DataManager;

class QLEDScrn : public QWidget
{
    Q_OBJECT

    enum
    {
        LED_MATRIX_X_COUNT = 64,
        LED_MATRIX_Y_COUNT = 16
    };
private:
    DataManager *matrixData;

public:
    QLEDScrn(DataManager *data, QWidget *parent= 0);
    ~QLEDScrn(void);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // #ifndef DIAN_MATRIX_LED_SCREEN_H
