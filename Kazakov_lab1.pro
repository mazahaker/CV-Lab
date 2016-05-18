QT += core
QT += gui

CONFIG += c++14

TARGET = Kazakov_lab1
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Image.cpp \
    ImageUtils.cpp \
    Pyramid.cpp \
    PyramidOctave.cpp \
    PyramidLevel.cpp \
    IntrestingPoint.cpp \
    Descriptor.cpp \
    DescriptorConstructor.cpp

HEADERS += \
    Image.h \
    ImageUtils.h \
    Pyramid.h \
    PyramidOctave.h \
    PyramidLevel.h \
    IntrestingPoint.h \
    Descriptor.h \
    Kernels.h \
    DescriptorConstructor.h

