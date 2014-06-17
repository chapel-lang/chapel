ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

COMPILER_ROOT = ..
COMPILER_SUBDIR = backend
COMP_CFLAGS += -I$(COMPILER_ROOT)/ifa

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

BACKEND_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share

TARGETS = $(BACKEND_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
