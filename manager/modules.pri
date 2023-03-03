

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
win32: LIBS += -L$$PWD/../_output/ -lCore

INCLUDEPATH += $$PWD/../templates/model
DEPENDPATH += $$PWD/../templates/model
win32: LIBS += -L$$PWD/../_output/ -lmodel

INCLUDEPATH += $$PWD/../templates/format
DEPENDPATH += $$PWD/../templates/format
win32: LIBS += -L$$PWD/../_output/ -lformat

//--------------------------------------------------------------------------------------------
#<FORMAT_LIBRARY>
INCLUDEPATH += $$PWD/../formats/F_TESTFORM
DEPENDPATH += $$PWD/../formats/F_TESTFORM
win32: LIBS += -L$$PWD/../_output/ -lF_TESTFORM

INCLUDEPATH += $$PWD/../formats/F_TEST
DEPENDPATH += $$PWD/../formats/F_TEST
win32: LIBS += -L$$PWD/../_output/ -lF_TEST

//--------------------------------------------------------------------------------------------
#<MODEL_LIBRARY>
INCLUDEPATH += $$PWD/../models/M_TEST
DEPENDPATH += $$PWD/../models/M_TEST
win32: LIBS += -L$$PWD/../_output/ -lm_test

