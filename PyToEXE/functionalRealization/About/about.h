#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

#include "functionalRealization/About/bugreport.h"

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QString version,QWidget *parent = 0);
    ~About();

private slots:
    void on_BugReport_Btn_clicked();

    void on_authorMsg_Btn_clicked();

private:
    Ui::About *ui;
};

#endif // ABOUT_H
