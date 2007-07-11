RUNTIME_ROOT = ..
RUNTIME_SUBDIR = threads-pthreads

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

THREADS_OBJDIR = $(CHPL_MAKE_PLATFORM)
include Makefile.share

TARGETS = $(THREADS_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
