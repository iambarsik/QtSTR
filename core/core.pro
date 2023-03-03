QT -= gui

TEMPLATE = lib
DEFINES += CORE_LIBRARY

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

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
