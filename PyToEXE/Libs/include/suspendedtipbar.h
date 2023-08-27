#ifndef SUSPENDEDTIPBAR_H
#define SUSPENDEDTIPBAR_H

#include "suspendedtipbar_global.h"

#include <QWidget>

namespace Ui {
class SuspendedTipBar;
}

class SUSPENDEDTIPBARSHARED_EXPORT SuspendedTipBar : public QWidget
{
    Q_OBJECT

public:
    explicit SuspendedTipBar(QWidget *parent,QString text,uint32_t time = 1500,bool theme = false,uint16_t toDown = 0,uint16_t toUp = 0);
    void adjustWindowSize();
    ~SuspendedTipBar();

private:
    Ui::SuspendedTipBar *ui;
};

#endif // SUSPENDEDTIPBAR_H
