## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source$
#     $Date$
# $Revision$
# Description: Makefile for GASNet portals conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=portals

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
      gasnet_portals.h       \
      gasnet_portals.c

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =          \
      $(srcdir)/gasnet_core.c \
      $(srcdir)/gasnet_portals.c \
      $(srcdir)/gasnet_extended.c

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = -I$(top_srcdir)/other/ammpi -I@PORTALS_INCLUDE@ -I@CATAMOUNT_INCLUDE@

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = $(top_srcdir)/other/ammpi/*.h 

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS =             \
      $(top_builddir)/other/ammpi/libammpi.a

$(top_builddir)/other/ammpi/libammpi.a: force
	cd $(top_builddir)/other/ammpi && $(MAKE) libammpi.a

# additional object files to be included in libgasnet that need to be compiled 
# using a special, conduit-specific command. These should also be included as
# forced targets in this file, and should probably use LIBINCLUDES/LIBDEFINES
CONDUIT_SPECIAL_OBJS = 

# the default job spawn command to be used for "make run-tests"
# The following substitutions are performed:
#   %P = program executable name
#   %N = requested node count
#   %A = program arguments
#CONDUIT_RUNCMD = mpirun -np %N %P %A
#CONDUIT_RUNCMD = @TOP_BUILDDIR@/mpi-conduit/contrib/gasnetrun_mpi -np %N %P %A
CONDUIT_RUNCMD = yod -np %N %P %A

# conduit-specific tests in ../tests directory
CONDUIT_TESTS =

# -------- Do not modify anything below this line --------

if HAVE_PTHREAD
libgasnet_portals_par_a_SOURCES =
libgasnet_portals_parsync_a_SOURCES =
threaded_libraries = libgasnet-portals-par.a libgasnet-portals-parsync.a
endif
libgasnet_portals_seq_a_SOURCES =
libraries = libgasnet-portals-seq.a $(threaded_libraries)

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


