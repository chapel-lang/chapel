
.PHONY: buildall buildtests buildextra

DIST_SUBDIRS = rose_bots

generic_benchmarks = \
                     time_gcd \
                     time_increments \
                     time_febs \
                     time_producerconsumer \
                     time_syncvar_producerconsumer \
                     time_threading \
                     time_threading.omp \
                     time_stencil_bsp \
                     time_stencil_feb \
                     time_stencil_pre \
                     time_halo_swap_all \
                     time_qt_loops \
                     time_qt_loopaccums
thesis_benchmarks = \
                    time_allpairs \
                    time_wavefront \
                    time_qutil_qsort
pmea09_benchmarks = \
                    time_qarray \
                    time_qarray_sizes \
                    time_qpool \
                    time_qlfqueue \
                    time_qdqueue \
                    time_qdqueue_sizes
mtaap08_benchmarks = \
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
                     time_cncthr_bench_pthread
uts_benchmarks = \
				 time_uts_omp \
				 time_uts_syncvar \
				 time_uts_sinc \
				 time_uts_donecount
mt_benchmarks = \
				time_task_spawn \
				time_eager_future \
				time_fib \
				time_omp_task_spawn \
				time_omp_eager_future \
				time_omp_fib

benchmarks = \
             $(generic_benchmarks) \
			 $(uts_benchmarks) \
             $(thesis_benchmarks) \
             $(pmea09_benchmarks) \
             $(mtaap08_benchmarks) \
	     $(mt_benchmarks)

EXTRA_PROGRAMS = $(benchmarks)

EXTRA_DIST = \
			 pmea09/time_tbbq.cpp \
			 pmea09/time_tbbq_sizes.cpp


INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir)/test/
outputdir = $(top_builddir)/src
qthreadlib = $(outputdir)/libqthread.la

LDADD = $(qthreadlib)

if COMPILE_ROSE_EXTENSIONS
buildall: $(benchmarks)
	$(MAKE) -C rose_bots_benchmarks buildall
buildextra: $(benchmarks)
	$(MAKE) -C rose_bots_benchmarks buildextra
else
buildall: $(benchmarks)
buildextra: $(benchmarks)
endif

time_allpairs_SOURCES = thesis/time_allpairs.c
time_allpairs_DEPENDENCIES = $(qthreadlib)

time_wavefront_SOURCES = thesis/time_wavefront.c
time_wavefront_DEPENDENCIES = $(qthreadlib)

time_qutil_qsort_SOURCES = thesis/time_qutil_qsort.c
time_qutil_qsort_DEPENDENCIES = $(qthreadlib)

time_qarray_SOURCES = pmea09/time_qarray.c
time_qarray_DEPENDENCIES = $(qthreadlib)

time_qarray_sizes_SOURCES = pmea09/time_qarray_sizes.c
time_qarray_sizes_DEPENDENCIES = $(qthreadlib)

time_qpool_SOURCES = pmea09/time_qpool.c
time_qpool_DEPENDENCIES = $(qthreadlib)

time_qlfqueue_SOURCES = pmea09/time_qlfqueue.c
time_qlfqueue_DEPENDENCIES = $(qthreadlib)

time_qdqueue_SOURCES = pmea09/time_qdqueue.c
time_qdqueue_DEPENDENCIES = $(qthreadlib)

time_qdqueue_sizes_SOURCES = pmea09/time_qdqueue_sizes.c
time_qdqueue_sizes_DEPENDENCIES = $(qthreadlib)

time_incr_bench_SOURCES = mtaap08/time_incr_bench.c
time_incr_bench_DEPENDENCIES = $(qthreadlib)

time_incr_bench_pthread_SOURCES = mtaap08/time_incr_bench_pthread.c
time_incr_bench_pthread_DEPENDENCIES = $(qthreadlib)

time_lul_bench_SOURCES = mtaap08/time_lul_bench.c
time_lul_bench_DEPENDENCIES = $(qthreadlib)

time_lul_bench_pthread_SOURCES = mtaap08/time_lul_bench_pthread.c
time_lul_bench_pthread_DEPENDENCIES = $(qthreadlib)

time_mutex_bench_SOURCES = mtaap08/time_mutex_bench.c
time_mutex_bench_DEPENDENCIES = $(qthreadlib)

time_mutex_bench_pthread_SOURCES = mtaap08/time_mutex_bench_pthread.c
time_mutex_bench_pthread_DEPENDENCIES = $(qthreadlib)

time_spin_bench_SOURCES = mtaap08/time_spin_bench.c
time_spin_bench_DEPENDENCIES = $(qthreadlib)

time_spin_bench_pthread_SOURCES = mtaap08/time_spin_bench_pthread.c
time_spin_bench_pthread_DEPENDENCIES = $(qthreadlib)

time_chain_bench_SOURCES = mtaap08/time_chain_bench.c
time_chain_bench_DEPENDENCIES = $(qthreadlib)

time_chain_bench_pthread_SOURCES = mtaap08/time_chain_bench_pthread.c
time_chain_bench_pthread_DEPENDENCIES = $(qthreadlib)

time_thrcrt_bench_SOURCES = mtaap08/time_thrcrt_bench.c
time_thrcrt_bench_DEPENDENCIES = $(qthreadlib)

time_thrcrt_bench_pthread_SOURCES = mtaap08/time_thrcrt_bench_pthread.c
time_thrcrt_bench_pthread_DEPENDENCIES = $(qthreadlib)

time_cncthr_bench_SOURCES = mtaap08/time_cncthr_bench.c
time_cncthr_bench_DEPENDENCIES = $(qthreadlib)

time_cncthr_bench_pthread_SOURCES = mtaap08/time_cncthr_bench_pthread.c
time_cncthr_bench_pthread_DEPENDENCIES = $(qthreadlib)

time_gcd_SOURCES = generic/time_gcd.c

time_increments_SOURCES = generic/time_increments.c
time_increments_DEPENDENCIES = $(qthreadlib)

time_febs_SOURCES = generic/time_febs.c
time_febs_DEPENDENCIES = $(qthreadlib)

time_fib_SOURCES = mt/time_fib.c
time_fib_DEPENDENCIES = $(qthreadlib)

time_producerconsumer_SOURCES = generic/time_producerconsumer.c
time_producerconsumer_DEPENDENCIES = $(qthreadlib)

time_syncvar_producerconsumer_SOURCES = generic/time_syncvar_producerconsumer.c
time_syncvar_producerconsumer_DEPENDENCIES = $(qthreadlib)

time_threading_SOURCES = generic/time_threading.c
time_threading_DEPENDENCIES = $(qthreadlib)

time_threading_omp_SOURCES = generic/time_threading.omp.c
time_threading_omp_DEPENDENCIES = $(qthreadlib)
time_threading_omp_CFLAGS = @OPENMP_CFLAGS@

time_stencil_bsp_SOURCES = generic/time_stencil_bsp.c
time_stencil_bsp_DEPENDENCIES = $(qthreadlib)

time_stencil_feb_SOURCES = generic/time_stencil_feb.c
time_stencil_feb_DEPENDENCIES = $(qthreadlib)

time_stencil_pre_SOURCES = generic/time_stencil_pre.c
time_stencil_pre_DEPENDENCIES = $(qthreadlib)

time_halo_swap_all_SOURCES = generic/time_halo_swap_all.c
time_halo_swap_all_DEPENDENCIES = $(qthreadlib)

time_qt_loops_SOURCES = generic/time_qt_loops.c
time_qt_loops_DEPENDENCIES = $(qthreadlib)

time_qt_loopaccums_SOURCES = generic/time_qt_loopaccums.c
time_qt_loopaccums_DEPENDENCIES = $(qthreadlib)

time_uts_omp_SOURCES = uts/time_uts_omp.c uts/rng/brg_sha1.c
time_uts_omp_DEPENDENCIES = $(qthreadlib)
time_uts_omp_CFLAGS = @OPENMP_CFLAGS@

time_uts_syncvar_SOURCES = uts/time_uts_syncvar.c uts/rng/brg_sha1.c
time_uts_syncvar_DEPENDENCIES = $(qthreadlib)

time_uts_sinc_SOURCES = uts/time_uts_sinc.c uts/rng/brg_sha1.c
time_uts_sinc_DEPENDENCIES = $(qthreadlib)

time_uts_donecount_SOURCES = uts/time_uts_donecount.c uts/rng/brg_sha1.c
time_uts_donecount_DEPENDENCIES = $(qthreadlib)

time_task_spawn_SOURCES = mt/time_task_spawn.c
time_task_spawn_DEPENDENCIES = $(qthreadlib)

time_omp_task_spawn_SOURCES = mt/time_omp_task_spawn.c
time_omp_task_spawn_DEPENDENCIES = $(qthreadlib)
time_omp_task_spawn_CFLAGS = @OPENMP_CFLAGS@

time_eager_future_SOURCES = mt/time_eager_future.c
time_eager_future_DEPENDENCIES = $(qthreadlib)

time_omp_fib_SOURCES = mt/time_omp_fib.c
time_omp_fib_DEPENDENCIES = $(qthreadlib)
time_omp_fib_CFLAGS = @OPENMP_CFLAGS@

time_omp_eager_future_SOURCES = mt/time_omp_eager_future.c
time_omp_eager_future_DEPENDENCIES = $(qthreadlib)
time_omp_eager_future_CFLAGS = @OPENMP_CFLAGS@
