SUBDIRS = mantevo finepoints

.PHONY: buildall buildtests buildextra benchmarks

DIST_SUBDIRS = mantevo finepoints

generic_benchmarks = \
                     time_gcd \
                     time_increments \
                     time_febs \
                     time_febs_graph_test \
                     time_febs_stream_test \
                     time_producerconsumer \
                     time_syncvar_producerconsumer \
                     time_threading \
                     time_stencil_bsp \
                     time_stencil_feb \
                     time_stencil_pre \
                     time_halo_swap_all \
                     time_prodcons_comm \
                     time_qt_loops \
                     time_qt_loopaccums \
                     time_thread_ring \
                     time_chpl_spawn

thesis_benchmarks = \
                    time_allpairs \
                    time_wavefront
if HAVE_LIBM
thesis_benchmarks += time_qutil_qsort
endif

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
uts_benchmarks =
if HAVE_LIBM
uts_benchmarks += \
                 time_uts_syncvar \
                 time_uts_aligned \
                 time_uts_sinc \
                 time_uts_donecount \
                 time_uts_donecount2 \
                 time_uts_donecount3
endif
mt_benchmarks = \
                time_task_spawn \
                time_eager_future \
                time_fib \
                time_fib2
sc12_benchmarks = \
                  spawn_sequential_qthreads \
                  spawn_parallel_qthreads \
                  fib_qthreads

if HAVE_LIBM
sc12_benchmarks += uts_qthreads
endif

if COMPILE_OMP_BENCHMARKS
generic_benchmarks += time_threading.omp
uts_benchmarks += time_uts_omp
mt_benchmarks += \
                 time_omp_task_spawn \
                 time_omp_eager_future \
                 time_omp_fib
sc12_benchmarks += \
                   spawn_sequential_omp \
                   spawn_parallel_omp \
                   fib_omp
if HAVE_LIBM
sc12_benchmarks += uts_omp
endif
endif

if COMPILE_TBB_BENCHMARKS
uts_benchmarks += time_uts_tbb
pmea09_benchmarks += \
                     time_tbbq \
                     time_tbbq_sizes
mt_benchmarks += \
                 time_tbb_eager_future \
                 time_tbb_fib \
                 time_tbb_task_spawn
sc12_benchmarks += \
                   spawn_sequential_tbb \
                   spawn_parallel_tbb \
                   fib_tbb
if HAVE_LIBM
sc12_benchmarks += uts_tbb
endif
endif

if COMPILE_CILK_BENCHMARKS
uts_benchmarks += time_uts_cilk
mt_benchmarks += \
                 time_cilk_eager_future \
                 time_cilk_fib \
                 time_cilk_task_spawn
sc12_benchmarks += \
                   spawn_sequential_cilk \
                   spawn_parallel_cilk \
                   fib_cilk
if HAVE_LIBM
sc12_benchmarks += uts_cilk
endif
endif

benchmarks = \
             $(generic_benchmarks) \
             $(uts_benchmarks) \
             $(thesis_benchmarks) \
             $(pmea09_benchmarks) \
             $(mtaap08_benchmarks) \
             $(mt_benchmarks) \
             $(sc12_benchmarks)

EXTRA_PROGRAMS = $(benchmarks)
CLEANFILES = $(benchmarks)

EXTRA_DIST = \
             pmea09/time_tbbq.cpp \
             pmea09/time_tbbq_sizes.cpp


AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/test/
outputdir = $(top_builddir)/src
qthreadlib = $(outputdir)/libqthread.la
utils_rnglib = $(top_builddir)/test/utils/rng/librng.la

LDADD = $(qthreadlib)

benchmarks: buildextra

$(qthreadlib):
	$(MAKE) -C $(top_builddir)/src libqthread.la

$(utils_rnglib):
	$(MAKE) -C $(top_builddir)/test/utils/rng librng.la

time_allpairs_SOURCES = thesis/time_allpairs.c

time_wavefront_SOURCES = thesis/time_wavefront.c

if HAVE_LIBM
time_qutil_qsort_SOURCES = thesis/time_qutil_qsort.c
time_qutil_qsort_LDADD = -lm $(qthreadlib)
endif

time_qarray_SOURCES = pmea09/time_qarray.c

time_qarray_sizes_SOURCES = pmea09/time_qarray_sizes.c

time_qpool_SOURCES = pmea09/time_qpool.c

time_qlfqueue_SOURCES = pmea09/time_qlfqueue.c

time_qdqueue_SOURCES = pmea09/time_qdqueue.c

time_qdqueue_sizes_SOURCES = pmea09/time_qdqueue_sizes.c

if COMPILE_TBB_BENCHMARKS
time_tbbq_SOURCES = pmea09/time_tbbq.cpp
time_tbbq_LDFLAGS = @TBB_LIBS@

time_tbbq_sizes_SOURCES = pmea09/time_tbbq_sizes.cpp
time_tbbq_sizes_LDFLAGS = @TBB_LIBS@
endif

time_incr_bench_SOURCES = mtaap08/time_incr_bench.c

time_incr_bench_pthread_SOURCES = mtaap08/time_incr_bench_pthread.c

time_lul_bench_SOURCES = mtaap08/time_lul_bench.c

time_lul_bench_pthread_SOURCES = mtaap08/time_lul_bench_pthread.c

time_mutex_bench_SOURCES = mtaap08/time_mutex_bench.c

time_mutex_bench_pthread_SOURCES = mtaap08/time_mutex_bench_pthread.c

time_spin_bench_SOURCES = mtaap08/time_spin_bench.c

time_spin_bench_pthread_SOURCES = mtaap08/time_spin_bench_pthread.c

time_chain_bench_SOURCES = mtaap08/time_chain_bench.c

time_chain_bench_pthread_SOURCES = mtaap08/time_chain_bench_pthread.c

time_thrcrt_bench_SOURCES = mtaap08/time_thrcrt_bench.c

time_thrcrt_bench_pthread_SOURCES = mtaap08/time_thrcrt_bench_pthread.c

time_cncthr_bench_SOURCES = mtaap08/time_cncthr_bench.c

time_cncthr_bench_pthread_SOURCES = mtaap08/time_cncthr_bench_pthread.c

time_gcd_SOURCES = generic/time_gcd.c

time_increments_SOURCES = generic/time_increments.c

time_febs_SOURCES = generic/time_febs.c

time_febs_graph_test_SOURCES = generic/time_febs_graph_test.c

time_febs_stream_test_SOURCES = generic/time_febs_stream_test.c

time_fib_SOURCES = mt/time_fib.c

time_fib2_SOURCES = mt/time_fib2.c

time_producerconsumer_SOURCES = generic/time_producerconsumer.c

time_syncvar_producerconsumer_SOURCES = generic/time_syncvar_producerconsumer.c

time_threading_SOURCES = generic/time_threading.c

time_thread_ring_SOURCES = generic/time_thread_ring.c

time_chpl_spawn_SOURCES = generic/time_chpl_spawn.c

if COMPILE_OMP_BENCHMARKS
time_threading_omp_SOURCES = generic/time_threading.omp.c
time_threading_omp_CFLAGS = @OPENMP_CFLAGS@
endif

time_stencil_bsp_SOURCES = generic/time_stencil_bsp.c

time_stencil_feb_SOURCES = generic/time_stencil_feb.c

time_stencil_pre_SOURCES = generic/time_stencil_pre.c

time_halo_swap_all_SOURCES = generic/time_halo_swap_all.c

time_prodcons_comm_SOURCES = generic/time_prodcons_comm.c

time_qt_loops_SOURCES = generic/time_qt_loops.c

time_qt_loopaccums_SOURCES = generic/time_qt_loopaccums.c

if HAVE_LIBM
if COMPILE_OMP_BENCHMARKS
time_uts_omp_SOURCES = uts/time_uts_omp.c
time_uts_omp_LDADD = $(LDADD) -lm $(utils_rnglib)
time_uts_omp_CFLAGS = @OPENMP_CFLAGS@
endif

if COMPILE_TBB_BENCHMARKS
time_uts_tbb_SOURCES = uts/time_uts_tbb.cc
time_uts_tbb_LDADD = $(LDADD) -lm $(utils_rnglib)
time_uts_tbb_LDFLAGS = @TBB_LIBS@
endif

if COMPILE_CILK_BENCHMARKS
time_uts_cilk_SOURCES = uts/time_uts_cilk.c
time_uts_cilk_LDADD = $(LDADD) -lm $(utils_rnglib)
endif

time_uts_syncvar_SOURCES = uts/time_uts_syncvar.c
time_uts_syncvar_LDADD = $(LDADD) -lm $(utils_rnglib)

time_uts_aligned_SOURCES = uts/time_uts_aligned.c
time_uts_aligned_LDADD = $(LDADD) -lm $(utils_rnglib)

time_uts_sinc_SOURCES = uts/time_uts_sinc.c
time_uts_sinc_LDADD = $(LDADD) -lm $(utils_rnglib)

time_uts_donecount_SOURCES = uts/time_uts_donecount.c
time_uts_donecount_LDADD = $(LDADD) -lm $(utils_rnglib)

time_uts_donecount2_SOURCES = uts/time_uts_donecount2.c
time_uts_donecount2_LDADD = $(LDADD) -lm $(utils_rnglib)

time_uts_donecount3_SOURCES = uts/time_uts_donecount3.c
time_uts_donecount3_LDADD = $(LDADD) -lm $(utils_rnglib)
endif

time_task_spawn_SOURCES = mt/time_task_spawn.c

time_eager_future_SOURCES = mt/time_eager_future.c

if COMPILE_OMP_BENCHMARKS
time_omp_task_spawn_SOURCES = mt/time_omp_task_spawn.c
time_omp_task_spawn_CFLAGS = @OPENMP_CFLAGS@

time_omp_fib_SOURCES = mt/time_omp_fib.c
time_omp_fib_CFLAGS = @OPENMP_CFLAGS@
endif

if COMPILE_TBB_BENCHMARKS
time_tbb_fib_SOURCES = mt/time_tbb_fib.cc
time_tbb_fib_LDFLAGS = @TBB_LIBS@

time_tbb_task_spawn_SOURCES = mt/time_tbb_task_spawn.cc
time_tbb_task_spawn_LDFLAGS = @TBB_LIBS@

time_tbb_eager_future_SOURCES = mt/time_tbb_eager_future.cc
time_tbb_eager_future_LDFLAGS = @TBB_LIBS@
endif

if COMPILE_CILK_BENCHMARKS
time_cilk_fib_SOURCES = mt/time_cilk_fib.c

time_cilk_task_spawn_SOURCES = mt/time_cilk_task_spawn.c

time_cilk_eager_future_SOURCES = mt/time_cilk_eager_future.c
endif

# SC12 Benchmarks

spawn_sequential_qthreads_SOURCES = sc12/spawn_sequential_qthreads.c

spawn_parallel_qthreads_SOURCES = sc12/spawn_parallel_qthreads.c

fib_qthreads_SOURCES = sc12/fib_qthreads.c

if HAVE_LIBM
uts_qthreads_SOURCES = sc12/uts_qthreads.c
uts_qthreads_LDADD = $(LDADD) -lm $(utils_rnglib)
endif

if COMPILE_OMP_BENCHMARKS
time_omp_eager_future_SOURCES = mt/time_omp_eager_future.c
time_omp_eager_future_CFLAGS = @OPENMP_CFLAGS@

spawn_sequential_omp_SOURCES = sc12/spawn_sequential_omp.c
spawn_sequential_omp_CFLAGS = @OPENMP_CFLAGS@

spawn_parallel_omp_SOURCES = sc12/spawn_parallel_omp.c
spawn_parallel_omp_CFLAGS = @OPENMP_CFLAGS@

fib_omp_SOURCES = sc12/fib_omp.c
fib_omp_CFLAGS = @OPENMP_CFLAGS@

if HAVE_LIBM
uts_omp_SOURCES = sc12/uts_omp.c
uts_omp_LDADD = $(LDADD) -lm $(utils_rnglib)
uts_omp_CFLAGS = @OPENMP_CFLAGS@
endif
endif

if COMPILE_TBB_BENCHMARKS
spawn_sequential_tbb_SOURCES = sc12/spawn_sequential_tbb.cc
spawn_sequential_tbb_LDFLAGS = @TBB_LIBS@

spawn_parallel_tbb_SOURCES = sc12/spawn_parallel_tbb.cc
spawn_parallel_tbb_LDFLAGS = @TBB_LIBS@

fib_tbb_SOURCES = sc12/fib_tbb.cc
fib_tbb_LDFLAGS = @TBB_LIBS@

if HAVE_LIBM
uts_tbb_SOURCES = sc12/uts_tbb.cc
uts_tbb_LDADD = $(LDADD) -lm $(utils_rnglib)
uts_tbb_LDFLAGS = @TBB_LIBS@
endif
endif

if COMPILE_CILK_BENCHMARKS
spawn_sequential_cilk_SOURCES = sc12/spawn_sequential_cilk.c

spawn_parallel_cilk_SOURCES = sc12/spawn_parallel_cilk.c

fib_cilk_SOURCES = sc12/fib_cilk.c

if HAVE_LIBM
uts_cilk_SOURCES = sc12/uts_cilk.c
uts_cilk_LDADD = $(LDADD) -lm $(utils_rnglib)
endif
endif

# vim: set expandtab
