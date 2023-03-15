# Copyright © 2009-2020 Inria.  All rights reserved.
#
# See COPYING in top-level directory.

# This makefile is only reached when building in standalone mode

AM_CFLAGS = $(HWLOC_CFLAGS)
AM_CPPFLAGS = $(HWLOC_CPPFLAGS)
AM_LDFLAGS = $(HWLOC_LDFLAGS)

LDADD = $(HWLOC_top_builddir)/hwloc/libhwloc.la

check_PROGRAMS = hwloc-tweak-osindex

dist_doc_DATA = dynamic_SVG_example.html

EXTRA_DIST = hwloc-export-sysfs
