# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

AM_CPPFLAGS = $(SST_INCLUDE) -I$(top_srcdir)/include
AM_LDFLAGS = $(SST_LIBS)

lib_LTLIBRARIES = libqthread.la

libqthread_la_SOURCES = \
	cacheline.c \
	qalloc.c \
	qloop.c \
	qt_c_hash.c \
	qt_feb_barrier.c \
	qutil.c \
	syncvar.c

include qtimer/Makefile.inc
include ds/Makefile.inc
include patterns/Makefile.inc

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
libqthread_la_LDFLAGS = -version-info 13:0:0 -no-undefined

libqthread_la_LIBADD =
libqthread_la_DEPENDENCIES =

if COMPILE_ROSE_EXTENSIONS
libqthread_la_SOURCES += qt_barrier.c \
						 rose_xomp.c \
						 rose_log_arrivaldetector.c
endif

if COMPILE_TARGET_SST
libqthread_la_SOURCES += \
	sst/qthread-sst.c \
	sst/futurelib-sst.c
endif

if COMPILE_TARGET_UNIX
libqthread_la_SOURCES += \
	qthread.c \
	threadqueues.c \
	affinity/@qthread_topo@_affinity.c \
	futurelib.c
if QTHREAD_MULTITHREADED_SHEPHERDS
libqthread_la_SOURCES += \
	touch.c
endif
endif

if COMPILE_POOLED_MEMORY
libqthread_la_SOURCES += qt_mpool.c
endif

if QTHREAD_NEED_OWN_MAKECONTEXT
include osx_compat/Makefile.inc
endif

EXTRA_DIST = \
			 affinity/hwloc_affinity.c \
			 affinity/no_affinity.c \
			 affinity/libnuma_affinity.c \
			 affinity/libnumaV2_affinity.c \
			 affinity/mach_affinity.c \
			 affinity/tile_affinity.c \
			 affinity/plpa_affinity.c \
			 affinity/lgrp_affinity.c \
			 affinity/shepcomp.h
