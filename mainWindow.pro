#-------------------------------------------------
#
# Project created by QtCreator 2017-03-17T09:13:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mainWindow
TEMPLATE = app


SOURCES += main.cpp \
    image_function.cpp

HEADERS  += \
    image_function.h

FORMS    +=

LIBS += `pkg-config opencv --libs`
