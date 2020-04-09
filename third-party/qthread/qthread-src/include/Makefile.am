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
	qt_asserts.h \
	qt_expect.h \
	qt_prefetch.h \
	qt_addrstat.h \
	qt_affinity.h \
	qt_alloc.h \
	qt_arrive_first.h \
	qt_atomics.h \
	qt_barrier.h \
	qt_blocking_structs.h \
	qt_context.h \
	qt_debug.h \
	qt_envariables.h \
	qt_filters.h \
	qt_gcd.h \
	qt_hash.h \
	qt_hazardptrs.h \
	qt_initialized.h \
	qt_int_ceil.h \
	qt_int_log.h \
	qt_io.h \
	qt_feb.h \
	qt_syncvar.h \
	qt_macros.h \
	qt_mpool.h \
	qt_output_macros.h \
	qt_profiling.h \
	qt_qthread_mgmt.h \
	qt_qthread_struct.h \
	qt_qthread_t.h \
	qt_queue.h \
	qt_shepherd_innards.h \
	qt_spawn_macros.h \
	qt_spawncache.h \
	qt_subsystems.h \
	qt_teams.h \
	qt_threadqueues.h \
	qt_threadqueue_scheduler.h \
	qt_threadstate.h \
	qt_touch.h \
	qt_visibility.h \
	spr_innards.h

if COMPILE_EUREKAS
noinst_HEADERS += qt_eurekas.h
endif

if COMPILE_MULTINODE
noinst_HEADERS += qt_multinode_innards.h
endif
