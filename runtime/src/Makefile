RUNTIME_ROOT = ..
RUNTIME_SUBDIR = src

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMMON_OBJDIR = $(OBJ_SUBDIR)/threads-$(CHPL_MAKE_THREADS)/comm-$(CHPL_MAKE_COMM)
include Makefile.share

TARGETS = $(COMMON_OBJS) $(MAIN_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
