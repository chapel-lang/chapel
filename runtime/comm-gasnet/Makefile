RUNTIME_ROOT = ..
RUNTIME_SUBDIR = comm-gasnet

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMM_OBJDIR = $(RUNTIME_OBJDIR)
COMM_LAUNCHER_OBJDIR = $(LAUNCHER_OBJDIR)
include Makefile.share

ifeq ($(MAKE_LAUNCHER),0)
TARGETS = \
	$(COMM_OBJS) \

else
TARGETS = \
	$(COMM_LAUNCHER_OBJS) \

endif

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
