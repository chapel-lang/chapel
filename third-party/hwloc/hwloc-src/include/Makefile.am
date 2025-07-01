# Copyright © 2009-2021 Inria.  All rights reserved.
# Copyright © 2009-2010 Université Bordeaux
# Copyright © 2009-2014 Cisco Systems, Inc.  All rights reserved.
# Copyright © 2011      Oracle and/or its affiliates.  All rights reserved.
# See COPYING in top-level directory.

# Only install the headers if we're in standalone mode (meaning:
# *don't* install the headers if we're in embedded mode).

if HWLOC_BUILD_STANDALONE
include_HEADERS = hwloc.h

include_hwlocdir = $(includedir)/hwloc
include_hwloc_HEADERS = \
        hwloc/bitmap.h \
        hwloc/helper.h \
        hwloc/inlines.h \
        hwloc/memattrs.h \
        hwloc/cpukinds.h \
        hwloc/diff.h \
        hwloc/shmem.h \
        hwloc/distances.h \
        hwloc/export.h \
        hwloc/openfabrics-verbs.h \
        hwloc/opencl.h \
        hwloc/cuda.h \
        hwloc/cudart.h \
        hwloc/nvml.h \
        hwloc/rsmi.h \
        hwloc/levelzero.h \
        hwloc/plugins.h \
        hwloc/gl.h \
        hwloc/rename.h \
        hwloc/deprecated.h
include_hwloc_autogendir = $(includedir)/hwloc/autogen
nodist_include_hwloc_autogen_HEADERS = hwloc/autogen/config.h

noinst_HEADERS = \
        private/private.h \
        private/debug.h \
        private/misc.h \
        private/xml.h \
        private/components.h \
        private/internal-components.h \
        private/cpuid-x86.h \
        private/windows.h

if HWLOC_HAVE_LINUX
include_hwloc_HEADERS += \
        hwloc/linux.h \
        hwloc/linux-libnuma.h
endif HWLOC_HAVE_LINUX

if HWLOC_HAVE_SOLARIS
include_hwloc_HEADERS += \
       private/solaris-chiptype.h
endif HWLOC_HAVE_SOLARIS

if HWLOC_HAVE_SCHED_SETAFFINITY
include_hwloc_HEADERS += hwloc/glibc-sched.h
endif HWLOC_HAVE_SCHED_SETAFFINITY

if HWLOC_HAVE_WINDOWS
include_hwloc_HEADERS += \
	hwloc/windows.h
endif HWLOC_HAVE_WINDOWS

endif HWLOC_BUILD_STANDALONE
