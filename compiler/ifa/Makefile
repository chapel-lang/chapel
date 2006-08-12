COMPILER_ROOT = ..
COMPILER_SUBDIR = ifa

CFLAGS += -DIN_CHAPEL

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


IFA_OBJDIR = $(PLATFORM)
include Makefile.share


TARGETS = $(IFA_OBJS)


include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


MAKE_PRIMS = ./make_prims/$(PLATFORM)/make_prims$(EXE_SUFFIX)

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
