#   $Source: /var/local/cvs/gasnet/other/testspawn/Makefile.am,v $
#     $Date: 2006/04/17 22:01:03 $
# $Revision: 1.3 $
# Description: 
# Copyright 2005, Regents of the University of California
# Terms of use are as specified in license.txt

all: 


CLEANFILES = testspawn-ssh$(EXEEXT) testspawn-mpi$(EXEEXT)
EXTRA_DIST = README license.txt $(testspawn_filelist)

testspawn_filelist = testspawn.c            \
		     gasnet_core.h          \
		     gasnet_core_fwd.h      \
		     gasnet_core_help.h     \
		     gasnet_core_internal.h \
		     gasnet_extended_fwd.h


spawner_dir = $(top_srcdir)/other/$(spawner)-spawner

testspawn_sources = $(srcdir)/testspawn.c\
		    $(top_srcdir)/gasnet_internal.c\
		    $(spawner_dir)/gasnet_bootstrap_$(spawner).c

testspawn_dependencies =               \
        $(CONFIG_HEADER)               \
        $(srcdir)/*.h                  \
        $(top_srcdir)/*.h              \
        $(top_srcdir)/extended-ref/*.h \
        $(top_srcdir)/other/*.h        \
	$(spawner_dir)/*.h	       \
	$(testspawn_sources)

testspawn_includes = -I$(srcdir) \
		     -I$(top_srcdir) \
		     -I$(top_builddir) \
		     -I$(srcdir) \
		     -I$(top_srcdir)/other \
	   	     -I$(top_srcdir)/extended-ref \
		     -I$(spawner_dir)

ssh_cflags = @CFLAGS@
ssh_cc = @CC@
ssh_ldflags =

mpi_cflags = @MPI_CFLAGS@
mpi_cc = @MPI_CC@
mpi_ldflags = @MPI_LIBS@

testspawn_cflags =                \
        @MISC_CFLAGS@             \
        -DGASNET_SEQ              \
        @GASNET_MACHINE_DEFINES@  \
        $(testspawn_includes)     \
        $(MANUAL_LIBCFLAGS)

# These two rules are 'active' when $(spawner) is empty
testspawn-ssh$(spawner)$(EXEEXT): force
	$(MAKE) $@ spawner=ssh
testspawn-mpi$(spawner)$(EXEEXT): force
	$(MAKE) $@ spawner=mpi

# This rule does the real work, when $(spawner) is non-empty
testspawn-$(spawner)$(EXEEXT): $(testspawn_dependencies)
	$($(spawner)_cc) $($(spawner)_cflags) $(testspawn_cflags) \
		-o $@ $(testspawn_sources) \
		@LIBGCC@ @LIBS@ @LIBM@ $($(spawner)_ldflags) \
		-DSPAWNER=$(spawner) $(MANUAL_LDFLAGS) $(MANUAL_LIBS)

.PHONY: force
force:
