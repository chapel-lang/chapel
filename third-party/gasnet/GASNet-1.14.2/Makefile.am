## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/Makefile.am,v $
#     $Date: 2009/10/29 22:24:43 $
# $Revision: 1.85 $
# Description: Makefile for GASNet 
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4
ACLOCAL_AMFLAGS = -I other/plpa/config

# Directories for automake to recurse into:
SUBDIRS = .          \
	config-aux   \
        other

# GASNET_TOOLSONLY_TRUE_BEGIN
# CONDUITMODE_DIST_SUBDIRS = 
# CONDUITMODE_headers = 
# CONDUITMODE_EXTRA_DIST = 	\
# 	.gasnet_toolsonly_mode 
# GASNET_TOOLSONLY_TRUE_END
# GASNET_TOOLSONLY_FALSE_BEGIN
  # Directories we manually recurse into:
CONDUITMODE_DIST_SUBDIRS =    	\
	elan-conduit 		\
	gm-conduit   		\
	lapi-conduit 		\
	mpi-conduit  		\
	sci-conduit  		\
	shmem-conduit  		\
	smp-conduit  		\
	udp-conduit  		\
	vapi-conduit 		\
	ibv-conduit 		\
	portals-conduit         \
	dcmf-conduit
CONDUITMODE_headers = 		\
	gasnet.h           	\
	gasnet_ammacros.h  	\
	gasnet_coll.h      	\
	gasnet_handler.h   	\
	gasnet_pshm.h		\
	gasnet_trace.h     	\
	gasnet_vis.h      
CONDUITMODE_EXTRA_DIST =    	\
	docs              	\
	extended-ref      	\
	template-conduit  	\
	gasnet_internal.h 	\
	gasnet_internal.c 	\
	gasnet_mmap.c  	  	\
	gasnet_diagnostic.c 	\
	gasnet_pshm.c		\
	gasnet_trace.c
# GASNET_TOOLSONLY_FALSE_END

DIST_SUBDIRS = $(SUBDIRS) $(CONDUITMODE_DIST_SUBDIRS)

configdir = $(includedir)
config_DATA = $(CONFIG_HEADER)
docdir = $(datadir)/doc/gasnet

headers =                  	\
        $(CONDUITMODE_headers) 	\
	gasnet_asm.h       	\
	gasnet_atomicops.h 	\
	gasnet_atomic_bits.h 	\
        gasnet_basic.h     	\
        gasnet_help.h      	\
	gasnet_membar.h    	\
	gasnet_syncops.h   	\
        gasnet_timer.h     	\
	gasnet_tools.h     	\
	gasnet_toolhelp.h  	

EXTRA_DIST =              		\
	README				\
 	README-tools 			\
	ChangeLog	  		\
	license.txt       		\
	.autom4te.cfg     		\
        Bootstrap         		\
        unBootstrap       		\
        configure.in      		\
        acconfig.h        		\
        acinclude.m4      		\
        $(headers)        		\
        gasnet_tools.c    		\
	$(CONDUITMODE_EXTRA_DIST)
#       tests handled below

include_HEADERS = $(headers)

INSTALL_DOCS =			\
	README			\
	README-tools		\
	license.txt		\
	docs/autotuner.txt	\
	docs/pshm-design.txt	\
	docs/gasnet.txt		\
	docs/gasnet.html	\
	docs/gasnet.pdf		\
	docs/gasnet.ps	

force:

#-------------------------------------------------------
# libgasnet_tools goop

if BUILD_SEQ_LIBS
libgasnet_tools_seq_a_SOURCES =
libraries_seq = libgasnet_tools-seq.a 
endif
if BUILD_PAR_LIBS
libgasnet_tools_par_a_SOURCES =
libraries_par = libgasnet_tools-par.a 
endif
libraries = $(libraries_seq) $(libraries_par)

# leading - prevents distclean failures
-include $(top_builddir)/other/Makefile-libgasnet.mak

libgasnet_tools-seq.a: $(libgasnet_tools_dependencies)
	@$(MAKE) do-libgasnet_tools-seq

libgasnet_tools-par.a: $(libgasnet_tools_dependencies)
	@$(MAKE) do-libgasnet_tools-par

lib_LIBRARIES = $(libraries)
#-------------------------------------------------------

# bug1758: cannot depend on $(top_builddir)/other/Makefile-libgasnet.mak here,
#  as that will cause a failure during distclean on some make versions
#  make should automatically introduce the desired dependency on Makefile-libgasnet.mak.in
#  due to the include operation above
Makefile: $(srcdir)/Makefile.in 
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= ./config.status

# echo an arbitrary make/environment variable
echovar: force
	@echo $(VARNAME)=\"$($(VARNAME))\"

if CONDUIT_MODE
test: tests-seq
tests: tests-seq

other-tests:
	@cd $(top_builddir)/other && $(MAKE) all tests

tests-seq tests-parsync tests-par: other-tests
	@for conduit in $(CONDUITS) ; do                                       \
	  ( cd "$(top_builddir)/$$conduit-conduit" && $(MAKE) $@ ) || exit $$?; \
	done

