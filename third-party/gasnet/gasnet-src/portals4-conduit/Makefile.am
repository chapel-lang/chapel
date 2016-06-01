## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: bitbucket.org:berkeleylab/gasnet.git/portals4-conduit/Makefile.am $
# Description: Makefile for GASNet portals4 conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=portals4

## generalized PMI support
pmi_srcdir = $(top_srcdir)/other/pmi-spawner

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
      gasnet_portals4.h      \
      gasnet_portals4.c      \
      gasnet_portals4_hash.h \
      gasnet_portals4_hash.c

# list of conduit core and extended .c source files 
# to be compiled into libgasnet on the compiler command line
CONDUIT_SOURCELIST =              \
      $(srcdir)/gasnet_core.c     \
      $(srcdir)/gasnet_extended.c \
      $(pmi_srcdir)/gasnet_bootstrap_pmi.c \
      $(srcdir)/gasnet_portals4.c \
      $(srcdir)/gasnet_portals4_hash.c

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = -I@PORTALS4_INCLUDE@ @PORTALS4_LIBCFLAGS@

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = 

# headers selected by default rules that should NOT be installed
CONDUIT_PRIVATEHEADERS = gasnet_portals4.h gasnet_portals4_hash.h

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = $(pmi_srcdir)/*.[ch]

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
CONDUIT_RUNCMD = yod -np %N -ppn 1 %P %A

# conduit-specific tests in ../tests directory
CONDUIT_TESTS =

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_portals4_seq_a_SOURCES =
libraries_seq = libgasnet-portals4-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_portals4_par_a_SOURCES =
libraries_par = libgasnet-portals4-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_portals4_parsync_a_SOURCES =
libraries_parsync = libgasnet-portals4-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-portals4-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-portals4-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-portals4-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_PORTALS4_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(makefile_fragments)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif


