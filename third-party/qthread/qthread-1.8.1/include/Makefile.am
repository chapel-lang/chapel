# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

SUBDIRS = qthread

noinst_HEADERS = \
    56reader-rwlock.h \
	fastcontext/taskimpl.h \
	fastcontext/power-ucontext.h \
	fastcontext/386-ucontext.h \
	fastcontext/tile-ucontext.h \
	net/net.h \
	qthread_innards.h \
	qloop_innards.h \
	qthread_asserts.h \
	qthread_expect.h \
	qthread_prefetch.h \
	qt_addrstat.h \
	qt_affinity.h \
	qt_aligned_alloc.h \
	qt_arrive_first.h \
	qt_atomics.h \
	qt_barrier.h \
	qt_blocking_structs.h \
	qt_context.h \
	qt_debug.h \
	qt_envariables.h \
	qt_gcd.h \
	qt_hash.h \
	qt_hazardptrs.h \
	qt_io.h \
	qt_internal_feb.h \
	qt_internal_syncvar.h \
	qt_locks.h \
	qt_macros.h \
	qt_mpool.h \
	qt_profiling.h \
	qt_qthread_struct.h \
	qt_qthread_t.h \
	qt_shepherd_innards.h \
	qt_spawncache.h \
	qt_threadqueues.h \
	qt_threadstate.h \
	qt_touch.h \
	qt_visibility.h \
	rose_xomp.h \
	rose_log_arrivaldetector.h \
	spr_innards.h \
	futurelib_innards.h

if COMPILE_MULTINODE
noinst_HEADERS += qt_multinode_innards.h
endif
