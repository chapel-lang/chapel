#
# Makefile: builds stuff related to compiler's unit tests
#

COMPILER_ROOT = ..
COMPILER_SUBDIR = test
USE_DPARSER = 1
CFLAGS += -DTEST_LIB

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


#
# cast-specific variables
#

TEST_LIB = test_lib$(EXE_SUFFIX)
COMPILER_SRCS = \
	list.cpp \
	map.cpp \
	misc.cpp \
	vec.cpp \
	files.cpp \
	beautify.cpp \
	mysystem.cpp \
	stringutil.cpp
TEST_CVS_SRCS = \
	$(COMPILER_SRCS) \
	test_lib.cpp
TEST_SRCS = $(TEST_CVS_SRCS)
TEST_OBJS = $(TEST_SRCS:%.cpp=$(PLATFORM)/%.o)


#
# standard subdirectory variables
#

CVS_SRCS = $(TEST_CVS_SRCS)
GEN_SRCS = 
EXECS = $(TEST_LIB)
CLEAN_TARGS = $(COMPILER_SRCS)
DEPEND_DEPS = $(COMPILER_SRCS)


#
# rules
#

all:

test: $(TEST_LIB)
	./$(TEST_LIB)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

#
# target-specific rules
#

$(TEST_LIB): $(PLATFORM)/$(TEST_LIB)
	cp $< $@

$(PLATFORM)/$(TEST_LIB): $(TEST_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

%.cpp: ../%.cpp
	cp $< $@

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
