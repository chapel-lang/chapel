COMPILER_ROOT = ..
COMPILER_SUBDIR = util

USE_DPARSER = 1

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

UTIL_OBJDIR = $(PLATFORM)
include Makefile.share

all: $(UTIL_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
