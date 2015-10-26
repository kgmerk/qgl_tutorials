#-------------------------------------------------
#
# Project created by QtCreator 2013-10-01T22:54:39
#
#-------------------------------------------------

QT       += core gui widgets opengl

TARGET = qgl_tutorial03
TEMPLATE = app

mac {
    CONFIG -= app_bundle
}


SOURCES +=\
        qgl_tutorial03a.cpp

HEADERS  += qgl_tutorial03a.h

RESOURCES += GLShaders.qrc
