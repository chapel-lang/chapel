# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

AM_CPPFLAGS = -I$(top_srcdir)/include
AM_CCASFLAGS = -I$(top_srcdir)/include -I$(top_builddir)/include -DHAVE_CONFIG_H

lib_LTLIBRARIES = libqthread.la
SUBDIRS =
noinst_HEADERS = affinity/shufflesheps.h

libqthread_la_SOURCES = \
	cacheline.c \
	envariables.c \
	feb.c \
	hazardptrs.c \
	io.c \
	performance.c \
	locks.c \
	qalloc.c \
	qloop.c \
	queue.c \
	barrier/@with_barrier@.c \
	qutil.c \
	syncvar.c \
	qthread.c \
	mpool.c \
	shepherds.c \
	workers.c \
	threadqueues/@with_scheduler@_threadqueues.c \
	sincs/@with_sinc@.c \
	alloc/@with_alloc@.c \
	affinity/common.c \
	affinity/@qthread_topo@.c \
	touch.c \
	tls.c \
	teams.c

EXTRA_DIST = 

if COMPILE_LF_HASH
libqthread_la_SOURCES += lf_hashmap.c
else
libqthread_la_SOURCES += hashmap.c
endif

if COMPILE_SPAWNCACHE
libqthread_la_SOURCES += spawncache.c
endif

if COMPILE_EUREKAS
libqthread_la_SOURCES += eurekas.c
endif

if COMPILE_COMPAT_ATOMIC
libqthread_la_SOURCES += compat_atomics.c
endif

include qtimer/Makefile.inc
include ds/Makefile.inc
include patterns/Makefile.inc
include syscalls/Makefile.inc

if COMPILE_MULTINODE
libqthread_la_SOURCES += spr.c

include net/Makefile.inc
include net/portals4/Makefile.inc
endif

# version-info fields are:
# 1. the current interface revision number (i.e. whenever arguments of existing
#    functions change, increment this number)
# 2. the revision number (the library with the highest revision number will be
#    preferred)
# 3. the age (how many previous interfaces are supported)
#
# So...
# - If you add functions, increase the interface number AND the age, and reset
#   the revision number to 0.
# - If you modify (or remove) existing functions, increase the interface number
#   and reset the revision and age numbers to 0
# - If you support all the same functions (just bugfixes or whatnot), increment
#   the revision number
libqthread_la_LDFLAGS = -version-info 19:0:0 -no-undefined

libqthread_la_LIBADD =
libqthread_la_DEPENDENCIES =

if QTHREAD_NEED_OWN_MAKECONTEXT
include fastcontext/Makefile.inc
endif

EXTRA_DIST += \
			 threadqueues/distrib_threadqueues.c \
			 threadqueues/lifo_threadqueues.c \
			 threadqueues/nemesis_threadqueues.c \
			 threadqueues/mutexfifo_threadqueues.c \
			 threadqueues/mtsfifo_threadqueues.c \
			 threadqueues/sherwood_threadqueues.c \
			 threadqueues/nottingham_threadqueues.c \
			 sincs/donecount.c \
			 sincs/donecount_cas.c \
			 sincs/original.c \
			 barrier/feb.c \
			 barrier/array.c \
			 barrier/log.c \
			 barrier/sinc.c \
			 alloc/base.c \
			 alloc/chapel.c \
			 affinity/common.c \
			 affinity/hwloc.c \
			 affinity/hwloc_via_chapel.c \
			 affinity/binders.c \
			 affinity/hwloc_v2.c \
			 affinity/no.c \
			 affinity/sys.c \
			 affinity/libnuma.c \
			 affinity/libnumaV2.c \
			 affinity/mach.c \
			 affinity/tilera.c \
			 affinity/plpa.c \
			 affinity/lgrp.c \
			 affinity/shepcomp.h
