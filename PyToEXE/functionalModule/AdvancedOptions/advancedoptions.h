#ifndef ADVANCEDOPTIONS_H
#define ADVANCEDOPTIONS_H

#include <QDialog>
#include <QSettings>
#include <QDebug>
#include <QValidator>

// 定义文件详细信息结构体
struct FileDetails
{
    bool enable;
    QString companyName;        // 公司名称
    QString fileDescription;    // 文件描述
    QString fileVersion;        // 文件版本
    QString internalName;       // 内部名称
    QString legalCopyright;     // 版权信息
    QString originalFilename;   // 原始文件名
    QString productName;        // 产品名称
    QString productVersion;     // 产品版本
};

// 定义打包选项结构体
struct PackageOptions
{
    enum Packetizer { pyinstaller , nuitka };
    Packetizer packetizer; // 打包器
    bool supportUnicode;   // 是否支持Unicode
    bool encryption;       // 是否启用加密
    QString key;           // 加密密钥
};

// 定义配置文件的结构体
struct AdvancedConfiguration {
    FileDetails fileDetails;
    PackageOptions packageOptions;
};

namespace Ui {
class AdvancedOptions;
}

#include <QLineEdit>
#include <QFocusEvent>
#include <QMouseEvent>

class CustomLineEdit : public QLineEdit
{
public:
    CustomLineEdit(QWidget *parent = nullptr) : QLineEdit(parent)
    {
        // 安装事件过滤器
        installEventFilter(this);
    }

protected:
    void focusInEvent(QFocusEvent *event) override
    {
        selectAll();  // 在焦点进入时选择输入框的内容
        QLineEdit::focusInEvent(event);
    }

    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonRelease)
        {
            auto mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
                selectAll();  // 在鼠标点击时选择输入框的内容
        }
        return QLineEdit::eventFilter(obj, event);
    }
};

class OneDigitValidator : public QValidator {
public:
    QValidator::State validate(QString& input, int& pos) const {
        Q_UNUSED(pos);
        if (input.isEmpty())
            return QValidator::Intermediate;

        bool ok;
        int value = input.toInt(&ok);
        if (ok && value >= 0 && value <= 9 && input.length() == 1)
            return QValidator::Acceptable;

        return QValidator::Invalid;
    }
};

class AdvancedOptions : public QDialog
{
    Q_OBJECT

public:
    explicit AdvancedOptions(QWidget *parent = nullptr, const AdvancedConfiguration& advancedConfiguration = AdvancedConfiguration());
    AdvancedConfiguration readConfigurationFromFile();
    ~AdvancedOptions();

signals:
    void AdvancedOptionsChanged(AdvancedConfiguration advanced);

private slots:
    void on_sure_Btn_clicked();

    void on_cancel_Btn_clicked();

    void on_SaveAsDefault_Btn_clicked();

    void on_RestoreDefault_Btn_clicked();

    void on_clear_Btn_clicked();

private:
    Ui::AdvancedOptions *ui;

    AdvancedConfiguration advancedConfig;  // 声明 AdvancedConfiguration 成员变量
    void UIrefresh(const AdvancedConfiguration& advanced);
    AdvancedConfiguration UIread();
    void clearTextFields();
};

#endif // ADVANCEDOPTIONS_H
