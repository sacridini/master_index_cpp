TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/gdal
LIBS += -lgdal

SOURCES += \
        main.cpp
