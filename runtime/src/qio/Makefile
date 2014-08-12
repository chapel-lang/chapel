RUNTIME_ROOT = ../..
RUNTIME_SUBDIR = src/qio

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

QIO_OBJDIR = $(RUNTIME_OBJDIR)
include Makefile.share

SUBDIRS = regexp/$(CHPL_MAKE_REGEXP)
SUBDIRS += auxFilesys/hdfs
TARGETS = $(QIO_OBJS)

ifneq (,$(findstring lustre,$(CHPL_MAKE_AUXFS)))
	RUNTIME_INCLS += -DSYS_HAS_LLAPI $(CHPL_AUXIO_INCLUDE) $(CHPL_AUXIO_LIBS)
endif

ifneq (,$(findstring clang,$(CHPL_MAKE_TARGET_COMPILER)))
	RUNTIME_INCLS += -Qunused-arguments
endif


include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules


FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
