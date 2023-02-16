QT -= gui

TEMPLATE = lib
DEFINES += CORE_LIBRARY

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    commandsq.cpp \
    core.cpp

HEADERS += \
    commandsq.h \
    core.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

BIN_PATH = $$PWD/../_output/
DESTDIR = $${BIN_PATH}/
