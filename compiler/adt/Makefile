ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

COMPILER_ROOT = ..
COMPILER_SUBDIR = adt

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

ADT_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share

TARGETS = $(ADT_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
