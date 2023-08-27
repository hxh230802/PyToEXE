#ifndef UPDATECHECKERDIALOG_H
#define UPDATECHECKERDIALOG_H

#include <QDialog>

#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QtCore/QTimer>
#include <QScrollBar>
#include <QDesktopServices>
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>

//网络相关头文件
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
//JSON相关头文件
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class UpdateCheckerDialog;
}

class UpdateCheckerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateCheckerDialog(QString styleSheet, bool showWindow, QString version,QWidget *parent = 0);
    ~UpdateCheckerDialog();

private slots:
    void on_cancel_Btn_clicked();

    void replyFinished(QNetworkReply *reply);	//网络数据接收完成槽函数的声明
    void sendUpdateRequest();

private:
    void HttpInit();
    void HttpInstallStart();
    void doProcessReadyRead();
    void doProcessFinished();
    void doProcessDownloadProgress(qint64,qint64);
    void doProcessError(QNetworkReply::NetworkError code);

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *myfile;

    QWinTaskbarButton *windowsTaskbarButton;
    QWinTaskbarProgress *windowsTaskbarProgress;

    QNetworkAccessManager *jmanager;		//定义网络请求对象
    int parse_UpdateJSON(QString str);		//解析数据函数的声明
    void update(QString updateLog);
    bool m_showWindow;
    QString m_styleSheet;
    QString m_version;

    Ui::UpdateCheckerDialog *ui;
};

#endif // UPDATECHECKERDIALOG_H
