TEMPLATE = subdirs

SUBDIRS = core \
		  database \
		  network_client \
		  network_server \
		  templates/format \
		  templates/model \
		  models/M_TEST
		  
qtHaveModule(widgets) {
    SUBDIRS += formats/F_TEST