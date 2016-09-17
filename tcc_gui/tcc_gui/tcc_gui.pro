#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T20:04:41
#
#-------------------------------------------------

QT       += core gui
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tcc_gui
TEMPLATE = app


SOURCES += main.cpp\
        logindialog.cpp \
    mycoursedialog.cpp \
    mydialog.cpp \
    maindialog.cpp \
    networkconnection.cpp

HEADERS  += logindialog.h \
    mycoursedialog.h \
    mydialog.h \
    maindialog.h \
    networkconnection.h

FORMS    += logindialog.ui \
    mycoursedialog.ui \
    mydialog.ui \
    maindialog.ui
    
CONFIG += c++11
QT += network