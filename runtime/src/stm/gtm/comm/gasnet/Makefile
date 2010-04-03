RUNTIME_ROOT = ../../../../..
RUNTIME_SUBDIR = src/stm/$(CHPL_MAKE_STM)/comm/$(CHPL_MAKE_COMM)

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

STM_COMM_OBJDIR = $(RUNTIME_OBJDIR)

include Makefile.share

TARGETS = \
	$(STM_COMM_OBJS) \

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
