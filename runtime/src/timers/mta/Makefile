RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/timers/$(CHPL_MAKE_TIMERS)

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
