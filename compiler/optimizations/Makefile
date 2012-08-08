ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

COMPILER_ROOT = ..
COMPILER_SUBDIR = optimizations

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

OPTIMIZATIONS_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share

TARGETS = $(OPTIMIZATIONS_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
