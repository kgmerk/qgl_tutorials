#-------------------------------------------------
#
# Project created by QtCreator 2013-10-02T22:05:16
#
#-------------------------------------------------

QT       += core gui widgets opengl

TARGET = qgl_tutorial05
TEMPLATE = app

mac {
    CONFIG -= app_bundle
}

SOURCES +=\
        qgl_tutorial05.cpp \
    mainwindow.cpp

HEADERS  += qgl_tutorial05.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    GLShaders.qrc
