RUNTIME_ROOT = ..
RUNTIME_SUBDIR = src

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMMON_OBJDIR = $(CHPL_MAKE_PLATFORM)/threads-$(CHPL_THREADS)
include Makefile.share

TARGETS = $(COMMON_OBJS) $(MAIN_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
