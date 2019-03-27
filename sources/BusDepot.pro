#-------------------------------------------------
#
# Project created by QtCreator 2018-11-11T12:08:47
#
#-------------------------------------------------

QT       += core gui
QT       += sql widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BusDepot
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainmenu.cpp \
    mainwindow.cpp \
    tablemenu.cpp \
    routeform.cpp \
    driverform.cpp \
    createmessage.cpp \
    busform.cpp \
    controlform.cpp \
    otherform.cpp \
    salaryform.cpp \
    typeform.cpp

HEADERS += \
        mainmenu.h \
    mainwindow.h \
    tablemenu.h \
    routeform.h \
    driverform.h \
    createmessage.h \
    busform.h \
    controlform.h \
    otherform.h \
    salaryform.h \
    typeform.h

FORMS += \
        mainmenu.ui \
    mainwindow.ui \
    tablemenu.ui \
    routeform.ui \
    driverform.ui \
    busform.ui \
    controlform.ui \
    otherform.ui \
    salaryform.ui \
    typeform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
