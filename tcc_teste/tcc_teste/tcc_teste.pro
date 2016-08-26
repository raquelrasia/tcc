#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T18:47:41
#
#-------------------------------------------------

QT       += core gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcc_teste
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mycoursedialog.cpp \
    mydialog.cpp

HEADERS  += mainwindow.h \
    mycoursedialog.h \
    mydialog.h

FORMS    += mainwindow.ui \
    mycoursedialog.ui \
    mydialog.ui

RESOURCES += \
    icons.qrc
