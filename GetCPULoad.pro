#-------------------------------------------------
#
# Project created by QtCreator 2019-08-10T07:09:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GetCPULoad
TEMPLATE = app


SOURCES +=\
        GetCPUWidget.cpp \
    GetCPUMain.cpp \
    TestCPUThread.cpp

HEADERS  += GetCPUWidget.h \
    TestCPUThread.h

FORMS    += GetCPUWidget.ui

LIBS += -lKernel32

TRANSLATIONS    = GetCPULoad_hu.ts 
