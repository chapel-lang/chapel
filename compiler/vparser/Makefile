COMPILER_ROOT = ..
COMPILER_SUBDIR = vparser

USE_DPARSER = 1

SUBDIRS = \
	cast \


#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

VPARSER_OBJDIR = $(PLATFORM)
include Makefile.share

TARGETS = $(VPARSER_OBJS)

OTHER_DEPEND_RULES = \
	@$(MAKEDEPEND) -MT V_G_INCLUDES $(CXXFLAGS) v.g_incs.cpp | sed s/:\ v.g_incs.cpp/\ =/ | grep -v \^\# >> $(CVS_DEPENDS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

CAST = cast/$(PLATFORM)/cast$(EXE_SUFFIX)

cast_code.cpp: $(CAST)
	./$(CAST)



# Here, prim_data.h needs to be mentioned explicitly to get
# prim_data.h created before we try to create %.g.d_parser.cpp, just
# in case the user hasn't already run a "make depend" (in which case
# GEYSA_INCLUDES will be set and the dependences in the generated file
# will capture this information redundantly).

$(PLATFORM)/v.g.d_parser.o: ../analysis/prim_data.h


#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot


#
# non-obvious dependences -- these must follow
# Makefile.compiler.foot (technically, Makefile.cvs.depend,
# which it includes) due to the use of the
# *_INCLUDES variables...
#

$(PLATFORM)/ast_cf.o: cast_code.cpp
$(PLATFORM)/v.g.d_parser.o: $(V_G_INCLUDES)
