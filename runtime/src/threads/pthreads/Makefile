RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/threads/$(CHPL_MAKE_THREADS)

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

THREADS_OBJDIR = $(RUNTIME_OBJDIR)
include Makefile.share

TARGETS = $(THREADS_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
