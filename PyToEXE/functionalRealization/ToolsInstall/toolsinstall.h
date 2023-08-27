#ifndef TOOLSINSTALL_H
#define TOOLSINSTALL_H

#include <QWidget>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QScrollBar>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QNetworkRequest>
#include <QWinTaskbarButton>
#include <QWinTaskbarProgress>

#include "Libs/cppsyntaxhighlighter.h"
#include "Libs/include/SuspendedTipBar"

namespace Ui {
class ToolsInstall;
}

#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

class MsgWidget : public QWidget {
    Q_OBJECT

public:
    MsgWidget(QWidget *parent = nullptr) : QWidget(parent) {

        this->setMinimumSize(800,500);

        QVBoxLayout *layout = new QVBoxLayout(this);

        OutMsg_te = new QTextEdit(this);
        layout->addWidget(OutMsg_te);

        // 创建 CppSyntaxHighlighter 对象并设置为文档的语法高亮器
        CppSyntaxHighlighter* highlighter = new CppSyntaxHighlighter(OutMsg_te->document());

    }

    QTextEdit *OutMsg_te;
};

class ToolsInstall : public QWidget
{
    Q_OBJECT

public:
    explicit ToolsInstall(QWidget *parent = 0);
    void HttpInit();
    void HttpInstallStart();
    void doProcessReadyRead();
    void doProcessFinished();
    void doProcessDownloadProgress(qint64,qint64);
    void doProcessError(QNetworkReply::NetworkError code);
    ~ToolsInstall();

private slots:
    void onProcessReadyRead();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void on_Python_Btn_clicked();

    void on_Pyinstaller_Btn_clicked();

    void on_nuitka_Btn_clicked();

    void on_ShowMsg_Btn_clicked();

    void on_OpenDownloadDir_Btn_clicked();

    void on_sure_Btn_clicked();

    void on_cancel_Btn_clicked();

private:
    Ui::ToolsInstall *ui;

    MsgWidget *msgwidget;

    QProcess *process;
    bool collectCompleted = false;
    bool isDark();

    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QFile *myfile;

    QWinTaskbarButton *windowsTaskbarButton;
    QWinTaskbarProgress *windowsTaskbarProgress;
};

#endif // TOOLSINSTALL_H
