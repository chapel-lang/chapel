## Process this file with automake to produce Makefile.in       -*- makefile -*-
#   $Source: bitbucket.org:berkeleylab/gasnet.git/ucx-conduit/contrib/Makefile.am $
# Description: Makefile for GASNet UCX spawner
# Terms of use are as specified in ucx-conduit/license.txt

AUTOMAKE_OPTIONS = foreign 1.4

# Use of the MPI/PMI spawning support is optional
if HAVE_BOOTSTRAP_MPI
mpi_target_pl = gasnetrun_ucx-mpi.pl
$(mpi_target_pl): $(top_srcdir)/other/spawner/gasnetrun_mpi.pl
	cp -f "$?" $@
else
if HAVE_BOOTSTRAP_PMI
mpi_target_pl = gasnetrun_ucx-mpi.pl
$(mpi_target_pl): $(top_srcdir)/other/spawner/gasnetrun_mpi.pl
	cp -f "$?" $@
else
mpi_target_pl =
endif
endif

target = gasnetrun_ucx
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
all-local: $(target_pl) $(target) $(mpi_target_pl)

$(target_pl): $(top_srcdir)/other/spawner/gasnetrun.pl
	cp -f "$<" $@

DISTCLEANFILES = $(target) $(target_pl) $(mpi_target_pl)

# Conditionally install the .pl and its wrapper
if USE_UCX_CONDUIT
scripts = $(target)
data = $(target_pl) $(mpi_target_pl)
else
scripts =
data =
endif
bin_SCRIPTS = $(scripts)
dotpl_DATA = $(data)

