COMPILER_ROOT = ..
COMPILER_SUBDIR = main

USE_DPARSER = 1

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

MAIN_OBJDIR = $(PLATFORM)
include Makefile.share

TARGETS = $(MAIN_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
