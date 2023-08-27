#include "bugreport.h"
#include "ui_bugreport.h"

#include <QMessageBox>

BugReport::BugReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BugReport)
{
    ui->setupUi(this);
}

void BugReport::on_submit_Btn_clicked()
{
    QString report = ui->BugReport_te->toPlainText();
    // 替换换行符
    report.replace('\n', ' ');

    // 创建QNetworkAccessManager对象来处理网络请求
    QNetworkAccessManager networkManager;

    // 创建POST请求
    QNetworkRequest request(QUrl("http://118.178.242.91:88/Projects/software/Compiler/PytoEXE/BugReport/bug_report.php"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // 将报告作为参数发送
    QByteArray postData;
    postData.append("report=" + QUrl::toPercentEncoding(report));

    // 发送POST请求
    QNetworkReply* reply = networkManager.post(request, postData);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    // 处理服务器的响应
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Bug report sent successfully!";
        QMessageBox::information(this,"提示","发送成功");
    } else {
        qDebug() << "Failed to send bug report:" << reply->errorString();
        QMessageBox::warning(this,"警告","发送失败\n错误代码:"+reply->errorString());
    }

    // 清理资源
    reply->deleteLater();

    close();
}

bool BugReport::isDark()
{
    QPalette palette = this->palette();  // 获取窗口的调色板
    QColor backgroundColor = palette.color(QPalette::Window);  // 获取窗口的背景色
    // 判断背景色是否属于暗色
    return backgroundColor.lightness() < 128;
}

BugReport::~BugReport()
{
    delete ui;
}
