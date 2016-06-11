## Process this file with automake to produce Makefile.in	-*- makefile -*-
# Description: Makefile for GASNet MXM conduit
# Copyright (c)  2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Copyright (c)  2011, Mellanox Technologies LTD. All rights reserved.
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=mxm

## Fca
if FCA_ENABLED
include $(top_builddir)/other/fca/fca.mak
endif
##
## SSH bootstrap support (built unless fork() is unavailable)
##
if MXM_BOOTSTRAP_SSH
ssh_defines = -DHAVE_SSH_SPAWNER
ssh_srcdir = $(top_srcdir)/other/ssh-spawner
ssh_sources = $(ssh_srcdir)/gasnet_bootstrap_ssh.c
ssh_deps = $(ssh_srcdir)/*.h
endif
##
## MPI bootstrap support (optional)
## XXX: should have a way to control this independent of MPI conduit
##
if MXM_BOOTSTRAP_MPI
mpi_defines = -DHAVE_MPI_SPAWNER
mpi_srcdir = $(top_srcdir)/other/mpi-spawner
mpi_deps = $(mpi_srcdir)/*.h $(mpi_srcdir)/*.c
# We must compile this one object with MPI_CC.
mpi_special_objs = $(builddir)/gasnet_bootstrap_mpi-$(THREAD_MODEL).o
$(builddir)/gasnet_bootstrap_mpi-$(THREAD_MODEL).o: force
	@MPI_CC@ @MPI_CFLAGS@ $(LIBDEFINES) $(CONDUIT_EXTRALIBCFLAGS) $(LIBINCLUDES) \
		-o $@ -c $(mpi_srcdir)/gasnet_bootstrap_mpi.c
endif
##
## PMI bootstrap support (optional)
##
if HAVE_BOOTSTRAP_PMI
pmi_cppflags = -DHAVE_PMI_SPAWNER @PMI_SPAWNER_INCLUDES@
pmi_srcdir = $(top_srcdir)/other/pmi-spawner
pmi_sources = $(pmi_srcdir)/gasnet_bootstrap_pmi.c
pmi_deps = $(pmi_srcdir)/*.h
endif

# any conduit-specific subdirectories containing Makefile.am's
SUBDIRS = contrib

# complete list of files in the conduit directory
# include all headers, documentation, etc. 
# and any subdirectories not containing Makefile.am's
CONDUIT_FILELIST =           \
      gasnet_core.c          \
      gasnet_core_connect.c  \
      gasnet_core_sndrcv.c   \
      gasnet_core.h          \
      gasnet_core_fwd.h      \
      gasnet_core_help.h     \
      gasnet_core_internal.h \
      gasnet_extended.c      \
      gasnet_extended_fwd.h  \
      gasnet_mxm_req.h       \
      license.txt

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =                  \
      $(srcdir)/gasnet_core.c         \
      $(srcdir)/gasnet_core_connect.c \
      $(srcdir)/gasnet_core_sndrcv.c  \
      $(srcdir)/gasnet_extended.c     \
      $(fca_sourcelist) $(ssh_sources) $(pmi_sources)

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = @MXM_INCLUDE@  $(ssh_defines) $(pmi_cppflags) $(mpi_defines) $(fca_extralibcflags)

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = $(fca_extraheaders)

# headers selected by default rules that should NOT be installed
CONDUIT_PRIVATEHEADERS = $(fca_privheaders) gasnet_mxm_req.h

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = $(ssh_deps) $(pmi_deps) $(mpi_deps) $(fca_extradeps)

# additional object files to be included in libgasnet that need to be compiled 
# using a special, conduit-specific command. These should also be included as
# forced targets in this file, and should probably use LIBINCLUDES/LIBDEFINES
CONDUIT_SPECIAL_OBJS = $(mpi_special_objs) $(fca_special_objs)

# the default job spawn command to be used for "make run-tests"
# The following substitutions are performed:
#   %P = program executable name
#   %N = requested node count
#   %A = program arguments
#   %Q = program arguments w/ an extra level of quotes
#   %D = the current working directory
#   %H = hostfile (if any)
CONDUIT_RUNCMD = @TOP_BUILDDIR@/mxm-conduit/contrib/gasnetrun_mxm -np %N %P %A

# conduit-specific tests in ../tests directory
CONDUIT_TESTS = testhello

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_mxm_seq_a_SOURCES =
libraries_seq = libgasnet-mxm-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_mxm_par_a_SOURCES =
libraries_par = libgasnet-mxm-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_mxm_parsync_a_SOURCES =
libraries_parsync = libgasnet-mxm-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-mxm-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-mxm-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-mxm-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_MXM_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(makefile_fragments)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif


