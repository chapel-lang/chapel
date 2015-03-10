## Process this file with automake to produce Makefile.in	-*- makefile -*-
# Description: Makefile for GASNet other/fca
# Copyright (c)  2012, Mellanox Technologies LTD. All rights reserved.
# Terms of use are as specified in license.txt

# No objects are built here, but we need a proper Makefile.am
# to ensure the built fca.mak is handled correctly.
all: fca.mak

AUTOMAKE_OPTIONS = foreign 1.4

SUBDIRS = .

docdir = $(datadir)/doc/gasnet
if FCA_ENABLED
docs = README-fca.txt
install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(docdir)
	for f in $(docs); do \
          $(INSTALL_DATA) $(srcdir)/$$f $(DESTDIR)$(docdir)/ || exit $$? ; \
        done
endif

EXTRA_DIST =        		\
	gasnet_fca_component.c	\
	gasnet_fca_ops.c	\
	gasnet_fca.h 		\
	gasnet_fca_api.h 	\
	README-fca.txt
