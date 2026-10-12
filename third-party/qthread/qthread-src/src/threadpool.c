#include <assert.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__STDC_NO_THREADS__)
#define QPOOL_USE_PTHREADS
#endif

#ifdef QPOOL_USE_PTHREADS
#include <pthread.h>
#else
#include <threads.h>
#endif

#include <qthread/common.h>

#include "qt_arithmetic.h"
#include "qt_asserts.h"
#include "qt_atomic_wait.h"
#include "qt_branching.h"
#include "qt_macros.h"
#include "qt_threadpool.h"
#include "qt_visibility.h"

// 3d-bit thread indices are good enough.
// At least for the forseeable future we never
// need/want billions of hardware threads in shared-memory.
_Thread_local uint32_t context_index;

// TODO: try other ways to pause/resume
// TODO: sched_yield implementation
// TODO: nanosleep implementation
// TODO: nanosleep with non-constant backoff schemes since parallel sections
// often happen in rapid succession.

// Synchronization mechanism for sleeping/waking worker threads
#define QPOOL_USE_FUTEX
// Synchronization mechanism for sleeping/waking the main thread
#define QPOOL_MAIN_USE_FUTEX

// Placeholder
#define DEFAULT_CACHE_LINE_SIZE 64

unsigned int get_cache_line_size() { return DEFAULT_CACHE_LINE_SIZE; }

// Reserved pointer value to signal pool shutdown.
#define POOL_END_SIGNAL ((void *)1u)

typedef struct {
  qt_threadpool_func_type func;
  void *arg;
} pooled_thread_work;

static int run_work(pooled_thread_work w) { return w.func(w.arg); }

typedef struct {
  // allocated array of thread headers (pooled_thread_control)
  // padded dynamically so each has its own cache line.
  void *_Atomic threads;
  _Atomic uint32_t num_threads;
  // alignment constraints usually mean this space is available anyway even
  // though we don't currently use it.
  _Atomic uint32_t reserved;
#ifdef QPOOL_USE_FUTEX
  // Note: we generally want num_active_threads and main_resume_flag on the
  // same cache line. Currently this usually just falls out naturally from the
  // alignment constraints here anyway.
  _Atomic uint32_t num_active_threads;
  qt_atomic_wait_t main_resume_flag;
#else
#error "no alternatives to futexes yet."
#endif
} pool_header;

typedef struct {
  // 16 byte aligned to allow loading it in one atomic instruction
  // on architectures where that makes sense (most of them).
  // Specifically this works on x86 with AVX (see https://rigtorp.se/isatomic/
  // for details).
  // alignas(16) pooled_thread_work work;
  alignas(16) _Atomic qt_threadpool_func_type func;
  void *_Atomic arg;
#ifdef QPOOL_USE_FUTEX
  _Atomic uint32_t waiting_flag;
#endif
  _Atomic uint32_t index;
  pool_header *_Atomic pool;
#ifdef QPOOL_USE_PTHREADS
  pthread_t thread;
#else
  thrd_t thread;
#endif
} pooled_thread_control;

// alignment spec really only necessary if we're going to do mixed-size atomic
// loads/stores on this.
alignas(64) static pool_header hw_pool;
// TODO: how exactly do we nest these things without requiring additional
// syscalls? Would it be better to offer different specializations of the base
// hardware thread or potentially incur additional overheads from nesting? Are
// there overheads from just nesting?

// Needed for the logic around cache lines later.
// It definitely won't fit in 32 bytes, but should easily fit
// in 64 which is also the most common cache line size at the moment.
// The main thing that could throw this off is if
// pthread_t is absolutely massive for some reason.
#ifdef QPOOL_USE_PTHREADS
qt_static_assert(sizeof(pooled_thread_control) <= 64u,
                 "pthread_t is too large.");
#else
qt_static_assert(sizeof(pooled_thread_control) <= 64u, "thrd_t is too large.");
#endif

static pooled_thread_work load_work(pooled_thread_control *control) {
  pooled_thread_work work;
  // Zero out the loaded work object in debug mode.
#ifndef NDEBUG
  work.func =
    atomic_exchange_explicit(&control->func, NULL, memory_order_relaxed);
#else
  work.func = atomic_load_explicit(&control->func, memory_order_relaxed);
#endif
  // TODO: is this branch helpful? Seems like this thread will already own the
  // cache line anyway. On some extremely old/small architectures it might be
  // beneficial to branch like this and exit the outer loop in
  // pooled_thread_func immediately but that wreaks havoc on the code structure
  // and likely doesn't matter on most modern architectures. if
  // unlikely(work.func == POOL_END_SIGNAL) return ret;
#ifndef NDEBUG
  work.arg =
    atomic_exchange_explicit(&control->arg, NULL, memory_order_relaxed);
#else
  work.arg = atomic_load_explicit(&control->arg, memory_order_relaxed);
#endif
  return work;
}

static void store_work(pooled_thread_control *control,
                       qt_threadpool_func_type func,
                       void *arg) {
  atomic_store_explicit(&control->func, func, memory_order_relaxed);
  atomic_store_explicit(&control->arg, arg, memory_order_relaxed);
}

