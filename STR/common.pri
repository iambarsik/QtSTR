

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

//--------------------------------------------------------------------------------------------

#INCLUDEPATH += $$PWD/../formats/F_TEST
#DEPENDPATH += $$PWD/../formats/F_TEST
#win32: LIBS += -L$$PWD/../_output/ -lf_test
