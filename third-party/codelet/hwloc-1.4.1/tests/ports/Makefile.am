# Copyright © 2009-2010 inria.  All rights reserved.
# Copyright © 2009, 2011-2012 Université Bordeaux 1
# Copyright © 2009-2010 Cisco Systems, Inc.  All rights reserved.
# See COPYING in top-level directory.

AM_CFLAGS = $(HWLOC_CFLAGS)
AM_CPPFLAGS = $(HWLOC_CPPFLAGS)
AM_LDFLAGS = $(HWLOC_LDFLAGS)

SRC = $(HWLOC_top_srcdir)/src

if HWLOC_BUILD_TESTS
if HWLOC_HAVE_LINUX
check_LTLIBRARIES = libhwloc-ports.la
endif HWLOC_HAVE_LINUX
endif HWLOC_BUILD_TESTS

# Note that AC_CONFIG_LINKS sets up the sym links for the files in
# this directory (back to the $top_srcdir/src directory).  So if you
# need more sym-linked files in here, go edit configure.ac.  Note that
# we have to use sym links in here rather than just directly
# referencing the files via $HWLOC_top_srcdir/src/foo.c because of
# dependencies issues when using the Automake option "subdir-objects".
# We nodist these because they're created by configure.
nodist_libhwloc_ports_la_SOURCES = \
        topology.c \
        traversal.c \
        topology-synthetic.c \
        topology-solaris.c \
        topology-solaris-chiptype.c \
        topology-aix.c \
        topology-osf.c \
        topology-windows.c \
        topology-darwin.c \
        topology-freebsd.c \
        topology-hpux.c

libhwloc_ports_la_SOURCES = \
        include/windows.h \
        include/kstat.h \
        include/picl.h \
        include/sys/rset.h \
        include/sys/lgrp_user.h \
        include/sys/sysctl.h \
        include/sys/procset.h \
        include/sys/processor.h \
        include/sys/thread.h \
        include/sys/mpctl.h \
        include/sys/cpuset.h \
        include/sys/systemcfg.h \
        include/sys/systeminfo.h \
        include/radset.h \
        include/cpuset.h \
        include/numa.h \
        include/pthread_np.h

libhwloc_ports_la_CPPFLAGS = \
        $(HWLOC_CPPFLAGS) \
        -I$(HWLOC_top_srcdir)/include \
        -I$(HWLOC_top_srcdir)/tests/ports/include \
        -DHWLOC_COMPILE_PORTS \
        -DHWLOC_SOLARIS_SYS -DHAVE_LIBLGRP -DHAVE_LIBKSTAT -DHAVE_LGRP_LATENCY_COOKIE \
        -DHWLOC_AIX_SYS \
        -DHWLOC_OSF_SYS \
        -DHWLOC_WIN_SYS \
        -DHWLOC_DARWIN_SYS \
        -DHWLOC_FREEBSD_SYS -DHAVE_SYS_CPUSET_H \
        -DHWLOC_HPUX_SYS \
        -DHWLOC_INSIDE_LIBHWLOC

