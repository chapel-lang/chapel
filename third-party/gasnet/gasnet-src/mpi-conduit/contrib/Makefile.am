## Process this file with automake to produce Makefile.in       -*- makefile -*-
#   $Source: bitbucket.org:berkeleylab/gasnet.git/mpi-conduit/contrib/Makefile.am $
# Description: Makefile for GASNet MPI spawner
# Copyright (c) 2015 Intel Corporation. All rights reserved.
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

target = gasnetrun_mpi
target_pl = $(target).pl

# Need a bin_DATA rule, but automake disallows that.  So, fake it.
dotpldir = $(bindir)

# Make sure the perl script and shell wrapper are always current locally.
# This ensures we can run from the build directory if needed.
$(top_builddir)/other/perlstart: $(top_srcdir)/other/perlstart.in
	@cd $(top_builddir)/other && $(MAKE) perlstart
$(top_builddir)/other/mpirun_common: $(top_srcdir)/other/mpirun_common.in
	@cd $(top_builddir)/other && $(MAKE) mpirun_common
$(target): $(top_builddir)/other/perlstart $(top_builddir)/other/mpirun_common
if USE_MPI
all_targets = $(target_pl) $(target)
endif
all-local: $(all_targets)

$(target_pl): $(top_srcdir)/other/spawner/gasnetrun_mpi.pl
	cp -f "$<" $@

DISTCLEANFILES = $(target_pl)

# Conditionally install the .pl and its wrapper
if USE_MPI_CONDUIT
scripts = $(target)
data = $(target_pl)
else
scripts =
data =
endif
bin_SCRIPTS = $(scripts)
dotpl_DATA = $(data)

