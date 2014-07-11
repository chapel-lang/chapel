RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/launch/pbs-aprun

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

LAUNCH_OBJDIR = $(LAUNCHER_OBJDIR)
include Makefile.share

aprun_utils.c: FORCE
	cp ../aprun/aprun-utils.c .

TARGETS = \
	aprun_utils.c \
	$(LAUNCHER_OBJS) \

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
