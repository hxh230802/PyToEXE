#-------------------------------------------------
#
# Project created by QtCreator 2023-08-03T20:00:14
#
#-------------------------------------------------

QT       += core gui network winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PyToEXE
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(Libs/Libs.pri)
include(programControl/programControl.pri)
include(functionalRealization/functionalRealization.pri)
include(functionalModule/functionalModule.pri)
include(Libs/MyFileSelectionButton/MyFileSelectionButton.pri)

RC_FILE += detailed.rc

LIBS += $$PWD/Libs/Lib/libSuspendedTipBar.a
LIBS += $$PWD/Libs/Lib/libFileAndSystem.a

#RESOURCES += \
#    titlebaricon.qrc \
#    theme.qrc \
#    mainwindow.qrc
