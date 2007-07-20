RUNTIME_ROOT = ..
RUNTIME_SUBDIR = comm-none

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMM_OBJDIR = $(CHPL_MAKE_PLATFORM)/threads-$(CHPL_THREADS)/comm-$(CHPL_COMM)
include Makefile.share

TARGETS = $(COMM_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
