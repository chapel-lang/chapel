## Process this file with automake to produce Makefile.in       -*- makefile -*-
#   $Source: /var/local/cvs/gasnet/dcmf-conduit/contrib/Makefile.am,v $
#     $Date: 2008/10/28 05:43:41 $
# $Revision: 1.2 $
# Description: Makefile for GASNet DCMF spawner
# Copyright 2008, Rajesh Nishtala <rajeshn@cs.berkeley.edu>
#                 Paul Hargrove <phhargrove@lbl.gov>
#                 Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in dcmf-conduit/license.txt

AUTOMAKE_OPTIONS = foreign 1.4

builddir = $(top_builddir)/dcmf-conduit/contrib
target = gasnetrun_dcmf

# Need a bin_DATA rule, but automake disallows that.  So, fake it.
mybindir = $(bindir)

# Make sure the perl script and shell wrapper are always current locally.
# This ensures we can run from the build directory if needed.
$(top_builddir)/other/perlstart: $(top_srcdir)/other/perlstart.in
	@$(MAKE) -C $(top_builddir)/other perlstart
$(target): $(top_builddir)/other/perlstart
all-local: $(target).pl $(target)
$(target).pl: $(top_srcdir)/mpi-conduit/contrib/gasnetrun_mpi.pl
	cp -f $< $@
distclean-local:
	rm -f $(target).pl

# Ensure the .pl is distributed, even if not installed
#EXTRA_DIST = $(target).pl
#  DON'T NEED THIS SINCE WE ARE COPYING FROM THE MPI-CONDUIT

# Conditionally install the .pl and its wrapper
if USE_DCMF_CONDUIT
scripts = $(target) $(target).pl
data = $(target).pl
else
scripts =
data =
endif
bin_SCRIPTS = $(scripts)
mybin_DATA = $(data)

.PHONY: force
force:

