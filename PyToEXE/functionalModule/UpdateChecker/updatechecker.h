#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QDialog>

#include <QMessageBox>
#include <QMessageBox>
#include <QFile>
#include <QCoreApplication>
#include <QtDebug>

//网络相关头文件
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
//JSON相关头文件
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "functionalModule/UpdateChecker/updatecheckerdialog.h"

class UpdateChecker : public QDialog
{
    Q_OBJECT
public:
    explicit UpdateChecker(QString styleSheet, bool showWindow, QString version, QWidget *parent = nullptr);

signals:

public slots:
    void replyFinished(QNetworkReply *reply);	//网络数据接收完成槽函数的声明
    void sendUpdateRequest();
private:
    QNetworkAccessManager *manager;		//定义网络请求对象
    int parse_UpdateJSON(QString str);		//解析数据函数的声明
    void update(QString updateLog);
    bool m_showWindow;
    QString m_styleSheet;
    QString m_version;
};

#endif // UPDATECHECKER_H
