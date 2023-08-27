#include "MyFileSelectionButton.h"

MyFileSelectionButton::MyFileSelectionButton(QWidget *parent) : QPushButton(parent)
{

    setFixedSize(120, 120); // 可根据需要调整大小

    // 创建布局来放置上下两个部分
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setAlignment(Qt::AlignCenter);

    // 创建上半部分的布局，包括图标和文本
    QWidget *topWidget = new QWidget;
    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);
    QLabel *iconLabel = new QLabel;
    iconLabel->setPixmap(QIcon(":/pic/icon.svg").pixmap(QSize(40,40)));
    QLabel *textLabel = new QLabel("拖\n入");
    textLabel->setStyleSheet("font: 8.5pt '微软雅黑';"
                             "text-align: left;");
    topLayout->addWidget(iconLabel);
    topLayout->addWidget(textLabel);
    layout->addWidget(topWidget);

    // 创建下半部分的文本标签
    QLabel *bottomLabel = new QLabel("或选择文件");
    bottomLabel->setStyleSheet("font: 8pt '微软雅黑';"
                               "text-align: left;");
    bottomLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(bottomLabel);

    // 将布局设置为按钮的布局
    setLayout(layout);

}

void MyFileSelectionButton::dragEnterEvent(QDragEnterEvent *event)  {
    if (event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1) {
        QString path = event->mimeData()->urls().first().toLocalFile();
        if (path.endsWith(".py")) {
            event->acceptProposedAction();
        }
    }
}

// 重写 dropEvent
void MyFileSelectionButton::dropEvent(QDropEvent *event)  {
    QString path = event->mimeData()->urls().first().toLocalFile();
    qDebug() << "Accepted file:" << path;
    emit FileAppeared(path);
}
