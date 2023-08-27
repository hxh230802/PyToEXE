#include "about.h"
#include "ui_about.h"

#include <QFile>
#include <QDesktopServices>
#include <QtDebug>

About::About(QString version, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    ui->title_lbl->setStyleSheet("font: 75 22pt \"等线\";");
    ui->version_lbl->setText("当前版本："+version);

    QFile file(QCoreApplication::applicationDirPath()+"/updateLog.txt");
    file.open(QFile::ReadOnly | QFile::Text);
    QString updateLog = file.readAll();
    ui->updateLog_te->setText(updateLog);
    ui->updateLog_te->setReadOnly(true);
}

About::~About()
{
    delete ui;
}

void About::on_BugReport_Btn_clicked()
{
    BugReport bugreport(this);
    bugreport.exec();
}

void About::on_authorMsg_Btn_clicked()
{
    // 打开我的主页
    QString url = "https://space.bilibili.com/1396650915?spm_id_from=333.1007.0.0";
    QUrl bilibiliUrl(url);

    if (QDesktopServices::openUrl(bilibiliUrl)) {
        qDebug() << "URL opened successfully.";
    } else {
        qDebug() << "Failed to open URL.";
    }
}
