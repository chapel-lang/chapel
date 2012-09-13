# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

AM_CPPFLAGS = $(SST_INCLUDE) -I$(top_srcdir)/include
AM_CCASFLAGS = $(SST_INCLUDE) -I$(top_srcdir)/include -I$(top_builddir)/include -DHAVE_CONFIG_H
AM_LDFLAGS = $(SST_LIBS)

lib_LTLIBRARIES = libqthread.la
SUBDIRS =
noinst_HEADERS = affinity/shufflesheps.h

libqthread_la_SOURCES = \
	cacheline.c \
	envariables.c \
	qalloc.c \
	qloop.c \
	qt_feb_barrier.c \
	qutil.c \
	io.c \
	locks.c \
	feb.c \
	hazardptrs.c \
	aligned_alloc.c \
	syncvar.c

if COMPILE_LF_HASH
libqthread_la_SOURCES += qt_lf_hash.c
else
libqthread_la_SOURCES += qt_c_hash.c
endif

include qtimer/Makefile.inc
include ds/Makefile.inc
include patterns/Makefile.inc
include syscalls/Makefile.inc
include rcrtool/Makefile.inc

if COMPILE_CHAPEL_INTERFACE
include interfaces/chapel/Makefile.inc
endif

if COMPILE_ROSE_EXTENSIONS
include interfaces/rose/Makefile.inc
endif

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
libqthread_la_LDFLAGS = -version-info 15:0:0 -no-undefined

libqthread_la_LIBADD =
libqthread_la_DEPENDENCIES =

if COMPILE_TARGET_SST
libqthread_la_SOURCES += \
	sst/qthread-sst.c \
	sst/futurelib-sst.c
endif

if COMPILE_TARGET_UNIX
libqthread_la_SOURCES += \
	qthread.c \
	qt_mpool.c \
	shepherds.c \
	workers.c \
	threadqueues/@with_scheduler@_threadqueues.c \
	sincs/@with_sinc@.c \
	ds/dictionary/dictionary_@with_dict@.c \
	affinity/@qthread_topo@.c \
	futurelib.c
if COMPILE_SPAWNCACHE
libqthread_la_SOURCES += spawncache.c
endif
if QTHREAD_MULTITHREADED_SHEPHERDS
libqthread_la_SOURCES += touch.c
endif
if COMPILE_COMPAT_ATOMIC
libqthread_la_SOURCES += compat_atomics.c
endif
endif

if QTHREAD_NEED_OWN_MAKECONTEXT
include fastcontext/Makefile.inc
endif

EXTRA_DIST = \
			 threadqueues/lifo_threadqueues.c \
			 threadqueues/nemesis_threadqueues.c \
			 threadqueues/mutexfifo_threadqueues.c \
			 threadqueues/mtsfifo_threadqueues.c \
			 threadqueues/sherwood_threadqueues.c \
			 threadqueues/nottingham_threadqueues.c \
			 sincs/donecount.c \
			 sincs/donecount_cas.c \
			 sincs/original.c \
			 affinity/hwloc.c \
			 affinity/no.c \
			 affinity/libnuma.c \
			 affinity/libnumaV2.c \
			 affinity/mach.c \
			 affinity/tilera.c \
			 affinity/plpa.c \
			 affinity/lgrp.c \
			 affinity/shepcomp.h
