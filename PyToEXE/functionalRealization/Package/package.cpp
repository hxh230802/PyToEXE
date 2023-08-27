#include "package.h"
#include "ui_package.h"

#include <QtDebug>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDateTime>

void printConfigValues(const Config& config) {
    qDebug() << "sourceFilePath: " << config.sourceFilePath;
    qDebug() << "sourceFileDir: " << config.sourceFileDir;
    qDebug() << "outputDir: " << config.outputDir;
    qDebug() << "outputName: " << config.outputName;
    qDebug() << "outputPath: " << config.outputPath;
    qDebug() << "fileOrDir: " << config.fileOrDir;
    qDebug() << "icon: " << config.icon;
    qDebug() << "iconPath: " << config.iconPath;
    qDebug() << "displayCMD: " << config.displayCMD;
    qDebug() << "workPath: " << config.workPath;
}

void CreateVersionResource(Config config,AdvancedConfiguration advancedConfiguration);

Package::Package(Config config,Settings setting,AdvancedConfiguration advancedConfiguration,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Package),
    process(new QProcess(this))
{
    ui->setupUi(this);

    outputconfig = config;

    // 创建工作目录
    fileandsystem.createDirectory(outputconfig.workPath);

    // 生成详细信息文件
    if(advancedConfiguration.fileDetails.enable)
        CreateVersionResource(config,advancedConfiguration);

    // 创建 CppSyntaxHighlighter 对象并设置为文档的语法高亮器
    CppSyntaxHighlighter* highlighter = new CppSyntaxHighlighter(ui->OutMsg_te->document());

    connect(process, &QProcess::readyReadStandardOutput, this, &Package::onProcessReadyRead);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Package::onProcessFinished);

    QString program = "pyinstaller";
    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(program, m_renewCommand(config,advancedConfiguration).commandList);

    settings = setting;

    // 更新commandString
    QString commandString = m_renewCommand(config,advancedConfiguration).commandList.join(" ");
    ui->CMD_le->setText(commandString);

}

QString addCharToQString(const QString& input,QChar insert) {
    QString result;
    for (int i = 0; i < input.size(); ++i) {
        result += input.at(i);
        if (i < input.size() - 1) {
            result += insert;
        }
    }
    return result;
}

void CreateVersionResource(Config config,AdvancedConfiguration advancedConfiguration)
{
    qDebug() << "CreateVersionResource:";
    // 构建VSVersionInfo格式的资源字符串
    QString versionResourceString
            = QString::fromUtf8("VSVersionInfo(\n"
                                "ffi=FixedFileInfo(\n"
                                "filevers=(%1),\n"
                                "prodvers=(%2),\n"
                                "mask=0x3f,\n"
                                "flags=0x0,\n"
                                "OS=0x4,\n"
                                "fileType=0x1,\n"
                                "subtype=0x0,\n"
                                "date=(0, 0)\n"
                                "),\n"
                                "kids=[\n"
                                "StringFileInfo([\n"
                                "StringTable(u'040904B0', [\n"
                                "StringStruct(u'CompanyName', u'%3'),\n"
                                "StringStruct(u'FileDescription', u'%4'),\n"
                                "StringStruct(u'FileVersion', u'%5'),\n"
                                "StringStruct(u'InternalName', u'%6'),\n"
                                "StringStruct(u'LegalCopyright', u'%7'),\n"
                                "StringStruct(u'OriginalFilename', u'%8'),\n"
                                "StringStruct(u'ProductName', u'%9'),\n"
                                "StringStruct(u'ProductVersion', u'%10')\n"
                                "])\n"
                                "]),\n"
                                "VarFileInfo([VarStruct(u'Translation', [2052, 1200])])\n"
                                "]"
                                ")"
                                )
            .arg(addCharToQString(advancedConfiguration.fileDetails.fileVersion,','))
            .arg(addCharToQString(advancedConfiguration.fileDetails.productVersion,','))
            .arg(advancedConfiguration.fileDetails.companyName)
            .arg(advancedConfiguration.fileDetails.fileDescription)
            .arg(addCharToQString(advancedConfiguration.fileDetails.fileVersion,'.'))
            .arg(advancedConfiguration.fileDetails.internalName)
            .arg(advancedConfiguration.fileDetails.legalCopyright)
            .arg(advancedConfiguration.fileDetails.originalFilename)
            .arg(advancedConfiguration.fileDetails.productName)
            .arg(addCharToQString(advancedConfiguration.fileDetails.productVersion,'.'));

    // 将资源字符串写入到新的文件中
    QString resourceFileName = config.workPath + "/fileDetails.txt";
    QFile resourceFile(resourceFileName);
    if (resourceFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&resourceFile);
        stream.setCodec("UTF-8");
        stream << versionResourceString;
        resourceFile.close();
    }
}

// 更新命令
CommandAndConfig m_renewCommand(Config config,AdvancedConfiguration advancedConfiguration)
{
    CommandAndConfig result;

    QStringList commandList;

    // 配置信息：单文件/文件夹
    if(config.fileOrDir) commandList.append("-F");
    else          commandList.append("-D");

    // 源文件路径
    commandList.append(config.sourceFilePath);

    // 输出文件名
    commandList.append("-n");
    commandList.append(config.outputName);

    // 输出路径
    commandList.append("--distpath");
    commandList.append(config.outputDir);

    // 工作目录
    config.workPath = QCoreApplication::applicationDirPath()+"/build/"+config.outputName;
    commandList.append("--workpath");
    commandList.append(config.workPath);

    // spec文件目录
    commandList.append("--specpath");
    commandList.append(config.workPath);

    // 自定义图标
    if(config.icon)
    {
        commandList.append("-i");
        commandList.append(config.iconPath);
    }

    // 是否显示CMD窗口
    if(!config.displayCMD) commandList.append("-w");

    // 清理以前的临时文件
    commandList.append("--clean");

    /*下面是高级选项*/
    // 指定了文件详细信息
    if(advancedConfiguration.fileDetails.enable)
    {
        commandList.append("--version-file");
        commandList.append(config.workPath+"/fileDetails.txt");
    }
    // 是否支持Unicode
    if(!advancedConfiguration.packageOptions.supportUnicode)
    {
        commandList.append("-a");
    }
    // 是否启用加密
    if(advancedConfiguration.packageOptions.encryption)
    {
        commandList.append("--key");
        commandList.append(advancedConfiguration.packageOptions.key);
    }

    result.commandList = commandList;
    result.config = config;

    return result;
}

void Package::onProcessReadyRead()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    if (process) {
        QByteArray buffer = process->readAllStandardOutput();
        QString output = QString::fromLocal8Bit(buffer);
        // 追加输出到QTextEdit的末尾
        ui->OutMsg_te->append(output);
        // 将滚动条滚动到文本末尾
        ui->OutMsg_te->verticalScrollBar()->setValue(ui->OutMsg_te->verticalScrollBar()->maximum());
        // 检查关键行，判断是否成功
        if (output.contains("Building EXE from EXE-00.toc completed successfully.")) collectCompleted = true;
        if (output.contains("Error")) Error = true;

    }
}

void Package::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Process finished with exit code" << exitCode;
    if (collectCompleted) {
        qDebug() << "successfully" << exitCode;
        // 提示转换成功
        SuspendedTipBar* p_widget = new SuspendedTipBar(this,"转换成功！",2500,isDark());
        p_widget->show();
    }
    if (!collectCompleted||Error)
    {
        qDebug() << "Error" << exitCode;
        // 提示转换失败
        SuspendedTipBar* p_widget = new SuspendedTipBar(this,"转换出现问题，详情请看输出信息！",2500,isDark());
        p_widget->show();
    }
    if(settings.openOutputDir) QDesktopServices::openUrl(QUrl::fromLocalFile(outputconfig.outputDir));
    if(!settings.keepSpecFile) QFile::remove(outputconfig.workPath+"/"+outputconfig.outputName+".spec");
    if(!settings.keepBuildDirectory) fileandsystem.removeDirectory(outputconfig.workPath+"/"+outputconfig.outputName);
    if(!settings.keepSpecFile&&!settings.keepBuildDirectory) fileandsystem.removeDirectory(outputconfig.workPath);
}

bool Package::isDark()
{
    QPalette palette = this->palette();  // 获取窗口的调色板
    QColor backgroundColor = palette.color(QPalette::Window);  // 获取窗口的背景色
    // 判断背景色是否属于暗色
    return backgroundColor.lightness() < 128;
}

Package::~Package()
{
    delete ui;
}

void Package::on_sure_Btn_clicked()
{
    close();
}

void Package::on_cancel_Btn_clicked()
{
    close();
}

void Package::on_clearOutput_Btn_clicked()
{
    ui->OutMsg_te->clear();
}

void Package::on_exportMsg_Btn_clicked()
{
    // 获取用户选择的输出目录
    QString outputDir = QFileDialog::getExistingDirectory(nullptr, "选择输出目录", QDir::homePath());

    if (outputDir.isEmpty()) {
        qWarning() << "未选择输出目录";
        return;
    }

    // 构建日志文件的完整路径
    QString logFileName = outputDir + "/PyToEXE_cmdOutputMmsg.log";

    QFile logFile(logFileName);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Failed to open log file:" << logFile.errorString();
        return;
    }

    QTextStream logStream(&logFile);
    logStream << "== Log started at " << QDateTime::currentDateTime().toString() << " ==" << endl;

    // 获取 ui->OutMsg_TextEdit 的文本内容
    QString text = ui->OutMsg_te->toPlainText();

    // 将文本内容写入日志文件
    logStream << text << endl;

    logFile.close();

    // 提示导出成功
    SuspendedTipBar* p_widget = new SuspendedTipBar(this,"导出成功！",2500,isDark());
    p_widget->show();
}
