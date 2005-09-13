COMPILER_ROOT = ..
COMPILER_SUBDIR = analysis
CFLAGS += -I$(COMPILER_ROOT)/ifa

# 
# some files in this directory (fa.cpp, ssu.cpp) need the
# -Wno-invalid-offsetof flag
#
DIR_USES_OFFSETOF = 1

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


ANALYSIS_OBJDIR = $(PLATFORM)
include Makefile.share

TARGETS = $(ANALYSIS_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
