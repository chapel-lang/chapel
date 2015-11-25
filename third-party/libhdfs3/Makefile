ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base
include Makefile.share

LIBHDFS3_FILE := libhdfs3/bootstrap

default: $(LIBHDFS3_FILE)

all: $(LIBHDFS3_FILE)

clean: FORCE
	rm -rf libhdfs3

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install libhdfs3

$(LIBHDFS3_FILE):
	if [ ! -d libhdfs3 ]; then ./fetch-libhdfs3.sh; fi
	mkdir -p libhdfs3/build
	cd libhdfs3/build && ../bootstrap --prefix=$(LIBHDFS3_INSTALL_DIR)
	cd libhdfs3/build && $(MAKE)
	cd libhdfs3/build && $(MAKE) install
	echo "libhdfs installed to $(LIBHDFS3_INSTALL_DIR)"

libhdfs3: $(LIBHDFS3_FILE)

FORCE:

.NOTPARALLEL:
