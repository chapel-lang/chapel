## Process this file with automake to produce Makefile.in       -*- makefile -*-
#   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/contrib/Makefile.am $
# Description: Makefile for GASNet PAMI spawner
# Copyright 2012, Lawrence Berkeley National Laboratory
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

target = gasnetrun_pami

# Need a bin_DATA rule, but automake disallows that.  So, fake it.
dotpldir = $(bindir)

# Make sure the perl script and shell wrapper are always current locally.
# This ensures we can run from the build directory if needed.
$(top_builddir)/other/perlstart: $(top_srcdir)/other/perlstart.in
	@cd $(top_builddir)/other && $(MAKE) perlstart
$(target): $(top_builddir)/other/perlstart
$(target).pl: $(top_srcdir)/mpi-conduit/contrib/gasnetrun_mpi.pl
	cp -f "$?" $@
all-local: $(target) $(target).pl

DISTCLEANFILES = $(target) $(target).pl

# Conditionally install the .pl and its wrapper
if USE_PAMI_CONDUIT
scripts = $(target)
data = $(target).pl
else
scripts =
data =
endif
bin_SCRIPTS = $(scripts)
dotpl_DATA = $(data)

.PHONY: force
force:

