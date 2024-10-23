# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

EXTRA_DIST = \
			 README.md \
			 README.affinity \
			 README.performance-monitoring.md \
			 SCHEDULING \
			 TODO.md \
			 autogen.sh

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = qthread.pc

SUBDIRS = man include src test

ACLOCAL_AMFLAGS = -I config

DISTCHECK_CONFIGURE_FLAGS = $(EXTRA_DISTCHECK_OPTIONS)

CONFIGURE_DEPENDENCIES = config/version-gen NEWS

.PHONY: core buildtests tests benchmarks buildextra buildall basictests featuretests stresstests

core:
	$(MAKE) -C src

basictests: core
	$(MAKE) -C test basictests

featuretests: core
	$(MAKE) -C test featuretests

stresstests: core
	$(MAKE) -C test stresstests

buildtests: tests

tests:
	$(MAKE) -C src
	$(MAKE) -C test tests

buildextra: benchmarks

benchmarks:
	$(MAKE) -C src
	$(MAKE) -C test benchmarks

buildall:
	$(MAKE) -C src
	$(MAKE) -C test buildall

