#ifndef MYFILESELECTIONBTN_H
#define MYFILESELECTIONBTN_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QFileDialog>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDebug>

class MyFileSelectionButton : public QPushButton
{
    Q_OBJECT
public:
    MyFileSelectionButton(QWidget *parent = 0);

signals:
    void FileAppeared(QString filePath);

protected:
    // 重写 dragEnterEvent
    void dragEnterEvent(QDragEnterEvent *event);

    // 重写 dropEvent
    void dropEvent(QDropEvent *event);

};

#endif // MYFILESELECTIONBTN_H
