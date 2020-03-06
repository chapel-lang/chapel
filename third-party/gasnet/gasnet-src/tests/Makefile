#   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/Makefile.in $
# Description: Makefile for GASNet tests
# tests/Makefile.  Generated from Makefile.in by configure.
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

all: 
	@echo This Makefile is not intended to be called directly
	@echo do a "make tests-seq" or "make tests-par" within the conduit directory

# linker to be used for MPI programs
GASNET_MPI_CC = 
GASNET_MPI_LD = $(GASNET_MPI_CC)

srcdir="/usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet/tests"

configfile=/dev/null # must be overridden by caller
include $(configfile)

tools_fragment = /usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet/gasnet_tools-par.mak
#tools_fragment = /usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet/gasnet_tools-seq.mak

$(tools_fragment): force
	cd /usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet && $(MAKE) `basename $@`

-include $(tools_fragment)


Makefile: /usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet/tests/Makefile.in $(tools_fragment)
	cd /usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet ; \
	CONFIG_HEADERS= CONFIG_FILES=tests/Makefile ./config.status

# GASNet-related MPI tests
# MPI_TESTS_SEQ = testmpi
# MPI_TESTS_PAR =

# C++ tests
CXX_TESTS = testcxx testlegacycxx testtoolscxx

# non-GASNet plain MPI tests
MPI1_TESTS = 			\
	testmpiperf 		\
	testmpicollperf         \
	testmpinbr		\
	testosumpi_bw  		\
	testosumpi_bw2 		\
	testosumpi_lat 		\
	testosumpi_bcast_lat 

MPI2_TESTS = 			\
	testosumpi2_put_lat 	\
	testosumpi2_put_bw 	\
	testosumpi2_put_bw2 	\
	testosumpi2_get_lat 	\
	testosumpi2_get_bw 	\
	testosumpi2_acc_lat  	

MPI_TESTS = $(MPI1_TESTS) $(MPI2_TESTS)

# tests that can be built in any mode
# in order of most useful execution
# NOTE: if you add a test here that is expected to pass on all conduits,
#   please also add it to upcr-harness/gasnet-tests/harness.conf
testprograms_seq =      \
        testgasnet      \
        testlegacy      \
        testtools       \
        testinternal    \
        testenv         \
        testhsl         \
        testsmall       \
        testlarge       \
        testval         \
        testam          \
        testmisc        \
        testcore1       \
        testcore2       \
        testcore3       \
        testcore4       \
        testcore5       \
        testslice       \
        testbarrier     \
        testbarrierconf \
        testbarrierlate \
        testlogGP       \
        testalign       \
        testrand        \
        testexit        \
        testvis         \
        testcoll        \
        testcollperf    \
        testteambarrier \
        testteambcast   \
        testreduce      \
        testteam        \
        testnbr         \
        testvisperf     \
        testqueue       \
        testratomic     \
        testfaddperf    \
        testimm         \
        testsplit       \
        testacc         \
        testreadonly    \
        $(CONDUIT_TESTS) \
        $(MPI_TESTS_SEQ)

# tests that should only be built in PARSYNC and PAR modes
testprograms_parsync =  \

# tests that should only be built in PAR mode
testprograms_par =  \
	testthreads \
	testcontend \
	testlockcontend \
	$(MPI_TESTS_PAR)

# misc files to clean
clean_files = \
	testhello \
	$(TESTEXIT_SCRIPT)

testprograms = $(testprograms_seq) $(testprograms_parsync) $(testprograms_par) $(MPI_TESTS) $(CXX_TESTS)

tests-seq: clean $(testprograms_seq)
	
tests-parsync: clean $(testprograms_seq) $(testprograms_parsync)
	
tests-par: clean $(testprograms_seq) $(testprograms_parsync) $(testprograms_par)
	
tests-mpi: $(MPI1_TESTS)

tests-mpi2: $(MPI2_TESTS)

clean:
	rm -f $(testprograms:=) $(clean_files)

distclean: 
	rm -f Makefile

GASNET_SEGMENT_STR="FAST"
#GASNET_SEGMENT_STR="LARGE"
#GASNET_SEGMENT_STR="EVERYTHING"
# GASNET_DEBUG_STR="debug"
GASNET_DEBUG_STR="nodebug"
GASNET_CONFIG_STR=$(GASNET_SEGMENT_STR)-$(GASNET_DEBUG_STR)
#CROSS_COMPILING=yes
CROSS_COMPILING=no

#DEBUG_MALLOC=MALLOC_CHECK_=$${MALLOC_CHECK_-2}
TEST_ENV=$(DEBUG_MALLOC)

TEST_INOUT=

# default node count
TEST_NODES=2
# provide args and node counts for specific tests, user can override with MANUAL_TESTPARAMS
TEST_PARAMS=                    					\
   testalign_args="$(TEST_INOUT)";   					\
   testam_args="$(TEST_INOUT)";   					\
   testlarge_args="$(TEST_INOUT)";   					\
   testsmall_args="$(TEST_INOUT)";   					\
   testcore2_args="$(TEST_INOUT)";   					\
   testcore1_args="1000";       					\
   testhsl_args="0";        						\
   testrand_args="1024";        					\
   testlogGP_args="1000 0 8 65536";					\
   testthreads_args="2";						\
   testmisc_nodes="1";							\
   testbarrierconf_nodes="3";						\
   testmpi_args="2";							\
   testmpiperf_args="BPFE 1000";					\
   testqueue_args="$(TEST_INOUT) 1 256 262144";				\
   testenv_args='GASNet is \"Global Address Space Networking\"';	\
   testvisperf_args="$(TEST_INOUT) -maxdata 131072 -maxcontig 512 10"; 	\
   testslice_args="1024000 10 10";			 		\
   testcollperf_args="-szfactor 4 16384 2 50 0";			\
   $(CONDUIT_TESTPARAMS) dummy=1 ; 					\
   $(MANUAL_TESTPARAMS) dummy=1 ;

# omit tests that are supposed to fail or known to cause failures
testprograms_err =      \
        testexit        \
	$(MPI_TESTS_SEQ) \
	$(MPI_TESTS_PAR)

# tests that do not link libgasnet and should execute directly on the frontend with no spawner
# (except in the case of cross-compilation, where we run them on the compute node using MPIRUN_CMD)
testprograms_nongasnet = \
	testtools testtoolscxx
CONDUIT_NAME_UC=`echo $(CONDUIT_NAME) | gawk  '{print toupper($$0)}'`
awk_cmdbuild={  qargs="'"'"$$args"'"'"; \
                gsub(ENVIRON["'"'"DQUOTE"'"'"],ENVIRON["'"'"BACKSLASH"'"'"]ENVIRON["'"'"DQUOTE"'"'"],qargs); \
                gsub("'"'"%C"'"'", "'"'"%P %A"'"'");       \
		gsub("'"'"%P"'"'", "'"'"./$$file"'"'");    \
		gsub("'"'"%L"'"'", "'"'"DUMMY"'"'");       \
		gsub("'"'"%V "'"'","'"'""'"'");            \
		gsub("'"'"%N"'"'", "'"'"$$nodes"'"'");     \
		gsub("'"'"%M"'"'", "'"'"$$nodes"'"'");     \
		gsub("'"'"%A"'"'", "'"'"$$args"'"'");      \
		gsub("'"'"%D"'"'", "'"'"$$cwd"'"'");       \
		gsub("'"'"%H"'"'", "'"'"$${GASNET_NODEFILE:-$$PBS_NODEFILE}"'"'");     \
		gsub("'"'"%Q"'"'", qargs); print }
perl_threadscan=print $$1 if (/\\\$$GASNetThreadModel: GASNET_([^ ]*) \\\$$/);

