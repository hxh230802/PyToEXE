#include "pytoexe.h"
#include <QApplication>
#include <QResource>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QResource::registerResource(qApp->applicationDirPath() + "/Resource/Resource.rcc");

    PyToEXE w;
    w.show();

    return a.exec();
}