static void init_thread_control(pooled_thread_control *control,
                                uint32_t index,
                                pool_header *pool) {
  store_work(control, NULL, NULL);
  qt_atomic_wait_set_empty(&control->waiting_flag);
  atomic_store_explicit(&control->index, index, memory_order_relaxed);
  atomic_store_explicit(&control->pool, pool, memory_order_relaxed);
}

static void launch_work_on_thread(pooled_thread_control *control,
                                  qt_threadpool_func_type func,
                                  void *arg) {
  // assign the work
  control->func = func;
  control->arg = arg;
  // TODO: check that the actual value of the futex for the threads is managed
  // right.
  qt_atomic_wait_set_full(&control->waiting_flag);
  qt_wake_one(&control->waiting_flag);
}

static pooled_thread_work worker_wait_for_work(pooled_thread_control *control) {
#ifdef QPOOL_USE_FUTEX
  for (;;) {
    qt_wait_on_address(&control->waiting_flag, qt_atomic_wait_empty);
    if likely (qt_atomic_wait_load(&control->waiting_flag) !=
               qt_atomic_wait_empty) {
      atomic_store_explicit(&control->waiting_flag, 0u, memory_order_relaxed);
      return load_work(control);
    }
    // TODO: pause instruction here? May not be necessary since we're already
    // using a futex or equivalent.
  }
#else
#error "No other synchronization available."
  // TODO: sched_yield implementation
  // TODO: nanosleep implementation
  // TODO: nanosleep with non-constant backoff schemes since parallel sections
  // often happen in rapid succession.
#endif
}

static void notify_worker_of_termination(pooled_thread_control *control) {
  atomic_store_explicit(&control->func, POOL_END_SIGNAL, memory_order_relaxed);
#ifndef NDEBUG
  atomic_store_explicit(&control->arg, NULL, memory_order_relaxed);
#endif
  qt_atomic_wait_set_full(&control->waiting_flag);
  qt_wake_one(&control->waiting_flag);
}

static void init_main_sync(pool_header *pool) {
  qt_atomic_wait_set_full(&pool->main_resume_flag);
}

static void notify_main_of_completion(pool_header *pool) {
#ifdef QPOOL_MAIN_USE_FUTEX
  // TODO: is it worthwhile to do something more sophisticated to count down the
  // number of threads remaining?
  if (!(atomic_fetch_sub_explicit(
          &pool->num_active_threads, 1u, memory_order_relaxed) -
        1u)) {
    assert(qt_atomic_wait_load(&pool->main_resume_flag) == qt_atomic_wait_full);
    qt_atomic_wait_set_empty(&pool->main_resume_flag);
    qt_wake_one(&pool->main_resume_flag);
  }
#else
#error "No other synchronization available."
#endif
}

static void suspend_main_while_working(pool_header *pool) {
  do {
    qt_wait_on_address(&pool->main_resume_flag, qt_atomic_wait_full);
  } while (qt_atomic_wait_load(&pool->main_resume_flag) !=
           qt_atomic_wait_empty); // TODO: pause instruction on spurious wake?
}

