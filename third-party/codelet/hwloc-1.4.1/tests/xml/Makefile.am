# Copyright © 2009 inria.  All rights reserved.
# Copyright © 2009-2010 Université Bordeaux 1
# Copyright © 2009-2010 Cisco Systems, Inc.  All rights reserved.
# See COPYING in top-level directory.

AM_CFLAGS = $(HWLOC_CFLAGS)
AM_CPPFLAGS = $(HWLOC_CPPFLAGS)
AM_LDFLAGS = $(HWLOC_LDFLAGS)

# Add your XML input files here.
xml_inputs = \
	16amd64-8n2c-cpusets.xml \
	16em64t-4s2c2t.xml \
	16em64t-4s2c2t-offlines.xml \
	24em64t-2n6c2t-pci.xml \
	8em64t-2mi2ma2c.xml \
	96em64t-4n4d3ca2co-pci.xml

# Only run the tests if we're building standalone, because the tests
# call hwloc executables.
if HWLOC_BUILD_TESTS
TESTS = $(xml_inputs)
endif HWLOC_BUILD_TESTS

EXTRA_DIST = $(xml_inputs)

TESTS_ENVIRONMENT = $(HWLOC_top_builddir)/tests/xml/test-topology.sh
