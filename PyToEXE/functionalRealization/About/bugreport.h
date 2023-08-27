#ifndef BUGREPORT_H
#define BUGREPORT_H

#include <QDialog>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "Libs/include/SuspendedTipBar"

namespace Ui {
class BugReport;
}

class BugReport : public QDialog
{
    Q_OBJECT

public:
    explicit BugReport(QWidget *parent = 0);
    ~BugReport();

private slots:
    void on_submit_Btn_clicked();

private:
    Ui::BugReport *ui;

    bool isDark();
};

#endif // BUGREPORT_H
