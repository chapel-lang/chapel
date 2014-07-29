ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

COMPILER_ROOT = ..
COMPILER_SUBDIR = ifa

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


IFA_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share


TARGETS = $(IFA_OBJS)


include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


MAKE_PRIMS = ./make_prims/$(CHPL_MAKE_PLATFORM)/make_prims$(EXE_SUFFIX)

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
