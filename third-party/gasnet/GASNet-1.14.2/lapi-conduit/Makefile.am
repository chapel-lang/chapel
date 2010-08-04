## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/lapi-conduit/Makefile.am,v $
#     $Date: 2009/10/10 03:46:09 $
# $Revision: 1.40 $
# Description: Makefile for GASNet lapi conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=lapi

if HAVE_LAPI_RDMA
# Firehose
fh_type=region
include $(top_builddir)/other/firehose/firehose.mak

# source file only needed when building for RDMA
extra_srcs = $(srcdir)/gasnet_firehose.c
endif

# any conduit-specific subdirectories containing Makefile.am's
SUBDIRS = .

# complete list of files in the conduit directory
# include all headers, documentation, etc. 
# and any subdirectories not containing Makefile.am's
CONDUIT_FILELIST =           \
      gasnet_core.c          \
      gasnet_core.h          \
      gasnet_core_fwd.h      \
      gasnet_core_help.h     \
      gasnet_core_internal.h \
      gasnet_extended.c      \
      gasnet_extended_fwd.h  \
      gasnet_extended_internal.h \
      gasnet_firehose.c      \
      firehose_fwd.h         \
      README-RDMA            \
      STATUS                 \
      license.txt

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =          \
      $(srcdir)/gasnet_core.c \
      $(srcdir)/gasnet_extended.c \
      $(fh_sourcelist) $(extra_srcs)

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = $(fh_extralibcflags)

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = $(fh_extraheaders)

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = $(fh_extradeps)

# additional object files to be included in libgasnet that need to be compiled 
# using a special, conduit-specific command. These should also be included as
# forced targets in this file, and should probably use LIBINCLUDES/LIBDEFINES
CONDUIT_SPECIAL_OBJS = $(fh_special_objs)

# the default job spawn command to be used for "make run-tests"
# The following substitutions are performed:
#   %P = program executable name
#   %N = requested node count
#   %A = program arguments
#   %Q = program arguments w/ an extra level of quotes (Bug1039 - poe requires overquoting)
#   %D = the current working directory 
#   %H = hostfile (if any)
CONDUIT_RUNCMD = @ENVCMD@ MP_FENCE=-- MP_TASK_AFFINITY=-1 MEMORY_AFFINITY=-1 poe %P -nodes %N -tasks_per_node 1 -rmpool 1 -msg_api LAPI -euilib us -retry 1 -retrycount 10000 @POE_RDMA_ARGS@ -- %Q

# conduit-specific tests in ../tests directory
CONDUIT_TESTS =

if HAVE_LAPI_RDMA
# conduit-specific hook to install README-RDMA
# stadard uninstall rules are sufficient
conduit_has_install_data_hook = yes
do-install-data-conduit:
	$(INSTALL_DATA) $(srcdir)/README-RDMA $(DESTDIR)$(docdir)/README-lapi_rdma
endif

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_lapi_seq_a_SOURCES =
libraries_seq = libgasnet-lapi-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_lapi_par_a_SOURCES =
libraries_par = libgasnet-lapi-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_lapi_parsync_a_SOURCES =
libraries_parsync = libgasnet-lapi-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-lapi-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-lapi-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-lapi-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_LAPI_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(makefile_fragments)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif


