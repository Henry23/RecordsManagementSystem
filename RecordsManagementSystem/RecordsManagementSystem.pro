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
        classes/recordsfile.cpp \
        dialogs/createfielddialog.cpp \
        dialogs/showfieldsdialog.cpp

HEADERS  += mainwindow.h \
        classes/adtfile.h \
        classes/recordsfile.h \
        dialogs/createfielddialog.h \
        dialogs/showfieldsdialog.h

FORMS    += mainwindow.ui \
        dialogs/createfielddialog.ui \
        dialogs/showfieldsdialog.ui

RESOURCES += \
    images.qrc
