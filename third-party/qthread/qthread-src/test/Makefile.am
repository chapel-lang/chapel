# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

SUBDIRS = basics features stress utils
if COMPILE_MULTINODE
SUBDIRS += multinode
endif

DIST_SUBDIRS = $(SUBDIRS) multinode benchmarks

basictests:
	$(MAKE) -C basics buildtests

featuretests:
	$(MAKE) -C features buildtests

multinodetests:
	$(MAKE) -C multinode buildtests

stresstests:
	$(MAKE) -C stress buildtests

tests: basictests featuretests stresstests multinodetests

buildtests: tests

buildextra: benchmarks

benchmarks:
	$(MAKE) -C benchmarks buildextra

buildall: buildtests buildextra

.PHONY: buildall tests buildtests benchmarks buildextra basictests featuretests stresstests

noinst_HEADERS = argparsing.h

AM_CPPFLAGS = -I$(top_srcdir)/include
outputdir = $(top_builddir)/src
qthreadlib = $(outputdir)/libqthread.la

LDADD = $(qthreadlib)
