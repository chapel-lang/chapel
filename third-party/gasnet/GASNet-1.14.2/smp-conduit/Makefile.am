## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/smp-conduit/Makefile.am,v $
#     $Date: 2010/04/17 02:36:40 $
# $Revision: 1.24.2.1 $
# Description: Makefile for GASNet smp conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=smp

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
      gasnet_coll_fwd.h      \
      gasnet_extended_fwd.h  \
      gasnet_extended_coll.c

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =          \
      $(srcdir)/gasnet_core.c \
      $(srcdir)/gasnet_extended_coll.c \
      $(top_srcdir)/extended-ref/gasnet_extended.c

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = 

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = 

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = 

# additional object files to be included in libgasnet that need to be compiled 
# using a special, conduit-specific command. These should also be included as
# forced targets in this file, and should probably use LIBINCLUDES/LIBDEFINES
CONDUIT_SPECIAL_OBJS = 

# the default job spawn command to be used for "make run-tests"
# The following substitutions are performed:
#   %P = program executable name
#   %N = requested node count
#   %A = program arguments
#   %Q = program arguments w/ an extra level of quotes
#   %D = the current working directory 
#   %H = hostfile (if any)
if USE_PSHM
CONDUIT_RUNCMD = env GASNET_PSHM_NODES=%N %P %A
else
CONDUIT_RUNCMD = %P %A
endif

# conduit-specific tests in ../tests directory
CONDUIT_TESTS =

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_smp_seq_a_SOURCES =
libraries_seq = libgasnet-smp-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_smp_par_a_SOURCES =
libraries_par = libgasnet-smp-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_smp_parsync_a_SOURCES =
libraries_parsync = libgasnet-smp-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-smp-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-smp-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-smp-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_SMP_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(makefile_fragments)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif


