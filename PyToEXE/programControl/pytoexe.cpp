#include "pytoexe.h"
#include "ui_pytoexe.h"

PyToEXE::PyToEXE(QWidget *parent) :
    QWidget(parent),
    isMousePressed(false), titleBarHeight(50), ui(new Ui::PyToEXE)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setMouseTracking(true);

    setting = new Setting;
    settings = setting->readSettingsFromFile();
    connect(setting, &Setting::settingChanged, [=](){
        settings = setting->readSettingsFromFile();
        ThemeRenewFromSettings();
        renewCommand();
    });

    AdvancedOptions advancedOptions;
    advancedConfiguration = advancedOptions.readConfigurationFromFile();

    ui->setupUi(this);

    UIinit();
    ThemeRenewFromSettings();

    updateMask();

    if(settings.autoCheckUpdates)
    {
        UpdateCheckerDialog updateChecker(this->styleSheet(),false,settings.version,this);
        updateChecker.exec();
    }

}

// 源文件选择
void PyToEXE::on_sourceFile_Btn_clicked()
{
    // 选择文件
    config.sourceFilePath = QFileDialog::getOpenFileName(nullptr, "选择Python源文件", "", "Python Files (*.py)");
    if (config.sourceFilePath.isEmpty()) return;
    // 获取文件信息
    QFileInfo fileInfo(config.sourceFilePath);
    ui->sourceFileName_lbl->setText(fileInfo.fileName()); // 源文件名显示

    // 默认输出配置
    config.outputDir = fileInfo.dir().path();
    config.sourceFileDir = config.outputDir;
    config.outputName = fileInfo.baseName();
    config.outputPath =config. outputDir + "/" + config.outputName + ".exe";
    ui->OutPutName_le->setText(fileInfo.baseName());   // 输出文件名显示
    ui->OutPutPath_le->setText(fileInfo.dir().path()); // 输出文件路径显示
    ui->OutPutPath_cmb->setCurrentIndex(0);            // 输出文件路径配置：源文件路径

    qDebug() << "on_sourceFile_Btn_clicked: "
             << "sourceFilePath:" << config.sourceFilePath
             << "outputPath:" << config.outputPath;
    renewCommand();
}

void PyToEXE::on_PythonFile_Btn_clicked()
{
    QUrl url = QUrl::fromLocalFile(config.sourceFileDir);
    QDesktopServices::openUrl(url);
}

// 自定义图标复选框
void PyToEXE::on_configIcon_chk_clicked(bool checked)
{
    if(checked == true)
    {
        ui->configIcon_Btn->setEnabled(true);
        config.icon = true;
    }
    if(checked == false)
    {
        ui->configIcon_Btn->setEnabled(false);
        config.icon = false;
    }
    qDebug() << "on_configIcon_checkBox_clicked: "
             << "icon:" << config.icon;
    renewCommand();
}

// 自定义图标选择
void PyToEXE::on_configIcon_Btn_clicked()
{
    // 选择文件
    config.iconPath = QFileDialog::getOpenFileName(nullptr, "选择图标文件", "", "图标文件 (*.ico)");
    if (config.iconPath.isEmpty()) return;

    // 获取文件信息
    QFileInfo fileInfo(config.iconPath);
    ui->configIconName_lbl->setText(fileInfo.fileName()); // 图标文件名显示

    qDebug() << "on_configIcon_Btn_clicked: "
             << "iconPath:" << config.iconPath;
    renewCommand();
}

// 显示CMD窗口复选框
void PyToEXE::on_DisplayCMD_chk_clicked(bool checked)
{
    if(checked == true) // 显示
    {
        config.displayCMD = true;
    }
    if(checked == false) // 不显示
    {
        config.displayCMD = false;
    }
    qDebug() << "on_DisplayCMD_checkBox_clicked: "
             << "displayCMD:" << config.displayCMD;
    renewCommand();
}

// 打开高级选项窗口
void PyToEXE::on_AdvancedOptions_Btn_clicked()
{
    AdvancedOptions* advancedOptions = new AdvancedOptions(this,advancedConfiguration);
    connect(advancedOptions, &AdvancedOptions::AdvancedOptionsChanged, this, [=](AdvancedConfiguration advanced) {
        advancedConfiguration= advanced;
        renewCommand();
    });
    advancedOptions->exec();
}

// 输出配置：单文件、文件夹
void PyToEXE::on_OutPutType_cmb_currentIndexChanged(int index)
{
    if(index == 0) // 单文件
    {
        config.fileOrDir = true;
    }
    if(index == 1) // 文件夹
    {
        config.fileOrDir = false;
    }
    qDebug() << "on_OutPutType_comboBox_currentIndexChanged: "
             << "fileOrDir:" << config.fileOrDir;
    renewCommand();
}

// 输出文件名被更改
void PyToEXE::on_OutPutName_le_textChanged(const QString &arg1)
{
    config.outputName  = arg1;
    config.outputPath = config.outputDir + "/" + config.outputName + ".exe";

    qDebug() << "on_OutPutName_lineEdit_textChanged: "
             << "outputPath:" << config.outputPath;
    renewCommand();
}

// 输出目录选择框
void PyToEXE::on_OutPutPath_cmb_currentIndexChanged(int index)
{
    if(index == 0) // 源文件路径
    {
        QFileInfo fileInfo(config.sourceFilePath);
        // 获取源文件目录
        config.outputDir = fileInfo.dir().path();
        ui->OutPutPath_Btn->setEnabled(false);
    }
    if(index == 1) // 桌面
    {
        QStringList desktopPaths = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);

        if (!desktopPaths.isEmpty()) {
            QString desktopPath = desktopPaths.first();
            config.outputDir = desktopPath;
            qDebug() << desktopPath;
        } else {
            qDebug() << "无法获取桌面路径";
        }
        ui->OutPutPath_Btn->setEnabled(false);
    }
    if(index == 2) // 程序目录
    {
        config.outputDir = QCoreApplication::applicationDirPath()+"/output";
        ui->OutPutPath_Btn->setEnabled(false);
    }
    if(index == 3) // 自定义
    {
        config.outputDir = "";
        ui->OutPutPath_Btn->setEnabled(true);
    }
    ui->OutPutPath_le->setText(config.outputDir);
    config.outputPath = config.outputDir + "/" + config.outputName + ".exe";
    qDebug() << "on_OutPutPath_comboBox_currentIndexChanged: "
             << "outputPath:" << config.outputPath;
    renewCommand();
}

// 选择输出目录
void PyToEXE::on_OutPutPath_Btn_clicked()
{
    // 选择目录
    config.outputDir = QFileDialog::getExistingDirectory(nullptr, "选择输出目录", "", QFileDialog::ShowDirsOnly);
    if (config.outputDir.isEmpty()) return;

    ui->OutPutPath_le->setText(config.outputDir);
    config.outputPath = config.outputDir + "/" + config.outputName + ".exe";
    qDebug() << "on_OutPutPath_Btn_clicked: "
             << "outputPath:" << config.outputPath;
    renewCommand();
}

// 复制按钮
void PyToEXE::on_CopyCmd_Btn_clicked()
{
    // 获取LineEdit的内容
    QString textToCopy = ui->CMD_le->text();
    // 将内容复制到剪贴板
    QApplication::clipboard()->setText(textToCopy);
    // 提示复制成功
    SuspendedTipBar *tipbar = new SuspendedTipBar(this,"复制成功！",1500,isDark);
    tipbar->show();
}

// 开始输出
void PyToEXE::on_start_Btn_clicked()
{
    // 判断是否有源文件
    if(config.sourceFilePath == "" )
    {
        SuspendedTipBar *tipbar = new SuspendedTipBar(this,"请选择源文件！",2000,isDark);
        tipbar->show();
        return;
    }
    // 开始打包
    Package *package = new Package(config,settings,advancedConfiguration);
    package->setStyleSheet(this->styleSheet());
    package->show();
}

// 使用教程
void PyToEXE::on_UsingTutorials_Btn_clicked()
{
    // 打开我的主页
    QString url = "https://space.bilibili.com/1396650915?spm_id_from=333.1007.0.0";
    QUrl bilibiliUrl(url);

    if (QDesktopServices::openUrl(bilibiliUrl)) {
        qDebug() << "URL opened successfully.";
    } else {
        qDebug() << "Failed to open URL.";
    }

    // 打开教程
    QString url_ = "https://www.bilibili.com/read/cv25555686";
    QUrl bilibiliUrl_(url_);

    if (QDesktopServices::openUrl(bilibiliUrl_)) {
        qDebug() << "URL opened successfully.";
    } else {
        qDebug() << "Failed to open URL.";
    }
}

// 安装工具
void PyToEXE::on_InstallTolls_Btn_clicked()
{
    ToolsInstall *toolsinstall = new ToolsInstall;
    toolsinstall->setStyleSheet(this->styleSheet());
    toolsinstall->show(); // 安装工具对话框
}

// 关于软件
void PyToEXE::on_About_Btn_clicked()
{
    About aboutDialog(settings.version,this);
    aboutDialog.exec();  // 执行阻塞式对话框
}

// 更新命令
void PyToEXE::renewCommand()
{
    commandList = m_renewCommand(config,advancedConfiguration).commandList;
    config = m_renewCommand(config,advancedConfiguration).config;

    // 更新commandString
    commandString = commandList.join(" ");
    ui->CMD_le->setText(commandString);
}

/*      下面是标题栏部分的代码      */
/*       按控件从左到右排序        */
/*      ==================      */

// 左上标题图标
void PyToEXE::on_titleIcon_Btn_clicked()
{
    on_UsingTutorials_Btn_clicked();
}

// 暗色主题复选框
void PyToEXE::on_DarkTheme_chk_clicked(bool checked)
{
    isDark = checked;

    ThemeRenew();
}

// 设置按钮
void PyToEXE::on_setting_Btn_clicked()
{
    setting->setStyleSheet(this->styleSheet());
    settings = setting->readSettingsFromFile();
    setting->show();
}

// 最小化按钮
void PyToEXE::on_minimize_Btn_clicked()
{
    showMinimized();
}

// 置顶按钮
void PyToEXE::on_settop_Btn_clicked()
{
    if(!isOnTop)
    {
        Qt::WindowFlags m_flags = windowFlags();
        setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
        QFile styleFile(":/Resource/pic/titleBarIcon/settop_on.qss");
        styleFile.open(QFile::ReadOnly | QFile::Text);
        QString styleSheet = styleFile.readAll();
        styleFile.close();
        ui->settop_Btn->setStyleSheet(styleSheet);
        show();
        qDebug()<<"always top";
    }else{
        setWindowFlags(Qt::FramelessWindowHint);
        QFile styleFile(":/Resource/pic/titleBarIcon/settop_off.qss");
        styleFile.open(QFile::ReadOnly | QFile::Text);
        QString styleSheet = styleFile.readAll();
        styleFile.close();
        ui->settop_Btn->setStyleSheet(styleSheet);
        show();
        qDebug()<<"no always top";
    }
    isOnTop=!isOnTop;

}

// 关闭按钮
void PyToEXE::on_close_Btn_clicked()
{
    qApp->quit();
}

/*      下面是GUI部分的代码      */
/*          按功能排序          */
/*     ==================     */

// UI初始化
void PyToEXE::UIinit()
{
    ThemeRenewFromSettings();

    ui->OutPutPath_le->setReadOnly(true);
    ui->CMD_le->setReadOnly(true);

    ui->configIcon_Btn->setEnabled(false);
    ui->OutPutPath_Btn->setEnabled(false);

    ui->DisplayCMD_chk->setChecked(true);
}

// 从设置中更新主题
void PyToEXE::ThemeRenewFromSettings()
{
    if(settings.theme == Settings::Light)
    {
        isDark = false;
        ui->DarkTheme_chk->setChecked(false);
    }
    else if(settings.theme == Settings::Dark)
    {
        isDark = true;
        ui->DarkTheme_chk->setChecked(true);
    }
    else if(settings.theme == Settings::System)
    {
        getSyStemTheme();
    }

    // 创建定时器对象
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true); // 只执行一次
    // 在 50 毫秒后连接定时器到相应槽函数
    connect(timer, &QTimer::timeout, [=](){
        ThemeRenew();
    });
    timer->start(50);  // 启动定时器
}

//获取系统主题
void PyToEXE::getSyStemTheme()
{
    // 打开注册表
    QSettings regSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", QSettings::NativeFormat);

    // 读取注册表中的AppUseLightTheme键值
    bool useLightTheme = regSettings.value("AppsUseLightTheme", true).toBool();

    if (useLightTheme)
    {
        qDebug() << "当前主题为浅色";
        ui->DarkTheme_chk->setChecked(false);
        isDark = false;
    }
    else
    {
        qDebug() << "当前主题为暗色";
        ui->DarkTheme_chk->setChecked(true);
        isDark = true;
    }
}

// 更新主题
void PyToEXE::ThemeRenew()
{
    if(!isDark) // 亮色
    {
        setStyleSheet(ReadStyleSheet(":/Resource/theme/light_theme.qss"));
    }
    else        // 暗色
    {
        setStyleSheet(ReadStyleSheet(":/Resource/theme/dark_theme.qss"));
    }
}

// 读取样式表
QString PyToEXE::ReadStyleSheet(QString path)
{
    QFile styleFile(path);
    styleFile.open(QFile::ReadOnly | QFile::Text);
    QString styleSheet = styleFile.readAll();
    styleFile.close();
    return styleSheet;
}

/*      下面是其他部分的代码      */
/*          按功能排序          */
/*     ==================     */

// 鼠标事件
void PyToEXE::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton /*&& event->pos().y() < titleBarHeight*/) {
        isMousePressed = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}
void PyToEXE::mouseMoveEvent(QMouseEvent *event)
{
    if (isMousePressed && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}
void PyToEXE::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        isMousePressed = false;
        event->accept();
    }
}

void PyToEXE::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1) {
        QString filePath = event->mimeData()->urls().first().toLocalFile();
        if (filePath.endsWith(".py"))
            event->acceptProposedAction();
    }
}

void PyToEXE::dropEvent(QDropEvent *event) {
    QString filePath = event->mimeData()->urls().first().toLocalFile();
    if (filePath.endsWith(".py")) {

        config.sourceFilePath = filePath;

        // 获取文件信息
        QFileInfo fileInfo(config.sourceFilePath);
        ui->sourceFileName_lbl->setText(fileInfo.fileName()); // 源文件名显示

        // 默认输出配置
        config.outputDir = fileInfo.dir().path();
        config.sourceFileDir = config.outputDir;
        config.outputName = fileInfo.baseName();
        config.outputPath =config. outputDir + "/" + config.outputName + ".exe";
        ui->OutPutName_le->setText(fileInfo.baseName());   // 输出文件名显示
        ui->OutPutPath_le->setText(fileInfo.dir().path()); // 输出文件路径显示
        ui->OutPutPath_cmb->setCurrentIndex(0);            // 输出文件路径配置：源文件路径

        qDebug() << "on_sourceFile_Btn_clicked: "
                 << "sourceFilePath:" << config.sourceFilePath
                 << "outputPath:" << config.outputPath;
        renewCommand();
    }
}

// 析构函数
PyToEXE::~PyToEXE()
{
    delete ui;
}

