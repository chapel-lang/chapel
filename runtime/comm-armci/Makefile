RUNTIME_ROOT = ..
RUNTIME_SUBDIR = comm-armci

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMM_OBJDIR = $(OBJ_SUBDIR)/threads-$(CHPL_MAKE_THREADS)/comm-$(CHPL_MAKE_COMM)
include Makefile.share

TARGETS = $(COMM_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
