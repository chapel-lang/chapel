ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# set up the directories
#
CREOLEPARSER_ABS_DIR = $(shell pwd)
CREOLEPARSER_INSTALL_DIR = $(CREOLEPARSER_ABS_DIR)/$(CREOLEPARSER_INSTALL_SUBDIR)
default: all

all: $(CREOLEPARSER_INSTALL_DIR)

clean:

clobber: clean
	rm -rf $(CREOLEPARSER_INSTALL_DIR)

creoleparser: $(CREOLEPARSER_INSTALL_DIR)

$(CREOLEPARSER_INSTALL_DIR):
	mkdir $(CREOLEPARSER_INSTALL_DIR)
	export PYTHONPATH=$(CREOLEPARSER_INSTALL_DIR):$$PYTHONPATH && easy_install --install-dir=$(CREOLEPARSER_INSTALL_DIR) Creoleparser

FORCE:
