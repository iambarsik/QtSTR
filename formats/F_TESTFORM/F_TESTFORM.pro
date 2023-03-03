QT       += core gui

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

TEMPLATE = lib

SOURCES += \
    F_TESTFORM.cpp

HEADERS += \
    F_TESTFORM.h

FORMS += \
    F_TESTFORM.ui


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

BIN_PATH = $$PWD/../../_output/
DESTDIR = $${BIN_PATH}/

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core
win32: LIBS += -L$$PWD/../../_output/ -lCore

INCLUDEPATH += $$PWD/../../templates/format
DEPENDPATH += $$PWD/../../templates/format
win32: LIBS += -L$$PWD/../../_output/ -lformat