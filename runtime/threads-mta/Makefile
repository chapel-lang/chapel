RUNTIME_ROOT = ..
RUNTIME_SUBDIR = threads-mta

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

THREADS_OBJDIR = $(OBJ_SUBDIR)/threads-$(CHPL_MAKE_THREADS)/comm-$(CHPL_MAKE_COMM)
include Makefile.share

TARGETS = $(THREADS_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
