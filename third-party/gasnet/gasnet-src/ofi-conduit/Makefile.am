## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: bitbucket.org:berkeleylab/gasnet.git/ofi-conduit/Makefile.am $
# Description: Makefile for GASNet ofi conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Copyright 2015, Intel Corporation
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=ofi

##
## SSH bootstrap support (built unless fork() is unavailable)
##
if HAVE_BOOTSTRAP_SSH
ssh_defines = -DHAVE_SSH_SPAWNER
ssh_srcdir = $(top_srcdir)/other/ssh-spawner
ssh_sources = $(ssh_srcdir)/gasnet_bootstrap_ssh.c
ssh_deps = $(ssh_srcdir)/*.[ch]
endif

##
## MPI bootstrap support (optional)
## XXX: should have a way to control this independent of MPI conduit
##
if HAVE_BOOTSTRAP_MPI
mpi_defines = -DHAVE_MPI_SPAWNER
mpi_srcdir = $(top_srcdir)/other/mpi-spawner
mpi_deps = $(mpi_srcdir)/*.[ch]
# We must compile this one object with MPI_CC.
mpi_special_objs = $(builddir)/gasnet_bootstrap_mpi-$(THREAD_MODEL).o
$(builddir)/gasnet_bootstrap_mpi-$(THREAD_MODEL).o: force
	@MPI_CC@ @MPI_CFLAGS@ $(LIBDEFINES) $(CONDUIT_EXTRALIBCFLAGS) $(LIBINCLUDES) \
		$(MANUAL_MPICFLAGS) -o $@ -c $(mpi_srcdir)/gasnet_bootstrap_mpi.c
endif

##
## PMI bootstrap support (optional)
##
if HAVE_BOOTSTRAP_PMI
pmi_cppflags = -DHAVE_PMI_SPAWNER @PMI_SPAWNER_CFLAGS@
pmi_srcdir = $(top_srcdir)/other/pmi-spawner
pmi_sources = $(pmi_srcdir)/gasnet_bootstrap_pmi.c
pmi_deps = $(pmi_srcdir)/*.[ch]
endif

# any conduit-specific subdirectories containing Makefile.am's
SUBDIRS = contrib

# complete list of files in the conduit directory
# include all headers, documentation, etc. 
# and any subdirectories not containing Makefile.am's
CONDUIT_FILELIST =           \
      license.txt            \
      gasnet_ofi.h           \
      gasnet_ofi.c          \
      gasnet_core.c          \
      gasnet_core.h          \
      gasnet_core_fwd.h      \
      gasnet_core_help.h     \
      gasnet_core_internal.h

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =          \
      $(srcdir)/gasnet_core.c \
      $(top_srcdir)/extended-ref/gasnet_extended.c \
      $(srcdir)/gasnet_ofi.c \
      $(ssh_sources) $(pmi_sources)

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = @OFI_CFLAGS@ $(ssh_defines) $(mpi_defines) $(pmi_cppflags)

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS =

# headers selected by default rules that should NOT be installed
CONDUIT_PRIVATEHEADERS =

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = $(ssh_deps) $(mpi_deps) $(pmi_deps)

# additional object files to be included in libgasnet that need to be compiled 
# using a special, conduit-specific command. These should also be included as
# forced targets in this file, and should probably use LIBINCLUDES/LIBDEFINES
CONDUIT_SPECIAL_OBJS = $(mpi_special_objs)

# the default job spawn command to be used for "make run-tests"
# The following substitutions are performed:
#   %P = program executable name
#   %N = requested node count
#   %A = program arguments
#   %Q = program arguments w/ an extra level of quotes
#   %D = the current working directory
#   %H = hostfile (if any)	
#CONDUIT_RUNCMD = mpirun -np %N %P %A
CONDUIT_RUNCMD = @TOP_BUILDDIR@/ofi-conduit/contrib/gasnetrun_ofi -np %N %P %A
#CONDUIT_RUNCMD = yod -np %N -ppn 1 %P %A

# conduit-specific tests in ../tests directory
CONDUIT_TESTS =

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_ofi_seq_a_SOURCES =
libraries_seq = libgasnet-ofi-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_ofi_par_a_SOURCES =
libraries_par = libgasnet-ofi-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_ofi_parsync_a_SOURCES =
libraries_parsync = libgasnet-ofi-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-ofi-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-ofi-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-ofi-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_OFI_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(pkgconfig_files)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnetex.h: do-error
endif


