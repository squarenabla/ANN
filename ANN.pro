#-------------------------------------------------
#
# Project created by QtCreator 2015-11-22T13:38:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ANN
TEMPLATE = app

LIBS += -L/usr/local/lib -lfann
LIBS += -I/usr/include/libusb-1.0/ -lusb-1.0

SOURCES += main.cpp\
    teacher.cpp \
    executor.cpp \
    tablewidget.cpp \
    device.cpp \
    controler.cpp \
    error.cpp \
    view.cpp

HEADERS  += \
    teacher.h \
    executor.h \
    types.h \
    tablewidget.h \
    device.h \
    controler.h \
    error.h \
    view.h \
    messages.h

FORMS    += \
    error.ui \
    main.ui
