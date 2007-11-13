COMPILER_ROOT = ..
COMPILER_SUBDIR = util
COMP_CFLAGS += -I$(COMPILER_ROOT)/ifa

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

UTIL_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share

TARGETS = $(UTIL_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
