QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = lib

SOURCES += \
    <GEN_FORMAT_NAME>.cpp

HEADERS += \
    <GEN_FORMAT_NAME>.h

FORMS += \
    <GEN_FORMAT_NAME>.ui


# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

BIN_PATH = $$PWD/../../_output/
DESTDIR = $${BIN_PATH}/

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core
LIBS += -L$$PWD/../../_output/ -lCore

INCLUDEPATH += $$PWD/../../templates/format
DEPENDPATH += $$PWD/../../templates/format
LIBS += -L$$PWD/../../_output/ -lformat
