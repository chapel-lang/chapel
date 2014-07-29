ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

COMPILER_ROOT = ..
COMPILER_SUBDIR = AST

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

AST_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share

TARGETS = $(AST_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
