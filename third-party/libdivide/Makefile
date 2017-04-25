include Makefile.include

LIBDIVIDE_FILE := libdivide/libdivide.h

default: $(LIBDIVIDE_FILE)

all: $(LIBDIVIDE_FILE)

clean: FORCE
	rm -rf libdivide

cleanall: FORCE
	rm -rf libdivide

clobber: FORCE
	rm -rf libdivide

$(LIBDIVIDE_FILE):
	if [ ! -d $(LIBDIVIDE_FILE) ]; then ./fetch-libdivide.sh; fi
	echo "libdivide installed to $(LIBDIVIDE_INSTALL_DIR)"
	echo "To complete libdivide support for Chapel set the following"
	echo "environment variables"
	echo "export CHPL_LIBDIVIDE=libdivide"

libdivide: $(LIBDIVIDE_FILE)

FORCE:

.NOTPARALLEL:
