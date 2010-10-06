## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/gm-conduit/Makefile.am,v $
#     $Date: 2009/04/18 21:53:19 $
# $Revision: 1.60 $
# Description: Makefile for GASNet gm conduit
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

CONDUIT_NAME=gm

# Firehose
fh_type=page
include $(top_builddir)/other/firehose/firehose.mak

# any conduit-specific subdirectories containing Makefile.am's
SUBDIRS = contrib

# complete list of files in the conduit directory
# include all headers, documentation, etc. 
# and any subdirectories not containing Makefile.am's
CONDUIT_FILELIST =               \
      gasnet_core.c              \
      gasnet_core.h              \
      gasnet_core_conf.c         \
      gasnet_core_fwd.h          \
      gasnet_core_help.h         \
      gasnet_core_internal.h     \
      gasnet_core_receive.c      \
      gasnet_extended.c          \
      gasnet_extended_firehose.c \
      gasnet_extended_fwd.h      \
      gasnet_extended_internal.h \
      gasnet_extended_op.c       \
      gasnet_extended_ref.c      \
      firehose_fwd.h		 \
      license.txt                

CONDUIT_SOURCELIST =                       \
      $(srcdir)/gasnet_core.c              \
      $(srcdir)/gasnet_core_receive.c      \
      $(srcdir)/gasnet_core_conf.c         \
      $(srcdir)/gasnet_extended.c          \
      $(srcdir)/gasnet_extended_op.c       \
      $(srcdir)/gasnet_extended_ref.c      \
      $(srcdir)/gasnet_extended_firehose.c \
      $(fh_sourcelist)

if GM_MPI_COMPAT
# If using MPI for bootstrap then we must compile this object with MPI_CC
mpi_srcdir = $(top_srcdir)/other/mpi-spawner
mpi_cflags = -I$(mpi_srcdir)
mpi_srcs = $(mpi_srcdir)/gasnet_bootstrap_mpi.c
mpi_deps = $(mpi_srcdir)/*.h $(mpi_srcdir)/*.c
mpi_objs = $(builddir)/gasnet_bootstrap_mpi-$(THREAD_MODEL).o
$(builddir)/gasnet_bootstrap_mpi-$(THREAD_MODEL).o: force
	@MPI_CC@ @MPI_CFLAGS@ $(LIBDEFINES) $(CONDUIT_EXTRALIBCFLAGS) $(LIBINCLUDES) -o $@ -c $(mpi_srcs)
mpi_subdirs = contrib
endif

if GM_RODATA_WORKAROUND
# bug 1036: ensure .rodata section in executable is actually writeable, because GM
# registration fails on read-only virtual pages
# bug 1044: this is now off by default, because the Linux linker is buggy and 
# intermittently barfs with fatal error "Not enough room for program headers" 
# when it mispredicts the section requirements of the program (instead of restarting 
# with a larger value, which would be a reasonable approach)
rodata_objs = $(builddir)/gasnet_rodata-$(THREAD_MODEL).o
rodata_flags = -DGASNETI_GM_RODATA_WORKAROUND=1
$(builddir)/gasnet_rodata-$(THREAD_MODEL).o: force
	echo 'const int gasneti_dummy_rodata_writable = 1;' > $@.tmp.c 
	$(CC) -c -o $@.tmp.o $@.tmp.c
	objcopy --set-section-flags .rodata=contents,alloc,load,data $@.tmp.o $@
	rm -f $@.tmp.*
endif

# additional -I or -D directives needed by this specific conduit
# other than the standard GASNet includes and flags
CONDUIT_EXTRALIBCFLAGS = $(fh_extralibcflags) $(mpi_cflags) $(rodata_flags) -I@GM_INCLUDE@ 
			 

# additional conduit header files to install from external, non-standard directories
CONDUIT_EXTRAHEADERS = $(fh_extraheaders)

# additional file dependencies not mentioned elsewhere 
# that should force libgasnet rebuild on update
CONDUIT_EXTRADEPS = $(fh_extradeps) $(mpi_deps)

# additional object files to be included in libgasnet that need to be compiled 
# using a special, conduit-specific command. These should also be included as
# forced targets in this file, and should probably use LIBINCLUDES/LIBDEFINES
CONDUIT_SPECIAL_OBJS = $(fh_special_objs) $(mpi_objs) $(rodata_objs) 

# the default job spawn command to be used for "make run-tests"
# The following substitutions are performed:
#   %P = program executable name
#   %N = requested node count
#   %A = program arguments
#   %Q = program arguments w/ an extra level of quotes
#   %D = the current working directory 
#   %H = hostfile (if any)
CONDUIT_RUNCMD = @TOP_BUILDDIR@/gm-conduit/contrib/gasnetrun_gm -np %N %P %A

# conduit-specific tests in ../tests directory
CONDUIT_TESTS =

# -------- Do not modify anything below this line --------

if BUILD_SEQ_LIBS
libgasnet_gm_seq_a_SOURCES =
libraries_seq = libgasnet-gm-seq.a
endif
if BUILD_PAR_LIBS
libgasnet_gm_par_a_SOURCES =
libraries_par = libgasnet-gm-par.a
endif
if BUILD_PARSYNC_LIBS
libgasnet_gm_parsync_a_SOURCES =
libraries_parsync = libgasnet-gm-parsync.a
endif
libraries = $(libraries_seq) $(libraries_par) $(libraries_parsync)

include $(top_builddir)/other/Makefile-conduit.mak

libgasnet-gm-seq.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-seq

libgasnet-gm-par.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-par

libgasnet-gm-parsync.a: $(libgasnet_dependencies)
	@$(MAKE) do-libgasnet-parsync

if USE_GM_CONDUIT  
lib_LIBRARIES = $(libraries) 
all-local: $(lib_LIBRARIES) $(makefile_fragments)
clean-local: do-clean-local
install-data-local: do-install-data-local
uninstall-local: do-uninstall-local
else
$(top_srcdir)/gasnet.h: do-error
endif


