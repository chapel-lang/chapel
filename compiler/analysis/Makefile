COMPILER_ROOT = ..
COMPILER_SUBDIR = analysis

SUBDIRS = \
	make_prims \

# 
# some files in this directory (fa.cpp, ssu.cpp) need the
# -Wno-invalid-offsetof flag
#
DIR_USES_OFFSETOF = 1

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


ANALYSIS_OBJDIR = $(PLATFORM)
include Makefile.share


TARGETS = $(ANALYSIS_OBJS)

OTHER_DEPEND_RULES = \
	@$(MAKEDEPEND) -MT PRIM_DATA_INCLUDES $(CXXFLAGS) prim_data_incs.cpp | sed s/:\ prim_data_incs.cpp/\ =/ | grep -v \^\# >> $(CVS_DEPENDS) \


include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


MAKE_PRIMS = ./make_prims/$(PLATFORM)/make_prims$(EXE_SUFFIX)

prim_data.h prim_data.cpp: $(MAKE_PRIMS) prim_data.dat
	$(MAKE_PRIMS) prim_data.dat

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

$(PLATFORM)/prim_data.o: $(PRIM_DATA_INCLUDES)
