QT -= gui

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

TEMPLATE = lib

SOURCES += \
    <GEN_MODEL_NAME>.cpp

HEADERS += \
    <GEN_MODEL_NAME>.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

BIN_PATH = $$PWD/../../_output/
DESTDIR = $${BIN_PATH}/

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core
win32: LIBS += -L$$PWD/../../_output/ -lcore

INCLUDEPATH += $$PWD/../../templates/model
DEPENDPATH += $$PWD/../../templates/model
win32: LIBS += -L$$PWD/../../_output/ -lmodel
