#
# Makefile: builds stuff related to cast
#

COMPILER_ROOT = ../..
COMPILER_SUBDIR = ./vparser/cast

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


#
# cast-specific variables
#

CAST = cast$(EXE_SUFFIX)
CAST_CVS_SRCS = \
	cast.cpp
CAST_SRCS = $(CAST_CVS_SRCS)
CAST_OBJS = $(CAST_SRCS:%.cpp=$(PLATFORM)/%.o)


#
# standard subdirectory variables
#

CVS_SRCS = $(CAST_CVS_SRCS)
GEN_SRCS = 
EXECS = $(CAST)


#
# rules
#

all: $(CAST)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


#
# target-specific rules
#

$(CAST): $(PLATFORM)/$(CAST)
	cp $< $@

$(PLATFORM)/$(CAST): $(CAST_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)


#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
