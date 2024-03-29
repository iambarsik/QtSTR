TEMPLATE = subdirs

HEADERS =   include/common.h \
            include/network_def.h

SUBDIRS = core \
          manager \
          network_client \
          network_server \
          templates/format \
          templates/model \
          models/M_TEST
		  
qtHaveModule(widgets) {
    SUBDIRS += formats/F_TEST
    SUBDIRS += formats/F_TESTFORM
