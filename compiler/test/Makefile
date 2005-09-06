#
# Makefile: builds stuff related to compiler's unit tests
#

COMPILER_ROOT = ..
COMPILER_SUBDIR = test

# 
# some files in this directory (list.test.cpp) need the
# -Wno-invalid-offsetof flag
#
DIR_USES_OFFSETOF = 1


#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


#
# test-specific variables
#

TEST_LIB = $(PLATFORM)/test_lib$(CHPL_MUNGE)$(EXE_SUFFIX)
TEST_CVS_SRCS = test_lib.cpp
TEST_GEN_SRCS = \
	beautify.test.cpp \
	files.test.cpp \
	list.test.cpp \
	map.test.cpp \
	misc.test.cpp \
	mysystem.test.cpp \
	stringutil.test.cpp \
	vec.test.cpp

TEST_SRCS = \
	$(TEST_CVS_SRCS) \
	$(TEST_GEN_SRCS)
TEST_OBJS = $(TEST_SRCS:%.cpp=$(PLATFORM)/%.$(OBJ_SUFFIX))


#
# standard subdirectory variables
#

CVS_SRCS = $(TEST_CVS_SRCS)
GEN_SRCS = $(TEST_GEN_SRCS)
TARGETS = $(TEST_LIB)
EXECS = $(TARGETS)


include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

#
# target-specific rules
#

$(TEST_LIB): $(TEST_OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)
	-$(TEST_LIB)

#
# we can't seem to rely on VPATH for these because it prevents
# the $(PLATFORM) subdirectory from being built because it
# finds it somewhere else.
#

beautify.test.cpp: ../backend/beautify.cpp
	cp $< $@

list.test.cpp: ../adt/list.cpp
	cp $< $@

map.test.cpp: ../adt/map.cpp
	cp $< $@

vec.test.cpp: ../adt/vec.cpp
	cp $< $@

files.test.cpp: ../util/files.cpp
	cp $< $@

misc.test.cpp: ../util/misc.cpp
	cp $< $@

mysystem.test.cpp: ../util/mysystem.cpp
	cp $< $@

stringutil.test.cpp: ../util/stringutil.cpp
	cp $< $@



#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
