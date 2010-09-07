## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/ibv-conduit/Makefile.am,v $
#     $Date: 2009/04/18 21:53:21 $
# $Revision: 1.8 $
# Description: Makefile for GASNet ibv conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=ibv

##
## vapi-conduit holds the actual sources
##
vapi_srcdir = $(top_srcdir)/vapi-conduit
vapi_install_hdrs = $(vapi_srcdir)/*.h

## Firehose
fh_type=region
include $(top_builddir)/other/firehose/firehose.mak

##
## SSH bootstrap support (built unless fork() is unavailable)
##
if IBV_BOOTSTRAP_SSH
ssh_defines = -DHAVE_SSH_SPAWNER
ssh_srcdir = $(top_srcdir)/other/ssh-spawner
ssh_sources = $(ssh_srcdir)/gasnet_bootstrap_ssh.c
ssh_deps = $(ssh_srcdir)/*.h
endif

##
## MPI bootstrap support (optional)
## XXX: should have a way to control this independent of MPI conduit
##
if IBV_BOOTSTRAP_MPI
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
## IBV AM receiver thead support (optional)
##
if IBV_RCV_THREAD
# IBV conduit needs GASNET_THREAD_DEFINES even for SEQ mode
rcv_thread_defines = @GASNET_THREAD_DEFINES@
endif

# any conduit-specific subdirectories containing Makefile.am's
SUBDIRS = contrib

# complete list of files in the conduit directory
# include all headers, documentation, etc. 
# and any subdirectories not containing Makefile.am's
CONDUIT_FILELIST =               \
      gasnet_empty_internal.h    \
      README                     \
      license.txt                

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =                 \
      $(vapi_srcdir)/gasnet_core.c        \
      $(vapi_srcdir)/gasnet_core_sndrcv.c \
      $(vapi_srcdir)/gasnet_extended.c    \
      $(vapi_srcdir)/gasnet_firehose.c    \
      $(fh_sourcelist) $(ssh_sources)

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = -I@IBV_INCLUDE@ -I$(vapi_srcdir) $(fh_extralibcflags) $(ssh_defines) $(mpi_defines)\
			 @IBV_LIBCFLAGS@ $(rcv_thread_defines) -DGASNET_CONDUIT_IBV

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = $(vapi_install_hdrs) $(fh_extraheaders)

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = $(vapi_srcdir)/*.[ch] $(fh_extradeps) $(ssh_deps) $(mpi_deps)

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
CONDUIT_RUNCMD = @TOP_BUILDDIR@/ibv-conduit/contrib/gasnetrun_ibv -np %N %P %A

# conduit-specific tests in ../tests directory
CONDUIT_TESTS =

# conduit-specific hook to install VAPI's README as our own
# The normal uninstall is sufficient to remove it.
conduit_has_install_data_hook = yes
do-install-data-conduit:
	@rm -f $(DESTDIR)$(docdir)/README-ibv
	$(INSTALL_DATA) $(srcdir)/../vapi-conduit/README $(DESTDIR)$(docdir)/README-ibv

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_ibv_seq_a_SOURCES =
libraries_seq = libgasnet-ibv-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_ibv_par_a_SOURCES =
libraries_par = libgasnet-ibv-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_ibv_parsync_a_SOURCES =
libraries_parsync = libgasnet-ibv-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-ibv-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-ibv-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-ibv-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_IBV_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(makefile_fragments)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif


