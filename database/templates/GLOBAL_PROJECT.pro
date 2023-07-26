TEMPLATE = subdirs

HEADERS =   include/common.h \
            include/network_def.h

SUBDIRS = core \
          manager \
		#<MODELS>
		  
qtHaveModule(widgets) {
		#<FORMATS>
