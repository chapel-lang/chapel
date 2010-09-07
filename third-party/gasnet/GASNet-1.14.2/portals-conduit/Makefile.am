## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/portals-conduit/Makefile.am,v $
#     $Date: 2009/08/07 20:20:01 $
# $Revision: 1.17 $
# Description: Makefile for GASNet portals conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=portals

# Firehose
if OS_CATAMOUNT
# no firehose for Catamount
else
fh_type=region
include $(top_builddir)/other/firehose/firehose.mak
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
      gasnet_extended_internal.h \
      gasnet_extended_fwd.h  \
      firehose_fwd.h         \
      gasnet_portals.h       \
      gasnet_portals.c

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =          \
      $(srcdir)/gasnet_core.c \
      $(srcdir)/gasnet_extended.c \
      $(srcdir)/gasnet_portals.c \
      $(fh_sourcelist)

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = @PORTALS_INCLUDES@ $(fh_extralibcflags)

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = $(fh_extraheaders)

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = $(fh_extradeps)

# additional object files to be included in libgasnet that need to be compiled 
# using a special, conduit-specific command. These should also be included as
# forced targets in this file, and should probably use LIBINCLUDES/LIBDEFINES
CONDUIT_SPECIAL_OBJS =  $(fh_special_objs)

# the default job spawn command to be used for "make run-tests"
# The following substitutions are performed:
#   %P = program executable name
#   %N = requested node count
#   %A = program arguments
#   %Q = program arguments w/ an extra level of quotes
#   %D = the current working directory 
#   %H = hostfile (if any)
# This will work for both CNL and Catamount
CONDUIT_RUNCMD = @TOP_BUILDDIR@/mpi-conduit/contrib/gasnetrun_mpi -np %N -c 0 %P %A
# Aprun for CNL:
#CONDUIT_RUNCMD = aprun -n %N -N 1 -cc none %P %A
# YOD for Catamount:
#CONDUIT_RUNCMD = yod -np %N %P %A

# conduit-specific tests in ../tests directory
CONDUIT_TESTS =

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_portals_seq_a_SOURCES =
libraries_seq = libgasnet-portals-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_portals_par_a_SOURCES =
libraries_par = libgasnet-portals-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_portals_parsync_a_SOURCES =
libraries_parsync = libgasnet-portals-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-portals-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-portals-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-portals-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_PORTALS_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(makefile_fragments)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif


