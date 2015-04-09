#-------------------------------------------------
#
# Project created by QtCreator 2015-03-28T23:49:34
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += widgets
QT += widgets printsupport


DEFINES += BCCWIN DAVE_LITTLE_ENDIAN

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DaveQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    plcqtlib.cpp \
    qcustomplot.cpp \
    mythread.cpp \
    plotwindow.cpp \
    item.cpp

HEADERS  += mainwindow.h \
    plcqtlib.h \
    qcustomplot.h \
    mythread.h \
    plotwindow.h \
    item.h

FORMS    += mainwindow.ui \
    plotwindow.ui

win32: LIBS += -L$$PWD/libnodave/ -llibnodave

INCLUDEPATH += $$PWD/libnodave
DEPENDPATH += $$PWD/libnodave
