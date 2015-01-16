#-------------------------------------------------
#
# Project created by QtCreator 2015-01-02T20:42:37
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = download-commander
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myclient.cpp \
    jcliehandler.cpp

HEADERS  += mainwindow.h \
    myclient.h \
    jcliehandler.h

FORMS    += mainwindow.ui
