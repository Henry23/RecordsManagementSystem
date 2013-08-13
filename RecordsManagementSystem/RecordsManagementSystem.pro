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
    createfielddialog.cpp

HEADERS  += mainwindow.h \
    createfielddialog.h

FORMS    += mainwindow.ui \
    createfielddialog.ui
