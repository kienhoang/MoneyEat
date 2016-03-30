#-------------------------------------------------
#
# Project created by QtCreator 2016-03-18T11:07:27
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MoneyEat_v2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    log.cpp \
    editdate.cpp

HEADERS  += mainwindow.h \
    log.h \
    editdate.h

FORMS    += mainwindow.ui \
    log.ui \
    editdate.ui
