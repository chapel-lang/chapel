COMPILER_ROOT = ..
COMPILER_SUBDIR = passes

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

PASSES_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share

TARGETS = $(PASSES_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
