## Process this file with automake to produce Makefile.in       -*- makefile -*-
#   $Source: bitbucket.org:berkeleylab/gasnet.git/ibv-conduit/contrib/Makefile.am $
# Description: Makefile for GASNet IBV spawner
# Terms of use are as specified in ibv-conduit/license.txt

AUTOMAKE_OPTIONS = foreign 1.4

# Use of the MPI spawning support is optional
if IBV_BOOTSTRAP_MPI
mpi_target_pl = gasnetrun_ibv-mpi.pl
$(mpi_target_pl): $(top_srcdir)/mpi-conduit/contrib/gasnetrun_mpi.pl
	cp -f "$?" $@
else
mpi_target_pl =
endif

target = gasnetrun_ibv

# Need a bin_DATA rule, but automake disallows that.  So, fake it.
dotpldir = $(bindir)

# Make sure the perl script and shell wrapper are always current locally.
# This ensures we can run from the build directory if needed.
$(top_builddir)/other/perlstart: $(top_srcdir)/other/perlstart.in
	@cd $(top_builddir)/other && $(MAKE) perlstart
$(target): $(top_builddir)/other/perlstart
all-local: $(target).pl $(target) $(mpi_target_pl)

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
DISTCLEANFILES = $(files_we_copy) $(mpi_target_pl)

# Ensure the .pl is distributed, even if not installed
EXTRA_DIST = $(target).pl

# Conditionally install the .pl and its wrapper
if USE_IBV_CONDUIT
scripts = $(target)
data = $(target).pl $(mpi_target_pl)
else
scripts =
data =
endif
bin_SCRIPTS = $(scripts)
dotpl_DATA = $(data)

.PHONY: force
force:

