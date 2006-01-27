# Makefile for IF1 library

# ******************************
# ***** User Level Options ***** 
# ******************************

DEBUG=1
#OPTIMIZE=1
#PROFILE=1
#USE_GC=1
#LEAK_DETECT=1

ifndef PREFIX
PREFIX=/usr/local
endif

# **********************************
# ***** End User Level Options *****
# **********************************

MAJOR=0
MINOR=1
RELEASE=$(MAJOR).$(MINOR)

CC = g++

.PHONY: all test install

OS_TYPE = $(shell uname -s | awk '{ split($$1,a,"_"); printf("%s", a[1]);  }')
OS_VERSION = $(shell uname -r | awk '{ split($$1,a,"."); sub("V","",a[1]); printf("%d%d%d",a[1],a[2],a[3]); }')
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

ifeq ($(OS_TYPE),CYGWIN)
GC_CFLAGS += -L/usr/local/lib
else
GC_CFLAGS += -I/usr/local/include -L/usr/local/lib
endif

ifdef USE_GC
CFLAGS += -DUSE_GC ${GC_CFLAGS}
LIBS += -lgc
endif
ifdef LEAK_DETECT
CFLAGS += -DLEAK_DETECT  ${GC_CFLAGS}
LIBS += -lleak
endif

VERSIONCFLAGS += -DMAJOR_VERSION=$(MAJOR) -DMINOR_VERSION=$(MINOR)

CFLAGS += -Wall -Wno-non-virtual-dtor
# debug flags
ifdef DEBUG
CFLAGS += -g -DDEBUG=1
endif
# optimized flags
ifdef OPTIMIZE
CFLAGS += -O3
endif
ifdef PROFILE
CFLAGS += -pg
endif

CPPFLAGS += $(CFLAGS)

LIBS += -lm 

DIR = libifa

AUX_FILES = $(DIR)/Makefile $(DIR)/COPYRIGHT $(DIR)/README $(DIR)/index.html $(DIR)/manual.html $(DIR)/faq.html $(DIR)/ifa.1 $(DIR)/ifa.cat
TAR_FILES = $(AUX_FILES) $(TEST_FILES)

LIB_SRCS = ast.cc builtin.cc cdb.cc cfg.cc cg.cc clone.cc \
	dead.cc dom.cc fa.cc fail.cc fun.cc graph.cc html.cc if1.cc ifa.cc inline.cc \
	list.cc log.cc loop.cc map.cc num.cc pattern.cc pdb.cc pnode.cc prim.cc prim_data.cc \
	region.cc ssu.cc sym.cc var.cc vec.cc
LIB_OBJS = $(LIB_SRCS:%.cc=%.o)

IFA_SRCS = main.cc
IFA_OBJS = $(IFA_SRCS:%.cc=%.o)

EXECUTABLE_FILES = ifa
LIBRARIES = ifa.a
INSTALL_LIBRARIES = ifa.a
INCLUDES = ifa.h
MANPAGES = ifa.1

ifeq ($(OS_TYPE),CYGWIN)
EXECUTABLES = $(EXECUTABLE_FILES:%=%.exe)
IFA = ifa.exe
else
EXECUTABLES = $(EXECUTABLE_FILES)
IFA = ifa
endif

SEPARATE_IFA_CPP_FILES_1 = $(shell ls ../*.cpp)
SEPARATE_IFA_CPP_FILES = $(SEPARATE_IFA_CPP_FILES_1:../%.cpp=%.cpp)
SEPARATE_IFA_H_FILES_1 = $(shell ls ../*.h)
SEPARATE_IFA_H_FILES = $(SEPARATE_IFA_H_FILES_1:../%.h=%.h)

ALL_SRCS = $(IFA_SRCS) $(LIB_SRCS)

all: $(EXECUTABLES) $(LIBRARIES) ifa.cat

version:
	echo $(OS_TYPE) $(OS_VERSION)

test:
	(MAKE=$(MAKE) ./ifa_tests)

version.o: version.cc
	$(CC) $(CFLAGS) $(VERSIONCFLAGS) -c version.cc

install:
	cp $(EXECUTABLES) $(PREFIX)/bin
	cp $(MANPAGES) $(PREFIX)/man/man1
	cp $(INCLUDES) $(PREFIX)/include
	cp $(INSTALL_LIBRARIES) $(PREFIX)/lib

uninstall:
	rm $(EXECUTABLES:%=$(PREFIX)/bin/%)
	rm $(MANPAGES:%=$(PREFIX)/man/man1/%)
	rm $(INCLUDES:%=$(PREFIX)/include/%)
	rm $(INSTALL_LIBRARIES:%=$(PREFIX)/lib/%)

$(IFA): $(IFA_OBJS) $(LIBRARIES)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) 

ifa.a:	$(LIB_OBJS)
	ar $(AR_FLAGS) $@ $^

ifa.cat: ifa.1
	rm -f ifa.cat
	nroff -man ifa.1 | sed -e 's/.//g' > ifa.cat

tar:
	(cd ..;tar czf libifa-$(RELEASE)-src.tar.gz libifa/*.cc $(DIR)/*.h $(TAR_FILES)) 

clean:
	\rm -f *.o core *.core *.gmon *.a $(EXEC_FILES)

depend:
	mkdep $(CFLAGS) $(ALL_SRCS)

separate:
	-for file in $(SEPARATE_IFA_CPP_FILES); do \
	  ln -s -f ../$$file $${file%cpp}cc; \
	done
	-for file in $(SEPARATE_IFA_H_FILES); do \
	  ln -s -f ../$$file $$file; \
	done
	ln -s -f ../cast_code.cc cast_code.cc
	rm defs.h
	ln -s -f ifa_defs.h defs.h

version.o: Makefile

-include .depend
