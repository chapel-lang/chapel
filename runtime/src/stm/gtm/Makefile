RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/stm/$(CHPL_MAKE_STM)

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

STM_OBJDIR = $(RUNTIME_OBJDIR)

#
# include source subdirectories here
#
include comm/$(CHPL_MAKE_COMM)/Makefile.include

include Makefile.share

TARGETS = \
	$(STM_OBJS) \

SUBDIRS = \
	comm \

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
