# Copyright © 2009-2022 Inria.  All rights reserved.
# Copyright © 2009-2012 Université Bordeaux
# Copyright © 2009-2014 Cisco Systems, Inc.  All rights reserved.
# See COPYING in top-level directory.

AM_CFLAGS = $(HWLOC_CFLAGS)
AM_CPPFLAGS = $(HWLOC_CPPFLAGS) -DXMLTESTDIR=\"$(abs_top_srcdir)/tests/hwloc/xml/\"
AM_LDFLAGS = $(HWLOC_LDFLAGS)

SUBDIRS = . ports xml
DIST_SUBDIRS = ports xml linux x86 x86+linux rename

if HWLOC_HAVE_LINUX
SUBDIRS += linux
endif HWLOC_HAVE_LINUX

if HWLOC_HAVE_X86_CPUID
SUBDIRS += x86
endif HWLOC_HAVE_X86_CPUID

if HWLOC_HAVE_LINUX
if HWLOC_HAVE_X86_CPUID
SUBDIRS += x86+linux
endif HWLOC_HAVE_X86_CPUID
endif HWLOC_HAVE_LINUX

LDADD =

LOG_COMPILER = $(builddir)/wrapper.sh

check_PROGRAMS = \
        hwloc_api_version \
        hwloc_list_components \
        hwloc_bitmap \
        hwloc_bitmap_string \
        hwloc_bitmap_compare_inclusion \
        hwloc_get_closest_objs \
        hwloc_get_obj_covering_cpuset \
        hwloc_get_cache_covering_cpuset \
        hwloc_get_largest_objs_inside_cpuset \
        hwloc_get_next_obj_covering_cpuset \
        hwloc_get_obj_inside_cpuset \
        hwloc_get_shared_cache_covering_obj \
        hwloc_get_obj_below_array_by_type \
        hwloc_get_obj_with_same_locality \
        hwloc_bitmap_first_last_weight \
        hwloc_bitmap_singlify \
        hwloc_type_depth \
        hwloc_type_sscanf \
        hwloc_bind \
        hwloc_get_last_cpu_location \
        hwloc_get_area_memlocation \
        hwloc_object_userdata \
        hwloc_synthetic \
        hwloc_backends \
        hwloc_pci_backend \
        hwloc_is_thissystem \
        hwloc_distances \
        hwloc_groups \
        hwloc_insert_misc \
        hwloc_topology_allow \
        hwloc_topology_restrict \
        hwloc_topology_dup \
        hwloc_topology_diff \
        hwloc_topology_abi \
        hwloc_obj_infos \
        hwloc_iodevs \
        cpuset_nodeset \
        memattrs \
        memtiers \
        cpukinds \
        xmlbuffer \
        gl

if HWLOC_HAVE_WINDOWS
check_PROGRAMS += windows_processor_groups
endif HWLOC_HAVE_WINDOWS

if !HWLOC_HAVE_WINDOWS
if !HWLOC_HAVE_DARWIN
check_PROGRAMS += shmem
endif !HWLOC_HAVE_DARWIN
endif !HWLOC_HAVE_WINDOWS

if HWLOC_HAVE_LINUX_LIBNUMA
check_PROGRAMS += linux-libnuma
endif HWLOC_HAVE_LINUX_LIBNUMA

if HWLOC_HAVE_SCHED_SETAFFINITY
check_PROGRAMS += glibc-sched
endif HWLOC_HAVE_SCHED_SETAFFINITY

if HWLOC_HAVE_LIBIBVERBS
check_PROGRAMS += openfabrics-verbs
endif HWLOC_HAVE_LIBIBVERBS

if HWLOC_HAVE_OPENCL
check_PROGRAMS += opencl
endif HWLOC_HAVE_OPENCL

if HWLOC_HAVE_CUDA
check_PROGRAMS += cuda
endif HWLOC_HAVE_CUDA

if HWLOC_HAVE_CUDART
check_PROGRAMS += cudart
endif HWLOC_HAVE_CUDART

if HWLOC_HAVE_NVML
check_PROGRAMS += nvml
endif HWLOC_HAVE_NVML

if HWLOC_HAVE_RSMI
check_PROGRAMS += rsmi
endif HWLOC_HAVE_RSMI

if HWLOC_HAVE_LEVELZERO
check_PROGRAMS += levelzero
endif HWLOC_HAVE_LEVELZERO

TESTS = $(check_PROGRAMS)

# The library has a different name depending on whether we are
# building in standalone or embedded mode.
if HWLOC_BUILD_STANDALONE
hwloc_lib = libhwloc.la
else
hwloc_lib = libhwloc_embedded.la
endif

LDADD += $(HWLOC_top_builddir)/hwloc/$(hwloc_lib)

linux_libnuma_CFLAGS = $(AM_CFLAGS) $(HWLOC_NUMA_CPPFLAGS) $(HWLOC_NUMA_CFLAGS)
linux_libnuma_LDADD = $(LDADD) $(HWLOC_NUMA_LIBS) $(HWLOC_NUMA_LDFLAGS)

openfabrics_verbs_CFLAGS = $(AM_CFLAGS) $(HWLOC_IBVERBS_CPPFLAGS) $(HWLOC_IBVERBS_CFLAGS)
openfabrics_verbs_LDADD = $(LDADD) $(HWLOC_IBVERBS_LIBS) $(HWLOC_IBVERBS_LDFLAGS)

opencl_CFLAGS = $(AM_CFLAGS) $(HWLOC_OPENCL_CPPFLAGS) $(HWLOC_OPENCL_CFLAGS)
opencl_LDADD = $(LDADD) $(HWLOC_OPENCL_LIBS) $(HWLOC_OPENCL_LDFLAGS)

cuda_CFLAGS = $(AM_CFLAGS) $(HWLOC_CUDA_CPPFLAGS) $(HWLOC_CUDA_CFLAGS)
cuda_LDADD = $(LDADD) $(HWLOC_CUDA_LIBS) $(HWLOC_CUDA_LDFLAGS)

cudart_CFLAGS = $(AM_CFLAGS) $(HWLOC_CUDART_CPPFLAGS) $(HWLOC_CUDART_CFLAGS)
cudart_LDADD = $(LDADD) $(HWLOC_CUDART_LIBS) $(HWLOC_CUDART_LDFLAGS)

nvml_CFLAGS = $(AM_CFLAGS) $(HWLOC_NVML_CPPFLAGS) $(HWLOC_NVML_CFLAGS)
nvml_LDADD = $(LDADD) $(HWLOC_NVML_LIBS) $(HWLOC_NVML_LDFLAGS)

rsmi_CFLAGS = $(AM_CFLAGS) $(HWLOC_RSMI_CPPFLAGS) $(HWLOC_RSMI_CFLAGS)
rsmi_LDADD = $(LDADD) $(HWLOC_RSMI_LIBS) $(HWLOC_RSMI_LDFLAGS)

levelzero_CFLAGS = $(AM_CFLAGS) $(HWLOC_LEVELZERO_CPPFLAGS) $(HWLOC_LEVELZERO_CFLAGS)
levelzero_LDADD = $(LDADD) $(HWLOC_LEVELZERO_LIBS) $(HWLOC_LEVELZERO_LDFLAGS)

hwloc_bind_LDADD = $(LDADD)
if HWLOC_HAVE_PTHREAD
hwloc_bind_LDADD += -lpthread
endif

# ship the embedded test code but don't actually let automake ever
# look at it because we have another configure stuff in there
EXTRA_DIST =    embedded/autogen.sh \
                embedded/configure.ac \
                embedded/Makefile.am \
                embedded/do_test.c \
                embedded/main.c \
                embedded/run-embedded-tests.sh \
                embedded/README.txt \
                embedded/config/README.txt \
                CMakeLists.txt
