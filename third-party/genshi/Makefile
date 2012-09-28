ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# set up the directories
#
GENSHI_ABS_DIR = $(shell pwd)
GENSHI_INSTALL_DIR = $(GENSHI_ABS_DIR)/$(GENSHI_INSTALL_SUBDIR)

default: all

all: $(GENSHI_INSTALL_DIR)

clean: FORCE

clobber: clean
	rm -rf $(GENSHI_INSTALL_DIR)

genshi: $(GENSHI_INSTALL_DIR)

$(GENSHI_INSTALL_DIR):
	PYTHONPATH=$(GENSHI_INSTALL_DIR) && easy_install --install-dir=$(GENSHI_INSTALL_DIR) Genshi

FORCE:
