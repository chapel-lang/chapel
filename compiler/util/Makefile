ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

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

#
# Compile this with -g so that this symbol is accessible from
# within gdb whether or not we're compiling the whole thing in
# debug mode.
#
$(OBJ_SUBDIR)/tmpdirname.o: tmpdirname.cpp $(OBJ_SUBDIR_MADE)
	$(CXX) -g -c $(COMP_CXXFLAGS) -o $@ $<
