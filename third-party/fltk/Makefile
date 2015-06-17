#
# Makefile to install fltk as prerequisite to chplvis
#

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

all: install

install: build
	cd $(FLTK_BUILD_DIR) && $(MAKE) install

build: extract
	cd $(FLTK_BUILD_DIR) && ./configure --prefix=$(FLTK_INSTALL_DIR) &&  $(MAKE)

extract:
	@echo Unpacking fltk-$(FLTK_VERSION)-source.tar.gz
	@tar xzf fltk-$(FLTK_VERSION)-source.tar.gz
	@mv fltk-$(FLTK_VERSION) $(FLTK_BUILD_DIR)

clean:
	cd $(FLTK_BUILD_DIR) && $(MAKE) clean

cleanall:
	cd $(FLTK_BUILD_DIR) && $(MAKE) cleanall

clobber:
	-rm -rf $(FLTK_BUILD_DIR) $(FLTK_INSTALL_DIR) *~

FORCE:
