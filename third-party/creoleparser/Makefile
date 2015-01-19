ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base

default: all

all: $(CREOLEPARSER_INSTALL_DIR)

clean:

cleanall:

clobber: clean
	rm -rf install

creoleparser: $(CREOLEPARSER_INSTALL_DIR)

$(CREOLEPARSER_INSTALL_DIR):
	mkdir $(CREOLEPARSER_INSTALL_DIR)
	export PYTHONPATH=$(CREOLEPARSER_INSTALL_DIR):$$PYTHONPATH && easy_install --install-dir=$(CREOLEPARSER_INSTALL_DIR) Creoleparser

FORCE:

.NOTPARALLEL:
