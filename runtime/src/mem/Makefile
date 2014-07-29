RUNTIME_ROOT = ../..
RUNTIME_SUBDIR = src/mem

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

TARGETS =

SUBDIRS = $(CHPL_MAKE_MEM)

include $(RUNTIME_ROOT)/make/Makefile.runtime.emptydirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
