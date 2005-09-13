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
MAKE_PRIMS_CVS_SRCS = \
	make_prims.cpp
MAKE_PRIMS_SRCS = $(MAKE_PRIMS_CVS_SRCS)
MAKE_PRIMS_OBJS = $(MAKE_PRIMS_SRCS:%.cpp=$(PLATFORM)/%.o)


#
# standard subdirectory variables
#

CVS_SRCS = $(MAKE_PRIMS_CVS_SRCS)
EXECS = $(MAKE_PRIMS)


#
# rules
#

TARGETS = $(MAKE_PRIMS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


#
# target-specific rules
#

$(MAKE_PRIMS): $(PLATFORM)/$(MAKE_PRIMS)
	cp -f $< $@

$(PLATFORM)/$(MAKE_PRIMS): $(MAKE_PRIMS_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)


#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
