# Copyright © 2009-2015 Inria.  All rights reserved.
# Copyright © 2009-2010 Université Bordeaux
# Copyright © 2009-2014 Cisco Systems, Inc.  All rights reserved.
# See COPYING in top-level directory.

AM_CFLAGS = $(HWLOC_CFLAGS)
AM_CPPFLAGS = $(HWLOC_CPPFLAGS)
AM_LDFLAGS = $(HWLOC_LDFLAGS)

# Add your output files here.
xml_outputs = \
        16amd64-8n2c-cpusets.xml \
        16em64t-4s2c2t.xml \
        16em64t-4s2c2t-offlines.xml \
        16em64t-4s2c2t.console.output \
        16-2gr2gr2n2c+misc.xml \
        16-2gr2gr2n2c+misc.console.output \
        16intel64-manyVFs.xml \
        16intel64-manyVFs.console.output \
        16intel64-manyVFs.console.nocollapse.output \
        24em64t-2n6c2t-pci.xml \
        32em64t-2n8c2t-pci-noio.xml \
        32em64t-2n8c2t-pci-normalio.xml \
        32em64t-2n8c2t-pci-wholeio.xml \
        64intel64-3g2n+2n-irregulargroups+pci.xml \
        64intel64-3g2n+2n-irregulargroups+pci.console.output \
        96em64t-4n4d3ca2co-pci.xml \
        192em64t-12gr2n8c2t-distancegroups.xml \
        192em64t-24n8c2t-nodistancegroups.xml \
        192em64t-24n8c2t-distancegroups.xml

# Each output `xyz.xml' may have a corresponding `xyz.source' file specifying
# its source file.
# Each output `xyz.output' must have a corresponding `xyz.xml' source file,
# or a `xyz.source' file specifying its source file.
xml_source = \
        16em64t-4s2c2t.console.source \
        16-2gr2gr2n2c+misc.console.source \
        16intel64-manyVFs.console.source \
        16intel64-manyVFs.console.nocollapse.source \
        32em64t-2n8c2t-pci-noio.source \
        32em64t-2n8c2t-pci-normalio.source \
        64intel64-3g2n+2n-irregulargroups+pci.console.source \
        192em64t-24n8c2t-distancegroups.source

# Each output `xyz.xml' or `xyz.output' may have a corresponding `xyz.options'
# file modifying the behavior of lstopo
xml_options = \
        16em64t-4s2c2t.console.options \
        16intel64-manyVFs.console.options \
        16intel64-manyVFs.console.nocollapse.options \
        16-2gr2gr2n2c+misc.console.options \
        32em64t-2n8c2t-pci-noio.options \
        32em64t-2n8c2t-pci-wholeio.options \
        64intel64-3g2n+2n-irregulargroups+pci.options \
        64intel64-3g2n+2n-irregulargroups+pci.console.options

# Each output `xyz.xml' or `xyz.output' may have a corresponding `xyz.env'
# modifying the environment of lstopo
xml_envs = \
        64intel64-3g2n+2n-irregulargroups+pci.console.env \
        192em64t-12gr2n8c2t-distancegroups.env \
        192em64t-24n8c2t-nodistancegroups.env \
        192em64t-24n8c2t-distancegroups.env

# Only run the tests if we're building standalone, because the tests
# call hwloc executables.
TESTS = $(xml_outputs)

EXTRA_DIST = $(xml_outputs) $(xml_source) $(xml_options) $(xml_envs)

LOG_COMPILER = $(HWLOC_top_builddir)/tests/xml/test-topology.sh
