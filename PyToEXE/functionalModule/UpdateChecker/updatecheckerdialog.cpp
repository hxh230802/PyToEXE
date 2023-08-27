#include "updatecheckerdialog.h"
#include "ui_updatecheckerdialog.h"

int compareVersion(const QString& version1, const QString& version2);

UpdateCheckerDialog::UpdateCheckerDialog(QString styleSheet, bool showWindow, QString version, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateCheckerDialog)
{
    ui->setupUi(this);

    m_showWindow = showWindow;
    m_styleSheet = styleSheet;
    m_version = version;

    manager = new QNetworkAccessManager(this);          //新建QNetworkAccessManager对象
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));//关联信号和槽

    QTimer::singleShot(0, this, SLOT(sendUpdateRequest())); // 使用定时器延迟执行发送网络请求

}

void UpdateCheckerDialog::sendUpdateRequest()
{
    qDebug() << "Sending update request...";  // 调试输出语句
    QNetworkRequest quest;
    quest.setUrl(QUrl("http://118.178.242.91:88/Projects/software/Compiler/PytoEXE/update/update.json"));
    quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
    manager->get(quest);
}

void UpdateCheckerDialog::replyFinished(QNetworkReply *reply)
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

int UpdateCheckerDialog::parse_UpdateJSON(QString str)
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

        int result = compareVersion(Verison, m_version);

        qDebug() << Verison << m_version << result;

        if(result > 0)
        {
            QString warningStr =  "检测到新版本!\n版本号：" + Verison + "\n" + "更新时间：" + UpdateTime + "\n" + "更新说明：\n" + ReleaseNote+"\n\n是否更新？";
            int ret = QMessageBox::warning(nullptr, "检查更新",  warningStr, "去更新", "不更新");
            if(ret == 0)    //点击了更新
            {
                update(ReleaseNote);
            }
            else
            {
                close();
            }
        }
        else
        {
            if(m_showWindow) QMessageBox::information(nullptr, "检查更新", "当前已经是最新版本!");
            close();
        }
    }
    return 0;
}

void UpdateCheckerDialog::update(QString updateLog)
{
    //文件保存到本地
    QFile file(QCoreApplication::applicationDirPath()+"/updateLog.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) //append 内容追加在文件后面
    {
        QMessageBox::critical(nullptr, "错误", "文件打开失败，信息未写入", "确定");
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8"); // 设置编码为UTF-8
    out << updateLog;     //输出到文件
    file.close();   //关闭文件
    qDebug() << "文件保存成功!";

    HttpInit();

    // 开始下载
    HttpInstallStart();
}


UpdateCheckerDialog::~UpdateCheckerDialog()
{
    delete ui;
}

void UpdateCheckerDialog::on_cancel_Btn_clicked()
{
    close();
}


void UpdateCheckerDialog::HttpInit()
{
    manager = new QNetworkAccessManager(this);
    myfile = new QFile(this);
    windowsTaskbarButton = new QWinTaskbarButton(this);    //创建显示进度的任务栏按钮
}
void UpdateCheckerDialog::HttpInstallStart()
{
    QNetworkRequest request;
    QString url = "http://118.178.242.91:88/Projects/software/Compiler/PytoEXE/update/update.exe";
    request.setUrl(QUrl(url));

    reply = manager->get(request);              //发送请求

    connect(reply,&QNetworkReply::readyRead,this,&UpdateCheckerDialog::doProcessReadyRead);                //可读
    connect(reply,&QNetworkReply::finished,this,&UpdateCheckerDialog::doProcessFinished);                  //结束
    connect(reply,&QNetworkReply::downloadProgress,this,&UpdateCheckerDialog::doProcessDownloadProgress);  //大小
    connect(reply,QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),          //异常
            this,&UpdateCheckerDialog::doProcessError);

    QStringList list = url.split("/");
    QString filename = list.at(list.length()-1);
    QString file = QCoreApplication::applicationDirPath() + "/" + filename;
    myfile->setFileName(file);
    bool ret = myfile->open(QIODevice::WriteOnly|QIODevice::Truncate);    //创建文件
    if(!ret)
    {
        QMessageBox::warning(this,"warning","打开失败");
        return;
    }

    // 界面进度条初始化
    ui->install_prb->setValue(0);             //设置进度条初始化为0
    ui->install_prb->setMinimum(0);

    // 任务栏进度条初始化
    windowsTaskbarButton->setWindow(windowHandle());    //将任务栏按钮与进度条关联，假设进度条是它自己的窗口
    windowsTaskbarProgress = windowsTaskbarButton->progress();//设置进度指示器
    windowsTaskbarProgress->setMinimum(0);   //设置最小值和最大值的值
}
void UpdateCheckerDialog::doProcessReadyRead()             //读取并写入
{
    while(!reply->atEnd())
    {
        QByteArray ba = reply->readAll();
        myfile->write(ba);
    }
}
void UpdateCheckerDialog::doProcessFinished()
{
    myfile->close();

    QUrl programUrl = QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/update.exe"); // 替换为要打开的程序路径
    QDesktopServices::openUrl(programUrl);

    close();
    // 结束主进程
    qApp->quit();
}
void UpdateCheckerDialog::doProcessDownloadProgress(qint64 recv_total, qint64 all_total)      //显示
{
    ui->install_prb->setMaximum(all_total);
    ui->install_prb->setValue(recv_total);

    windowsTaskbarProgress->setMaximum(all_total);
    windowsTaskbarProgress->setValue(recv_total);//设置TaskbarProgress的当前值
    windowsTaskbarProgress->show(); //显示TaskbarProgress

    // 下载成功
    if(recv_total == all_total)
    {

        ui->install_prb->setRange(0,100);
        ui->install_prb->setValue(0);

        windowsTaskbarProgress->reset();//重置进度指示器

    }
}
void UpdateCheckerDialog::doProcessError(QNetworkReply::NetworkError code)
{
    qDebug() << code;
}

int compareVersion(const QString& version1, const QString& version2)
{
    QStringList parts1 = version1.split('.');
    QStringList parts2 = version2.split('.');

    int numParts = qMax(parts1.size(), parts2.size());
    for (int i = 0; i < numParts; ++i) {
        int part1 = i < parts1.size() ? parts1.at(i).toInt() : 0;
        int part2 = i < parts2.size() ? parts2.at(i).toInt() : 0;

        if (part1 < part2) {
            return -1;
        } else if (part1 > part2) {
            return 1;
        }
    }

    return 0; // 两个版本号相等
}
