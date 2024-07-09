# Copyright © 2009-2022 Inria.  All rights reserved.
# Copyright © 2009-2011 Université Bordeaux
# Copyright © 2009-2010 Cisco Systems, Inc.  All rights reserved.
# See COPYING in top-level directory.

AM_CFLAGS = $(HWLOC_CFLAGS)
AM_CPPFLAGS = $(HWLOC_CPPFLAGS)
AM_LDFLAGS = $(HWLOC_LDFLAGS)

# Add your expected output file here.
# By default, it shows the output of `lstopo - -v'.
sysfs_outputs = \
		2amd64-2n.output \
		2i386-2t-hugepagesizecount.xml.output \
		2i386-2c-nohugepage.xml.output \
		2ps3-2t.output \
		4em64t-2c2t-ignore-reorder.output \
		4ia64-4s.output \
		4fake-4gr1nu1pu.output \
		256ppc-8n8s4t.output \
		32amd64-4s2n4c-cgroup2.output \
		32amd64-4s2n4c-cgroup2.xml.output \
		8amd64-4n2c.output  \
		8em64t-4c2t.output \
		8em64t-2s2ca2c.output \
		8em64t-2s2ca2c-buggynuma.output \
		8em64t-2s4c-heterogeneous.output \
		8ia64-2n2s2c.output \
		8ia64-2s2c2t.output \
		8ia64-4s2c.output \
		16amd64-4n4c-cgroup-distance-merge.output \
		16amd64-8n2c.output \
		16amd64-8n2c-cpusets.output \
		16amd64-8n2c-cpusets.xml.output \
		16amd64-8n2c-cpusets_noadmin.output \
		16em64t-4s2ca2c-cpusetreorder.output \
		16em64t-4s2ca2c-cpusetreorder-merge.output \
		16em64t-4s2c2t.output \
		16em64t-4s2c2t-offlines.output \
		16em64t-4s2c2t-offlines.xml.output \
		16em64t-4s2c2t_ncaches.output \
		16em64t-4s2c2t_merge.output \
		16em64t-4s2c2t.xml.output \
		16ia64-8n2s.output \
		32em64t-2n8c+1mic.output \
		32intel64-2p8co2t+8ve.output \
		40intel64-2g2n4c+pci.output \
		40intel64-4n10c+pci-conflicts.output \
		48amd64-4d2n6c-sparse.output \
		64amd64-4s2n4ca2co.output \
		64fake-4n2s2ca2c2t.output \
		64intel64-fakeKNL-A2A-cache.output \
		64intel64-fakeKNL-SNC4-hybrid.output \
		64intel64-fakeKNL-SNC4-hybrid-msc.output \
		96em64t-4n4d3ca2co.output \
		128ia64-17n4s2c.output \
		128arm-2pa2n8cluster4co.output \
		256ia64-64n2s2c.output \
		2s390-2c.output \
		20s390-2g6s4c.output \
		2arm-2c.output \
		nvidiagpunumanodes.output \
		nvidiagpunumanodes.kept.output \
		fakememinitiators-1np2c+1npp+gi.output \
		fakecpuid1f-64intel64-2p4d2n2c2t.output \
		fakeheterocpunuma.output \
		fakeheteromemtiers.output \
		offline-cpu0-node0.output

if HWLOC_HAVE_32BITS_PCI_DOMAIN
sysfs_outputs += 2pa-pcidomain32bits.output
EXTRA_DIST = 2pa-pcidomain32bits-disabled.output
else
sysfs_outputs += 2pa-pcidomain32bits-disabled.output
EXTRA_DIST = 2pa-pcidomain32bits.output
endif

# Each output `xyz.output' must have a corresponding tarball `xyz.tar.bz2'
# or a corresponding `xyz.source' specifying which tarball to use
sysfs_tarballs = \
		2amd64-2n.tar.bz2 \
		2i386-2t-hugepagesizecount.tar.bz2 \
		2i386-2t-hugepagesizecount.xml.source \
		2i386-2c-nohugepage.tar.bz2 \
		2i386-2c-nohugepage.xml.source \
		2ps3-2t.tar.bz2 \
		4em64t-2c2t-ignore-reorder.tar.bz2 \
		4ia64-4s.tar.bz2 \
		4fake-4gr1nu1pu.tar.bz2 \
		256ppc-8n8s4t.tar.bz2 \
		32amd64-4s2n4c-cgroup2.tar.bz2 \
		32amd64-4s2n4c-cgroup2.xml.source \
		8amd64-4n2c.tar.bz2  \
		8em64t-4c2t.tar.bz2 \
		8em64t-2s2ca2c.tar.bz2 \
		8em64t-2s2ca2c-buggynuma.tar.bz2 \
		8em64t-2s4c-heterogeneous.tar.bz2 \
		8ia64-2n2s2c.tar.bz2 \
		8ia64-2s2c2t.tar.bz2 \
		8ia64-4s2c.tar.bz2 \
		16amd64-4n4c-cgroup-distance-merge.tar.bz2 \
		16amd64-8n2c.tar.bz2 \
		16amd64-8n2c-cpusets.tar.bz2 \
		16amd64-8n2c-cpusets.xml.source \
		16amd64-8n2c-cpusets_noadmin.source \
		16em64t-4s2ca2c-cpusetreorder.tar.bz2 \
		16em64t-4s2ca2c-cpusetreorder-merge.source \
		16em64t-4s2c2t.tar.bz2 \
		16em64t-4s2c2t-offlines.tar.bz2 \
		16em64t-4s2c2t-offlines.xml.source \
		16em64t-4s2c2t_merge.source \
		16em64t-4s2c2t_ncaches.source \
		16em64t-4s2c2t.xml.source \
		16ia64-8n2s.tar.bz2 \
		32em64t-2n8c+1mic.tar.bz2 \
		32intel64-2p8co2t+8ve.tar.bz2 \
		40intel64-2g2n4c+pci.tar.bz2 \
		40intel64-4n10c+pci-conflicts.tar.bz2 \
		48amd64-4d2n6c-sparse.tar.bz2 \
		64amd64-4s2n4ca2co.tar.bz2 \
		64fake-4n2s2ca2c2t.tar.bz2 \
		64intel64-fakeKNL-A2A-cache.tar.bz2 \
		64intel64-fakeKNL-SNC4-hybrid.tar.bz2 \
		64intel64-fakeKNL-SNC4-hybrid-msc.source \
		96em64t-4n4d3ca2co.tar.bz2 \
		128ia64-17n4s2c.tar.bz2 \
		128arm-2pa2n8cluster4co.tar.bz2 \
		256ia64-64n2s2c.tar.bz2 \
		2s390-2c.tar.bz2 \
		20s390-2g6s4c.tar.bz2 \
		2arm-2c.tar.bz2 \
		2pa-pcidomain32bits.tar.bz2 \
		2pa-pcidomain32bits-disabled.source \
		nvidiagpunumanodes.tar.bz2 \
		nvidiagpunumanodes.kept.source \
		fakememinitiators-1np2c+1npp+gi.tar.bz2 \
		fakecpuid1f-64intel64-2p4d2n2c2t.tar.bz2 \
		fakeheterocpunuma.tar.bz2 \
		fakeheteromemtiers.tar.bz2 \
		offline-cpu0-node0.tar.bz2

# Each output `xyz.output' may have a corresponding exclude `xyz.exclude'
sysfs_excludes = 

# Each output `xyz.output' may have a corresponding `xyz.options'
# file modifying the behavior of lstopo
sysfs_options = \
		2i386-2t-hugepagesizecount.xml.options \
		2i386-2c-nohugepage.xml.options \
		4em64t-2c2t-ignore-reorder.options \
		16amd64-4n4c-cgroup-distance-merge.options \
		16amd64-8n2c-cpusets.xml.options \
		16amd64-8n2c-cpusets_noadmin.options \
		16em64t-4s2ca2c-cpusetreorder-merge.options \
		16em64t-4s2c2t-offlines.xml.options \
		16em64t-4s2c2t_ncaches.options \
		16em64t-4s2c2t_merge.options \
		16em64t-4s2c2t.xml.options \
		32amd64-4s2n4c-cgroup2.xml.options \
		32em64t-2n8c+1mic.options \
		40intel64-2g2n4c+pci.options \
		fakeheterocpunuma.options

# Each output `xyz.output' may have a corresponding `xyz.env'
# modifying the environment of lstopo
sysfs_envs = \
		40intel64-2g2n4c+pci.env \
		40intel64-4n10c+pci-conflicts.env \
		64intel64-fakeKNL-SNC4-hybrid-msc.env \
		nvidiagpunumanodes.kept.env \
		fakeheterocpunuma.env

if HWLOC_HAVE_OPENAT
if HWLOC_HAVE_BUNZIPP
TESTS = $(sysfs_outputs)
endif HWLOC_HAVE_BUNZIPP
endif HWLOC_HAVE_OPENAT

EXTRA_DIST += $(sysfs_outputs) $(sysfs_tarballs) $(sysfs_excludes) $(sysfs_options) $(sysfs_envs)

LOG_COMPILER = $(builddir)/test-topology.sh

SUBDIRS = . allowed gather
