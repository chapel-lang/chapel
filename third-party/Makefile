
CHAPEL_ROOT = ..
include $(CHAPEL_ROOT)/make/Makefile.base

all: $(THIRD_GC_DIRNAME) $(THIRD_DPARSE_DIRNAME)

clean:
	cd $(THIRD_GC_DIRNAME) && make clean
	cd $(THIRD_DPARSE_DIRNAME) && make clean

$(THIRD_GC_DIRNAME): FORCE
	cd $(THIRD_GC_DIRNAME) && make

$(THIRD_DPARSE_DIRNAME): FORCE
	cd $(THIRD_DPARSE_DIRNAME) && make

FORCE:
