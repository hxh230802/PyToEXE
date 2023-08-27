#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include <QSettings>
#include <QDebug>

#include "functionalModule/UpdateChecker/updatecheckerdialog.h"

// 定义设置项结构体
struct Settings {
    enum Theme { Light, Dark, System };  // 主题选项：亮色、暗色、系统
    Theme theme;                         // 当前选中的主题
    QString version;                     // 版本号
    bool autoCheckUpdates;               // 是否自动检查更新
    bool openOutputDir;                  // 是否打开输出目录
    bool keepSpecFile;                   // 是否保留规范文件
    bool keepBuildDirectory;             // 是否保留构建目录
};

namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = 0);
    Settings readSettingsFromFile();
    void writeSettingsToFile();
    ~Setting();

signals:
    void settingChanged();

private slots:
    void on_cancel_Btn_clicked();

    void on_sure_Btn_clicked();

    void on_checkUpdate_Btn_clicked();

private:
    Ui::Setting *ui;
    Settings settings;

    void UIrefresh();
    void UIread();
};

#endif // SETTING_H
