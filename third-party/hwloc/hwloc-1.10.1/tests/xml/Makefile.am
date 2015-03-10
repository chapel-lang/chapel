# Copyright © 2009-2014 Inria.  All rights reserved.
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
	32em64t-2n8c2t-pci-noio.xml \
	32em64t-2n8c2t-pci-normalio.xml \
	32em64t-2n8c2t-pci-wholeio.xml \
	96em64t-4n4d3ca2co-pci.xml \
	192em64t-12gr2n8c2t-distancegroups.xml \
	192em64t-24n8c2t-nodistancegroups.xml \
	192em64t-24n8c2t-distancegroups.xml

# Each output `xyz.source' may have a corresponding `xyz.source'
# file modifying its source file
xml_source = \
	32em64t-2n8c2t-pci-noio.source \
	32em64t-2n8c2t-pci-normalio.source \
	192em64t-24n8c2t-distancegroups.source

# Each output `xyz.xml' may have a corresponding `xyz.options'
# file modifying the behavior of lstopo
xml_options = \
	32em64t-2n8c2t-pci-noio.options \
	32em64t-2n8c2t-pci-wholeio.options

# Each output `xyz.xml' may have a corresponding `xyz.env'
# modifying the environment of lstopo
xml_envs = \
	192em64t-12gr2n8c2t-distancegroups.env \
	192em64t-24n8c2t-nodistancegroups.env \
	192em64t-24n8c2t-distancegroups.env

# Only run the tests if we're building standalone, because the tests
# call hwloc executables.
TESTS = $(xml_inputs)

EXTRA_DIST = $(xml_inputs) $(xml_source) $(xml_options) $(xml_envs)

LOG_COMPILER = $(HWLOC_top_builddir)/tests/xml/test-topology.sh
