#-------------------------------------------------
#
# Project created by QtCreator 2013-08-12T20:02:00
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RecordsManagementSystem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        dialogs/createfielddialog.cpp \
        classes/recordsfile.cpp

HEADERS  += mainwindow.h \
        dialogs/createfielddialog.h \
        classes/recordsfile.h \
        classes/adtfile.h

FORMS    += mainwindow.ui \
        dialogs/createfielddialog.ui

RESOURCES += \
    images.qrc
