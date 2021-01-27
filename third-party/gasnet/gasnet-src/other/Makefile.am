## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: bitbucket.org:berkeleylab/gasnet.git/other/Makefile.am $
# Description: Makefile for GASNet other 
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

# GASNET_TOOLSONLY_TRUE_BEGIN
# CONDUITMODE_SUBDIRS =
# CONDUITMODE_EXTRADIST =
# GASNET_TOOLSONLY_TRUE_END
# GASNET_TOOLSONLY_FALSE_BEGIN
CONDUITMODE_SUBDIRS = \
        ammpi      \
	amudp	   \
	contrib    \
	firehose   \
	valgrind

CONDUITMODE_EXTRADIST = \
	amx         \
	ssh-spawner \
	mpi-spawner \
	pmi-spawner \
	spawner     \
	myxml	\
	detect-pshm.c \
	pkgconfig-conduit.pc \
  # 	exclude these from distribution for now
  # 	cross-configure-help.c
# GASNET_TOOLSONLY_FALSE_END
SUBDIRS = $(CONDUITMODE_SUBDIRS)

EXTRA_DIST =        			\
        $(CONDUITMODE_EXTRADIST) 	\
	Makefile-echovar.mak            \
	pkgconfig-tools.pc              \
	check-pp.c                      \
	check-header-erase.pl           \
	tools-toggle.pl
# Excluded from distribution:
#	detect-cachesz.c 		\
#

include_HEADERS = \
	gasnet_arch_arm.h \
	portable_inttypes.h \
	portable_platform.h \
	gasnet_portable_platform.h

if CONDUIT_MODE
if USE_UDP_CONDUIT                           
  UDP_dir=$(top_builddir)/other/amudp
endif
if USE_AMMPI                           
  MPI_dir=$(top_builddir)/other/ammpi
endif
testdirs= $(UDP_dir) $(MPI_dir)

tests tests-clean run-tests check-exports: 
	@list='$(testdirs)'; for dir in $$list ; do \
	  ( cd "$$dir" && $(MAKE) $@ ) || exit $$?; \
	done
CONDUITMODE_TARGETS = Makefile-conduit.mak mpirun_common
endif

# Automated correctness checks for portable_platform.h
#
CHECK_PP_C = $(srcdir)/check-pp.c
$(CHECK_PP_C): gasnet_portable_platform.h portable_platform.h

check-pp-tgt: $(CHECK_PP_C)
	@CC@ @CFLAGS@ @LDFLAGS@ -I$(srcdir) -I$(top_builddir) '-DCONFIG_HEADER="gasnet_config.h"' -D_IN_GASNET_TOOLS_H=1 $(MANUAL_DEFINES) $(MANUAL_CFLAGS) $(MANUAL_LDFLAGS) -o $@ $< @LIBS@ $(MANUAL_LIBS)

# Disabled until we have a more compelling need for configure detection of HOST_CC
#check-pp-host: $(CHECK_PP_C)
#	@HOST_CC@ @HOST_CFLAGS@ @HOST_LDFLAGS@ -I$(srcdir) $(MANUAL_DEFINES) $(MANUAL_CFLAGS) $(MANUAL_LDFLAGS) -o $@ $< @HOST_LIBS@ $(MANUAL_LIBS)

if CROSS_COMPILING
check-pp-run: check-pp-tgt
else
check-pp-run: check-pp-tgt
	./check-pp-tgt
endif
.PHONY: check-pp-run
CLEANFILES = check-pp-tgt$(EXESUFFIX)

check-pp: check-pp-run
	@PERL@ $(srcdir)/check-header-erase.pl $(srcdir)/gasnet_portable_platform.h 1

check-local: check-pp

# If we add plpa to EXTRA_DIST, then automake-1.9 tries to package
# plpa/config/acinclude.m4 more than once.
dist-hook:
	rm -f $(distdir)/plpa/config/acinclude.m4
	cp -pR $(srcdir)/plpa $(distdir)/

all-local: $(CONDUITMODE_TARGETS) perlstart

# GASNET_TOOLSONLY_FALSE_BEGIN
## Docs to install from any subdirs
if NEED_DOCDIR
docdir = $(datadir)/doc/GASNet
endif
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	$(INSTALL_DATA) $(srcdir)/ssh-spawner/README $(DESTDIR)$(docdir)/README-ssh-spawner
	$(INSTALL_DATA) $(srcdir)/mpi-spawner/README $(DESTDIR)$(docdir)/README-mpi-spawner
	$(INSTALL_DATA) $(srcdir)/pmi-spawner/README $(DESTDIR)$(docdir)/README-pmi-spawner
uninstall-local:
	rm -Rf $(DESTDIR)$(docdir)
# GASNET_TOOLSONLY_FALSE_END

.PHONY: force clean tests tests-clean run-tests

force:
	
