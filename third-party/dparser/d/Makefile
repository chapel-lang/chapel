# Makefile for D_Parser

D_DEBUG=1
#D_OPTIMIZE=1
#D_PROFILE=1
D_USE_GC=1
#D_LEAK_DETECT=1

MAJOR=1
MINOR=12
RELEASE=$(MAJOR).$(MINOR)

CC = gcc

ifndef PREFIX
PREFIX=/usr/local
endif

.PHONY: all gram test install myexample

OS_TYPE = $(shell uname -s | \
  awk '{ split($$1,a,"_"); printf("%s", a[1]);  }')
OS_VERSION = $(shell uname -r | \
  awk '{ split($$1,a,"."); sub("V","",a[1]); \
  printf("%d%d%d",a[1],a[2],a[3]); }')
ARCH = $(shell uname -m)
ifeq ($(ARCH),i386)
  ARCH = x86
endif
ifeq ($(ARCH),i486)
  ARCH = x86
endif
ifeq ($(ARCH),i586)
  ARCH = x86
endif
ifeq ($(ARCH),i686)
  ARCH = x86
endif

ifeq ($(OS_TYPE),Darwin)
  AR_FLAGS = crvs
else
  AR_FLAGS = crv
endif

GC_CFLAGS += -I$(GCROOT)/include -L$(GCROOT)/lib

ifdef D_USE_GC
CFLAGS += -DUSE_GC ${GC_CFLAGS}
LIBS += $(GCROOT)/lib/libgc.a
endif
ifdef D_LEAK_DETECT
CFLAGS += -DLEAK_DETECT  ${GC_CFLAGS}
LIBS += -lleak
endif

D_BUILD_VERSION = $(shell touch BUILD_VERSION;cat BUILD_VERSION)
CFLAGS += -DD_MAJOR_VERSION=$(MAJOR) -DD_MINOR_VERSION=$(MINOR) -DD_BUILD_VERSION=$(D_BUILD_VERSION)

CFLAGS += -Wall
# debug flags
ifdef D_DEBUG
CFLAGS += -g -DD_DEBUG=1
endif
# optimized flags
ifdef D_OPTIMIZE
CFLAGS += -O3
ifeq ($(ARCH),x86)
#CFLAGS += -march=pentiumpro 
ifndef D_PROFILE
CFLAGS += -fomit-frame-pointer 
endif
endif
endif
ifdef D_PROFILE
CFLAGS += -pg
endif

LIBS += -lm 

