# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

AM_CXXFLAGS = @RESTRICT_CXXFLAGS@

basic_tests = test_basic \
			  test_basic_syncvar \
			  test_reinit \
			  test_cas \
			  test_cacheline \
			  test_id \
			  test_incr test_fincr test_dincr \
			  test_stackleft \
			  test_migrate \
			  test_disabled \
			  test_qtimer \
			  test_syncvar \
			  test_syncvar2 \
			  test_precond \
			  test_qalloc \
			  test_user_defined_block \
			  test_sinc \
			  test_sinc_null \
			  test_tasklocal \
			  test_tasklocal_no_default \
			  test_tasklocal_no_argcopy \
			  test_external_fork \
			  test_external_syncvar \
			  test_read \
			  test_teams

complex_tests = test_qt_loop \
				test_qt_loop_sinc \
				test_qt_loop_balance \
				test_qt_loop_balance_sinc \
				test_qutil \
				test_qutil_qsort \
				test_feb_barrier \
				test_precond_fib \
				test_qloop_utils \
				test_queue_loop \
				test_qpool \
				test_qarray \
				test_qarray_accum \
				test_qlfqueue \
				test_qdqueue \
				test_allpairs

if ENABLE_CXX_TESTS
basic_tests += \
			   test_cxx_1 \
			   test_cxx_2
complex_tests += test_cxx_oldapi
endif

SUBDIRS = 
DIST_SUBDIRS = benchmarks

TESTS =  $(basic_tests) $(complex_tests)
EXTRA_PROGRAMS = 

unbuildable = test_wavefront

if COMPILE_ROSE_EXTENSIONS
TESTS += test_log_barrier
endif

if WANT_MULTINODE
TESTS += test_multinode_init
endif

benchmarks =

EXTRA_PROGRAMS += $(unbuildable) \
				  $(benchmarks) \
				  test_signal

CLEANFILES += $(unbuildable) $(benchmarks)

if HAVE_PROG_TIMELIMIT
TESTS_ENVIRONMENT = timelimit -T 30
endif

buildtests: $(TESTS)
buildall: $(TESTS) $(benchmarks)
	$(MAKE) -C benchmarks buildall
buildextra: $(benchmarks)
	$(MAKE) -C benchmarks buildextra

.PHONY: buildall buildtests buildextra

check_PROGRAMS = $(TESTS)

noinst_HEADERS = argparsing.h

INCLUDES = -I$(top_srcdir)/include
outputdir = $(top_builddir)/src
qthreadlib = $(outputdir)/libqthread.la

LDADD = $(qthreadlib)

test_qt_loop_SOURCES = test_qt_loop.c
test_qt_loop_DEPENDENCIES = $(qthreadlib)

test_qt_loop_sinc_SOURCES = test_qt_loop_sinc.c
test_qt_loop_sinc_DEPENDENCIES = $(qthreadlib)

test_qt_loop_balance_SOURCES = test_qt_loop_balance.c
test_qt_loop_balance_DEPENDENCIES = $(qthreadlib)

test_qt_loop_balance_sinc_SOURCES = test_qt_loop_balance_sinc.c
test_qt_loop_balance_sinc_DEPENDENCIES = $(qthreadlib)

test_qutil_SOURCES = test_qutil.c
test_qutil_DEPENDENCIES = $(qthreadlib)

test_stackleft_SOURCES = test_stackleft.c
test_stackleft_DEPENDENCIES = $(qthreadlib)

test_basic_SOURCES = test_basic.c
test_basic_DEPENDENCIES = $(qthreadlib)

test_signal_SOURCES = test_signal.c
test_signal_DEPENDENCIES = $(qthreadlib)

test_tasklocal_SOURCES = test_tasklocal.c
test_tasklocal_DEPENDENCIES = $(qthreadlib)

test_tasklocal_no_default_SOURCES = test_tasklocal_no_default.c
test_tasklocal_no_default_DEPENDENCIES = $(qthreadlib)

test_tasklocal_no_argcopy_SOURCES = test_tasklocal_no_argcopy.c
test_tasklocal_no_argcopy_DEPENDENCIES = $(qthreadlib)

test_precond_SOURCES = test_precond.c
test_precond_DEPENDENCIES = $(qthreadlib)

test_precond_fib_SOURCES = test_precond_fib.c
test_precond_fib_DEPENDENCIES = $(qthreadlib)

test_reinit_SOURCES = test_reinit.c
test_reinit_DEPENDENCIES = $(qthreadlib)

test_qalloc_SOURCES = test_qalloc.c
test_qalloc_DEPENDENCIES = $(qthreadlib)

test_cxx_oldapi_SOURCES = test_cxx_oldapi.cpp
test_cxx_oldapi_DEPENDENCIES = $(qthreadlib)

test_cxx_1_SOURCES = test_cxx_1.cpp
test_cxx_1_DEPENDENCIES = $(qthreadlib)

test_cxx_2_SOURCES = test_cxx_2.cpp
test_cxx_2_DEPENDENCIES = $(qthreadlib)

test_qloop_utils_SOURCES = test_qloop_utils.c
test_qloop_utils_DEPENDENCIES = $(qthreadlib)

test_queue_loop_SOURCES = test_queue_loop.c
test_queue_loop_DEPENDENCIES = $(qthreadlib)

test_incr_SOURCES = test_incr.c
test_incr_DEPENDENCIES = $(qthreadlib)

test_fincr_SOURCES = test_fincr.c
test_fincr_DEPENDENCIES = $(qthreadlib)

test_dincr_SOURCES = test_dincr.c
test_dincr_DEPENDENCIES = $(qthreadlib)

test_disabled_SOURCES = test_disabled.c
test_disabled_DEPENDENCIES = $(qthreadlib)

test_cas_SOURCES = test_cas.c
test_cas_DEPENDENCIES = $(qthreadlib)

test_cacheline_SOURCES = test_cacheline.c
test_cacheline_DEPENDENCIES = $(qthreadlib)

test_id_SOURCES = test_id.c
test_id_DEPENDENCIES = $(qthreadlib)

test_allpairs_SOURCES = test_allpairs.c
test_allpairs_DEPENDENCIES = $(qthreadlib)

test_wavefront_SOURCES = test_wavefront.c
test_wavefront_DEPENDENCIES = $(qthreadlib)

test_migrate_SOURCES = test_migrate.c
test_migrate_DEPENDENCIES = $(qthreadlib)

test_qutil_qsort_SOURCES = test_qutil_qsort.c
test_qutil_qsort_DEPENDENCIES = $(qthreadlib)

test_qarray_SOURCES = test_qarray.c
test_qarray_DEPENDENCIES = $(qthreadlib)

test_qarray_accum_SOURCES = test_qarray_accum.c
test_qarray_accum_DEPENDENCIES = $(qthreadlib)

test_qlfqueue_SOURCES = test_qlfqueue.c
test_qlfqueue_DEPENDENCIES = $(qthreadlib)

test_qdqueue_SOURCES = test_qdqueue.c
test_qdqueue_DEPENDENCIES = $(qthreadlib)

test_qpool_SOURCES = test_qpool.c
test_qpool_DEPENDENCIES = $(qthreadlib)

test_feb_barrier_SOURCES = test_feb_barrier.c
test_feb_barrier_DEPENDENCIES = $(qthreadlib)

test_log_barrier_SOURCES = test_log_barrier.c
test_log_barrier_DEPENDENCIES = $(qthreadlib)

test_external_fork_SOURCES = test_external_fork.c
test_external_fork_DEPENDENCIES = $(qthreadlib)

test_external_syncvar_SOURCES = test_external_syncvar.c
test_external_syncvar_DEPENDENCIES = $(qthreadlib)

test_read_SOURCES = test_read.c
test_read_DEPENDENCIES = $(qthreadlib)

test_teams_SOURCES = test_teams.c
test_teams_DEPENDENCIES = $(qthreadlib)

test_sinc_SOURCES = test_sinc.c
test_sinc_DEPENDENCIES = $(qthreadlib)

test_sinc_null_SOURCES = test_sinc_null.c
test_sinc_null_DEPENDENCIES = $(qthreadlib)

test_multinode_init_SOURCES = test_multinode_init.c
test_multinode_init_DEPENDENCIES = $(qthreadlib)
