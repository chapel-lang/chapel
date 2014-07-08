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
SUBDIRS += auxFilesys/$(CHPL_MAKE_AUXFS)
TARGETS = $(QIO_OBJS)


include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules


FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
