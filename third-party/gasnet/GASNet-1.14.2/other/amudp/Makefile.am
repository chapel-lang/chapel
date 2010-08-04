## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/other/amudp/Makefile.am,v $
#     $Date: 2009/03/31 01:03:35 $
# $Revision: 1.42 $
# Description: Makefile for AMUDP/GASNet 
# Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt
# NOTE: This Makefile is used to tie in with the GASNet build infrastructure
#       the stand-alone AMUDP product doesn't currently provide an automake framework

AUTOMAKE_OPTIONS = foreign 1.4 no-dependencies

if PLATFORM_ILP32
  apputils_flags =
else
  apputils_flags = -DAPPUTILS_OMIT_READWRITE
endif

if BUILDCONFIG_DEBUG
  BUILDCONFIG_DEFINES = -DAMUDP_DEBUG=1
else
  BUILDCONFIG_DEFINES = -DAMUDP_NDEBUG=1
endif

if USE_STRICT_PROTOTYPES
  strict_proto_flags = -DGASNET_USE_STRICT_PROTOTYPES
else
  strict_proto_flags = 
endif

CC  = @CC@
CFLAGS = @CFLAGS@ @MISC_CFLAGS@ $(MANUAL_CFLAGS) $(apputils_flags) $(strict_proto_flags)
CXX = @CXX@
CXXFLAGS = @CXXFLAGS@ @MISC_CXXFLAGS@ $(MANUAL_CXXFLAGS)
LDFLAGS = @LDFLAGS@ @cxxLDFLAGS@ $(MANUAL_LDFLAGS)

PERL = @PERL@

platform_defines = @GASNET_MACHINE_DEFINES@ $(BUILDCONFIG_DEFINES) @INTTYPES_DEFINES@ -DAMUDP_ENV_PREFIX=GASNET $(MANUAL_DEFINES)

platform_libraries = -L$(top_builddir) -lgasnet_tools-seq @LIBS@ @GASNET_UDP_LIBS@

VPATH = $(srcdir)
testdir = $(srcdir)/../amxtests
altincdir = $(srcdir)/..
gasnet_tools = -I$(srcdir)/../.. -I../.. -DHAVE_GASNET_TOOLS
INCLUDES = -I$(srcdir) -I$(altincdir) $(platform_defines) $(gasnet_tools)
compile = $(CC) $(CFLAGS) $(INCLUDES)
cxxcompile = $(CXX) $(CXXFLAGS) $(INCLUDES)
link = $(CXX) $(CXXFLAGS) $(LDFLAGS) $(INCLUDES) 
linkend = $(platform_libraries) $(MANUAL_LIBS)

if USE_UDP_CONDUIT
lib_LIBRARIES = libamudp.a
bin_PROGRAMS = amudprun
endif
headers = \
	amudp.h			\
	amudp_internal.h	\
	amudp_spmd.h		\
	exc.h			\
	sig.h			\
	sockaddr.h		\
	socket.h		\
	socklist.h		\
	sockutil.h		\
	ueth.h			\
	ufxp.h

libamudp_a_SOURCES = \
	$(headers)		\
	amudp_cdefs.c		\
	amudp_ep.cpp		\
	amudp_reqrep.cpp	\
	amudp_spawn.cpp		\
	amudp_spmd.cpp		\
	exc.cpp			\
	sig.cpp			\
	socklist.cpp		\
	sockutil.cpp		

libamudp_a_DEPENDENCIES = $(libamudp_a_SOURCES:%.cpp=%.o)

EXTRA_DIST = \
	Makefile.common 	\
	Makefile.generic 	\
	Makefile.standalone 	\
	Makefile.tests  	\
	Makefile.titanium.in 	\
	amudprun.c              \
	license.txt

# ---------- test programs -------------
# all the test executables
if PLATFORM_ILP32
  testprograms_ILP32only = 	\
    testgetput    		\
    testreadwrite 
else
  testprograms_ILP32only = 
endif

testprograms =    		\
    testam        		\
    testbounce    		\
    testbulk      		\
    testlatency   		\
    testlatencyM   		\
    testping      		\
    testreduce			\
    $(testprograms_ILP32only)

tests: apputils.o $(testprograms)

test%: test%.o apputils.o libamudp.a
	$(link) -o $@ $< apputils.o -L. -lamudp $(linkend)

tests-clean:
	rm -f $(testprograms)

include Makefile.tests

test%.o : $(testdir)/test%.c $(testdir)/*.h $(headers)
	$(compile) -DAMUDP -I$(testdir) -c $< -o $@

apputils.o : $(testdir)/apputils.c $(testdir)/apputils.h $(headers)
	$(compile) -DAMUDP -I$(testdir) -c $(testdir)/apputils.c -o apputils.o

amudprun$(EXEEXT): amudprun.o $(headers) libamudp.a
	$(link) -o $@ $< -L. -lamudp $(linkend)

%.o: %.c $(headers)
	$(compile) -o $@ -c $<

%.o: %.cpp $(headers) $(top_builddir)/libgasnet_tools-seq.a
	$(cxxcompile) -o $@ -c $<

$(top_builddir)/libgasnet_tools-seq.a: force
	@cd $(top_builddir) && $(MAKE) libgasnet_tools-seq.a

if USE_UDP_CONDUIT
if GNU_NM
check-exports: libamudp.a
	@echo Checking libamudp exports...
	@lib=libamudp.a ; \
            @NM@ --defined-only $$lib |                                             \
            grep -v -e ' [\._]*amudp_' -e ' [\._]*AMUDP_' -e ' [\._]*AM_'           \
                    -e ' [\._]*gasnett_' -e ' [\._]*gasneti_'                       \
                    -e __FUNCTION__ -e __PRETTY_FUNCTION__ -e debug_info_seg |      \
            @PERL@ -n -e 'print if /^[0-9a-fA-F]+\s+[A-Z]\s+/' > .$$lib.exp;        \
          if test -s .$$lib.exp ; then                                              \
            cat .$$lib.exp ;                                                        \
            echo FAILED ;                                                           \
            failed=1 ;                                                              \
          else                                                                      \
            echo PASSED ;                                                           \
          fi ;                                                                      \
          rm -f .$$lib.exp ;                                                        \
          exit $$failed
else
check-exports: libamudp.a
	@echo check-exports test SKIPPED
endif

# No check-exports by default due to name mangling
check: tests

clean_local_files = libamudp.a *.o core .test-results
clean-local:
	@for f in $(testprograms); do                                \
	  cmd="rm -f '$$f$(EXESUFFIX)'"; echo "$$cmd"; eval "$$cmd" || exit $$? ; \
	 done;
else
clean_local_files =
clean-local:
	@: #empty rule
endif

CLEANFILES = $(clean_local_files)

force:

.PHONY: force tests run-tests

