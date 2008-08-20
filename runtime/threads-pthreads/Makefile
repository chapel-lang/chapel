RUNTIME_ROOT = ..
RUNTIME_SUBDIR = threads-pthreads

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

RUNTIME_INCLS += -I../src/

THREADS_OBJDIR = $(RUNTIME_OBJDIR)
include Makefile.share

TARGETS = $(THREADS_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
