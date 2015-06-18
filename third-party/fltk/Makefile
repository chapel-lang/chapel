#
# Makefile to install fltk as prerequisite to chplvis
#

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

all: install.done

install.done: $(FLTK_INSTALL_DIR)
	touch install.done

$(FLTK_INSTALL_DIR): build.done
	cd $(FLTK_BUILD_DIR) && $(MAKE) install

build.done: $(FLTK_BUILD_DIR)
	cd $(FLTK_BUILD_DIR) && ./configure --prefix=$(FLTK_INSTALL_DIR) &&  $(MAKE)
	touch build.done

$(FLTK_BUILD_DIR):
	@echo Unpacking fltk-$(FLTK_VERSION)-source.tar.gz
	@tar xzf fltk-$(FLTK_VERSION)-source.tar.gz
	@mv fltk-$(FLTK_VERSION) $(FLTK_BUILD_DIR)

clean:
	cd $(FLTK_BUILD_DIR) && $(MAKE) clean
	rm -f build.done install.done

cleanall:
	cd $(FLTK_BUILD_DIR) && $(MAKE) cleanall
	rm -f build.done install.done

clobber:
	-rm -rf $(FLTK_BUILD_DIR) $(FLTK_INSTALL_DIR) *~
	-rm -f build.done install.done

FORCE:
