# -*- Makefile -*-
#
# Copyright (c)      2012  Sandia Corporation
#

.PHONY: buildall buildtests buildextra

TESTS = \
		hello_world \
		aligned_prodcons \
		aligned_readXX_basic \
		aligned_purge_basic \
		aligned_purge_wakes \
		aligned_writeFF_basic \
		aligned_writeFF_waits \
		hello_world_multi \
		syncvar_prodcons \
		reinitialization \
		qthread_cas \
		qthread_cacheline \
		qthread_readstate \
		qthread_id \
		qthread_incr qthread_fincr qthread_dincr \
		qthread_stackleft \
		qtimer \
		qalloc \
		arbitrary_blocking_operation \
		sinc_null \
		sinc_workers \
		sinc \
		tasklocal_data \
		tasklocal_data_no_default \
		tasklocal_data_no_argcopy \
		external_fork \
		external_syncvar \
		read \
		test_teams \
		test_subteams \
 		qthread_fork_precond \
		qthread_migrate_to  \
		qthread_disable_shepherd 


if QTHREAD_PERFORMANCE
TESTS += performance
endif

check_PROGRAMS = $(TESTS)

TESTS_ENVIRONMENT =

if HAVE_PROG_TIMELIMIT
TESTS_ENVIRONMENT += @timelimit_path@ -T 1
endif

if WANT_SINGLE_WORKER_SCHEDULER
TESTS_ENVIRONMENT += env QT_NUM_SHEPHERDS=2 QT_NUM_WORKERS_PER_SHEPHERD=1
endif

AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/test/
qthreadlib = $(top_builddir)/src/libqthread.la

buildall: $(TESTS)

buildtests: $(TESTS)

buildextra: $(TESTS)

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la

LDADD = $(qthreadlib)

hello_world_SOURCES = hello_world.c

aligned_prodcons_SOURCES = aligned_prodcons.c

aligned_readXX_basic_SOURCES = aligned_readXX_basic.c

aligned_purge_basic_SOURCES = aligned_purge_basic.c

aligned_purge_wakes_SOURCES = aligned_purge_wakes.c

aligned_writeFF_basic_SOURCES = aligned_writeFF_basic.c

aligned_writeFF_waits_SOURCES = aligned_writeFF_waits.c

hello_world_multi_SOURCES = hello_world_multi.c

syncvar_prodcons_SOURCES = syncvar_prodcons.c

reinitialization_SOURCES = reinitialization.c

qthread_cas_SOURCES = qthread_cas.c

qthread_cacheline_SOURCES = qthread_cacheline.c

qthread_id_SOURCES = qthread_id.c

qthread_incr_SOURCES = qthread_incr.c

qthread_fincr_SOURCES = qthread_fincr.c

qthread_dincr_SOURCES = qthread_dincr.c

qthread_stackleft_SOURCES = qthread_stackleft.c

qthread_migrate_to_SOURCES = qthread_migrate_to.c

qthread_disable_shepherd_SOURCES = qthread_disable_shepherd.c

qtimer_SOURCES = qtimer.c

#queue_SOURCES = queue.c

qthread_fork_precond_SOURCES = qthread_fork_precond.c

qalloc_SOURCES = qalloc.c

arbitrary_blocking_operation_SOURCES = arbitrary_blocking_operation.c

sinc_null_SOURCES = sinc_null.c

sinc_workers_SOURCES = sinc_workers.c

sinc_SOURCES = sinc.c

tasklocal_data_SOURCES = tasklocal_data.c

tasklocal_data_no_default_SOURCES = tasklocal_data_no_default.c

tasklocal_data_no_argcopy_SOURCES = tasklocal_data_no_argcopy.c

external_fork_SOURCES = external_fork.c

external_syncvar_SOURCES = external_syncvar.c

read_SOURCES = read.c

test_teams_SOURCES = test_teams.c

test_subteams_SOURCES = test_subteams.c

if QTHREAD_PERFORMANCE
performance_SOURCES = performance.c
endif
