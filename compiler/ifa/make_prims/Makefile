#
# Makefile: builds stuff related to make_prims
#

COMPILER_ROOT = ../..
COMPILER_SUBDIR = ifa/make_prims

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


#
# make_prims-specific variables
#

MAKE_PRIMS = make_prims$(EXE_SUFFIX)
MAKE_PRIMS_SVN_SRCS = \
	make_prims.cpp
MAKE_PRIMS_SRCS = $(MAKE_PRIMS_SVN_SRCS)
MAKE_PRIMS_OBJS = $(MAKE_PRIMS_SRCS:%.cpp=$(OBJ_SUBDIR)/%.o)


#
# standard subdirectory variables
#

SVN_SRCS = $(MAKE_PRIMS_SVN_SRCS)
EXECS = $(MAKE_PRIMS)


#
# rules
#

TARGETS = $(MAKE_PRIMS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


#
# target-specific rules
#

$(MAKE_PRIMS): $(CHPL_MAKE_PLATFORM)/$(MAKE_PRIMS)
	cp -f $< $@

$(CHPL_MAKE_PLATFORM)/$(MAKE_PRIMS): $(MAKE_PRIMS_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)


#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
