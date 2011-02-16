## Process this file with automake to produce Makefile.in	-*- makefile -*-
#   $Source: /var/local/cvs/gasnet/other/contrib/Makefile.am,v $
#     $Date: 2009/03/06 21:13:38 $
# $Revision: 1.22 $
# Description: Makefile for GASNet other/contrib
# Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
# Terms of use are as specified in license.txt

AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = .

EXTRA_DIST = $(dotpl_DATA) $(vg_DATA) \
	gasnet_ibrun \
 	cross-configure-bgp \
	cross-configure-crayxt-catamount \
	cross-configure-crayxt-linux \
	crayxt-nptl-pthread-patch
# exclude these from distribution for now
#	cross-configure-bgl \
#	cross-configure-phoenix \
#	cross-configure-k42ppc64 \
#	cross-configure-linux-arm \
#	cross-configure-osxi686 \
#	cross-configure-osxppc64 \
#	cross-configure-osxx86_64 \
#	cross-configure-uclinux-microblaze

dotpldir = $(bindir)
dotpl_DATA = gasnet_trace.pl

vgdir = $(libdir)/valgrind
vg_DATA = gasnet.supp

bin_SCRIPTS = gasnet_trace

all-local: $(bin_SCRIPTS) $(dotpl_DATA)

$(top_builddir)/other/perlstart: $(top_srcdir)/other/perlstart.in
	@cd $(top_builddir)/other && $(MAKE) perlstart

gasnet_trace: $(top_builddir)/other/perlstart gasnet_trace.pl

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
	
