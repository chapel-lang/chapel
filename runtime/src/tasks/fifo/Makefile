RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/tasks/$(CHPL_MAKE_TASKS)

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

RUNTIME_INCLS += -I$(RUNTIME_ROOT)/src/

TASKS_OBJDIR = $(RUNTIME_OBJDIR)
include Makefile.share

TARGETS = $(TASKS_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
