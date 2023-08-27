#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);

    // 移除窗口样式的最大化按钮标志位
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
}

// 读取设置文件
Settings Setting::readSettingsFromFile()
{

    const QString& filePath = QCoreApplication::applicationDirPath()+"/settings.ini";
    QSettings config(filePath, QSettings::IniFormat);

    // 读取设置项
    config.beginGroup("Settings");
    settings.theme = static_cast<Settings::Theme>(config.value("Theme").toInt());
    settings.version = config.value("Version").toString();
    settings.autoCheckUpdates = config.value("AutoCheckUpdates").toBool();
    settings.openOutputDir = config.value("OpenOutputDir").toBool();
    settings.keepSpecFile = config.value("KeepSpecFile").toBool();
    settings.keepBuildDirectory = config.value("KeepBuildDirectory").toBool();
    config.endGroup();

    UIrefresh();

    return settings;
}

// 写入设置文件
void Setting::writeSettingsToFile()
{
    const QString& filePath = QCoreApplication::applicationDirPath()+"/settings.ini";
    QSettings config(filePath, QSettings::IniFormat);

    // 写入设置项
    config.beginGroup("Settings");
    config.setValue("Theme", settings.theme);
    config.setValue("Version", settings.version);
    config.setValue("AutoCheckUpdates", settings.autoCheckUpdates);
    config.setValue("OpenOutputDir", settings.openOutputDir);
    config.setValue("KeepSpecFile", settings.keepSpecFile);
    config.setValue("KeepBuildDirectory", settings.keepBuildDirectory);
    config.endGroup();

    // 保存设置
    config.sync();
}

// 刷新UI
void Setting::UIrefresh()
{
    //    // 输出读取的设置项
    //    qDebug() << "Read settings:";
    //    qDebug() << "Theme: " << settings.theme;
    //    qDebug() << "Version: " << settings.version;
    //    qDebug() << "Auto Check Updates: " << settings.autoCheckUpdates;
    //    qDebug() << "Open Output Dir: " << settings.openOutputDir;
    //    qDebug() << "Keep Spec File: " << settings.keepSpecFile;
    //    qDebug() << "Keep Build Directory: " << settings.keepBuildDirectory;
    // 根据枚举值刷新UI界面的主题部分
    if (settings.theme == Settings::Light) {
        ui->lightTheme_rdo->setChecked(true);
    } else if (settings.theme == Settings::Dark) {
        ui->darkTheme_rdo->setChecked(true);
    } else if (settings.theme == Settings::System) {
        ui->SystemTheme_rdo->setChecked(true);
    }
    // 版本及更新部分
    ui->version_lbl->setText(settings.version);
    ui->autoCheckUpdates_chk->setChecked(settings.autoCheckUpdates);
    // 高级部分
    ui->openOutputDir_chk->setChecked(settings.openOutputDir);
    ui->keepSpecFile_chk->setChecked(settings.keepSpecFile);
    ui->keepBuildDirectory_chk->setChecked(settings.keepBuildDirectory);
}

// 从UI界面读取设置项
void Setting::UIread()
{
    // 根据UI界面的主题部分刷新枚举值
    if (ui->lightTheme_rdo->isChecked()) {
        settings.theme = Settings::Light;
    } else if (ui->darkTheme_rdo->isChecked()) {
        settings.theme = Settings::Dark;
    } else if (ui->SystemTheme_rdo->isChecked()) {
        settings.theme = Settings::System;
    }
    // 版本及更新部分
    settings.autoCheckUpdates = ui->autoCheckUpdates_chk->isChecked();
    // 高级部分
    settings.openOutputDir = ui->openOutputDir_chk->isChecked();
    settings.keepSpecFile = ui->keepSpecFile_chk->isChecked();
    settings.keepBuildDirectory = ui->keepBuildDirectory_chk->isChecked();
}

// 取消按钮
void Setting::on_cancel_Btn_clicked()
{
    emit settingChanged();
    hide();
}

// 确定按钮
void Setting::on_sure_Btn_clicked()
{
    UIread();
    writeSettingsToFile();
    emit settingChanged();
    hide();
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on_checkUpdate_Btn_clicked()
{
    UpdateCheckerDialog updateChecker(this->styleSheet(),true,settings.version,this);
    updateChecker.exec();
}