run-tests: force
	@$(TEST_PARAMS)                                                                 \
	export `echo "$(TEST_PARAMS)" | gawk 'BEGIN {FS="=";RS=";"}{ print $$1 }'` ;   \
	line='+++++++++++++++++++++++++++++++++++++++' ; line="$$line$$line" ;          \
	BACKSLASH='\' export BACKSLASH ;                                                \
	DQUOTE='"' export DQUOTE ;                                                      \
	tests_to_run="" export tests_to_run ;                                           \
	tests_to_ignore="" export tests_to_ignore ;                                     \
	for file in $(testprograms) ; do if test -x "$$file" ; then                     \
	  if test "`echo \" $(testprograms_err) \"| grep \" $$file \"; exit 0`"; then   \
	    tests_to_ignore="$$tests_to_ignore $$file" ;                                \
	  elif test -z "`echo \" $$tests_to_run \"| grep \" $$file \"; exit 0`"; then   \
	    tests_to_run="$$tests_to_run $$file" ;                                      \
	  fi ;                                                                          \
	fi ; done ;                                                                     \
	echo $$line ;                                                                   \
	echo " Running $(CONDUIT_NAME)-conduit tests:" ;                                \
        if test "$(TESTS)" ; then                                                       \
          tests_to_run="$(TESTS)" ; tests_to_ignore="" ;                                \
	fi ;                                                                            \
	for file in $$tests_to_run ; do echo "   $$file" ; done ;                       \
	if test "$$tests_to_ignore" ; then                                              \
	  echo " Ignoring tests: $$tests_to_ignore" ;                                   \
	fi ;                                                                            \
	echo " If this fails to spawn a job,"                                           \
	     "you may need to re-run with a command like:" ;                            \
	echo "  gmake run-tests $(CONDUIT_NAME_UC)_RUNCMD=\"$(CONDUIT_RUNCMD)\"" ;      \
	echo $$line ;                                                                   \
	TEST_RUNCMD="$$$(CONDUIT_NAME_UC)_RUNCMD" export TEST_RUNCMD ;                  \
	TEST_RUNCMD="`eval echo $$TEST_RUNCMD`" ;                                       \
	if test -z "$$TEST_RUNCMD" ; then                                               \
	  TEST_RUNCMD="$(CONDUIT_RUNCMD)" ;                                             \
	fi ;                                                                            \
	MPI_RUNCMD="$${MPI_RUNCMD:=/usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet/mpi-conduit/contrib/gasnetrun_mpi -np %N %C}" \
        export MPI_RUNCMD ;                                                             \
	if test -z "$$TIMECMD" ; then                                                   \
	  TIMECMD="/usr/bin/time" ;                                                         \
	fi ;                                                                            \
	for file in $$tests_to_run ; do                                                 \
            test_nongasnet=`echo " $(testprograms_nongasnet) "| grep " $$file "` ;      \
            test_mpi=`echo " $(MPI_TESTS) " | grep " $$file "` ;                        \
            if test "$$test_nongasnet" -a "$(CROSS_COMPILING)" = "yes"; then            \
              test_mpi=1 ; eval $${file}_nodes=1 ;                                      \
            fi ;                                                                        \
	    args="$$""$${file}_args" ;                                                  \
	    args="`eval echo $$args`" ;                                                 \
	    nodes="$$\{""$${file}_nodes-$(TEST_NODES)\}" ;                              \
	    nodes="`eval eval echo $$nodes`" ;                                          \
	    cwd=`pwd` ;                                          			\
	    if test "$$test_mpi" ; then                                                 \
	      echo "*** Running MPI test $$file ***" ;                                  \
              awk_cmdbuild=`echo "$(awk_cmdbuild)"` ; 					\
	      cmd=`echo "$$MPI_RUNCMD" | eval gawk \'$$awk_cmdbuild\'` ;         	\
	      faildesc="MPI test: $$file $$args" ;                                      \
	    elif test "$$test_nongasnet" ; then                                         \
	      echo "*** Running conduit-independent $$file ***" ;                       \
	      cmd="./$$file $$args" ;                                                   \
	      faildesc="$(CONDUIT_NAME)-conduit/$(GASNET_CONFIG_STR)/$$file $$args" ;   \
	    else                                                                        \
	      threadmode=`/usr/bin/perl -n -e '$(perl_threadscan)'                             \
                          $$file 2> /dev/null` ;                             \
	      echo "*** Running $(CONDUIT_NAME)-conduit $$file"                         \
		   "in $$threadmode/$(GASNET_SEGMENT_STR) mode ***" ;                   \
              awk_cmdbuild=`echo "$(awk_cmdbuild)"` ; 					\
	      cmd=`echo "$$TEST_RUNCMD" | eval gawk \'$$awk_cmdbuild\'` ;         	\
	      faildesc="$(CONDUIT_NAME)-conduit/$$threadmode-$(GASNET_CONFIG_STR)/$$file $$args" ; \
	    fi;                                                                         \
	    echo "$$cmd" ;                                                              \
	    ( eval env GASNET_DUMMY=\"GASNet dummy value\" $(TEST_ENV)                  \
	    $$TIMECMD $$cmd || echo "ERROR: Test exited with failure code=$$?" )        \
                             2>&1 | tee .test-output ;                                  \
            failure=`/usr/bin/perl -ne 'print if ((/ERROR/ || /fatal signal/) && !(/Retrying allocation/ || /GASNET_FREEZE_ON_ERROR/));' .test-output` ; \
            if test "$$failure" != "" ; then                                            \
               echo " " >> $(TESTLOG) ;                                                 \
               echo " *-* $$faildesc *-*" >> $(TESTLOG) ;                               \
               echo " " >> $(TESTLOG) ;                                                 \
               echo "$$failure" >> $(TESTLOG) ;                                         \
            fi ;                                                                        \
            rm -f .test-output ;                                                        \
	    echo $$line ;                                                               \
	done ;                                                                          \
	echo "TESTS COMPLETE"

TESTEXIT_SCRIPT=run-testexit
TESTEXIT_SEQMAX=13
TESTEXIT_PARMAX=18
do-run-testexit: force
	@echo "Building $(TESTEXIT_SCRIPT) script..."
	@echo "#!/bin/sh" > $(TESTEXIT_SCRIPT)
	@nodes=$${TEST_NODES-2} ; args="" ; file="testexit" ;             \
         BACKSLASH='\' export BACKSLASH ;                                 \
         DQUOTE='"' export DQUOTE ;                                       \
         awk_cmdbuild=`echo "$(awk_cmdbuild)"` ; 			  \
         cmd=`echo "$(CONDUIT_RUNCMD)" | eval gawk \'$$awk_cmdbuild\'` ; \
	 echo "INTERACTIVE_RUNCMD='$$cmd'"                        >> $(TESTEXIT_SCRIPT)
	@echo "BATCH_RUNCMD='testexit'"                           >> $(TESTEXIT_SCRIPT)
	@echo '#BATCH_RUNCMD="$$INTERACTIVE_RUNCMD"'              >> $(TESTEXIT_SCRIPT)
	@echo 'if test "$$PBS_O_WORKDIR" ; then '                 >> $(TESTEXIT_SCRIPT)
	@echo '  cd "$$PBS_O_WORKDIR"'                            >> $(TESTEXIT_SCRIPT)
	@echo 'fi'                                                >> $(TESTEXIT_SCRIPT)
	@echo 'if test "$$MP_PROCS" -o "$$PBS_NODEFILE" ; then'   >> $(TESTEXIT_SCRIPT)
	@echo '  RUNCMD="$$BATCH_RUNCMD"'                         >> $(TESTEXIT_SCRIPT)
	@echo "else"                                              >> $(TESTEXIT_SCRIPT)
	@echo '  RUNCMD="$$INTERACTIVE_RUNCMD"'                   >> $(TESTEXIT_SCRIPT)
	@echo "fi"                                                >> $(TESTEXIT_SCRIPT)
	@echo "MALLOC_CHECK_=$${MALLOC_CHECK_-2}; export MALLOC_CHECK_" >> $(TESTEXIT_SCRIPT)
	@if test "`echo ' mpi udp ofi ' | grep ' $(CONDUIT_NAME) '`" != "" ; then \
	   echo "have_interrupt=0"                                >> $(TESTEXIT_SCRIPT) ; \
	 else                                                                             \
	   echo "have_interrupt=1"                                >> $(TESTEXIT_SCRIPT) ; \
	 fi
	@echo 'echo "Starting run-testexit at: `date`"'           >> $(TESTEXIT_SCRIPT)
	@echo 'ID="`hostname`-$$MP_CHILD$$PBS_NODENUM"'           >> $(TESTEXIT_SCRIPT)
	@echo 'line="----------------------------------"'         >> $(TESTEXIT_SCRIPT)
	@echo 'line="$$line$$line"'                               >> $(TESTEXIT_SCRIPT)
	@i=0; \
	 needinterrupt="5 9 16" ; export needinterrupt ;                        \
	 while test "$$i" != "$(TESTEXIT_PARMAX)" ; do                       \
           i=`expr $$i + 1`;                                                 \
	   echo ""                                   >> $(TESTEXIT_SCRIPT) ; \
	   if test "`echo ' '$$needinterrupt' ' | grep ' '$$i' '`" != "" ; then  \
	     echo 'if test "$$have_interrupt" = "1" ; then' >> $(TESTEXIT_SCRIPT) ; \
	   fi ;                                                              \
	   echo 'echo "$$line"'                      >> $(TESTEXIT_SCRIPT) ; \
	   echo 'echo "$$ID: testexit '$$i'"'        >> $(TESTEXIT_SCRIPT) ; \
	   echo 'echo "$$RUNCMD"'" $$i"              >> $(TESTEXIT_SCRIPT) ; \
	   echo '$$RUNCMD'" $$i"                     >> $(TESTEXIT_SCRIPT) ; \
	   echo 'echo "$$ID: result=$$?"'            >> $(TESTEXIT_SCRIPT) ; \
	   echo "sleep 1"                            >> $(TESTEXIT_SCRIPT) ; \
	   if test "`echo ' '$$needinterrupt' ' | grep ' '$$i' '`" != "" ; then  \
	     echo 'else'                             >> $(TESTEXIT_SCRIPT) ; \
	     echo "echo 'Test $$i SKIPPED - conduit lacks interrupts'" >> $(TESTEXIT_SCRIPT) ; \
	     echo 'fi'                               >> $(TESTEXIT_SCRIPT) ; \
	   fi ;                                                              \
	   if test "$$i" = "$(TESTEXIT_SEQMAX)" ; then                       \
	     echo 'threadmode=`/usr/bin/perl -n -e '"'"'$(perl_threadscan)'"'"' ' \
                  'testexit 2> /dev/null`'      >> $(TESTEXIT_SCRIPT) ; \
	     echo 'if test "$$threadmode" = "SEQ" -o ' \
                       '"$$threadmode" = "PARSYNC" ; then' >> $(TESTEXIT_SCRIPT) ; \
	     echo '  echo "WARNING: testexit was compiled' \
                  'in GASNET_$$threadmode mode"'           >> $(TESTEXIT_SCRIPT) ; \
	     echo '  echo "WARNING: be sure to recompile the test in' \
                  'GASNET_PAR mode for full results"'      >> $(TESTEXIT_SCRIPT) ; \
	     echo 'else' >> $(TESTEXIT_SCRIPT) ; \
	   fi ; \
	 done ; \
	 echo 'fi' >> $(TESTEXIT_SCRIPT) 
	@echo 'echo "$$ID: done."'  >> $(TESTEXIT_SCRIPT)
	@chmod +x $(TESTEXIT_SCRIPT)
	@echo "Complete. Starting execution..."
	@./$(TESTEXIT_SCRIPT)

KEEPTMP_CFLAGS = $(patsubst %,-save-temps,$(KEEPTMPS))

# testtools is a special minimal case for an app that just uses GASNet tools
# it uses the variables provided by the gasnet_tools-{seq,par}.mak
##FAKE_DEFS = -DGASNETI_TOOLS_CONDUIT=MPI
testtools: force
	@cd /usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet && $(MAKE) lib$(GASNETTOOLS_TOOLLIB_NAME).a
	$(GASNETTOOLS_CC) $(GASNETTOOLS_CPPFLAGS) $(GASNETTOOLS_CFLAGS) $(FAKE_DEFS) -c -o $@.o $(srcdir)/$@.c
	$(GASNETTOOLS_LD) $(GASNETTOOLS_LDFLAGS) -o $@ $@.o $(GASNETTOOLS_LIBS)
	@test -n '$(KEEPTMPS)' || ( cmd="rm -f $@.o"; echo "$$cmd"; eval "$$cmd" )

# As above w/ a c++ tools client
testtoolscxx: force
	@if test -z "$(GASNETTOOLS_CXX)"; then \
		echo "ERROR: testtoolscxx target requested, but no c++ compiler is configured"; \
		exit 1; \
	 fi
	@cd /usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet && $(MAKE) lib$(GASNETTOOLS_TOOLLIB_NAME).a
	$(GASNETTOOLS_CXX) $(GASNETTOOLS_CPPFLAGS) $(GASNETTOOLS_CXXFLAGS) $(FAKE_DEFS) -c -o $@.o $(srcdir)/$@.cc
	$(GASNETTOOLS_CXX) $(GASNETTOOLS_CXXFLAGS) $(GASNETTOOLS_LDFLAGS) -o $@ $@.o $(GASNETTOOLS_LIBS)
	@test -n '$(KEEPTMPS)' || ( cmd="rm -f $@.o"; echo "$$cmd"; eval "$$cmd" )

# some tests need delay.o
testlogGP: delay.o
testlogGP_extraobjs = delay.o
testbarrierlate: delay.o
testbarrierlate_extraobjs = delay.o

# some tests need other/amx/testam.h
AMX_INC = -I/usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet/other/amx
testgasnet_extraincs    = $(AMX_INC)
testlegacy_extraincs    = $(AMX_INC)
testcxx_extraincs       = $(AMX_INC)
testlegacycxx_extraincs = $(AMX_INC)

# delay function, used by multiple tests
# need to deliberately exclude and disable optimization for this object file to prevent breaking delay
NOOPT_FLAG=-O0
delay.o: force
	$(GASNET_CC) $(GASNET_CPPFLAGS) $(GASNET_MISC_CFLAGS) $(KEEPTMP_CFLAGS) $(NOOPT_FLAG) $(MANUAL_CFLAGS) -c -o $@ $(srcdir)/$*.c

mpi-check: force
	@if test -z "$(GASNET_MPI_CC)" ; then \
	  echo "ERROR: requested building an MPI test, but configure detected no MPI compiler." ; \
	  echo "       Issue make again with GASNET_MPI_CC=<your mpi compiler>" ; \
	  exit 1 ; \
	fi

# testmpi must be compiled like an MPI program with GASNet support
testmpi: mpi-check force
	$(GASNET_MPI_CC) -I$(srcdir) $(GASNET_CPPFLAGS)  $(KEEPTMP_CFLAGS) $(MANUAL_MPICFLAGS) -c -o $@.o $(srcdir)/$@.c
	$(GASNET_MPI_LD) $(GASNET_LDFLAGS)  -o $@ $@.o $(GASNET_LIBS)  $(MANUAL_LIBS)
	@test -n '$(KEEPTMPS)' || ( cmd="rm -f $@.o"; echo "$$cmd"; eval "$$cmd" )

# the following are just plain MPI programs
# testmpinbr requires libm
$(MPI_TESTS): mpi-check force
	$(GASNET_MPI_CC) -I$(srcdir)/mpi -I/usr/local/pkg/upc/nightly/webtar_gasnet-stable/gasnet/other  $(KEEPTMP_CFLAGS) $(MANUAL_MPICFLAGS) -c -o $@.o $(srcdir)/mpi/$@.c
	$(GASNET_MPI_LD) $(GASNET_LDFLAGS)  -o $@ $@.o  -lm $(MANUAL_LIBS)
	@test -n '$(KEEPTMPS)' || ( cmd="rm -f $@.o"; echo "$$cmd"; eval "$$cmd" )

# test{,legacy}cxx test support for compiling a C++-based GASNet client
# this is unlikely to work on conduits requiring a special linker (eg mpi-conduit)
testcxx testlegacycxx: force
	@if test -z "$(GASNET_CXX)"; then \
		echo "ERROR: $@ target requested, but no c++ compiler is configured"; \
		exit 1; \
	 fi
	$(GASNET_CXX) -I$(srcdir) $($@_extraincs) $(GASNET_CXXCPPFLAGS) $(KEEPTMP_CFLAGS) $(GASNET_DEVWARN_CXXFLAGS) $(GASNET_CXXFLAGS) -c -o $@.o $(srcdir)/$@.cc
	$(GASNET_CXX) $(GASNET_CXXFLAGS) $(GASNET_LDFLAGS) -o $@ $@.o $($@_extraobjs) $(GASNET_LIBS)
	@test -n '$(KEEPTMPS)' || ( cmd="rm -f $@.o"; echo "$$cmd"; eval "$$cmd" )

# This is a model for how GASNet clients should be built:
test% : force
	$(GASNET_CC) -I$(srcdir) $($@_extraincs) $(GASNET_CPPFLAGS) $(KEEPTMP_CFLAGS) $(GASNET_DEVWARN_CFLAGS) $(GASNET_CFLAGS) -c -o $@.o $(srcdir)/$@.c
	$(GASNET_LD) $(GASNET_LDFLAGS) -o $@ $@.o $($@_extraobjs) $(GASNET_LIBS)
	@test -n '$(KEEPTMPS)' || ( cmd="rm -f $@.o"; echo "$$cmd"; eval "$$cmd" )

force:
	

.PHONY: force all tests-mpi tests-mpi2
