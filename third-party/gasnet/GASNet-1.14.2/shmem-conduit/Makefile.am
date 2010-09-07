## Process this file with automake to produce Makefile.in	-*- makefile -*-
# $Header: /var/local/cvs/gasnet/shmem-conduit/Makefile.am,v 1.9 2009/04/18 22:43:04 phargrov Exp $
# Description: Makefile for GASNet shmem conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=shmem

# any conduit-specific subdirectories containing Makefile.am's
#SUBDIRS = contrib Nothing to make in this contrib
SUBDIRS = .

# complete list of files in the conduit directory
# include all headers, documentation, etc. 
# and any subdirectories not containing Makefile.am's
CONDUIT_FILELIST =               \
      gasnet_core.c              \
      gasnet_core.h              \
      gasnet_core_fwd.h          \
      gasnet_core_help.h         \
      gasnet_core_internal.h     \
      gasnet_extended.c          \
      gasnet_extended_fwd.h	 \
      gasnet_extended_help_extra.h

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =                       \
      $(srcdir)/gasnet_core.c              \
      $(srcdir)/gasnet_extended.c

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = @SHMEM_FLAGS@

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = 

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = 

# additional object files to be included in libgasnet that need to be compiled 
# using a special, conduit-specific command. These should also be included as
# forced targets in this file, and should probably use LIBINCLUDES/LIBDEFINES
CONDUIT_SPECIAL_OBJS=

# the default job spawn command to be used for "make run-tests"
# The following substitutions are performed:
#   %P = program executable name
#   %N = requested node count
#   %A = program arguments
#   %Q = program arguments w/ an extra level of quotes
#   %D = the current working directory 
#   %H = hostfile (if any)
#
# Assume we can use mpirun for shmem apps, as this works on most platforms
#CONDUIT_RUNCMD = @MPIRUN_CMD@
CONDUIT_RUNCMD = @TOP_BUILDDIR@/mpi-conduit/contrib/gasnetrun_mpi -np %N %P %A

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_shmem_seq_a_SOURCES =
libraries_seq = libgasnet-shmem-seq.a
endif
# shmem-conduit does not support pthreads, due to shmem's lack of pthread safety
CHECK_TEST_TARGET_OVERRIDE = tests-seq
if ALWAYS_FALSECOND
if BUILD_PAR_LIBS
libgasnet_shmem_par_a_SOURCES =
libraries_par = libgasnet-shmem-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_shmem_parsync_a_SOURCES =
libraries_parsync = libgasnet-shmem-parsync.a
endif
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-shmem-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

if USE_SHMEM_CONDUIT  
lib_LIBRARIES = $(libraries) 
# only make the seq fragment:
all: $(lib_LIBRARIES) shmem-seq.mak
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif

