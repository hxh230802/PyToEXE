#include "advancedoptions.h"
#include "ui_advancedoptions.h"

void writeConfigurationToFile(const AdvancedConfiguration& advanced);

AdvancedOptions::AdvancedOptions(QWidget *parent, const AdvancedConfiguration& advancedConfiguration)
    : QDialog(parent), ui(new Ui::AdvancedOptions), advancedConfig(advancedConfiguration)
{
    ui->setupUi(this);

    // 设置一位数字验证器
    OneDigitValidator* validator = new OneDigitValidator();
    ui->productVersion_le_1->setValidator(validator);
    ui->productVersion_le_2->setValidator(validator);
    ui->productVersion_le_3->setValidator(validator);
    ui->productVersion_le_4->setValidator(validator);
    ui->fileVersion_le_1->setValidator(validator);
    ui->fileVersion_le_2->setValidator(validator);
    ui->fileVersion_le_3->setValidator(validator);
    ui->fileVersion_le_4->setValidator(validator);

    // 连接信号和槽，实现跳转到下一个输入框
    connect(ui->productVersion_le_1, &QLineEdit::textChanged, this, [=](const QString& text) {
        if (text.length() == 1) {
            ui->productVersion_le_2->setFocus();  // 将焦点设置到下一个输入框
        }
    });

    connect(ui->productVersion_le_2, &QLineEdit::textChanged, this, [=](const QString& text) {
        if (text.length() == 1) {
            ui->productVersion_le_3->setFocus();  // 将焦点设置到下一个输入框
        }
    });

    connect(ui->productVersion_le_3, &QLineEdit::textChanged, this, [=](const QString& text) {
        if (text.length() == 1) {
            ui->productVersion_le_4->setFocus();  // 将焦点设置到下一个输入框
        }
    });

    // 连接信号和槽，实现跳转到下一个输入框
    connect(ui->fileVersion_le_1, &QLineEdit::textChanged, this, [=](const QString& text) {
        if (text.length() == 1) {
            ui->fileVersion_le_2->setFocus();  // 将焦点设置到下一个输入框
        }
    });

    connect(ui->fileVersion_le_2, &QLineEdit::textChanged, this, [=](const QString& text) {
        if (text.length() == 1) {
            ui->fileVersion_le_3->setFocus();  // 将焦点设置到下一个输入框
        }
    });

    connect(ui->fileVersion_le_3, &QLineEdit::textChanged, this, [=](const QString& text) {
        if (text.length() == 1) {
            ui->fileVersion_le_4->setFocus();  // 将焦点设置到下一个输入框
        }
    });
    UIrefresh(advancedConfiguration);
    ui->Packetizer->setFocus();
}

void AdvancedOptions::on_sure_Btn_clicked()
{
    emit AdvancedOptionsChanged(UIread());
    close();
}

void AdvancedOptions::on_cancel_Btn_clicked()
{
    close();
}

void AdvancedOptions::on_SaveAsDefault_Btn_clicked()
{
    writeConfigurationToFile(UIread());
}

void AdvancedOptions::on_RestoreDefault_Btn_clicked()
{
    UIrefresh(readConfigurationFromFile());
}

void AdvancedOptions::on_clear_Btn_clicked()
{
    clearTextFields();
}

// 读取ini
AdvancedConfiguration AdvancedOptions::readConfigurationFromFile()
{
    const QString& filePath = QCoreApplication::applicationDirPath()+"/AdvancedOptions.ini";

    QSettings settings(filePath, QSettings::IniFormat);

    AdvancedConfiguration advanced;

    // 读取打包器选项
    int packetizerValue = settings.value("Packetizer/Packetizer").toInt();
    advanced.packageOptions.packetizer = static_cast<PackageOptions::Packetizer>(packetizerValue);

    // 读取文件详细信息
    advanced.fileDetails.enable = settings.value("Details/enable").toBool();
    advanced.fileDetails.productVersion = settings.value("Details/productVersion").toString();
    advanced.fileDetails.fileVersion = settings.value("Details/fileVersion").toString();
    advanced.fileDetails.originalFilename = settings.value("Details/originalFilename").toString();
    advanced.fileDetails.fileDescription = settings.value("Details/fileDescription").toString();
    advanced.fileDetails.legalCopyright = settings.value("Details/legalCopyright").toString();
    advanced.fileDetails.companyName = settings.value("Details/companyName").toString();
    advanced.fileDetails.internalName = settings.value("Details/internalName").toString();
    advanced.fileDetails.productName = settings.value("Details/productName").toString();

    // 读取其他选项
    advanced.packageOptions.supportUnicode = settings.value("Other/SupportUnicode").toBool();
    advanced.packageOptions.encryption = settings.value("Other/Encryption").toBool();
    advanced.packageOptions.key = settings.value("Other/Key").toString();

    return advanced;
}

// 写入ini
void writeConfigurationToFile(const AdvancedConfiguration& advanced) {
    const QString& filePath = QCoreApplication::applicationDirPath() + "/AdvancedOptions.ini";

    QSettings settings(filePath, QSettings::IniFormat);

    // 写入打包器选项
    settings.setValue("Packetizer/Packetizer", static_cast<int>(advanced.packageOptions.packetizer));

    // 写入文件详细信息
    settings.setValue("Details/enable", advanced.fileDetails.enable);
    settings.setValue("Details/productVersion", advanced.fileDetails.productVersion);
    settings.setValue("Details/fileVersion", advanced.fileDetails.fileVersion);
    settings.setValue("Details/originalFilename", advanced.fileDetails.originalFilename);
    settings.setValue("Details/fileDescription", advanced.fileDetails.fileDescription);
    settings.setValue("Details/legalCopyright", advanced.fileDetails.legalCopyright);
    settings.setValue("Details/companyName", advanced.fileDetails.companyName);
    settings.setValue("Details/internalName", advanced.fileDetails.internalName);
    settings.setValue("Details/productName", advanced.fileDetails.productName);

    // 写入其他选项
    settings.setValue("Other/SupportUnicode", advanced.packageOptions.supportUnicode);
    settings.setValue("Other/Encryption", advanced.packageOptions.encryption);
    settings.setValue("Other/Key", advanced.packageOptions.key);

    // 在写入完成后，需要调用sync函数使设置立即生效
    settings.sync();
}

// 刷新UI
void AdvancedOptions::UIrefresh(const AdvancedConfiguration& advanced)
{
    // 打包器
    if(advanced.packageOptions.packetizer == advanced.packageOptions.pyinstaller)
    {
        ui->pyinstaller_rdo->setChecked(true);
    }else if(advanced.packageOptions.packetizer == advanced.packageOptions.nuitka)
    {
        ui->nuitka_rdo->setChecked(true);
    }
    // 文件详细信息
    ui->enableFileDetails_chk->setChecked(advanced.fileDetails.enable);
    ui->productName_le->setText(advanced.fileDetails.productName);
    if(advanced.fileDetails.productVersion.size() == 4){
    ui->productVersion_le_1->setText(advanced.fileDetails.productVersion.at(0));
    ui->productVersion_le_2->setText(advanced.fileDetails.productVersion.at(1));
    ui->productVersion_le_3->setText(advanced.fileDetails.productVersion.at(2));
    ui->productVersion_le_4->setText(advanced.fileDetails.productVersion.at(3));}
    if(advanced.fileDetails.fileVersion.size() == 4){
    ui->fileVersion_le_1->setText(advanced.fileDetails.fileVersion.at(0));
    ui->fileVersion_le_2->setText(advanced.fileDetails.fileVersion.at(1));
    ui->fileVersion_le_3->setText(advanced.fileDetails.fileVersion.at(2));
    ui->fileVersion_le_4->setText(advanced.fileDetails.fileVersion.at(3));}
    ui->originalFilename_le->setText(advanced.fileDetails.originalFilename);
    ui->internalName_le->setText(advanced.fileDetails.internalName);
    ui->fileDescription_le->setText(advanced.fileDetails.fileDescription);
    ui->companyName_le->setText(advanced.fileDetails.companyName);
    ui->legalCopyright_le->setText(advanced.fileDetails.legalCopyright);
    // 其他
    ui->supportUnicode_chk->setChecked(advanced.packageOptions.supportUnicode);
    ui->encryption_chk->setChecked(advanced.packageOptions.encryption);
    ui->key_le->setText(advanced.packageOptions.key);
}

// 读取UI
AdvancedConfiguration AdvancedOptions::UIread()
{
    AdvancedConfiguration advanced;

    // 打包器
    if (ui->pyinstaller_rdo->isChecked()) {
        advanced.packageOptions.packetizer = advanced.packageOptions.pyinstaller;
    } else if (ui->nuitka_rdo->isChecked()) {
        advanced.packageOptions.packetizer = advanced.packageOptions.nuitka;
    }

    // 文件详细信息
    advanced.fileDetails.enable = ui->enableFileDetails_chk->isChecked();
    advanced.fileDetails.productName = ui->productName_le->text();
    advanced.fileDetails.productVersion.clear();
    advanced.fileDetails.productVersion.append(ui->productVersion_le_1->text());
    advanced.fileDetails.productVersion.append(ui->productVersion_le_2->text());
    advanced.fileDetails.productVersion.append(ui->productVersion_le_3->text());
    advanced.fileDetails.productVersion.append(ui->productVersion_le_4->text());
    advanced.fileDetails.fileVersion.clear();
    advanced.fileDetails.fileVersion.append(ui->fileVersion_le_1->text());
    advanced.fileDetails.fileVersion.append(ui->fileVersion_le_2->text());
    advanced.fileDetails.fileVersion.append(ui->fileVersion_le_3->text());
    advanced.fileDetails.fileVersion.append(ui->fileVersion_le_4->text());
    advanced.fileDetails.originalFilename = ui->originalFilename_le->text();
    advanced.fileDetails.internalName = ui->internalName_le->text();
    advanced.fileDetails.fileDescription = ui->fileDescription_le->text();
    advanced.fileDetails.companyName = ui->companyName_le->text();
    advanced.fileDetails.legalCopyright = ui->legalCopyright_le->text();

    // 其他
    advanced.packageOptions.supportUnicode = ui->supportUnicode_chk->isChecked();
    advanced.packageOptions.encryption = ui->encryption_chk->isChecked();
    advanced.packageOptions.key = ui->key_le->text();

    return advanced;
}

// 清空
void AdvancedOptions::clearTextFields()
{
    // 打包器
    ui->pyinstaller_rdo->setChecked(true);
    ui->nuitka_rdo->setChecked(false);

    // 文件详细信息
    ui->enableFileDetails_chk->setChecked(false);
    ui->productName_le->clear();
    ui->productVersion_le_1->clear();
    ui->productVersion_le_2->clear();
    ui->productVersion_le_3->clear();
    ui->productVersion_le_4->clear();
    ui->fileVersion_le_1->clear();
    ui->fileVersion_le_2->clear();
    ui->fileVersion_le_3->clear();
    ui->fileVersion_le_4->clear();
    ui->originalFilename_le->clear();
    ui->internalName_le->clear();
    ui->fileDescription_le->clear();
    ui->companyName_le->clear();
    ui->legalCopyright_le->clear();

    // 其他
    ui->supportUnicode_chk->setChecked(false);
    ui->encryption_chk->setChecked(false);
    ui->key_le->clear();
}

AdvancedOptions::~AdvancedOptions()
{
    delete ui;
}