AUX_FILES = d/Makefile d/COPYRIGHT d/README d/CHANGES d/UPGRADING d/my.g d/my.c d/index.html d/manual.html d/faq.html d/make_dparser.1 d/make_dparser.cat
TESTS = $(shell ls tests/*g tests/*[0-9] tests/*.check tests/*.flags)
TEST_FILES = d/parser_tests d/baseline $(TESTS:%=d/%)
PYTHON_FILES = d/python/Makefile d/python/*.py d/python/*.c d/python/*.h d/python/*.i d/python/README d/python/*.html d/python/contrib/d* d/python/tests/*.py
VERILOG_FILES = d/verilog/Makefile d/verilog/verilog.g d/verilog/README d/verilog/ambig.c \
d/verilog/main.c d/verilog/vparse.c d/verilog/vparse.h d/verilog/verilog_tests
TAR_FILES = $(AUX_FILES) $(TEST_FILES) $(PYTHON_FILES) $(VERILOG_FILES) d/BUILD_VERSION \
d/grammar.g d/sample.g d/my.g 

LIB_SRCS = arg.c parse.c scan.c symtab.c util.c read_binary.c
LIB_OBJS = $(LIB_SRCS:%.c=%.o)

MK_LIB_SRCS = mkdparse.c write_tables.c grammar.g.c gram.c lex.c lr.c
MK_LIB_OBJS = $(MK_LIB_SRCS:%.c=%.o)

ifdef D_USE_GC
LIBMKDPARSE = libmkdparse_gc.a
LIBDPARSE = libdparse_gc.a
else
LIBMKDPARSE = libmkdparse.a
LIBDPARSE = libdparse.a
endif

MAKE_PARSER_SRCS = make_dparser.c
MAKE_PARSER_OBJS = $(MAKE_PARSER_SRCS:%.c=%.o)

SAMPLE_GRAMMAR = sample.g
BASE_SAMPLE_PARSER_SRCS = sample_parser.c
SAMPLE_PARSER_SRCS = sample_parser.c $(SAMPLE_GRAMMAR).d_parser.c
SAMPLE_PARSER_OBJS = $(SAMPLE_PARSER_SRCS:%.c=%.o)

TEST_PARSER_SRCS = test_parser.c
TEST_PARSER_OBJS = $(TEST_PARSER_SRCS:%.c=%.o)

MAKE_DPARSER = ./make_dparser

EXECUTABLES = make_dparser
LIBRARIES = $(LIBMKDPARSE) $(LIBDPARSE)
INSTALL_LIBRARIES = $(LIBDPARSE)
INCLUDES = dparse.h dparse_tables.h dsymtab.h
MANPAGES = make_dparser.1

EXECS = $(EXECUTABLES) sample_parser test_parser
ifeq ($(OS_TYPE),CYGWIN)
EXEC_FILES = $(EXECS:%=%.exe)
EXECUTABLE_FILES = $(EXECUTABLES:%=%.exe)
else
EXEC_FILES = $(EXECS)
EXECUTABLE_FILES = $(EXECUTABLES)
endif

ALL_SRCS = $(MAKE_PARSER_SRCS) $(BASE_SAMPLE_PARSER_SRCS) $(LIB_SRCS) $(MK_LIB_SRCS)

all: $(EXECS) $(LIBRARIES) BUILD_VERSION make_dparser.cat

version:
	echo $(OS_TYPE) $(OS_VERSION)

test:
	(MAKE=$(MAKE) ./parser_tests)

install:
	cp $(EXECUTABLES) $(PREFIX)/bin
	cp $(INCLUDES) $(PREFIX)/include
	cp $(INSTALL_LIBRARIES) $(PREFIX)/lib
	cp $(MANPAGES) $(PREFIX)/man/man1

deinstall:
	rm $(EXECUTABLES:%=$(PREFIX)/bin/%)
	rm $(INCLUDES:%=$(PREFIX)/include/%)
	rm $(INSTALL_LIBRARIES:%=$(PREFIX)/lib/%)
	rm $(MANPAGES:%=$(PREFIX)/man/man1/%)

make_dparser: $(MAKE_PARSER_OBJS) $(LIBRARIES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ version.c $(LIBS) 

$(LIBDPARSE): $(LIB_OBJS)
	ar $(AR_FLAGS) $@ $^

$(LIBMKDPARSE): $(MK_LIB_OBJS)
	ar $(AR_FLAGS) $@ $^

%.d_parser.c: %
	$(MAKE_DPARSER) $<

sample_parser: $(SAMPLE_PARSER_OBJS) $(INSTALL_LIBRARIES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ version.c $(LIBS)

test_parser: $(TEST_PARSER_OBJS) $(LIBRARIES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ version.c $(LIBS)

myexample:
	$(MAKE_DPARSER) my.g
	cc -I/usr/local/include my.c my.g.d_parser.c -L/usr/local/lib -ldparse

gram:
	$(MAKE_DPARSER) -i dparser_gram grammar.g
	mv grammar.g.d_parser.c grammar.g.c
	rm -f grammar.g.o
	$(MAKE) make_dparser

make_dparser.cat: make_dparser.1
	rm -f make_dparser.cat
	nroff -man make_dparser.1 | sed -e 's/.//g' > make_dparser.cat

BUILD_VERSION: $(EXECUTABLES)
	rm -f BUILD_VERSION.tmp
	echo $(D_BUILD_VERSION) > BUILD_VERSION.tmp
	mv BUILD_VERSION.tmp BUILD_VERSION

tar:
	(cd ..;tar czf d-$(RELEASE)-src.tar.gz d/*.c d/*.h $(TAR_FILES)) 

bintar:
	(cd ..;tar czf d-$(RELEASE)-$(OS_TYPE)-bin.tar.gz $(AUX_FILES) $(LIBRARIES:%=d/%) $(INCLUDES:%=d/%) $(EXECUTABLE_FILES:%=d/%))

clean:
	\rm -f *.o core *.core *.gmon *.d_parser.c *.a $(EXEC_FILES)
	(cd python;make clean)

depend:
	mkdep $(CFLAGS) $(ALL_SRCS)

-include .depend
