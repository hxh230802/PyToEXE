#ifndef PYTOEXE_H
#define PYTOEXE_H

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include <QProcess>
#include <QScrollBar>
#include <QThread>
#include <QDesktopServices>
#include <QDateTime>
#include <QMouseEvent>
#include <QStandardPaths>
#include <QPainter>
#include <QTimer>
#include <QBitmap>
#include <QSettings>
#include <QClipboard>

#include "Libs/include/SuspendedTipBar"

#include "functionalModule/Setting/setting.h"
#include "functionalModule/AdvancedOptions/advancedoptions.h"

#include "functionalRealization/About/about.h"
#include "functionalRealization/Package/package.h"
#include "functionalRealization/ToolsInstall/toolsinstall.h"

namespace Ui {
class PyToEXE;
}

class PyToEXE : public QWidget
{
    Q_OBJECT

public:
    explicit PyToEXE(QWidget *parent = 0);
    ~PyToEXE();

private slots:
    void on_sourceFile_Btn_clicked();

    void on_OutPutPath_cmb_currentIndexChanged(int index);

    void on_OutPutPath_Btn_clicked();

    void on_configIcon_chk_clicked(bool checked);

    void on_configIcon_Btn_clicked();

    void on_DisplayCMD_chk_clicked(bool checked);

    void on_OutPutName_le_textChanged(const QString &arg1);

    void on_start_Btn_clicked();

    void on_OutPutType_cmb_currentIndexChanged(int index);

    void on_InstallTolls_Btn_clicked();

    void on_UsingTutorials_Btn_clicked();

    void on_settop_Btn_clicked();

    void on_close_Btn_clicked();

    void on_minimize_Btn_clicked();

    void on_titleIcon_Btn_clicked();

    void on_About_Btn_clicked();

    void on_DarkTheme_chk_clicked(bool checked);

    void on_setting_Btn_clicked();

    void on_PythonFile_Btn_clicked();

    void on_AdvancedOptions_Btn_clicked();

    void on_CopyCmd_Btn_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        Q_UNUSED(event);
        updateMask();
    }

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    Ui::PyToEXE *ui;

    Config config;
    Settings settings;
    AdvancedConfiguration advancedConfiguration;
    //    FileAndSystem fileandsystem;

    Setting *setting;

    QString commandString;
    QStringList commandList;

    void UIinit();
    void renewCommand();

    bool isDark = false;
    void ThemeRenewFromSettings();
    void ThemeRenew();
    QString ReadStyleSheet(QString path);
    void getSyStemTheme();

    int titleBarHeight; // 标题栏高度
    bool isMousePressed;
    QPoint dragPosition;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool isOnTop = false;

    // 窗口圆角
    void updateMask()
    {
        QBitmap bmp(size());
        bmp.fill();
        QPainter p(&bmp);
        p.setPen(Qt::NoPen);
        p.setBrush(Qt::black);
        p.drawRoundedRect(bmp.rect(), 15, 15);  // 设置圆角半径
        setMask(bmp);
    }

    // 绘制窗口外边框线
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        // 设置线的颜色为 RGB(104, 104, 104)
        QColor lineColor(104, 104, 104);
        // 绘制窗口外边框线
        painter.setPen(QPen(lineColor, 2));
        painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 15, 15);
    }
};

#endif // PYTOEXE_H
