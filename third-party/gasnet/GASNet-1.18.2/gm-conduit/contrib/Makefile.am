## Process this file with automake to produce Makefile.in       -*- makefile -*-
#   $Source: /var/local/cvs/gasnet/gm-conduit/contrib/Makefile.am,v $
#     $Date: 2005/05/22 13:01:50 $
# $Revision: 1.17 $
# Description: Makefile for GASNet gm conduit
# Copyright 2002, Christian Bell <csbell@cs.berkeley.edu>
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

target = gasnetrun_gm

# Need a bin_DATA rule, but automake disallows that.  So, fake it.
dotpldir = $(bindir)

# Make sure the perl script and shell wrapper are always current locally.
# This ensures we can run from the build directory if needed.
$(top_builddir)/other/perlstart: $(top_srcdir)/other/perlstart.in
	@cd $(top_builddir)/other && $(MAKE) perlstart
$(target): $(top_builddir)/other/perlstart
all-local: $(target).pl $(target)

# Files to copy from srcdir to builddir if they differ
if BUILD_IS_SRC
files_we_copy =
else
files_we_copy = $(target).pl
$(files_we_copy): force
	@srcfile="$(srcdir)/`basename $@`" ; \
         if test ! -f "$@" -o "`find $$srcfile -newer '$@' 2>&1`" ; then \
          echo cp -f "$$srcfile" . ;         \
          cp -f "$$srcfile" . ;              \
         fi
endif
DISTCLEANFILES = $(files_we_copy)

# Ensure the .pl is distributed, even if not installed
EXTRA_DIST = $(target).pl

# Conditionally install the .pl and its wrapper
if USE_GM_CONDUIT
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

