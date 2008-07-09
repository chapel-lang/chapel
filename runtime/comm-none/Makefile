RUNTIME_ROOT = ..
RUNTIME_SUBDIR = comm-none

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMM_OBJDIR = $(RUNTIME_OBJDIR)
COMM_LAUNCHER_OBJDIR = $(LAUNCHER_OBJDIR)
include Makefile.share

TARGETS = \
	$(COMM_OBJS) \
	$(COMM_LAUNCHER_OBJS) \

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
