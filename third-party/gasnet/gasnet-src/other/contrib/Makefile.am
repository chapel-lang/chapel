## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: bitbucket.org:berkeleylab/gasnet.git/other/contrib/Makefile.am $
# Description: Makefile for GASNet other/contrib
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = .

EXTRA_DIST = $(dotpl_DATA) $(vg_DATA) \
	gasnet_handler.h \
	gasnetrun_mic \
	gasnet_ibrun \
	cross-configure-cray-aries-alps \
	cross-configure-cray-aries-slurm \
	cross-configure-intel-knc \
	cross-configure-intel-knl
# exclude from distribution for now
#	cross-configure-linux-arm \
#

dotpldir = $(bindir)
dotpl_DATA = gasnet_trace.pl

vgdir = $(libdir)/valgrind
vg_DATA = gasnet.supp

bin_SCRIPTS = gasnet_trace

all-local: $(bin_SCRIPTS) $(dotpl_DATA)

$(top_builddir)/other/perlstart: $(top_srcdir)/other/perlstart.in
	@cd $(top_builddir)/other && $(MAKE) perlstart

gasnet_trace: $(top_builddir)/other/perlstart

# Files to copy from srcdir to builddir if they differ
if BUILD_IS_SRC
files_we_copy = 
else
files_we_copy = $(dotpl_DATA)
$(files_we_copy): force
	@srcfile="$(srcdir)/`basename $@`" ; \
         if test ! -f "$@" -o "`find $$srcfile -newer '$@' 2>&1`" ; then \
          echo cp -f "$$srcfile" . ;         \
          cp -f "$$srcfile" . ;              \
         fi
endif
DISTCLEANFILES = $(files_we_copy)

.PHONY: force

force:
	
