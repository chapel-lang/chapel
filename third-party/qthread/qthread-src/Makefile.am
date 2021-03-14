# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

# The config files here are included because, if I build the distribution
# tarball with AC 2.60 or better, they aren't included by acinclude.m4; but I
# want them distributed in case someone with AC 2.59 wants to run autoreconf
EXTRA_DIST = \
			 config/ac_prog_cc_c99.m4 \
			 config/ac_prog_sed.m4 \
			 config/ac_use_system_extensions.m4 \
			 README.md \
			 README.affinity \
			 README.multinode \
			 README.performance-monitoring.md \
			 SCHEDULING \
			 TODO.md \
			 autogen.sh

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

multinodetests: core
	$(MAKE) -C test multinodetests

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

