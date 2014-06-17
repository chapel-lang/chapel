RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/timers/$(CHPL_MAKE_TIMERS)

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

TIMERS_OBJDIR = $(RUNTIME_OBJDIR)
include Makefile.share

TARGETS = $(TIMERS_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