TESTINSTANCE=$${PPID:-xxxx}
TESTLOG=`pwd`/.test-results


run-tests run-tests-seq run-tests-parsync run-tests-par: do-begin-tests
	@testinst=$(TESTINSTANCE) ; 							\
	testlog=$(TESTLOG) ; 								\
        err=0 ; 									\
	for conduit in $(CONDUITS) ; do                                        		\
	  ( cd "$(top_builddir)/$$conduit-conduit" &&                          		\
            $(MAKE) TESTINSTANCE=$$testinst TESTLOG=$$testlog 				\
		    TESTLOG_INHERIT=1 $@ ) || err=$$? ;					\
          if test "$$err" != "0" ; then                             			\
		echo " *-* COMPILE FAILURE: $$conduit-conduit $@ *-*" >> $(TESTLOG) ;   \
		break ; 								\
	  fi ;                        							\
	done ; 										\
        if test "$$err" = "0" ; then  							\
	  ( cd $(top_builddir)/other &&  						\
            $(MAKE) TESTINSTANCE=$$testinst TESTLOG=$$testlog 				\
		    TESTLOG_INHERIT=1 run-tests ) || err=$$? ; 				\
        fi ; 										\
	$(MAKE) TESTINSTANCE=$(TESTINSTANCE) TESTLOG=$(TESTLOG) do-end-tests ; 		\
	exit $$err

do-begin-tests:
	@testinst=$(TESTINSTANCE) ; \
	testlog=$(TESTLOG) ; \
	if test "`sed 1q $$testlog 2> /dev/null`" != "$$testinst" ; then \
	  echo "$$testinst" > $$testlog ; \
	  echo Starting a new test instance, log=$$testlog ; \
	fi 

do-end-tests:
	@testinst=$(TESTINSTANCE) ; \
	testlog=$(TESTLOG) ; \
	if test "`sed 1q $$testlog 2> /dev/null`" = "$$testinst" ; then \
	  line='+++++++++++++++++++++++++++++++++++++++' ; line="$$line$$line" ; \
	  echo "$$line" ; \
	  echo Test results: `sed -n '1d;/\*-\*/p' $$testlog | wc -l` failures ; \
	  sed 1d $$testlog ; \
	  echo "$$line" ; \
	fi
CLEANFILES = $(TESTLOG) 
else
TOOLSONLY_CHECK = run-tests
TOOLSONLY_TESTS = testtools
test: tests
tests: force
	@$(MAKE) -f $(top_builddir)/tests/Makefile \
	        GASNET_INCLUDES="-I@TOP_BUILDDIR@ -I@TOP_SRCDIR@ -I@TOP_SRCDIR@/other" \
		$(TOOLSONLY_TESTS)
run-tests: tests
	./testtools
CLEANFILES = $(TOOLSONLY_TESTS)
endif

# Build, clean, install, uninstall, etc. the conduits - in parallel if possible.
# Instead of automake's loop over subdir, we expand to one target per subdir.
# Care is taken w/ the 'clean-local' rule in case distclean has wiped the 
# local or conduit Makefiles (order depends on the version of automake).
# The same goes for 'distclean-local' rule.
# Note that if ($VAR:%=%FOO) doesn't work for a given make, then we are
# reduced to using actual conduit names as PHONY targets, but the rule for
# $(recurse_local) is tailored to work correctly regardless.
recurse_local = $(CONDUITS:%=%-recurse) recurse_local_dummy
$(recurse_local):
	@if test "$@" != "recurse_local_dummy" ; then \
	  cd `echo $@ | sed s/-recurse//`-conduit && $(MAKE) $(DO_WHAT) || exit $$? ; \
	 fi
# conduit subdirectories that should be made, even when the conduit is disabled
SPECIAL_SUBDIRS = mpi-conduit/contrib
recurse_special:
	@for dir in $(SPECIAL_SUBDIRS) ; do \
	  if test -f $$dir/Makefile ; then \
	    cd $$dir && $(MAKE) $(DO_WHAT) || exit $$? ; \
	  fi ; \
	 done
all-local: $(lib_LIBRARIES)
	@$(MAKE) $(recurse_local) DO_WHAT=$${DO_WHAT=all}
	@$(MAKE) recurse_special  DO_WHAT=$${DO_WHAT=all}
clean-local:
	@if test -f Makefile -a -f smp-conduit/Makefile; then \
	  $(MAKE) $(recurse_local) DO_WHAT=clean || exit $$? ; \
	 fi
	@$(MAKE) recurse_special DO_WHAT=clean
check-local: check-exports $(TOOLSONLY_CHECK)
seq par parsync:
	@$(MAKE) $(recurse_local) DO_WHAT=$@
check tests-clean:
	@$(MAKE) $(recurse_local) DO_WHAT=$@
	@cd "$(top_builddir)/other" && $(MAKE) $@

install-data-local: 
	@if test -n "$(DESTDIR)" -a `expr "X$(DESTDIR)" : X/` = 0; then \
          echo "ERROR: \$$DESTDIR must begin with a '/' character."; \
	  exit 1; \
	 fi
	@if test `expr "X$(prefix)" : X/` = 0; then \
          echo "ERROR: \$$prefix must begin with a '/' character."; \
	  exit 1; \
	 fi
	@$(MAKE) $(recurse_local) DO_WHAT=install-data
	@$(MAKE) recurse_special DO_WHAT=install-data
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	if test -f $(top_srcdir)/extended-ref/README ; then \
	  $(INSTALL_DATA) $(top_srcdir)/extended-ref/README $(DESTDIR)$(docdir)/README-extref || exit $$? ; \
	fi
	for file in $(INSTALL_DOCS) ; do \
	  if test -f $(top_srcdir)/$$file ; then \
	    $(INSTALL_DATA) $(top_srcdir)/$$file $(DESTDIR)$(docdir)/`basename $$file` || exit $$? ; \
	  fi ; \
	done
	for file in @PATCHED_HEADER@ DUMMY ; do \
	  if test -f $$file ; then \
	    $(mkinstalldirs) $(DESTDIR)$(includedir)/patched-headers ; \
	    $(INSTALL_DATA) patched-headers/$$file $(DESTDIR)$(includedir)/patched-headers/$$file || exit $$? ; \
	  fi ; \
	done
install-exec-local:
	@if test -n "$(DESTDIR)" -a `expr "X$(DESTDIR)" : X/` = 0; then \
          echo "ERROR: \$$DESTDIR must begin with a '/' character."; \
	  exit 1; \
	 fi
	@if test `expr "X$(prefix)" : X/` = 0; then \
          echo "ERROR: \$$prefix must begin with a '/' character."; \
	  exit 1; \
	 fi
	@$(MAKE) $(recurse_local) DO_WHAT=install-exec
	@$(MAKE) recurse_special DO_WHAT=install-exec
uninstall-local: 
	@if test -n "$(DESTDIR)" -a `expr "X$(DESTDIR)" : X/` = 0; then \
          echo "ERROR: \$$DESTDIR must begin with a '/' character."; \
	  exit 1; \
	 fi
	@if test `expr "X$(prefix)" : X/` = 0; then \
          echo "ERROR: \$$prefix must begin with a '/' character."; \
	  exit 1; \
	 fi
	@$(MAKE) $(recurse_local) DO_WHAT=uninstall
	@$(MAKE) recurse_special DO_WHAT=uninstall
	rm -Rf $(DESTDIR)$(docdir)
	for file in @PATCHED_HEADER@ ; do \
	  rm -f $(DESTDIR)$(includedir)/patched-headers/$$file ; \
	done

# automake 1.4 doesn't automatically make distclean in DIST_SUBDIRS
# So, we run it here if it doesn't appear to have already been run.
# We also take care of tests, which contains a generated Makefile
# that older autotools fail to automatically remove as well.
distclean-local:
	@for dir in $(CONDUIT_DIRS) tests; do               \
	   if test -f "$$dir/Makefile"; then                \
	     ( cd "$$dir" && $(MAKE) distclean ) || exit 1; \
	   fi;                                              \
	 done
	@if test -f Makefile; then                   \
	  $(MAKE) recurse_special DO_WHAT=distclean; \
	 fi

.PHONY: force tests test do-begin-tests do-end-tests                  \
        other-tests other-run-tests tests-seq tests-par tests-parsync \
        run-tests-seq run-tests-par run-tests-parsync                 \
	recurse_special $(recurse_local)

if TOOLSONLY_MODE
TESTDIST = README testtools.c test.h Makefile.in
else
TESTDIST = `find . -print`
endif

dist-hook:
	if test -f $(distdir)/docs/Makefile ; then \
          cd $(distdir)/docs && $(MAKE) dist-hook || exit $$? ; \
        fi
	mkdir -p $(distdir)/tests  # need this special hack to handle tests or distcheck will fail
	fulldistdir=`cd $(distdir) ; @PWD_PROG@` export fulldistdir ; \
        cd $(srcdir)/tests ; for file in $(TESTDIST) ; do \
 	  if test -d $$file -a ! -d $$fulldistdir/tests/$$file ; then \
	     mkdir -p $$fulldistdir/tests/$$file || exit $$? ; \
          fi; \
	  if test -f $$file -a ! -f $$fulldistdir/tests/$$file ; then \
	     cp -p $$file $$fulldistdir/tests/$$file || exit $$? ; \
          fi ; \
        done
	rm -Rf `find $(distdir) -name CVS -type d -print`
	rm -f `find $(distdir) -name .#\* -print`

# If we've enabled PSHM, also enabled for 'make distcheck'
if USE_PSHM
PSHM_CONFIG_FLAG=--enable-pshm
else
PSHM_CONFIG_FLAG=
endif
DISTCHECK_CONFIGURE_FLAGS = $(PSHM_CONFIG_FLAG)

