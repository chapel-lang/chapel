COMPILER_ROOT = ..
COMPILER_SUBDIR = backend
COMP_CFLAGS += -I$(COMPILER_ROOT)/ifa

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

BACKEND_OBJDIR = $(CHPL_MAKE_PLATFORM)
include Makefile.share

TARGETS = $(BACKEND_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
