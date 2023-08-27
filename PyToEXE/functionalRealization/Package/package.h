#ifndef PACKAGE_H
#define PACKAGE_H

#include "Libs/cppsyntaxhighlighter.h"
#include "Libs/include/SuspendedTipBar"
#include "Libs/include/FileAndSystem"

#include "functionalModule/Setting/setting.h"
#include "functionalModule/AdvancedOptions/advancedoptions.h"

#include <QWidget>
#include <QProcess>
#include <QScrollBar>

struct Config {
    QString sourceFilePath; // 源文件路径
    QString sourceFileDir;  // 源文件目录
    QString outputDir;      // 输出目录
    QString outputName;     // 输出文件名
    QString outputPath;     // 输出路径
    bool fileOrDir = true;  // 配置信息：单文件/文件夹 T:onefile F:dir
    bool icon = false;      // 配置信息：是否自定义图标
    QString iconPath;       // 配置信息：自定义图标路径
    bool displayCMD = true; // 配置信息：是否显示CMD窗口
    QString workPath;       // 工作目录
};

struct CommandAndConfig
{
    QStringList commandList;
    Config config;
};

CommandAndConfig m_renewCommand(Config config, AdvancedConfiguration advancedConfiguration);

namespace Ui {
class Package;
}

class Package : public QWidget
{
    Q_OBJECT

public:
    explicit Package(Config config,Settings setting,AdvancedConfiguration advancedConfiguration,QWidget *parent = 0);
    ~Package();

private slots:
    void onProcessReadyRead();
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void on_sure_Btn_clicked();

    void on_cancel_Btn_clicked();

    void on_clearOutput_Btn_clicked();

    void on_exportMsg_Btn_clicked();

private:
    Ui::Package *ui;
    QProcess *process;
    Settings settings;
    Config outputconfig;
    FileAndSystem fileandsystem;
    QString outputDir;      // 输出目录
    bool collectCompleted = false;
    bool Error = false;
    bool isDark();
};

#endif // PACKAGE_H
