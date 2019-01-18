## Process this file with automake to produce Makefile.in	-*- makefile -*-
# Description: Makefile for GASNet other/blcr
# Copyright 2016, Regents of the University of California
# Terms of use are as specified in license.txt

# No objects are built here, but we need a proper Makefile.am
# to handle the built blck.mak and installation of docs.

all: blcr.mak

AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = .

if NEED_DOCDIR
docdir = $(datadir)/doc/GASNet
endif
if BLCR_ENABLED
docs = README-blcr
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	for f in $(docs); do \
          $(INSTALL_DATA) $(srcdir)/$$f $(DESTDIR)$(docdir)/ || exit $$? ; \
        done
endif
EXTRA_DIST =              \
	gasnet_blcr.h     \
	gasnet_blcr.c     \
	README-blcr
