RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/comm/ugni

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMM_OBJDIR = $(RUNTIME_OBJDIR)
COMM_LAUNCHER_OBJDIR = $(LAUNCHER_OBJDIR)
include Makefile.share

ifneq ($(MAKE_LAUNCHER),1)
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
