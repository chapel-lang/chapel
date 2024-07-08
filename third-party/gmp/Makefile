ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# Cray builds are cross-compilations
#
GMP_CROSS_COMPILED=no
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_GMP_CFG_OPTIONS += --host=$(CHPL_MAKE_TARGET_ARCH)-cle-linux-gnu
GMP_CROSS_COMPILED=yes
else ifneq (, $(filter cray-cs hpe-cray-ex,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_GMP_CFG_OPTIONS += --host=$(CHPL_MAKE_TARGET_ARCH)-unknown-linux-gnu
GMP_CROSS_COMPILED=yes
endif

# Disable use of alloca for pgi, it seems to cause stack overflows.
ifneq (, $(findstring pgi,$(CHPL_MAKE_TARGET_COMPILER)))
CHPL_GMP_CFG_OPTIONS += --disable-alloca
endif

#
# On cross-compiled systems, building the shared libraries causes issues.
# On Macs, not building the shared libraries causes warnings.
#
ifneq (, $(filter cray-x% hpe-cray-ex,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_GMP_CFG_OPTIONS += --enable-static --disable-shared
endif

#
# On 32bit platforms (i.e. when CHPL_TARGET_PLATFORM ends with 32), set
# ABI=32. Because we are setting CFLAGS in the ./configure step, it will not
# auto-detect ABI=32.
#
CHPL_GMP_ABI_ARG =
ifneq (, $(filter %32,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_GMP_ABI_ARG = ABI=32
endif

ifeq ($(CHPL_LIB_PIC),pic)
CFLAGS += $(SHARED_LIB_CFLAGS)
CXXFLAGS += $(SHARED_LIB_CFLAGS)
endif

CHPL_GMP_CFG_OPTIONS += $(CHPL_GMP_MORE_CFG_OPTIONS)

default: all

all: gmp

clean: FORCE
	rm -rf $(GMP_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install

$(GMP_BUILD_SUBDIR):
	mkdir -p $@


$(GMP_H_FILE): $(GMP_BUILD_SUBDIR)
	touch $(GMP_SUBDIR)/doc/gmp.info*
	touch $(GMP_SUBDIR)/demos/calc/calc*.c
	cd $(GMP_BUILD_DIR) && $(GMP_SUBDIR)/configure CC_FOR_BUILD='$(CHPL_MAKE_HOST_CC)' CC='$(CC)' CFLAGS='$(CFLAGS) $(CHPL_GMP_CFLAGS)' CXX='$(CXX)' CXXFLAGS='$(CXXFLAGS) $(CHPL_GMP_CXXFLAGS)' $(CHPL_GMP_ABI_ARG) --prefix=$(GMP_INSTALL_DIR) $(CHPL_GMP_CFG_OPTIONS)
	cd $(GMP_BUILD_DIR) && $(MAKE)
ifeq ($(GMP_CROSS_COMPILED),no)
	cd $(GMP_BUILD_DIR) && $(MAKE) check
endif
	cd $(GMP_BUILD_DIR) && $(MAKE) install

gmp: $(GMP_H_FILE)

FORCE:

.NOTPARALLEL:
