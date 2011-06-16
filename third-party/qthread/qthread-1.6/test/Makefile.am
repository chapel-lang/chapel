# -*- Makefile -*-
#
# Copyright (c)      2008  Sandia Corporation
#

AM_CXXFLAGS = @RESTRICT_CXXFLAGS@

TESTS = test_basic \
		test_basic_syncvar \
		test_reinit \
		test_cas \
		test_cacheline \
		test_id \
		test_incr test_fincr test_dincr \
		test_stackleft \
		test_external_fork \
		test_external_syncvar \
		test_migrate \
		test_disabled \
		test_qtimer \
		test_syncvar \
		test_syncvar2 \
		test_qutil \
		test_qutil_qsort \
		test_feb_barrier \
		testqloop \
		test_queue_loop \
		test_qpool \
		test_qarray \
		test_qarray_accum \
		test_qlfqueue \
		test_qdqueue \
		test_allpairs \
		test_qalloc \
		test_user_defined_block

EXTRA_PROGRAMS = test_wavefront

if USE_SYSCALL_INTERCEPT
TESTS += test_read
endif

if ENABLE_CXX_TESTS
TESTS += testloop \
		 test_cpp
endif

if COMPILE_ROSE_EXTENSIONS
TESTS += test_log_barrier
endif

EXTRA_PROGRAMS += \
	 time_increments \
	 time_febs \
	 time_qarray \
	 time_qarray_sizes \
	 time_producerconsumer \
	 time_syncvar_producerconsumer \
	 time_qpool \
	 time_qlfqueue \
	 time_qdqueue \
	 time_qdqueue_sizes \
	 time_qutil_qsort \
	 time_allpairs \
	 time_wavefront \
	 time_incr_bench \
	 time_incr_bench_pthread \
	 time_lul_bench \
	 time_lul_bench_pthread \
	 time_mutex_bench \
	 time_mutex_bench_pthread \
	 time_spin_bench \
	 time_spin_bench_pthread \
	 time_chain_bench \
	 time_chain_bench_pthread \
	 time_thrcrt_bench \
	 time_thrcrt_bench_pthread \
	 time_cncthr_bench \
	 time_cncthr_bench_pthread \
	 time_swap \
	 time_threading \
	 time_gcd \
	 test_cpp_interface
CLEANFILES += $(EXTRA_PROGRAMS)

buildall: $(TESTS) $(EXTRA_PROGRAMS)
buildtests: $(TESTS)
buildextra: $(EXTRA_PROGRAMS)
.PHONY: buildall buildtests buildextra

check_PROGRAMS = $(TESTS)

noinst_HEADERS = argparsing.h

INCLUDES = -I$(top_srcdir)/include
OUTPUTDIR = $(top_builddir)/src
QTHREADLIB = $(OUTPUTDIR)/libqthread.la
#CONTEXTLIB = $(OUTPUTDIR)/fastcontext/libfastcontext.la

LDADD = $(QTHREADLIB)

test_qutil_SOURCES = test_qutil.c
test_qutil_DEPENDENCIES = $(QTHREADLIB)

test_stackleft_SOURCES = test_stackleft.c
test_stackleft_DEPENDENCIES = $(QTHREADLIB)

test_basic_SOURCES = test_basic.c
test_basic_DEPENDENCIES = $(QTHREADLIB)

test_reinit_SOURCES = test_reinit.c
test_reinit_DEPENDENCIES = $(QTHREADLIB)

test_qalloc_SOURCES = testq.c
test_qalloc_DEPENDENCIES = $(QTHREADLIB)

testloop_SOURCES = testloop.cpp
testloop_DEPENDENCIES = $(QTHREADLIB)

test_cpp_SOURCES = test_cpp.cpp
test_cpp_DEPENDENCIES = $(QTHREADLIB)

test_cpp_interface_SOURCES = test_cpp_interface.cpp
test_cpp_interface_DEPENDENCIES = $(QTHREADLIB)

testqloop_SOURCES = testqloop.c
testqloop_DEPENDENCIES = $(QTHREADLIB)

test_queue_loop_SOURCES = test_queue_loop.c
test_queue_loop_DEPENDENCIES = $(QTHREADLIB)

test_incr_SOURCES = testincr.c
test_incr_DEPENDENCIES = $(QTHREADLIB)

test_fincr_SOURCES = testfincr.c
test_fincr_DEPENDENCIES = $(QTHREADLIB)

test_dincr_SOURCES = testdincr.c
test_dincr_DEPENDENCIES = $(QTHREADLIB)

test_disabled_SOURCES = test_disabled.c
test_disabled_DEPENDENCIES = $(QTHREADLIB)

test_cas_SOURCES = test_cas.c
test_cas_DEPENDENCIES = $(QTHREADLIB)

test_cacheline_SOURCES = test_cacheline.c
test_cacheline_DEPENDENCIES = $(QTHREADLIB)

test_id_SOURCES = test_id.c
test_id_DEPENDENCIES = $(QTHREADLIB)

test_allpairs_SOURCES = test_allpairs.c
test_allpairs_DEPENDENCIES = $(QTHREADLIB)

test_wavefront_SOURCES = test_wavefront.c
test_wavefront_DEPENDENCIES = $(QTHREADLIB)

test_migrate_SOURCES = test_migrate.c
test_migrate_DEPENDENCIES = $(QTHREADLIB)

test_qutil_qsort_SOURCES = test_qutil_qsort.c
test_qutil_qsort_DEPENDENCIES = $(QTHREADLIB)

time_qutil_qsort_SOURCES = time_qutil_qsort.c
time_qutil_qsort_DEPENDENCIES = $(QTHREADLIB)

test_qarray_SOURCES = test_qarray.c
test_qarray_DEPENDENCIES = $(QTHREADLIB)

test_qarray_accum_SOURCES = test_qarray_accum.c
test_qarray_accum_DEPENDENCIES = $(QTHREADLIB)

test_qlfqueue_SOURCES = test_qlfqueue.c
test_qlfqueue_DEPENDENCIES = $(QTHREADLIB)

test_qdqueue_SOURCES = test_qdqueue.c
test_qdqueue_DEPENDENCIES = $(QTHREADLIB)

test_qpool_SOURCES = test_qpool.c
test_qpool_DEPENDENCIES = $(QTHREADLIB)

time_gcd_SOURCES = time_gcd.c

time_threading_SOURCES = time_threading.c
time_threading_DEPENDENCIES = $(QTHREADLIB)

time_swap_SOURCES = time_swap.c
time_swap_LDADD = $(QTHREADLIB)
time_swap_DEPENDENCIES = $(QTHREADLIB)
#if QTHREAD_NEED_OWN_MAKECONTEXT
#time_swap_LDADD += $(CONTEXTLIB)
#time_swap_DEPENDENCIES += $(CONTEXTLIB)
#endif

time_producerconsumer_SOURCES = time_producerconsumer.c
time_producerconsumer_DEPENDENCIES = $(QTHREADLIB)

time_syncvar_producerconsumer_SOURCES = time_syncvar_producerconsumer.c
time_syncvar_producerconsumer_DEPENDENCIES = $(QTHREADLIB)

time_increments_SOURCES = time_increments.c
time_increments_DEPENDENCIES = $(QTHREADLIB)

time_febs_SOURCES = time_febs.c
time_febs_DEPENDENCIES = $(QTHREADLIB)

time_qarray_SOURCES = time_qarray.c
time_qarray_DEPENDENCIES = $(QTHREADLIB)

time_qarray_sizes_SOURCES = time_qarray_sizes.c
time_qarray_sizes_DEPENDENCIES = $(QTHREADLIB)

time_qpool_SOURCES = time_qpool.c
time_qpool_DEPENDENCIES = $(QTHREADLIB)

time_qlfqueue_SOURCES = time_qlfqueue.c
time_qlfqueue_DEPENDENCIES = $(QTHREADLIB)

time_qdqueue_SOURCES = time_qdqueue.c
time_qdqueue_DEPENDENCIES = $(QTHREADLIB)

time_allpairs_SOURCES = time_allpairs.c
time_allpairs_DEPENDENCIES = $(QTHREADLIB)

time_wavefront_SOURCES = time_wavefront.c
time_wavefront_DEPENDENCIES = $(QTHREADLIB)

time_incr_bench_SOURCES = time_incr_bench.c
time_incr_bench_DEPENDENCIES = $(QTHREADLIB)

time_incr_bench_pthread_SOURCES = time_incr_bench_pthread.c
time_incr_bench_pthread_DEPENDENCIES = $(QTHREADLIB)

time_lul_bench_SOURCES = time_lul_bench.c
time_lul_bench_DEPENDENCIES = $(QTHREADLIB)

time_lul_bench_pthread_SOURCES = time_lul_bench_pthread.c
time_lul_bench_pthread_DEPENDENCIES = $(QTHREADLIB)

time_mutex_bench_SOURCES = time_mutex_bench.c
time_mutex_bench_DEPENDENCIES = $(QTHREADLIB)

time_mutex_bench_pthread_SOURCES = time_mutex_bench_pthread.c
time_mutex_bench_pthread_DEPENDENCIES = $(QTHREADLIB)

time_spin_bench_SOURCES = time_spin_bench.c
time_spin_bench_DEPENDENCIES = $(QTHREADLIB)

time_spin_bench_pthread_SOURCES = time_spin_bench_pthread.c
time_spin_bench_pthread_DEPENDENCIES = $(QTHREADLIB)

time_chain_bench_SOURCES = time_chain_bench.c
time_chain_bench_DEPENDENCIES = $(QTHREADLIB)

time_chain_bench_pthread_SOURCES = time_chain_bench_pthread.c
time_chain_bench_pthread_DEPENDENCIES = $(QTHREADLIB)

time_thrcrt_bench_SOURCES = time_thrcrt_bench.c
time_thrcrt_bench_DEPENDENCIES = $(QTHREADLIB)

time_thrcrt_bench_pthread_SOURCES = time_thrcrt_bench_pthread.c
time_thrcrt_bench_pthread_DEPENDENCIES = $(QTHREADLIB)

time_cncthr_bench_SOURCES = time_cncthr_bench.c
time_cncthr_bench_DEPENDENCIES = $(QTHREADLIB)

time_cncthr_bench_pthread_SOURCES = time_cncthr_bench_pthread.c
time_cncthr_bench_pthread_DEPENDENCIES = $(QTHREADLIB)

test_feb_barrier_SOURCES = test_feb_barrier.c
test_feb_barrier_DEPENDENCIES = $(QTHREADLIB)

test_log_barrier_SOURCES = test_log_barrier.c
test_log_barrier_DEPENDENCIES = $(QTHREADLIB)

test_external_fork_SOURCES = test_external_fork.c
test_external_fork_DEPENDENCIES = $(QTHREADLIB)

test_external_syncvar_SOURCES = test_external_syncvar.c
test_external_syncvar_DEPENDENCIES = $(QTHREADLIB)

test_read_SOURCES = test_read.c
test_read_DEPENDENCIES = $(QTHEADLIB)
