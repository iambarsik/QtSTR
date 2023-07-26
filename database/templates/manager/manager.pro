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

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
LIBS += -L$$PWD/../_output/ -lCore

INCLUDEPATH += $$PWD/../templates/model
DEPENDPATH += $$PWD/../templates/model
LIBS += -L$$PWD/../_output/ -lmodel

INCLUDEPATH += $$PWD/../templates/format
DEPENDPATH += $$PWD/../templates/format
LIBS += -L$$PWD/../_output/ -lformat

include($${_PRO_FILE_PWD_}/modules.pri)
