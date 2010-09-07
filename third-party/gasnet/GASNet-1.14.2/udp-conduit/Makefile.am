## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/udp-conduit/Makefile.am,v $
#     $Date: 2009/04/18 21:53:37 $
# $Revision: 1.23 $
# Description: Makefile for GASNet udp conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=udp

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
      gasnet_core_internal.h 

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiled command line
CONDUIT_SOURCELIST =          \
      $(srcdir)/gasnet_core.c \
      $(top_srcdir)/extended-ref/gasnet_extended.c 

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = -I$(top_srcdir)/other/amudp

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = $(top_srcdir)/other/amudp/*.h 

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = $(top_builddir)/other/amudp/libamudp.a

$(top_builddir)/other/amudp/libamudp.a: force
	cd $(top_builddir)/other/amudp && $(MAKE) libamudp.a

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
CONDUIT_RUNCMD = %P %N %A
#CONDUIT_RUNCMD = $(top_builddir)/other/amudp/amudprun -np %N -spawn $${GASNET_SPAWNFN:-L} %P %A

# conduit-specific tests in ../tests directory
CONDUIT_TESTS = testcxx testtoolscxx

# disable MPI tests for udp-*, because we probably don't have an MPI-capable C++ linker
CONDUIT_TEST_MAKEARGS = MPI_TESTS="" MPI_TESTS_SEQ="" MPI_TESTS_PAR=""

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_udp_seq_a_SOURCES =
libraries_seq = libgasnet-udp-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_udp_par_a_SOURCES =
libraries_par = libgasnet-udp-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_udp_parsync_a_SOURCES =
libraries_parsync = libgasnet-udp-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-udp-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-udp-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-udp-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_UDP_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(makefile_fragments)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif


