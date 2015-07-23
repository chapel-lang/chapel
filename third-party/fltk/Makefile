#
# Makefile to install fltk as prerequisite to chplvis
#

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

ifndef CHPL_HOST_PLATFORM
first:
	@echo third-party/fltk error:  CHPL_HOST_PLATFORM not set
endif

all: install.$(CHPL_HOST_PLATFORM)

install.$(CHPL_HOST_PLATFORM): $(FLTK_INSTALL_DIR)
	touch install.$(CHPL_HOST_PLATFORM)

$(FLTK_INSTALL_DIR): build.$(CHPL_HOST_PLATFORM)
	cd $(FLTK_BUILD_DIR) && $(MAKE) install

build.$(CHPL_HOST_PLATFORM): $(FLTK_BUILD_DIR)
	cd $(FLTK_BUILD_DIR) && ./configure --prefix=$(FLTK_INSTALL_DIR) &&  $(MAKE)
	touch build.$(CHPL_HOST_PLATFORM)

$(FLTK_BUILD_DIR):
	@echo Unpacking fltk-$(FLTK_VERSION)-source.tar.gz
	@tar xzf fltk-$(FLTK_VERSION)-source.tar.gz
	@mv fltk-$(FLTK_VERSION) $(FLTK_BUILD_DIR)

clean:
	-cd $(FLTK_BUILD_DIR) && $(MAKE) clean
	rm -f build.$(CHPL_HOST_PLATFORM) install.$(CHPL_HOST_PLATFORM)

cleanall:
	-cd $(FLTK_BUILD_DIR) && $(MAKE) cleanall
	rm -f build.$(CHPL_HOST_PLATFORM) install.$(CHPL_HOST_PLATFORM)

clobber:
	rm -rf $(FLTK_BUILD_DIR) $(FLTK_INSTALL_DIR)
	rm -f build.$(CHPL_HOST_PLATFORM) install.$(CHPL_HOST_PLATFORM)

FORCE:
