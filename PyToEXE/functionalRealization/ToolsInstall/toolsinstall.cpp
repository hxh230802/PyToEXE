#include "toolsinstall.h"
#include "ui_toolsinstall.h"

#include <QtCore/QTimer>

ToolsInstall::ToolsInstall(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolsInstall),
    process(new QProcess(this))
{
    ui->setupUi(this);

    // 移除窗口样式的最大化按钮标志位
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    msgwidget = new MsgWidget;

    connect(process, &QProcess::readyReadStandardOutput, this, &ToolsInstall::onProcessReadyRead);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &ToolsInstall::onProcessFinished);

    HttpInit();
}

void ToolsInstall::on_Python_Btn_clicked()
{
    // 打开安装教程
    QString resourceFilePath = QCoreApplication::applicationDirPath() + "/Resource/Python.pdf"; // 资源文件路径

    QUrl fileUrl = QUrl::fromLocalFile(resourceFilePath);

    if (QDesktopServices::openUrl(fileUrl)) {
        qDebug() << "PDF file opened successfully.";
    } else {
        qDebug() << "Failed to open PDF file.";
    }


    // 开始下载
    HttpInstallStart();
}

void ToolsInstall::on_Pyinstaller_Btn_clicked()
{
    QString program = "pip";
    QStringList command = {"install", "-i", "https://pypi.tuna.tsinghua.edu.cn/simple", "pyinstaller"};
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(program, command);
    ui->nuitka_Btn->setCheckable(false);
    ui->install_prb->setRange(0,0);
}

void ToolsInstall::on_nuitka_Btn_clicked()
{
    QString program = "pip";
    QStringList command = {"install", "-i", "https://pypi.tuna.tsinghua.edu.cn/simple", "nuitka"};
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(program, command);
    ui->Pyinstaller_Btn->setCheckable(false);
    ui->install_prb->setRange(0,0);
}

void ToolsInstall::on_ShowMsg_Btn_clicked()
{
    msgwidget->setStyleSheet(this->styleSheet());
    msgwidget->show();
}

void ToolsInstall::on_OpenDownloadDir_Btn_clicked()
{
    QUrl url = QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/Download/");
    QDesktopServices::openUrl(url);
}

void ToolsInstall::onProcessReadyRead()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    if (process) {
        QByteArray buffer = process->readAllStandardOutput();
        QString output = QString::fromLocal8Bit(buffer);
        // 追加输出到QTextEdit的末尾
        msgwidget->OutMsg_te->append(output);
        // 将滚动条滚动到文本末尾
        msgwidget->OutMsg_te->verticalScrollBar()->setValue(msgwidget->OutMsg_te->verticalScrollBar()->maximum());
        // 检查关键行，判断是否成功
        if (output.contains("Successfully")) collectCompleted = true;

    }
}
void ToolsInstall::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process finished with exit code" << exitCode;
    if (collectCompleted) {
        qDebug() << "successfully" << exitCode;
        // 提示转换成功
        SuspendedTipBar* p_widget = new SuspendedTipBar(this,"安装成功！",2500,isDark());
        p_widget->show();
    }
    else
    {
        qDebug() << "Error" << exitCode;
        // 提示转换失败
        SuspendedTipBar* p_widget = new SuspendedTipBar(this,"安装失败，详情请看输出信息！",3500,isDark());
        p_widget->show();
    }
    ui->Pyinstaller_Btn->setCheckable(true);
    ui->nuitka_Btn->setCheckable(true);
    ui->install_prb->setRange(0,100);
    ui->install_prb->setValue(0);
}

void ToolsInstall::HttpInit()
{
    manager = new QNetworkAccessManager(this);
    myfile = new QFile(this);
    windowsTaskbarButton = new QWinTaskbarButton(this);    //创建显示进度的任务栏按钮
}
void ToolsInstall::HttpInstallStart()
{
    QNetworkRequest request;
    QString url = "http://118.178.242.91:88/Projects/software/Compiler/PytoEXE/Tools/python-3.8.5.exe";
    request.setUrl(QUrl(url));

    reply = manager->get(request);              //发送请求

    connect(reply,&QNetworkReply::readyRead,this,&ToolsInstall::doProcessReadyRead);                //可读
    connect(reply,&QNetworkReply::finished,this,&ToolsInstall::doProcessFinished);                  //结束
    connect(reply,&QNetworkReply::downloadProgress,this,&ToolsInstall::doProcessDownloadProgress);  //大小
    connect(reply,QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),          //异常
            this,&ToolsInstall::doProcessError);

    QStringList list = url.split("/");
    QString filename = list.at(list.length()-1);
    QString file = QCoreApplication::applicationDirPath() + "/Download/" + filename;
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
void ToolsInstall::doProcessReadyRead()             //读取并写入
{
    while(!reply->atEnd())
    {
        QByteArray ba = reply->readAll();
        myfile->write(ba);
    }
}
void ToolsInstall::doProcessFinished()
{
    myfile->close();
}
void ToolsInstall::doProcessDownloadProgress(qint64 recv_total, qint64 all_total)      //显示
{
    ui->install_prb->setMaximum(all_total);
    ui->install_prb->setValue(recv_total);

    windowsTaskbarProgress->setMaximum(all_total);
    windowsTaskbarProgress->setValue(recv_total);//设置TaskbarProgress的当前值
    windowsTaskbarProgress->show(); //显示TaskbarProgress

    // 下载成功
    if(recv_total == all_total)
    {
        // 提示下载成功
        SuspendedTipBar* p_widget = new SuspendedTipBar(this,"下载成功！\n正在启动安装进程。",3500,isDark());
        p_widget->show();

        ui->install_prb->setRange(0,100);
        ui->install_prb->setValue(0);

        windowsTaskbarProgress->reset();//重置进度指示器

        QUrl programUrl = QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/Download/python-3.8.5.exe"); // 替换为要打开的程序路径

        // 创建定时器对象
        QTimer* timer = new QTimer;
        timer->setSingleShot(true); // 只执行一次
        // 在 500 毫秒后连接定时器到相应槽函数
        connect(timer, &QTimer::timeout, [=](){
            // 打开安装程序
            if (QDesktopServices::openUrl(programUrl)) {
                qDebug() << "Program opened successfully.";
                // 提示启动成功
                SuspendedTipBar* p_widget = new SuspendedTipBar(this,"安装进程启动成功！",3500,isDark());
                p_widget->show();
            } else {
                qDebug() << "Failed to open program.";
                // 提示启动失败
                SuspendedTipBar* p_widget = new SuspendedTipBar(this,"安装进程启动失败！你可以手动启动",3500,isDark());
                p_widget->show();
                on_OpenDownloadDir_Btn_clicked();
            }
            delete timer;
        });
        timer->start(50);  // 启动定时器
    }
}
void ToolsInstall::doProcessError(QNetworkReply::NetworkError code)
{
    qDebug() << code;
    // 提示下载失败
    SuspendedTipBar* p_widget = new SuspendedTipBar(this,"下载失败！\n错误代码"+code,5000,isDark());
    p_widget->show();
}

bool ToolsInstall::isDark()
{
    QPalette palette = this->palette();  // 获取窗口的调色板
    QColor backgroundColor = palette.color(QPalette::Window);  // 获取窗口的背景色
    // 判断背景色是否属于暗色
    return backgroundColor.lightness() < 128;
}

ToolsInstall::~ToolsInstall()
{
    delete ui;
}

void ToolsInstall::on_sure_Btn_clicked()
{
    close();
}

void ToolsInstall::on_cancel_Btn_clicked()
{
    close();
}
