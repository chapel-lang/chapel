#
# Makefile: builds Chapel compiler
#

CHAPEL_ROOT=..

DEBUG=1
#OPTIMIZE=1
#PROFILE=1
USE_GC=1
USE_DPARSER=1
#LEAK_DETECT=1

include $(CHAPEL_ROOT)/make/Makefile.base

#
# source languages accepted by the tool and settings based on them
#
LANG_V = 1
LANG_CHPL = 1

ifdef LANG_V
LANGS += v
CFLAGS += -DLANG_V=1
endif
ifdef LANG_CHPL
LANGS += chpl
CFLAGS += -DLANG_CHPL=1
endif



.PHONY: default all clean cleandepend clobber depend version test tar bintar install



LIBS += -lm 
CPPFLAGS = $(CFLAGS)

LIB_SRCS = \
	vec.cpp \
	list.cpp \
	map.cpp \
	misc.cpp

GEYSA_SRCS = \
	arg.cpp \
	ast.cpp \
	ast_cf.cpp \
	builtin.cpp \
	cfg.cpp \
	cg.cpp \
	clone.cpp \
	driver.cpp \
	fa.cpp \
	fun.cpp \
	grammar.cpp \
	if1.cpp \
	log.cpp \
	parse.cpp \
	pdb.cpp \
	pnode.cpp \
	prim.cpp \
	prim_data.cpp \
	region.cpp \
	scope.cpp \
	ssu.cpp \
	var.cpp \
	version.cpp \
	$(LIB_SRCS)

CHPL_SRCS = $(LANGS:%=%.g.d_parser.cpp) $(GEYSA_SRCS)
CHPL_OBJS = $(CHPL_SRCS:%.cpp=$(PLATFORM)/%.o)
CHPL_DEPS = $(GEYSA_SRCS:%.cpp=$(PLATFORM)/%.dep)

MAKE_PRIMS_SRCS = \
	make_prims.cpp \
	make_prims.g.d_parser.cpp
MAKE_PRIMS_OBJS = $(MAKE_PRIMS_SRCS:%.cpp=$(PLATFORM)/%.o)
MAKE_PRIMS_DEPS = $(MAKE_PRIMS_SRCS:%.cpp=$(PLATFORM)/%.dep)
CAST_SRCS = cast.cpp
CAST_OBJS = $(CAST_SRCS:%.cpp=$(PLATFORM)/%.o)
CAST_DEPS = $(CAST_SRCS:%.cpp=$(PLATFORM)/%.dep)

ALL_DEPS = $(CHPL_DEPS) $(MAKE_PRIMS_DEPS) $(CAST_DEPS)

MAKE_PRIMS = make_prims$(EXE_SUFFIX)
CAST = cast$(EXE_SUFFIX)
MAKE_DPARSER = $(D_PARSER_BIN_DIR)/make_dparser$(EXE_SUFFIX)

CHPL = chpl$(EXE_SUFFIX)
TEST_LIB = test_lib$(EXE_SUFFIX)
EXECS = $(CHPL) $(TEST_LIB)


#
# rules
#

all: $(EXECS)

clean:
	rm -f ./$(PLATFORM)/*.o core *.core *.gmon *.a $(EXECS) $(MAKE_PRIMS) *.g.d_parser.cpp prim_data.h prim_data.cpp

cleandepend:
	rm -f ./$(PLATFORM)/*.dep

clobber: clean
	rm -rf ./$(PLATFORM)

depend: $(ALL_DEPS)

test: test_lib
	test_lib
# do_tests

version:
	@echo $(OS_TYPE) $(OS_VERSION)

$(TEST_LIB): test_lib.cpp $(LIB_SRCS)
	$(CC) $(CFLAGS) -DTEST_LIB=1 $(LDFLAGS) -o $@ $^ $(LIBS)

$(CHPL): $(CHPL_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

$(MAKE_PRIMS): $(MAKE_PRIMS_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

$(CAST): $(CAST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

prim_data.h: $(MAKE_PRIMS) prim_data.dat
	$(MAKE_PRIMS) prim_data.dat

prim_data.cpp: prim_data.h

%.g.d_parser.cpp: %.g
	$(MAKE_DPARSER) -v -Xcpp -I $<

.PRECIOUS: %.g.d_parser.cpp

#
# tar-related stuff
#

GEYSA_FILES = \
	geysa/*.v \
	geysa/*.g \
	geysa/*.dat \
	geysa/*.cpp \
	geysa/*.c \
	geysa/*.h \
	geysa/BUILD_VERSION

AUX_FILES = \
	geysa/Makefile \
	geysa/COPYRIGHT \
	geysa/README \
	geysa/CHANGES

EXTERNAL_FILES = \
	geysa/d-1.8-src.tar.gz \
	geysa/gc6.2.tar.gz

TAR_FILES = $(GEYSA_FILES) $(AUX_FILES) $(EXTERNAL_FILES)

tar:
	(cd ..;tar czf geysa-$(RELEASE)-src.tar.gz $(TAR_FILES)) 

bintar:
	(cd ..;tar czf geysa-$(RELEASE)-$(OS_TYPE)-bin.tar.gz $(AUX_FILES) $(LIBRARIES:%=v/%) $(INCLUDES:%=v/%) $(EXECS:%=v/%))


#
# install-related stuff
#

INSTALL_ROOT=/usr/local
LIBRARIES =
INCLUDES = 
MANPAGES =

install:
	cp $(EXECS) $(INSTALL_ROOT)/bin
#	cp $(INCLUDES) $(INSTALL_ROOT)/include
#	cp $(MANPAGES) $(INSTALL_ROOT)/man/man1
#	cp $(LIBRARIES) $(INSTALL_ROOT)/lib


.SUFFIXES: .cpp .dep

PLATFORM_DIR = $(PLATFORM)/.timestamp

$(PLATFORM_DIR):
	-mkdir $(PLATFORM) > /dev/null 2>&1
	-touch $(PLATFORM_DIR)

$(PLATFORM)/%.o: %.cpp $(PLATFORM_DIR)
	$(CC) -c $(CFLAGS) -o $@ $<

$(PLATFORM)/%.dep: %.cpp $(PLATFORM_DIR)
	$(CCDEP) -M -MG $(CFLAGS) -MT $(PLATFORM)/$*.o -MT $(PLATFORM)/$*.dep -MF $@ $<

# |& grep -v "invalid access to non-static data member" | grep -v offsetof

$(PLATFORM)/v.g.d_parser.o $(PLATFORM)/v.g.d_parser.dep: prim_data.h

-include $(PLATFORM)/*.dep

