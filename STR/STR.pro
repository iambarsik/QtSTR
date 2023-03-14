QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    formatcontainer.cpp \
    functions.cpp \
    main.cpp \
    str.cpp

HEADERS += \
    formatcontainer.h \
    str.h

FORMS += \
    formatcontainer.ui \
    str.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../core
DEPENDPATH  += $$PWD/../core
win32: LIBS += -L$$PWD/../_output/ -lCore

INCLUDEPATH += $$PWD/../manager
DEPENDPATH  += $$PWD/../manager
win32: LIBS += -L$$PWD/../_output/ -lmanager

INCLUDEPATH += $$PWD/../templates/model
DEPENDPATH  += $$PWD/../templates/model
win32: LIBS += -L$$PWD/../_output/ -lmodel

INCLUDEPATH += $$PWD/../templates/format
DEPENDPATH  += $$PWD/../templates/format
win32: LIBS += -L$$PWD/../_output/ -lformat

INCLUDEPATH += $$PWD/../network_server
DEPENDPATH  += $$PWD/../network_server
win32: LIBS += -L$$PWD/../_output/ -lnetwork_server

INCLUDEPATH += $$PWD/../network_client
DEPENDPATH  += $$PWD/../network_client
win32: LIBS += -L$$PWD/../_output/ -lnetwork_client

BIN_PATH = $$PWD/../_output/
DESTDIR = $${BIN_PATH}/

#include($${_PRO_FILE_PWD_}/common.pri)
