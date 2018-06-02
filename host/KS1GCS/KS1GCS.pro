#-------------------------------------------------
#
# Project created by QtCreator 2016-10-26T18:37:11
#
#-------------------------------------------------

QT       += core gui network serialport sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KS1GCS
TEMPLATE = app

QMAKE_CFLAGS += -std=gnu99

SOURCES += main.cpp\
        mainwindow.cpp \
    csploop.cpp \

HEADERS  += mainwindow.h \
    dm_data_eps.h \
    dm_data_obc.h \
    dm_data_ttc.h \
    dm_gcs.h \
    csploop.h \

FORMS    += mainwindow.ui

DISTFILES +=

LIBS += -lcsp

RESOURCES += \
    rc.qrc
