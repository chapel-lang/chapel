RUNTIME_ROOT = ..
RUNTIME_SUBDIR = threads-nanos-wd

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

RUNTIME_INCLS += -I../src/

GENERIC_THREADS_OBJDIR = $(RUNTIME_OBJDIR)
include Makefile.share

TARGETS = $(GENERIC_THREADS_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