#ifdef QPOOL_USE_PTHREADS
static void *pooled_thread_func(void *void_arg) {
#else
static int pooled_thread_func(void *void_arg) {
#endif
  pooled_thread_control *control = (pooled_thread_control *)void_arg;
  context_index = atomic_load_explicit(&control->index, memory_order_relaxed);
  pooled_thread_work current_work;
  for (;;) {
    pooled_thread_work work = worker_wait_for_work(control);
    if unlikely (work.func == POOL_END_SIGNAL) break;
    run_work(work);
    notify_main_of_completion(control->pool);
  }
  // Return values are not consistently sized/typed across runtimes, so we don't
  // currently use them. Technically there's 32 bits of info that could be used
  // here. Errors should be forwarded to the main thread when a block of pooled
  // work ends though, so most errors won't need to go through here.
#ifdef QPOOL_USE_PTHREADS
  return NULL;
#else
  return 0;
#endif
}

API_FUNC hw_pool_init_status hw_pool_init(uint32_t num_threads) {
  if unlikely (!num_threads) return POOL_INIT_NO_THREADS_SPECIFIED;
  uint32_t old = 0u;
  assert(num_threads < UINT32_MAX);
  if unlikely (!atomic_compare_exchange_strong_explicit(&hw_pool.num_threads,
                                                        &old,
                                                        num_threads,
                                                        memory_order_acquire,
                                                        memory_order_relaxed))
    return POOL_INIT_ALREADY_INITIALIZED;
  size_t alignment = QTHREAD_MAX((size_t)64u, get_cache_line_size());
  size_t size = (size_t)num_threads * alignment;
  char *buffer = aligned_alloc(alignment, size);
  hw_pool_init_status retval;
  if unlikely (!buffer) {
    retval = POOL_INIT_OUT_OF_MEMORY;
    goto release_pool;
  }
  hw_pool.threads = buffer;
  int status;
#ifdef QPOOL_USE_PTHREADS
  pthread_attr_t attr;
  status = pthread_attr_init(&attr);
  if unlikely (status) {
    retval = POOL_INIT_ERROR;
    goto release_buffer;
  }
#endif
  uint32_t i = 0u;
  while (i < num_threads) {
    pooled_thread_control *thread_control =
      (pooled_thread_control *)(buffer + alignment * (size_t)i);
    // Initialize the thread control struct in two 128b atomic writes.
    // TODO: It's possible to just do this in a single 256b atomic write on most
    // x86 platforms. That may also require increasing the alignment constraints
    // for the control_slice.
    // TODO: also ifdef in an implementation for platforms that can't do
    // lock-free 128b writes or that don't handle mixed-size atomic writes.
    // TODO: making some kind of ifunc to handle this initialization is probably
    // actually the right way to do it because it's hard to know enough about
    // the CPU at compile-time.
    init_thread_control(thread_control, i, &hw_pool);
    int status;
#ifdef QPOOL_USE_PTHREADS
    status = pthread_create(
      &thread_control->thread, &attr, pooled_thread_func, thread_control);
    if unlikely (status) goto cleanup_threads;
#else
    status =
      thrd_create(&thread_control->thread, pooled_thread_func, thread_control);
    if unlikely (status != thrd_success) goto cleanup_threads;
#endif
    ++i;
  }
#ifdef QPOOL_USE_PTHREADS
  pthread_attr_destroy(&attr);
#endif
  return POOL_INIT_SUCCESS;
cleanup_threads:
  if (i) {
    uint32_t j = --i;
    while (i) {
      // TODO: fix deinit to match new layout and interrupt mechanism.
      pooled_thread_control *thread_control =
        (pooled_thread_control *)(buffer + alignment * (size_t)i);
      atomic_store_explicit(
        &thread_control->func, POOL_END_SIGNAL, memory_order_release);
#ifndef NDEBUG
      atomic_store_explicit(&thread_control->arg, NULL, memory_order_relaxed);
#endif
      --i;
    }
    i = j;
    while (i) {
      pooled_thread_control *thread_control =
        (pooled_thread_control *)(buffer + alignment * (size_t)i);
      // TODO: crash informatively if join fails.
#ifdef QPOOL_USE_PTHREADS
      pthread_join(thread_control->thread, NULL);
#else
      thrd_join(thread_control->thread, NULL);
#endif
      --i;
    }
  }
#ifdef QPOOL_USE_PTHREADS
  // No specific label needed for this one since failing immediately after
  // setting up attr is the zero case for the loops.
  pthread_attr_destroy(&attr);
#endif
release_buffer:
  atomic_store_explicit(&hw_pool.threads, NULL, memory_order_relaxed);
  free(buffer);
release_pool:
  atomic_store_explicit(&hw_pool.num_threads, 0, memory_order_release);
  return retval;
}

API_FUNC QTHREAD_SUPPRESS_MSAN void hw_pool_destroy() {
  uint32_t num_threads =
    atomic_load_explicit(&hw_pool.num_threads, memory_order_relaxed);
  char *buffer = atomic_load_explicit(&hw_pool.threads, memory_order_relaxed);
  size_t alignment = QTHREAD_MAX((size_t)64u, get_cache_line_size());
  uint32_t i = num_threads;
  while (i) {
    --i;
    // TODO: fix deinit to match new layout and interrupt mechanism.
    pooled_thread_control *thread_control =
      (pooled_thread_control *)(buffer + alignment * (size_t)i);
    notify_worker_of_termination(thread_control);
  }
  i = num_threads;
  while (i) {
    --i;
    pooled_thread_control *thread_control =
      (pooled_thread_control *)(buffer + alignment * (size_t)i);
    // TODO: crash informatively if join fails somehow.
#ifdef QPOOL_USE_PTHREADS
    pthread_join(thread_control->thread, NULL);
#else
    thrd_join(thread_control->thread, NULL);
#endif
  }

  atomic_store_explicit(&hw_pool.threads, NULL, memory_order_relaxed);
  free(buffer);
  atomic_store_explicit(&hw_pool.num_threads, 0, memory_order_release);
}

API_FUNC void
pool_run_on_all(pool_header *pool, qt_threadpool_func_type func, void *arg) {
  uint32_t num_threads =
    atomic_load_explicit(&pool->num_threads, memory_order_relaxed);
  assert(num_threads);
  assert(num_threads < UINT32_MAX);
  char *buffer =
    (char *)atomic_load_explicit(&pool->threads, memory_order_relaxed);
  atomic_store_explicit(
    &pool->num_active_threads, num_threads, memory_order_relaxed);
  init_main_sync(pool);
  size_t alignment = QTHREAD_MAX((size_t)64u, get_cache_line_size());
  for (uint32_t i = 0u;
       i < atomic_load_explicit(&pool->num_threads, memory_order_relaxed);
       i++) {
    pooled_thread_control *thread_control =
      (pooled_thread_control *)(buffer + alignment * (size_t)i);
    launch_work_on_thread(thread_control, func, arg);
  }
  suspend_main_while_working(pool);
}

API_FUNC void hw_pool_run_on_all(qt_threadpool_func_type func, void *arg) {
  pool_run_on_all(&hw_pool, func, arg);
}

