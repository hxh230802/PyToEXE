#include "updatechecker.h"

#include <QtCore/QTimer>

UpdateChecker::UpdateChecker(QString styleSheet, bool showWindow, QString version,QWidget *parent) : QDialog(parent)
{
    qDebug() << "UpdateChecker";

    m_showWindow = showWindow;
    m_styleSheet = styleSheet;
    m_version = version;

    manager = new QNetworkAccessManager(this);          //新建QNetworkAccessManager对象
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//关联信号和槽

    QTimer::singleShot(500, this, SLOT(sendUpdateRequest())); // 使用定时器延迟执行发送网络请求

}

void UpdateChecker::sendUpdateRequest()
{
//    hide();
    qDebug() << "Sending update request...";  // 调试输出语句
    QNetworkRequest quest;
    quest.setUrl(QUrl("http://118.178.242.91:88/Projects/software/Compiler/PytoEXE/update/update.json"));
    quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
    manager->get(quest);
}

void UpdateChecker::replyFinished(QNetworkReply *reply)
{
    // 检查网络请求是否出错
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "网络请求错误:" << reply->errorString();
    }
    QString str = reply->readAll();//读取接收到的数据
    parse_UpdateJSON(str);
    qDebug() << str;
    reply->deleteLater();               //销毁请求对象
}

int UpdateChecker::parse_UpdateJSON(QString str)
{
    QJsonParseError err_rpt;
    QJsonDocument  root_Doc = QJsonDocument::fromJson(str.toUtf8(),&err_rpt);//字符串格式化为JSON
    if(err_rpt.error != QJsonParseError::NoError)
    {
        QMessageBox::critical(nullptr, "检查失败", "服务器地址错误或JSON格式错误!");
        return -1;
    }
    if(root_Doc.isObject())
    {
        QJsonObject  root_Obj = root_Doc.object();   //创建JSON对象，不是字符串
        QJsonObject PyToEXE = root_Obj.value("PyToEXE").toObject();
        QString Verison = PyToEXE.value("LatestVersion").toString();
        QString UpdateTime = PyToEXE.value("UpdateTime").toString();
        QString ReleaseNote = PyToEXE.value("ReleaseNote").toString();

        if(Verison > m_version)
        {
            QString warningStr =  "检测到新版本!\n版本号：" + Verison + "\n" + "更新时间：" + UpdateTime + "\n" + "更新说明：\n" + ReleaseNote+"\n\n是否更新？";
            int ret = QMessageBox::warning(nullptr, "检查更新",  warningStr, "去更新", "不更新");
            if(ret == 0)    //点击了更新
            {
                update(ReleaseNote);
            }
        }
        else
            if(m_showWindow) QMessageBox::information(nullptr, "检查更新", "当前已经是最新版本!");
    }
    return 0;
}

void UpdateChecker::update(QString updateLog)
{
    //文件保存到本地
    QFile file(QCoreApplication::applicationDirPath()+"/updateLog.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) //append 内容追加在文件后面
    {
        QMessageBox::critical(nullptr, "错误", "文件打开失败，信息未写入", "确定");
        return;
    }
    QTextStream out(&file);
    out << updateLog;     //输出到文件
    file.close();   //关闭文件
    qDebug() << "文件保存成功!";

    UpdateCheckerDialog updateCheckerDialog;
    updateCheckerDialog.setStyleSheet(m_styleSheet);
    updateCheckerDialog.exec();
}
