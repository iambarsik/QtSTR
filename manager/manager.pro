QT -= gui
QT += widgets

TEMPLATE = lib
DEFINES += MANAGER_LIBRARY

CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    manager.cpp

HEADERS += \
    modules.h \
    manager.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

BIN_PATH = $$PWD/../_output/
DESTDIR = $${BIN_PATH}/

include($${_PRO_FILE_PWD_}/modules.pri)
