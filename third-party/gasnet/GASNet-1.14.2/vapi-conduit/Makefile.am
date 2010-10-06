## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/vapi-conduit/Makefile.am,v $
#     $Date: 2009/04/18 21:53:39 $
# $Revision: 1.47 $
# Description: Makefile for GASNet vapi conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=vapi

## Firehose
fh_type=region
include $(top_builddir)/other/firehose/firehose.mak

##
## SSH bootstrap support (built unless fork() is unavailable)
##
if VAPI_BOOTSTRAP_SSH
ssh_defines = -DHAVE_SSH_SPAWNER
ssh_srcdir = $(top_srcdir)/other/ssh-spawner
ssh_sources = $(ssh_srcdir)/gasnet_bootstrap_ssh.c
ssh_deps = $(ssh_srcdir)/*.h
endif

##
## MPI bootstrap support (optional)
## XXX: should have a way to control this independent of MPI conduit
##
if VAPI_BOOTSTRAP_MPI
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
## VAPI AM receiver thead support (optional)
##
if VAPI_RCV_THREAD
# VAPI conduit needs GASNET_THREAD_DEFINES even for SEQ mode
rcv_thread_defines = @GASNET_THREAD_DEFINES@
endif

# any conduit-specific subdirectories containing Makefile.am's
SUBDIRS = contrib

# complete list of files in the conduit directory
# include all headers, documentation, etc. 
# and any subdirectories not containing Makefile.am's
CONDUIT_FILELIST =               \
      gasnet_core.c              \
      gasnet_core_sndrcv.c       \
      gasnet_core.h              \
      gasnet_core_fwd.h          \
      gasnet_core_help.h         \
      gasnet_core_internal.h     \
      gasnet_extended.c          \
      gasnet_extended_fwd.h      \
      gasnet_extended_internal.h \
      gasnet_firehose.c          \
      firehose_fwd.h             \
      license.txt                

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =                 \
      $(srcdir)/gasnet_core.c        \
      $(srcdir)/gasnet_core_sndrcv.c \
      $(srcdir)/gasnet_extended.c    \
      $(srcdir)/gasnet_firehose.c    \
      $(fh_sourcelist) $(ssh_sources)

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = -I@VAPI_INCLUDE@ $(fh_extralibcflags) $(ssh_defines) $(mpi_defines)\
			 @VAPI_LIBCFLAGS@ $(rcv_thread_defines) -DGASNET_CONDUIT_VAPI

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = $(fh_extraheaders)

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = $(fh_extradeps) $(ssh_deps) $(mpi_deps)

# additional object files to be included in libgasnet that need to be compiled 
# using a special, conduit-specific command. These should also be included as
# forced targets in this file, and should probably use LIBINCLUDES/LIBDEFINES
CONDUIT_SPECIAL_OBJS = $(mpi_special_objs) $(fh_special_objs)

# the default job spawn command to be used for "make run-tests"
# The following substitutions are performed:
#   %P = program executable name
#   %N = requested node count
#   %A = program arguments
#   %Q = program arguments w/ an extra level of quotes
#   %D = the current working directory
#   %H = hostfile (if any)
CONDUIT_RUNCMD = @TOP_BUILDDIR@/vapi-conduit/contrib/gasnetrun_vapi -np %N %P %A

# conduit-specific tests in ../tests directory
CONDUIT_TESTS =

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_vapi_seq_a_SOURCES =
libraries_seq = libgasnet-vapi-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_vapi_par_a_SOURCES =
libraries_par = libgasnet-vapi-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_vapi_parsync_a_SOURCES =
libraries_parsync = libgasnet-vapi-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-vapi-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-vapi-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-vapi-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_VAPI_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(makefile_fragments)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif


