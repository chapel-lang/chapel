
CHAPEL_ROOT = ..
CHAPEL_SUBDIR = third-party
include $(CHAPEL_ROOT)/make/Makefile.base

all: $(THIRD_GC_DIRNAME) $(THIRD_DPARSE_DIRNAME)

clean:
	cd $(THIRD_GC_DIRNAME) && $(MAKE) clean
	cd $(THIRD_DPARSE_DIRNAME) && $(MAKE) clean

clobber:
	cd $(THIRD_GC_DIRNAME) && $(MAKE) clobber
	cd $(THIRD_DPARSE_DIRNAME) && $(MAKE) clobber

$(THIRD_GC_DIRNAME): FORCE
	cd $(THIRD_GC_DIRNAME) && $(MAKE)

$(THIRD_DPARSE_DIRNAME): FORCE
	cd $(THIRD_DPARSE_DIRNAME) && $(MAKE)

FORCE:
