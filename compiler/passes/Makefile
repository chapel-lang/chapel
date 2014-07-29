ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

COMPILER_ROOT = ..
COMPILER_SUBDIR = passes

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

PASSES_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share

TARGETS = reservedSymbolNames.h $(PASSES_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot

reservedSymbolNames.h: reservedSymbolNames
	sed -e 's/^\([ 	]*\)\([A-Za-z_][A-Za-z0-9_]*\)/\1cnames.set_add(astr("\2"));/' <$< >$@
