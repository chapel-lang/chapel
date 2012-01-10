CHAPEL_ROOT = ..
CHAPEL_SUBDIR = modules
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base

SYS_MODULES_DIR=standard/gen/$(CHPL_MAKE_PLATFORM)-$(CHPL_MAKE_COMPILER)

SYS_CTYPES_MODULE=$(SYS_MODULES_DIR)/SysCTypes.chpl

default: all

all: $(SYS_CTYPES_MODULE)

clean: FORCE
	rm -rf $(SYS_MODULES_DIR)

clobber: FORCE
	rm -rf standard/gen

depend:

MAKE_SYS_BASIC_TYPES=$(CHAPEL_ROOT)/util/config/makeSysBasicTypes

$(SYS_CTYPES_MODULE): $(MAKE_SYS_BASIC_TYPES)
	mkdir -p $(@D)
	cd $(@D) && (../../../$(MAKE_SYS_BASIC_TYPES) > $(@F) || rm -f $(@F))

FORCE:

