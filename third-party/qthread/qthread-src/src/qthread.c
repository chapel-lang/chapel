/******************************************************/
/* The API                                            */
/******************************************************/
#include "qthread/qthread.h"

/******************************************************/
/* System Headers                                     */
/******************************************************/
#include <limits.h> /* for INT_MAX */
#include <pthread.h>
#include <sched.h>
#include <stdarg.h> /* for va_list, va_start() and va_end() */
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> /* for malloc() and abort() */
#include <string.h> /* for memset() */
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h> /* for getpagesize() */
#ifdef QTHREAD_USE_VALGRIND
#include <valgrind/memcheck.h>
#endif
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/types.h>

#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#include <sanitizer/tsan_interface.h>
#endif
#endif

/******************************************************/
/* Public Headers                                     */
/******************************************************/
#include "qthread/barrier.h"
#include "qthread/cacheline.h"

/******************************************************/
/* Internal Headers                                   */
/******************************************************/
#include "qt_alloc.h"
#include "qt_asserts.h"
#include "qt_atomics.h"
#include "qt_barrier.h"
#include "qt_expect.h"
#include "qt_mpool.h"
#include "qt_prefetch.h"
#include "qthread_innards.h"
#ifdef QTHREAD_OMP_AFFINITY
#include "omp_affinity.h"
#endif
#include "qt_affinity.h"
#include "qt_alloc.h"
#include "qt_blocking_structs.h"
#include "qt_envariables.h"
#include "qt_feb.h"
#include "qt_hash.h"
#include "qt_int_log.h"
#include "qt_io.h"
#include "qt_locks.h"
#include "qt_output_macros.h"
#include "qt_qthread_mgmt.h"
#include "qt_qthread_struct.h"
#include "qt_queue.h"
#include "qt_shepherd_innards.h"
#include "qt_subsystems.h"
#include "qt_syncvar.h"
#include "qt_teams.h"
#include "qt_threadqueue_scheduler.h"
#include "qt_threadqueues.h"

#define QTHREAD_STACK_ALIGNMENT 16u

/* Shared Globals */
qlib_t qlib = NULL;
int qaffinity = 1;
QTHREAD_FASTLOCK_ATTRVAR;

struct qt_cleanup_funcs_s {
  void (*func)(void);
  struct qt_cleanup_funcs_s *next;
} *qt_cleanup_funcs = NULL, *qt_cleanup_early_funcs = NULL,
  *qt_cleanup_late_funcs = NULL;

#ifdef QTHREAD_COUNT_THREADS
aligned_t threadcount;
aligned_t maxconcurrentthreads;
double avg_concurrent_threads;
aligned_t maxeffconcurrentthreads;
double avg_eff_concurrent_threads;
aligned_t effconcurrentthreads;
aligned_t concurrentthreads;
QTHREAD_FASTLOCK_TYPE concurrentthreads_lock;
QTHREAD_FASTLOCK_TYPE effconcurrentthreads_lock;
#endif

#ifdef QTHREAD_GUARD_PAGES
int GUARD_PAGES = 1;
#else
#define GUARD_PAGES 0
#endif

/* Internal Prototypes */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void qthread_wrapper(unsigned int high, unsigned int low);
#else
static void qthread_wrapper(void *ptr);
#endif

static inline void qthread_makecontext(qt_context_t *const c,
                                       void *const stack,
                                       size_t const stacksize,
                                       void (*func)(void),
                                       void const *const arg,
                                       qt_context_t *const returnc);
static inline qthread_t *qthread_thread_new(qthread_f f,
                                            void const *arg,
                                            size_t arg_size,
                                            void *ret,
                                            qt_team_t *team,
                                            int team_leader);

/*Make method externally available for the schedulers*/
void qthread_thread_free(qthread_t *t);

qt_mpool generic_qthread_pool = NULL;
qt_mpool generic_big_qthread_pool = NULL;
#define ALLOC_QTHREAD() (qthread_t *)qt_mpool_alloc(generic_qthread_pool)
#define ALLOC_BIG_QTHREAD()                                                    \
  (qthread_t *)qt_mpool_alloc(generic_big_qthread_pool)
#define FREE_QTHREAD(t) qt_mpool_free(generic_qthread_pool, t)
#define FREE_BIG_QTHREAD(t) qt_mpool_free(generic_big_qthread_pool, t)

static qt_mpool generic_stack_pool = NULL;
#ifdef QTHREAD_GUARD_PAGES
static inline void *ALLOC_STACK(void) {
  if (GUARD_PAGES) {
    uint8_t *tmp = qt_mpool_alloc(generic_stack_pool);

    assert(tmp);
    if (tmp == NULL) { return NULL; }
    if (mprotect(tmp, getpagesize(), PROT_NONE) != 0) {
      perror("mprotect in ALLOC_STACK (1)");
    }
    if (mprotect(tmp + qlib->qthread_stack_size + getpagesize(),
                 getpagesize(),
                 PROT_NONE) != 0) {
      perror("mprotect in ALLOC_STACK (2)");
    }
    return tmp + getpagesize();
  } else {
    return qt_mpool_alloc(generic_stack_pool);
  }
}

static inline void FREE_STACK(void *t) {
  if (GUARD_PAGES) {
    assert(t);
    t = (uint8_t *)t - getpagesize();
    if (mprotect(t, getpagesize(), PROT_READ | PROT_WRITE) != 0) {
      perror("mprotect in FREE_STACK (1)");
    }
    if (mprotect(((uint8_t *)t) + qlib->qthread_stack_size + getpagesize(),
                 getpagesize(),
                 PROT_READ | PROT_WRITE) != 0) {
      perror("mprotect in FREE_STACK (2)");
    }
  }
  qt_mpool_free(generic_stack_pool, t);
}

#else /* ifdef QTHREAD_GUARD_PAGES */
#define ALLOC_STACK() qt_mpool_alloc(generic_stack_pool)
#define FREE_STACK(t) qt_mpool_free(generic_stack_pool, t)
#endif /* ifdef QTHREAD_GUARD_PAGES */

static qt_mpool generic_rdata_pool = NULL;
#define ALLOC_RDATA()                                                          \
  (struct qthread_runtime_data_s *)qt_mpool_alloc(generic_rdata_pool)
#define FREE_RDATA(r) qt_mpool_free(generic_rdata_pool, (r))

#include "qt_profiling.h"

#ifndef NDEBUG
int qthread_library_initialized = 0;
void *shep0arg = NULL;
#endif

static inline void alloc_rdata(qthread_shepherd_t *me, qthread_t *t) {
  void *stack = NULL;
  struct qthread_runtime_data_s *rdata;

  if (atomic_load_explicit(&t->flags, memory_order_relaxed) & QTHREAD_SIMPLE) {
    rdata = t->rdata = ALLOC_RDATA();
  } else {
    stack = ALLOC_STACK();
    assert(stack);
    if (GUARD_PAGES) {
      rdata = t->rdata =
        (struct qthread_runtime_data_s *)(((uint8_t *)stack) + getpagesize() +
                                          qlib->qthread_stack_size);
    } else {
      rdata = t->rdata =
        (struct qthread_runtime_data_s *)(((uint8_t *)stack) +
                                          qlib->qthread_stack_size);
    }
  }
  rdata->tasklocal_size = 0;
  rdata->criticalsect = 0;
  rdata->stack = stack;
  rdata->shepherd_ptr = me;
  rdata->blockedon.io = NULL;
#ifdef QTHREAD_USE_VALGRIND
  if (stack) {
    rdata->valgrind_stack_id =
      VALGRIND_STACK_REGISTER(stack, qlib->qthread_stack_size);
  }
#endif
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
  atomic_store_explicit(
    &rdata->tsan_fiber, __tsan_create_fiber(0u), memory_order_relaxed);
  assert(atomic_load_explicit(&rdata->tsan_fiber, memory_order_relaxed));
#endif
#endif
}

#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
#define qthread_before_swap_to_qthread(t)                                      \
  do {                                                                         \
    void *tsan_local_fiber =                                                   \
      atomic_load_explicit(&t->rdata->tsan_fiber, memory_order_relaxed);       \
    atomic_store_explicit(&t->rdata->tsan_fiber,                               \
                          __tsan_get_current_fiber(),                          \
                          memory_order_relaxed);                               \
    __tsan_switch_to_fiber(tsan_local_fiber, 0u);                              \
  } while (0)

#define qthread_after_swap_to_qthread(t)

#define qthread_before_swap_from_qthread(t)                                    \
  do {                                                                         \
    void *tsan_local_fiber =                                                   \
      atomic_load_explicit(&t->rdata->tsan_fiber, memory_order_relaxed);       \
    atomic_store_explicit(&t->rdata->tsan_fiber,                               \
                          __tsan_get_current_fiber(),                          \
                          memory_order_relaxed);                               \
    __tsan_switch_to_fiber(tsan_local_fiber, 0u);                              \
  } while (0)

#define qthread_after_swap_from_qthread(t)

#define qthread_before_swap_to_main()                                          \
  do {                                                                         \
    void *tsan_local_fiber_main = qlib->tsan_main_fiber;                       \
    qlib->tsan_main_fiber = __tsan_get_current_fiber();                        \
    __tsan_switch_to_fiber(tsan_local_fiber_main, 0u);                         \
  } while (0)

#define qthread_after_swap_to_main()

#define qthread_before_swap_from_main()                                        \
  do {                                                                         \
    void *tsan_local_fiber_main = qlib->tsan_main_fiber;                       \
    qlib->tsan_main_fiber = __tsan_get_current_fiber();                        \
    __tsan_switch_to_fiber(tsan_local_fiber_main, 0u);                         \
  } while (0)

#define qthread_after_swap_from_main()

#else
#define qthread_before_swap_to_qthread(t)
#define qthread_after_swap_to_qthread(t)
#define qthread_before_swap_from_qthread(t)
#define qthread_after_swap_from_qthread(t)
#define qthread_before_swap_to_main()
#define qthread_after_swap_to_main()
#define qthread_before_swap_from_main()
#define qthread_after_swap_from_main()
#endif
#else
#define qthread_before_swap_to_qthread(t)
#define qthread_after_swap_to_qthread(t)
#define qthread_before_swap_from_qthread(t)
#define qthread_after_swap_from_qthread(t)
#define qthread_before_swap_to_main()
#define qthread_after_swap_to_main()
#define qthread_before_swap_from_main()
#define qthread_after_swap_from_main()
#endif

/* the qthread_master() function is the loop responsible for actually
 * executing the work units
 *
 * this function is the workhorse of the library: this is the function that
 * gets spawned several times and runs all the qthreads. */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void *qthread_master(void *arg);

static void *qthread_master_wrapper(unsigned int high, unsigned int low) {
  qthread_shepherd_t *me =
    (qthread_shepherd_t *)((((uintptr_t)high) << 32) | low);

  return qthread_master(me);
}

#endif /* ifdef QTHREAD_MAKECONTEXT_SPLIT */
#include <time.h>
extern int volatile *allowed_workers;

static void *qthread_master(void *arg) {
  qthread_worker_t *me_worker = (qthread_worker_t *)arg;
  qthread_shepherd_t *me = (qthread_shepherd_t *)me_worker->shepherd;
  qthread_shepherd_id_t my_id = me->shepherd_id;
  if (my_id == 0 && me_worker->worker_id == 0) { qthread_after_swap_to_main(); }
  qt_context_t my_context;
  qt_threadqueue_t *threadqueue;
  qthread_t *t;
  qthread_t *_Atomic *current;
  int done = 0;

  assert(me != NULL);
  assert(me->shepherd_id <= qlib->nshepherds);
#ifndef NDEBUG
  if ((shep0arg != NULL) && (my_id == 0)) {
    if (arg != shep0arg) {
      print_error("arg = %p, shep0arg = %p\n", arg, shep0arg);
    }
    assert(arg == shep0arg);
    shep0arg = NULL;
  }
#endif

  /*******************************************************************************/
  /* Initialize myself */
  /*******************************************************************************/
  TLS_SET(shepherd_structs, arg);

  current = &(me_worker->current);

  if (qaffinity && (me->node != UINT_MAX)) {
    qt_affinity_set(me_worker, qlib->nworkerspershep);
  }

  /*******************************************************************************/
  /* Workhorse Loop */
  /*******************************************************************************/

  threadqueue = me->ready;
  assert(threadqueue);
  while (!done) {
    while (!atomic_load_explicit(&me_worker->active, memory_order_relaxed)) {
      SPINLOCK_BODY();
    }
    t = qt_scheduler_get_thread(
      threadqueue, atomic_load_explicit(&me->active, memory_order_relaxed));
    assert(t);

    // Process input preconditions if this is a NASCENT thread
    if (atomic_load_explicit(&t->thread_state, memory_order_relaxed) ==
        QTHREAD_STATE_NASCENT) {
      assert(0 && "All preconditions should be satisfied before reaching the "
                  "main scheduling loop");
      if (qthread_check_feb_preconds(t) == 1) { continue; }
    }

    if (atomic_load_explicit(&t->thread_state, memory_order_relaxed) ==
        QTHREAD_STATE_TERM_SHEP) {
      done = 1;
      qthread_thread_free(t); /* free qthread data structures */
    } else {
      /* yielded only happens for the first thread */
      assert((t->thread_state == QTHREAD_STATE_NEW) ||
             (t->thread_state == QTHREAD_STATE_RUNNING) ||
             (t->thread_state == QTHREAD_STATE_YIELDED &&
              atomic_load_explicit(&t->flags, memory_order_relaxed) &
                QTHREAD_REAL_MCCOY));

      assert(t->f != NULL ||
             atomic_load_explicit(&t->flags, memory_order_relaxed) &
               QTHREAD_REAL_MCCOY);
      if (t->rdata == NULL) {
        alloc_rdata(me, t);
      } else {
        assert(t->rdata->shepherd_ptr != NULL);
        if (t->rdata->shepherd_ptr != me) { t->rdata->shepherd_ptr = me; }
      }

      if ((t->target_shepherd != NO_SHEPHERD) &&
          (t->target_shepherd != my_id) &&
          atomic_load_explicit(&qlib->shepherds[t->target_shepherd].active,
                               memory_order_relaxed)) {
        /* send this thread home */
        t->rdata->shepherd_ptr = &qlib->shepherds[t->target_shepherd];
        assert(t->rdata->shepherd_ptr->ready != NULL);
        qt_threadqueue_enqueue(qlib->shepherds[t->target_shepherd].ready, t);
      } else if (!atomic_load_explicit(&me->active, memory_order_relaxed)) {
        if ((t->target_shepherd == NO_SHEPHERD) ||
            (t->target_shepherd == my_id)) {
          /* send to the closest shepherd */
          assert(me->sorted_sheplist);
          assert(me->shep_dists);
          t->rdata->shepherd_ptr =
            qthread_find_active_shepherd(me->sorted_sheplist, me->shep_dists);
        } else {
          /* find a shepherd somewhere near the preferred shepherd
           *
           * Note: if the preferred shep was active, we would have sent
           * this thread home above */
          t->rdata->shepherd_ptr = qthread_find_active_shepherd(
            qlib->shepherds[t->target_shepherd].sorted_sheplist,
            qlib->shepherds[t->target_shepherd].shep_dists);
        }
        assert(t->rdata->shepherd_ptr);
        if (t->rdata->shepherd_ptr == NULL) { t->rdata->shepherd_ptr = me; }
        assert(t->rdata->shepherd_ptr->ready != NULL);
        qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t);
      } else { /* me->active */

        *current = t;

#ifdef USE_SYSTEM_SWAPCONTEXT
        getcontext(&my_context);
#endif
        qthread_exec(t, &my_context);

        t = *current; // necessary for direct-swap sanity
        atomic_store_explicit(
          current, NULL, memory_order_relaxed); // neessary for "queue sanity"

        /* now clean up, based on the thread's state */
        switch (atomic_load_explicit(&t->thread_state, memory_order_relaxed)) {
          case QTHREAD_STATE_MIGRATING:
            atomic_store_explicit(
              &t->thread_state, QTHREAD_STATE_RUNNING, memory_order_relaxed);
            t->rdata->shepherd_ptr = &qlib->shepherds[t->target_shepherd];
            assert(t->rdata->shepherd_ptr->ready != NULL);
            qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t);
            break;
          default: assert(0 && "Illegal thread state"); break;

          case QTHREAD_STATE_YIELDED_NEAR: /* reschedule it */
            atomic_store_explicit(
              &t->thread_state, QTHREAD_STATE_RUNNING, memory_order_relaxed);
            {
              qthread_t *f = qt_scheduler_get_thread(
                threadqueue,
                atomic_load_explicit(&me->active, memory_order_relaxed));
              qt_threadqueue_enqueue(me->ready, t);
              qt_threadqueue_enqueue(me->ready, f);
            }
            break;
          case QTHREAD_STATE_YIELDED: /* reschedule it */
            atomic_store_explicit(
              &t->thread_state, QTHREAD_STATE_RUNNING, memory_order_relaxed);
            assert(me->ready != NULL);
            qt_threadqueue_enqueue_yielded(me->ready, t);
            break;

          case QTHREAD_STATE_QUEUE: {
            qthread_queue_t q = t->rdata->blockedon.queue;
            assert(q);
            qthread_queue_internal_enqueue(q, t);
            break;
          }
          case QTHREAD_STATE_FEB_BLOCKED: /* unlock the related FEB address
                                             locks, and re-arrange memory to be
                                             correct */
          {
            qthread_addrstat_t *m = t->rdata->blockedon.addr;
            QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
            break;
          }

          case QTHREAD_STATE_PARENT_YIELD:
            atomic_store_explicit(&t->thread_state,
                                  QTHREAD_STATE_PARENT_BLOCKED,
                                  memory_order_relaxed);
            break;

          case QTHREAD_STATE_PARENT_BLOCKED:
            assert(0 && "Illegal thread state.");
            break;

          case QTHREAD_STATE_PARENT_UNBLOCKED:
            assert(0 && "Illegal thread state.");
            break;
          case QTHREAD_STATE_SYSCALL:
            atomic_store_explicit(
              &t->thread_state, QTHREAD_STATE_RUNNING, memory_order_relaxed);
            qt_blocking_subsystem_enqueue(t->rdata->blockedon.io);
            break;
          case QTHREAD_STATE_TERMINATED:
            /* we can remove the stack etc. */
            Q_PREFETCH(threadqueue);
            qthread_thread_free(t);
            break;
        }
      }
    }
  }

  if (my_id == 0 && me_worker->worker_id == 0) {
    qthread_before_swap_from_main();
  }

  pthread_exit(NULL);
  return NULL;
}

int API_FUNC qthread_init(qthread_shepherd_id_t nshepherds) {
  char newenv[100];

  snprintf(newenv, 99, "%i", (int)nshepherds);
  qassert(setenv("QT_NUM_SHEPHERDS", newenv, 1), 0);
  qassert(setenv("QT_NUM_WORKERS_PER_SHEPHERD", "1", 1), 0);
  return qthread_initialize();
}

/**
 *
 *
 *
 * Use these functions to initialize the qthreads environment before using any
 * other qthread functions. The <b>qthread_init()</b> function is deprecated in
 * favor of <b>qthread_initialize()</b> which will attempt to auto-detect the
 * correct number of shepherds for the system. The number of shepherds can be
 * forcibly specified with the environment variable QTHREAD_NUM_SHEPHERDS. The
 * <b>qthread_init()</b> function is a wrapper around
 * <b>qthread_initialize()</b> that simply exports the
 <b>QTHREAD_NUM_SHEPHERDS</b>
 * environment variable. If <b>QTHREAD_NUM_SHEPHERDS</b> is 0 or unset, the
 library
 * will attempt to guess the correct number of shepherds, defaulting to a single
 * shepherd if no information about the system could be found. Shepherds will
 * attempt to pin themselves to processors using whatever CPU affinity libraries
 * are available.

 * @name qthread_initialize
 *
 * @synopsis
 *
 * @return int On success, the system is ready to fork threads and 0 is
 *         returned. On error, an non-zero error code is returned.
 *
 * @error ENOMEM Not enough memory could be allocated.
 */

int API_FUNC qthread_initialize(void) {
  int r;
  size_t i;
  uint_fast8_t print_info = 0;
  uint_fast8_t need_sync = 1;
  qthread_shepherd_id_t nshepherds = 0;
  qthread_worker_id_t nworkerspershep = 0;
  size_t hw_par = 0;
  extern unsigned int QTHREAD_LOCKING_STRIPES;
  // qtlog(1,"qthread_initialize");
  print_info = qt_internal_get_env_num("INFO", 0, 1);

  QTHREAD_FASTLOCK_SETUP();

  if (qlib != NULL) { return QTHREAD_SUCCESS; }
  qlib = (qlib_t)MALLOC(sizeof(struct qlib_s));
  qassert_ret(qlib, QTHREAD_MALLOC_ERROR);

  qt_internal_alignment_init();
  qt_hash_initialize_subsystem();

#ifndef NDEBUG
  qthread_library_initialized = 1;
  MACHINE_FENCE;
#endif

  qt_topology_init(&nshepherds, &nworkerspershep, &hw_par);

  if ((nshepherds == 1) && (nworkerspershep == 1)) { need_sync = 0; }
  QTHREAD_LOCKING_STRIPES = 2 << (QT_INT_LOG(nshepherds * nworkerspershep) + 1);

#ifdef QTHREAD_COUNT_THREADS
  threadcount = 1;
  maxconcurrentthreads = 1;
  maxeffconcurrentthreads = 1;
  concurrentthreads = 1;
  effconcurrentthreads = 1;
  avg_concurrent_threads = 1;
  avg_eff_concurrent_threads = 1;
  QTHREAD_FASTLOCK_INIT(concurrentthreads_lock);
  QTHREAD_FASTLOCK_INIT(effconcurrentthreads_lock);
#endif

  /* initialize the kernel threads and scheduler */
  TLS_INIT(shepherd_structs);
  qlib->nshepherds = nshepherds;
  qlib->nworkerspershep = nworkerspershep;
  qlib->nshepherds_active = nshepherds;
  qlib->shepherds =
    (qthread_shepherd_t *)qt_calloc(nshepherds, sizeof(qthread_shepherd_t));
  qlib->threadqueues =
    (qt_threadqueue_t **)MALLOC(nshepherds * sizeof(qt_threadqueue_t *));
  qassert_ret(qlib->shepherds, QTHREAD_MALLOC_ERROR);

  qt_mpool_subsystem_init();

  qlib->qthread_stack_size = qt_internal_get_env_num(
    "STACK_SIZE", QTHREAD_DEFAULT_STACK_SIZE, QTHREAD_DEFAULT_STACK_SIZE);
#ifdef QTHREAD_GUARD_PAGES
  GUARD_PAGES = qt_internal_get_env_bool("GUARD_PAGES", 1);
#endif
  if (GUARD_PAGES) {
    if (print_info) { print_status("Guard Pages Enabled\n"); }
    /* round stack size to nearest page */
    if (qlib->qthread_stack_size % pagesize) {
      qlib->qthread_stack_size +=
        pagesize - (qlib->qthread_stack_size % pagesize);
    }
  }
  if (print_info) {
    print_status("Using %u byte stack size.\n", qlib->qthread_stack_size);
  }

  qlib->max_thread_id = 1;
  qlib->max_unique_id = 1;
  qlib->sched_shepherd = 0;
  QTHREAD_FASTLOCK_INIT(qlib->max_thread_id_lock);
  QTHREAD_FASTLOCK_INIT(qlib->max_unique_id_lock);
  QTHREAD_FASTLOCK_INIT(qlib->sched_shepherd_lock);

  {
    struct rlimit rlp;

    qassert(getrlimit(RLIMIT_STACK, &rlp), 0);
    if ((rlp.rlim_cur == RLIM_INFINITY) || (rlp.rlim_cur == 0)) {
      if (rlp.rlim_cur == 0) {}
      rlp.rlim_cur = 8 * 1024 * 1024;
    }
    qlib->master_stack_size = rlp.rlim_cur;
    if (rlp.rlim_max < rlp.rlim_cur) { rlp.rlim_max = rlp.rlim_cur; }
    qlib->max_stack_size = rlp.rlim_max;
  }

  /* initialize the shepherds as having no affinity */
  for (i = 0; i < nshepherds; i++) {
    qlib->shepherds[i].node = UINT32_MAX;
    qlib->shepherds[i].shep_dists = NULL;
    qlib->shepherds[i].sorted_sheplist = NULL;
    qlib->shepherds[i].workers =
      (qthread_worker_t *)qt_calloc(nworkerspershep, sizeof(qthread_worker_t));
    qassert_ret(qlib->shepherds[i].workers, QTHREAD_MALLOC_ERROR);
  }
  qaffinity = qt_internal_get_env_bool("AFFINITY", 1);

  {
    int ret = qt_affinity_gendists(qlib->shepherds, nshepherds);
    if (ret != QTHREAD_SUCCESS) { return ret; }
    assert(qlib->shepherds[0].sorted_sheplist);
    assert(qlib->shepherds[0].shep_dists);
  }

  // Set task argument buffer size
  qlib->qthread_argcopy_size =
    qt_internal_get_env_num("ARGCOPY_SIZE", ARGCOPY_DEFAULT, 0);

  // Set task-local data size
  qlib->qthread_tasklocal_size = qt_internal_get_env_num(
    "TASKLOCAL_SIZE", TASKLOCAL_DEFAULT, sizeof(void *));

  generic_qthread_pool = qt_mpool_create_aligned(
    sizeof(qthread_t) + sizeof(void *) + qlib->qthread_tasklocal_size,
    qthread_cacheline());
  generic_big_qthread_pool =
    qt_mpool_create(sizeof(qthread_t) + qlib->qthread_argcopy_size +
                    qlib->qthread_tasklocal_size);
  if (GUARD_PAGES) {
    generic_stack_pool = qt_mpool_create_aligned(
      qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s) +
        (2 * getpagesize()),
      getpagesize());
  } else {
    generic_stack_pool = qt_mpool_create_aligned(
      qlib->qthread_stack_size + sizeof(struct qthread_runtime_data_s),
      QTHREAD_STACK_ALIGNMENT);
  }
  generic_rdata_pool = qt_mpool_create(sizeof(struct qthread_runtime_data_s));
  initialize_hazardptrs();
  qt_internal_teams_init();
  qthread_queue_subsystem_init();
  qt_feb_subsystem_init(need_sync);
  qt_syncvar_subsystem_init(need_sync);
  qt_threadqueue_subsystem_init();
  qt_blocking_subsystem_init();

  /* initialize the shepherd structures */
  for (i = 0; i < nshepherds; i++) {
    qlib->shepherds[i].shepherd_id = (qthread_shepherd_id_t)i;
    QTHREAD_CASLOCK_INIT(qlib->shepherds[i].active, 1);
    qlib->shepherds[i].ready = qt_threadqueue_new();
    qassert_ret(qlib->shepherds[i].ready, QTHREAD_MALLOC_ERROR);
    qlib->threadqueues[i] = qlib->shepherds[i].ready;
  }

  spinlocks_initialize();

  /* now, transform the current main context into a qthread,
   * and make the main thread a shepherd (shepherd 0).
   * What will happen is this:
   * qlib->mccoy_thread represents the original execution thread, and so will
   * receive a context based on the current execution state.
   * qlib->master_context is a context for the new shepherd that will be
   * created (shepherd 0).
   * qlib->master_stack is a stack for that shepherd, and is huge, because
   * the shepherd expects a "standard" size stack. The mccoy_thread, as it is
   * for the *current* thread, also expects a full-size stack. The point of
   * this weirdness is so that the current thread can block the same way that
   * a qthread can. */
  qlib->mccoy_thread = qthread_thread_new(NULL, NULL, 0, NULL, NULL, 0);
  qassert_ret(qlib->mccoy_thread, QTHREAD_MALLOC_ERROR);

  qlib->master_stack =
    qt_internal_aligned_alloc(qlib->master_stack_size, QTHREAD_STACK_ALIGNMENT);
  qassert_ret(qlib->master_stack, QTHREAD_MALLOC_ERROR);
#ifdef QTHREAD_USE_VALGRIND
  qlib->valgrind_masterstack_id =
    VALGRIND_STACK_REGISTER(qlib->master_stack, qlib->master_stack_size);
#endif

  /* the context will have its own stack ptr */
  atomic_store_explicit(&qlib->mccoy_thread->thread_state,
                        QTHREAD_STATE_YIELDED,
                        memory_order_relaxed); /* avoid re-launching */
  atomic_store_explicit(
    &qlib->mccoy_thread->flags,
    QTHREAD_REAL_MCCOY | QTHREAD_UNSTEALABLE,
    memory_order_relaxed); /* i.e. this is THE parent thread */
  assert(qlib->mccoy_thread->rdata == NULL);

  qlib->mccoy_thread->rdata = MALLOC(sizeof(struct qthread_runtime_data_s));

  assert(qlib->mccoy_thread->rdata != NULL);
  qlib->mccoy_thread->rdata->shepherd_ptr = &(qlib->shepherds[0]);
  qlib->mccoy_thread->rdata->stack = NULL;
  qlib->mccoy_thread->rdata->tasklocal_size = 0;

#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
  atomic_store_explicit(&qlib->mccoy_thread->rdata->tsan_fiber,
                        __tsan_get_current_fiber(),
                        memory_order_relaxed);
  qlib->tsan_main_fiber = __tsan_create_fiber(0u);
  assert(atomic_load_explicit(&qlib->mccoy_thread->rdata->tsan_fiber,
                              memory_order_relaxed) &&
         qlib->tsan_main_fiber);
#endif
#endif
  TLS_SET(shepherd_structs,
          (qthread_shepherd_t *)&(qlib->shepherds[0].workers[0]));
  qt_threadqueue_enqueue(qlib->shepherds[0].ready, qlib->mccoy_thread);
  qassert(getcontext(&(qlib->mccoy_thread->rdata->context)), 0);
  /* now build the context for the shepherd 0 */
  qlib->shepherds[0].workers[0].worker = pthread_self();
  qlib->shepherds[0].workers[0].shepherd = &qlib->shepherds[0];
  atomic_store_explicit(
    &qlib->shepherds[0].workers[0].active, 1, memory_order_relaxed);
  qlib->shepherds[0].workers[0].worker_id = 0;
  qlib->shepherds[0].workers[0].unique_id =
    qthread_internal_incr(&(qlib->max_unique_id), &qlib->max_unique_id_lock, 1);
  qthread_makecontext(&(qlib->master_context),
                      qlib->master_stack,
                      qlib->master_stack_size,
#ifdef QTHREAD_MAKECONTEXT_SPLIT
                      (void (*)(void))qthread_master_wrapper,
#else
                      (void (*)(void))qthread_master,
#endif
                      &(qlib->shepherds[0].workers[0]),
                      &(qlib->mccoy_thread->rdata->context));
#ifndef NDEBUG
  shep0arg = &(qlib->shepherds[0].workers[0]);
#endif
  /* this launches shepherd 0 */
#ifdef QTHREAD_USE_VALGRIND
  VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&qlib->mccoy_thread->rdata->context,
                                    sizeof(qt_context_t));
  VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&(qlib->master_context),
                                    sizeof(qt_context_t));
  VALGRIND_MAKE_MEM_DEFINED(&qlib->mccoy_thread->rdata->context,
                            sizeof(qt_context_t));
  VALGRIND_MAKE_MEM_DEFINED(&(qlib->master_context), sizeof(qt_context_t));
#endif

  qthread_before_swap_to_main();

#ifdef USE_SYSTEM_SWAPCONTEXT
  qassert(
    swapcontext(&qlib->mccoy_thread->rdata->context, &(qlib->master_context)),
    0);
#else
  qassert(
    qt_swapctxt(&qlib->mccoy_thread->rdata->context, &(qlib->master_context)),
    0);
#endif

  qthread_after_swap_from_main();

  assert(hw_par > 0);
  qlib->nworkers_active = hw_par;

  /* spawn the shepherds */
  /*  XXX: where does WKR_DBG come from? */
  for (i = 0; i < nshepherds; ++i) {
    qthread_worker_id_t j;
    for (j = 0; j < nworkerspershep; ++j) {
      qlib->shepherds[i].workers[j].nostealbuffer =
        qt_calloc(STEAL_BUFFER_LENGTH, sizeof(qthread_t *));
      qlib->shepherds[i].workers[j].stealbuffer =
        qt_calloc(STEAL_BUFFER_LENGTH, sizeof(qthread_t *));

      if ((i == 0) && (j == 0)) {
        continue; // original pthread becomes shep 0 worker 0
      }
      qlib->shepherds[i].workers[j].shepherd = &qlib->shepherds[i];
      qlib->shepherds[i].workers[j].worker_id = j;
      qlib->shepherds[i].workers[j].unique_id = qthread_internal_incr(
        &(qlib->max_unique_id), &qlib->max_unique_id_lock, 1);
      qlib->shepherds[i].workers[j].packed_worker_id =
        j + (i * nworkerspershep);

      if ((j * nshepherds) + i + 1 > hw_par) {
        atomic_store_explicit(
          &qlib->shepherds[i].workers[j].active, 0, memory_order_relaxed);
      } else {
        atomic_store_explicit(
          &qlib->shepherds[i].workers[j].active, 1, memory_order_relaxed);
      }
      qlib->shepherds[i].workers[j].shepherd = &qlib->shepherds[i];
      if ((r = pthread_create(&qlib->shepherds[i].workers[j].worker,
                              NULL,
                              qthread_master,
                              &qlib->shepherds[i].workers[j])) != 0) {
        print_error("qthread_init: pthread_create() failed (%d): %s\n",
                    r,
                    strerror(errno));
        return QTHREAD_THIRD_PARTY_ERROR;
      }
    }
  }

  atexit(qthread_finalize);

  qt_barrier_internal_init();

  return QTHREAD_SUCCESS;
}

/* This initializes a context (c) to run the function (func) with a single
 * argument (arg). This is just a wrapper around makecontext that isolates some
 * of the portability garbage. */
static inline void qthread_makecontext(qt_context_t *const c,
                                       void *const stack,
                                       size_t const stacksize,
                                       void (*func)(void),
                                       void const *const arg,
                                       qt_context_t *const returnc) {
#ifdef QTHREAD_MAKECONTEXT_SPLIT
  unsigned int const high = ((uintptr_t)arg) >> 32;
  unsigned int const low = ((uintptr_t)arg) & 0xffffffff;
#endif

  assert(c != NULL);
  assert(stack != NULL);
  assert(func != NULL);
  assert(arg != NULL);
  assert(returnc != NULL);

  qassert(getcontext(c), 0); /* initialization required by makecontext API */

  /* Several other libraries that do this reserve a few words on either end
   * of the stack for some reason. To avoid problems, I'll also do this (even
   * though I have no idea why they do this). */
  c->uc_stack.ss_sp = (uint8_t *)(stack) + 8;
  c->uc_stack.ss_size = stacksize - 64;
#ifdef UCSTACK_HAS_SSFLAGS
  c->uc_stack.ss_flags = 0;
#endif
#ifdef USE_SYSTEM_SWAPCONTEXT
  /* the makecontext man page (Linux) says: set the uc_link FIRST.
   * why? no idea */
  c->uc_link = returnc; /* NULL pthread_exit() */
#endif
#ifdef USE_SYSTEM_SWAPCONTEXT
#ifdef QTHREAD_MAKECONTEXT_SPLIT
  makecontext(c, func, 2, high, low);
#else  /* QTHREAD_MAKECONTEXT_SPLIT */
  makecontext(c, func, 1, arg);
#endif /* QTHREAD_MAKECONTEXT_SPLIT */
  assert((void *)c->uc_link == (void *)returnc);
#else  /* ifdef USE_SYSTEM_SWAPCONTEXT */
  qt_makectxt(c, func, 1, arg);
#endif /* ifdef USE_SYSTEM_SWAPCONTEXT */
}

/* this adds a function to the list of cleanup functions to call at finalize;
 * these functions get called *way* AFTER shepherds are torn down */
void INTERNAL qthread_internal_cleanup_late(void (*function)(void)) {
  struct qt_cleanup_funcs_s *ng = MALLOC(sizeof(struct qt_cleanup_funcs_s));

  assert(ng);
  ng->func = function;
  ng->next = qt_cleanup_late_funcs;
  qt_cleanup_late_funcs = ng;
}

/* this adds a function to the list of cleanup functions to call at finalize;
 * these functions get called AFTER shepherds are torn down */
void INTERNAL qthread_internal_cleanup(void (*function)(void)) {
  struct qt_cleanup_funcs_s *ng = MALLOC(sizeof(struct qt_cleanup_funcs_s));

  assert(ng);
  ng->func = function;
  ng->next = qt_cleanup_funcs;
  qt_cleanup_funcs = ng;
}

/* this adds a function to the list of cleanup functions to call at finalize;
 * these functions get called BEFORE shepherds are torn down */
void INTERNAL qthread_internal_cleanup_early(void (*function)(void)) {
  struct qt_cleanup_funcs_s *ng = MALLOC(sizeof(struct qt_cleanup_funcs_s));

  assert(ng);
  ng->func = function;
  ng->next = qt_cleanup_early_funcs;
  qt_cleanup_early_funcs = ng;
}

void API_FUNC qthread_finalize(void) {
  int r;
  qthread_shepherd_id_t i;
  qthread_t *t;
  qthread_worker_t *worker;

  /***********************************************************************/
  /* Sanity check - THIS *MUST* BE THE FIRST CODE RUN IN QTHREAD_FINALIZE
   *
   * Those who insert other code before this will discover that bad things
   * happen, and will be forced to re-invent this code, badly. */
  /***********************************************************************/
  if ((qlib == NULL) || (qthread_shep() != 0) || (qthread_worker(NULL) != 0)) {
    /* in essence, qthread_finalize can easily be called by threads
     * external to qthreads (via the atexit() setup); if that happens, we
     * need to ignore it. */
    return;
  } else {
    /* also, if a thread other than worker0, shep0, real-mccoy calls it, we
     * should ignore that too. */
    qthread_t *t = qthread_internal_self();
    if (0 == (atomic_load_explicit(&t->flags, memory_order_relaxed) &
              QTHREAD_REAL_MCCOY)) {
      return;
    }
  }
  /***********************************************************************/

  qt_internal_teams_reclaim();

  qthread_shepherd_t *shep0 = &(qlib->shepherds[0]);

  worker = qthread_internal_getworker();
  if (worker && (worker->packed_worker_id !=
                 0)) { /* Only run finalize on shepherd 0 worker 0*/
    atomic_store_explicit(&worker->current->thread_state,
                          QTHREAD_STATE_YIELDED,
                          memory_order_relaxed); /* Otherwise, put back */
    //      qt_threadqueue_enqueue(shep0->ready, worker->current);
    return; // AKP 11/2/11 I think that is if statement catches the case that
            // exit is called within a
    // parallel region so a random stream reaches here.  we return rather than
    // requeue because we just want to exit (something bad happened) [my
    // speculation]
  }

  /* enqueue the termination thread sentinal */
  for (i = 0; i < qlib->nshepherds; i++) {
    qthread_worker_id_t j;
    for (j = 0; j < qlib->nworkerspershep; j++) {
      if ((i == 0) && (j == 0)) {
        continue; /* None for shepard 0's worker 0 */
      }
      t = qthread_thread_new(NULL, NULL, 0, NULL, NULL, 0);
      assert(t != NULL); /* what else can we do? */
      atomic_store_explicit(
        &t->thread_state, QTHREAD_STATE_TERM_SHEP, memory_order_relaxed);
      t->thread_id = QTHREAD_NON_TASK_ID;
      atomic_store_explicit(
        &t->flags, QTHREAD_UNSTEALABLE, memory_order_relaxed);
      qt_threadqueue_enqueue(qlib->shepherds[i].ready, t);
      if (!atomic_load_explicit(&qlib->shepherds[i].workers[j].active,
                                memory_order_relaxed)) {
        atomic_store_explicit(
          &qlib->shepherds[i].workers[j].active, 1, memory_order_relaxed);
      }
    }
  }

  while (qt_cleanup_early_funcs != NULL) {
    struct qt_cleanup_funcs_s *tmp = qt_cleanup_early_funcs;
    qt_cleanup_early_funcs = tmp->next;
    tmp->func();
    FREE(tmp, sizeof(struct qt_cleanup_funcs_s));
  }

  /* wait for each SPAWNED shepherd to drain it's queue
   * (note: not shepherd 0, because that one wasn't spawned) */
  /**********************************************************************
   *                   DO NOT FREE MEMORY YET!
   **********************************************************************
   * When some shepherds are still alive, they may be attempting to steal,
   * and this is a race condition to see if they access free'd memory. */
  for (i = 0; i < qlib->nshepherds; i++) {
    /* With multi-threaded shepherds, do join shepherd 0 workers, but not worker
     * 0 */
    qthread_worker_id_t j;
    qthread_shepherd_t *shep = &(qlib->shepherds[i]);
    for (j = 0; j < qlib->nworkerspershep; j++) {
      if ((i == 0) && (j == 0)) {
        continue; /* This leaves out shepherd 0's worker 0 */
      }
      if ((r = pthread_join(shep->workers[j].worker, NULL)) != 0) {
        print_error("qthread_finalize: pthread_join() of shep %i worker %i "
                    "failed (%d, or \"%s\")\n",
                    (int)i,
                    (int)j,
                    r,
                    strerror(r));
        abort();
      }
    }
    if (i == 0) { continue; }
  }
  /**********************************************************************/
  for (i = 0; i < qlib->nshepherds; i++) {
    /* With multi-threaded shepherds, do join shepherd 0 workers, but not worker
     * 0 */
    qthread_worker_id_t j;
    qthread_shepherd_t *shep = &(qlib->shepherds[i]);
    for (j = 0; j < qlib->nworkerspershep; j++) {
      if ((i == 0) && (j == 0)) {
        continue; /* This leaves out shepherd 0's worker 0 */
      }
      FREE(shep->workers[j].nostealbuffer,
           STEAL_BUFFER_LENGTH * sizeof(qthread_t *));
      FREE(shep->workers[j].stealbuffer,
           STEAL_BUFFER_LENGTH * sizeof(qthread_t *));
    }
    if (i == 0) {
      FREE(shep0->workers[0].nostealbuffer,
           STEAL_BUFFER_LENGTH * sizeof(qthread_t *));
      FREE(shep0->workers[0].stealbuffer,
           STEAL_BUFFER_LENGTH * sizeof(qthread_t *));
    }
    FREE(qlib->shepherds[i].workers,
         qlib->nworkerspershep * sizeof(qthread_worker_t));
    if (i == 0) { continue; }
    qt_threadqueue_free(shep->ready);
  }
  qt_threadqueue_free(shep0->ready);

  spinlocks_finalize();

  while (qt_cleanup_funcs != NULL) {
    struct qt_cleanup_funcs_s *tmp = qt_cleanup_funcs;
    qt_cleanup_funcs = tmp->next;
    tmp->func();
    FREE(tmp, sizeof(struct qt_cleanup_funcs_s));
  }

#ifdef LOCK_FREE_FEBS
  extern unsigned int QTHREAD_LOCKING_STRIPES;
  QTHREAD_LOCKING_STRIPES = 1;
#endif
#ifdef QTHREAD_COUNT_THREADS
  print_status("spawned %lu threads, max realized concurrency %lu, avg "
               "realized concurrency %g\n",
               (unsigned long)threadcount,
               (unsigned long)maxeffconcurrentthreads,
               avg_eff_concurrent_threads);
  print_status(
    "max theoretical concurrency %lu, avg theoretical concurrency %g\n",
    (unsigned long)maxconcurrentthreads,
    avg_concurrent_threads);
  QTHREAD_FASTLOCK_DESTROY(concurrentthreads_lock);
  QTHREAD_FASTLOCK_DESTROY(effconcurrentthreads_lock);
#endif

  QTHREAD_FASTLOCK_DESTROY(qlib->max_thread_id_lock);
  QTHREAD_FASTLOCK_DESTROY(qlib->max_unique_id_lock);
  QTHREAD_FASTLOCK_DESTROY(qlib->sched_shepherd_lock);

#ifdef QTHREAD_USE_VALGRIND
  VALGRIND_STACK_DEREGISTER(qlib->mccoy_thread->rdata->valgrind_stack_id);
  VALGRIND_STACK_DEREGISTER(qlib->valgrind_masterstack_id);
#endif
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
  assert(atomic_load_explicit(&qlib->mccoy_thread->rdata->tsan_fiber,
                              memory_order_relaxed) &&
         qlib->tsan_main_fiber);
  //__tsan_destroy_fiber(qlib->tsan_main_fiber);
#endif
#endif
  assert(qlib->mccoy_thread->rdata->stack == NULL);
  if (qlib->mccoy_thread->rdata->tasklocal_size > 0) {
    FREE(*(void **)&qlib->mccoy_thread->data[0],
         qlib->mccoy_thread->rdata->tasklocal_size);
  }
  FREE(qlib->mccoy_thread->rdata, sizeof(struct qthread_runtime_data_s));
  FREE_QTHREAD(qlib->mccoy_thread);
  FREE(qlib->master_stack, qlib->master_stack_size);
  while (qt_cleanup_late_funcs != NULL) {
    struct qt_cleanup_funcs_s *tmp = qt_cleanup_late_funcs;
    qt_cleanup_late_funcs = tmp->next;
    tmp->func();
    FREE(tmp, sizeof(struct qt_cleanup_funcs_s));
  }

  for (i = 0; i < qlib->nshepherds; ++i) {
    if (qlib->shepherds[i].shep_dists) {
      FREE(qlib->shepherds[i].shep_dists,
           (qlib->nshepherds - 1) * sizeof(unsigned int));
    }
    if (qlib->shepherds[i].sorted_sheplist) {
      FREE(qlib->shepherds[i].sorted_sheplist,
           (qlib->nshepherds - 1) * sizeof(qthread_shepherd_id_t));
    }
  }

  qt_mpool_destroy(generic_qthread_pool);
  generic_qthread_pool = NULL;
  qt_mpool_destroy(generic_big_qthread_pool);
  generic_big_qthread_pool = NULL;
  qt_mpool_destroy(generic_stack_pool);
  generic_stack_pool = NULL;
  qt_mpool_destroy(generic_rdata_pool);
  generic_rdata_pool = NULL;
  FREE(qlib->shepherds, qlib->nshepherds * sizeof(qthread_shepherd_t));
  FREE(qlib->threadqueues, qlib->nshepherds * sizeof(qt_threadqueue_t *));
  FREE(qlib, sizeof(struct qlib_s));
  qlib = NULL;
  TLS_DELETE(shepherd_structs);

  qt_topology_deinit();

#ifndef NDEBUG
  MACHINE_FENCE;
  qthread_library_initialized = 0;
  MACHINE_FENCE;
#endif
  fflush(stdout);
}

qthread_t INTERNAL *qthread_internal_self(void) {
  extern TLS_DECL(qthread_t *, IO_task_struct);

  qthread_worker_t *worker = qthread_internal_getworker();
  if (worker == NULL) {
    // this may also be null, but in the slow path, the logic is sound
    return TLS_GET(IO_task_struct);
  } else {
    return worker->current;
  }
}

void API_FUNC *qthread_get_tasklocal(unsigned int size) {
  qthread_t *f = qthread_internal_self();

  if (NULL != f) {
    unsigned int const tl_sz = f->rdata->tasklocal_size;
    if ((0 == tl_sz) && (size <= qlib->qthread_tasklocal_size)) {
      // Use default space
      if (atomic_load_explicit(&f->flags, memory_order_relaxed) &
          QTHREAD_BIG_STRUCT) {
        return &f->data[qlib->qthread_argcopy_size];
      } else {
        return &f->data;
      }
    } else {
      void **data_blob;
      if (atomic_load_explicit(&f->flags, memory_order_relaxed) &
          QTHREAD_BIG_STRUCT) {
        data_blob = (void **)&f->data[qlib->qthread_argcopy_size];
      } else {
        data_blob = (void **)&f->data[0];
      }
      if (0 == tl_sz) {
        void *tmp_data = MALLOC(size);
        assert(NULL != tmp_data);

        memcpy(tmp_data, data_blob, qlib->qthread_tasklocal_size);
        *data_blob = tmp_data;

        f->rdata->tasklocal_size = size;
        return *data_blob;
      } else if (size <= tl_sz) {
        return *data_blob;
      } else {
        *data_blob = qt_realloc(*data_blob, size);
        assert(NULL != *data_blob);

        f->rdata->tasklocal_size = size;
        return *data_blob;
      }
    }
  }

  return NULL;
}

unsigned API_FUNC qthread_size_tasklocal(void) {
  qthread_t const *f = qthread_internal_self();

  assert(NULL != f);
  assert(NULL != f->rdata);

  return f->rdata->tasklocal_size ? f->rdata->tasklocal_size
                                  : qlib->qthread_tasklocal_size;
}

API_FUNC void *qthread_tos(void) {
  qthread_t const *f = qthread_internal_self();

  return f->rdata->stack;
}

API_FUNC void *qthread_bos(void) {
  qthread_t const *f = qthread_internal_self();

  return (void *)((char *)f->rdata->stack + qlib->qthread_stack_size);
}

size_t API_FUNC qthread_stackleft(void) {
  qthread_t const *f = qthread_internal_self();

  if ((f != NULL) && (f->rdata->stack != NULL)) {
#if defined(__INTEL_COMPILER)
    size_t current = (size_t)&f;
#else
    size_t current = (size_t)__builtin_frame_address(0);
#endif
    assert(current > (size_t)f->rdata->stack &&
           current < ((size_t)f->rdata->stack + qlib->qthread_stack_size));
#ifdef STACK_GROWS_DOWN
    /* not tested */
    assert(((size_t)(f->rdata->stack) + qlib->qthread_stack_size) - current <
           qlib->qthread_stack_size);
    return ((size_t)(f->rdata->stack) + qlib->qthread_stack_size) - current;

#else
    assert(current - (size_t)(f->rdata->stack) < qlib->qthread_stack_size);
    return current - (size_t)(f->rdata->stack);
#endif
  } else {
    return 0;
  }
}

size_t API_FUNC qthread_readstate(const enum introspective_state type) {
  switch (type) {
    case STACK_SIZE: return qlib->qthread_stack_size;

    case RUNTIME_DATA_SIZE: return sizeof(struct qthread_runtime_data_s);

    case BUSYNESS: {
      qthread_shepherd_t *shep = qthread_internal_getshep();
      if (shep == NULL) {
        return (size_t)(-1);
      } else {
        return qt_threadqueue_advisory_queuelen(shep->ready) + 1;
      }
    }
    case NODE_BUSYNESS: {
      size_t sum = 0;
      qthread_shepherd_t const *sheps = qlib->shepherds;
      for (qthread_shepherd_id_t s = 0; s < qlib->nshepherds; s++) {
        sum += qt_threadqueue_advisory_queuelen(sheps[s].ready);
        qthread_worker_t const *wkrs = sheps[s].workers;
        for (qthread_worker_id_t w = 0; w < qlib->nworkerspershep; w++) {
          sum += (atomic_load_explicit(&wkrs[w].current,
                                       memory_order_relaxed) != NULL);
        }
      }
      return sum;
    }
    case WORKER_OCCUPATION: {
      size_t count = 0;
      qthread_shepherd_t const *sheps = qlib->shepherds;
      for (qthread_shepherd_id_t s = 0; s < qlib->nshepherds; s++) {
        qthread_worker_t const *wkrs = sheps[s].workers;
        for (qthread_worker_id_t w = 0; w < qlib->nworkerspershep; w++) {
          count += (atomic_load_explicit(&wkrs[w].current,
                                         memory_order_relaxed) != NULL);
        }
      }
      return count;
    }
    case ACTIVE_SHEPHERDS: return (size_t)(qlib->nshepherds_active);

    case TOTAL_SHEPHERDS: return (size_t)(qlib->nshepherds);

    case ACTIVE_WORKERS: return (size_t)(qlib->nworkers_active);

    case TOTAL_WORKERS:
      return (size_t)(qlib->nworkerspershep * qlib->nshepherds);

    case CURRENT_SHEPHERD: return qthread_shep();

    case CURRENT_WORKER: {
      qthread_worker_t *worker = (qthread_worker_t *)TLS_GET(shepherd_structs);

      return worker ? (worker->worker_id) : NO_WORKER;
    }

    case CURRENT_UNIQUE_WORKER: {
      qthread_worker_t *worker = (qthread_worker_t *)TLS_GET(shepherd_structs);

      return worker ? (worker->unique_id - 1) : NO_WORKER;
    }

    case CURRENT_TEAM:
      if (NULL != qlib) {
        qthread_t *self = qthread_internal_self();

        if ((NULL != self) && (NULL != self->team)) {
          return self->team->team_id;
        } else {
          return 1;
        }
      } else {
        return 1;
      }

    case PARENT_TEAM:
      if (NULL != qlib) {
        qthread_t *self = qthread_internal_self();

        if ((NULL != self) && (NULL != self->team)) {
          return self->team->parent_id;
        } else {
          return 0;
        }
      } else {
        return 0;
      }

    default: return (size_t)(-1);
  }
}

aligned_t API_FUNC *qthread_retloc(void) {
  qthread_t *me = qthread_internal_self();

  if (me) {
    return me->ret;
  } else {
    return NULL;
  }
}

/************************************************************/
/* functions to manage thread stack allocation/deallocation */
/************************************************************/
static inline qthread_t *qthread_thread_new(qthread_f const f,
                                            void const *arg,
                                            size_t arg_size,
                                            void *ret,
                                            qt_team_t *team,
                                            int team_leader) {
  qthread_t *t;

  if ((arg_size > 0) && (arg_size <= qlib->qthread_argcopy_size)) {
    t = ALLOC_BIG_QTHREAD();
  } else {
    t = ALLOC_QTHREAD();
  }

  t->f = f;
  t->arg = (void *)arg;
  t->ret = ret;
  t->rdata = NULL;
  t->team = team;

  t->thread_id = QTHREAD_NON_TASK_ID;

  t->target_shepherd = NO_SHEPHERD;

  // should I use the builtin block for args?
  if (arg_size > 0) {
    if (arg_size <= qlib->qthread_argcopy_size) {
      t->arg = (void *)(&t->data);
      atomic_store_explicit(
        &t->flags, QTHREAD_BIG_STRUCT, memory_order_relaxed);
    } else {
      t->arg = MALLOC(arg_size);
      atomic_store_explicit(
        &t->flags, QTHREAD_HAS_ARGCOPY, memory_order_relaxed);
    }
    memcpy(t->arg, arg, arg_size);
  } else {
    atomic_store_explicit(&t->flags, 0, memory_order_relaxed);
  }

  // am I the team leader?
  if (team_leader) {
    atomic_fetch_or_explicit(
      &t->flags, QTHREAD_TEAM_LEADER, memory_order_relaxed);
  }

  atomic_store_explicit(
    &t->thread_state, QTHREAD_STATE_NEW, memory_order_relaxed);

  return t;
}

void qthread_thread_free(qthread_t *t) {
  assert(t != NULL);

  if (t->rdata != NULL) {
    if (t->rdata->tasklocal_size > 0) {
      if (atomic_load_explicit(&t->flags, memory_order_relaxed) &
          QTHREAD_BIG_STRUCT) {
        FREE(*(void **)&t->data[qlib->qthread_argcopy_size],
             t->rdata->tasklocal_size);
        *(void **)&t->data[qlib->qthread_argcopy_size] = NULL;
      } else {
        FREE(*(void **)&t->data[0], t->rdata->tasklocal_size);
        *(void **)&t->data[0] = NULL;
      }
    }
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_STACK_DEREGISTER(t->rdata->valgrind_stack_id);
#endif
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
    assert(atomic_load_explicit(&t->rdata->tsan_fiber, memory_order_relaxed));
    //__tsan_destroy_fiber(t->rdata->tsan_fiber);
#endif
#endif
    if (atomic_load_explicit(&t->flags, memory_order_relaxed) &
        QTHREAD_SIMPLE) {
      FREE_RDATA(t->rdata);
    } else {
      assert(t->rdata->stack);
      FREE_STACK(t->rdata->stack);
    }

    t->rdata = NULL;
  }
  if (atomic_load_explicit(&t->flags, memory_order_relaxed) &
      QTHREAD_HAS_ARGCOPY) {
    assert(&t->data != t->arg);
    qt_free(t->arg); // I don't record the size of this anywhere, so I can't
                     // scribble it
    t->arg = NULL;
  }
  if (atomic_load_explicit(&t->flags, memory_order_relaxed) &
      QTHREAD_BIG_STRUCT) {
    FREE_BIG_QTHREAD(t);
  } else {
    FREE_QTHREAD(t);
  }
}

#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING

int in_qthread_fence(void *addr);
extern void *qthread_fence1;
extern void *qthread_fence2;

#define MONITOR_ASM_LABEL(name)                                                \
  __asm__ __volatile__(".globl " #name);                                       \
  __asm__ __volatile__(#name ":")
#endif /* ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING */

void API_FUNC
qthread_call_method(qthread_f f, void *arg, void *ret, uint16_t flags) {
  if (ret) {
    if (flags & QTHREAD_RET_IS_SINC) {
      if (flags & QTHREAD_RET_IS_VOID_SINC) {
        (f)(arg);
        qt_sinc_submit((qt_sinc_t *)ret, NULL);
      } else {
        aligned_t retval = (f)(arg);
        qt_sinc_submit((qt_sinc_t *)ret, &retval);
      }
    } else if (flags & QTHREAD_RET_IS_SYNCVAR) {
      /* this should avoid problems with irresponsible return values */
      uint64_t retval = INT64TOINT60((f)(arg));
      qassert(qthread_syncvar_writeEF_const((syncvar_t *)ret, retval),
              QTHREAD_SUCCESS);
    } else {
      aligned_t retval = (f)(arg);
      qassert(qthread_writeEF_const((aligned_t *)ret, retval), QTHREAD_SUCCESS);
    }
  } else (f)(arg);
}

/* this function runs a thread until it completes or yields */
#ifdef QTHREAD_MAKECONTEXT_SPLIT
static void qthread_wrapper(unsigned int high, unsigned int low) {
  qthread_t *t = (qthread_t *)((((uintptr_t)high) << 32) | low);

#else
static void qthread_wrapper(void *ptr) {
  qthread_t *t = (qthread_t *)ptr;
#endif
  if ((t->flags & QTHREAD_SIMPLE) == 0) { qthread_after_swap_to_qthread(t); }
#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
  MONITOR_ASM_LABEL(qthread_fence1); // add label for HPCToolkit stack unwind
#endif

  if (atomic_load_explicit(&t->thread_state, memory_order_relaxed) ==
      QTHREAD_STATE_YIELDED) {
    /* This means that I've direct-swapped, and need to clean up a little. */
    qthread_t *prev_t = t->rdata->blockedon.thread;
    atomic_store_explicit(
      &t->thread_state, QTHREAD_STATE_RUNNING, memory_order_relaxed);
    assert(prev_t->rdata);
    assert(prev_t->rdata->shepherd_ptr);
    assert(prev_t->rdata->shepherd_ptr->ready);
    assert(t->rdata);
    assert(t->rdata->shepherd_ptr);
    assert(t->rdata->shepherd_ptr->ready);
    assert(prev_t->thread_state == QTHREAD_STATE_RUNNING);
    qthread_worker_t *me_worker = (qthread_worker_t *)TLS_GET(shepherd_structs);
    me_worker->current = t;
    qt_threadqueue_enqueue_yielded(t->rdata->shepherd_ptr->ready, prev_t);
  }

  if ((atomic_load_explicit(&t->flags, memory_order_relaxed) &
       QTHREAD_SIMPLE) == 0) {
    assert((size_t)&t > (size_t)t->rdata->stack &&
           (size_t)&t < ((size_t)t->rdata->stack + qlib->qthread_stack_size));
  }
#ifdef QTHREAD_COUNT_THREADS
  QTHREAD_FASTLOCK_LOCK(&effconcurrentthreads_lock);
  effconcurrentthreads++;
  if (effconcurrentthreads > maxeffconcurrentthreads) {
    maxeffconcurrentthreads = effconcurrentthreads;
  }
  avg_eff_concurrent_threads =
    (avg_eff_concurrent_threads * (double)(threadcount - 1.0) / threadcount) +
    ((double)effconcurrentthreads / threadcount);
  QTHREAD_FASTLOCK_UNLOCK(&effconcurrentthreads_lock);
#endif /* ifdef QTHREAD_COUNT_THREADS */

  if ((NULL != t->team) &&
      (atomic_load_explicit(&t->flags, memory_order_relaxed) &
       QTHREAD_TEAM_LEADER)) {
    if (NULL != t->team->parent_eureka) {
      // This is a subteam's team-leader
      qt_internal_subteam_leader(t);
    }
  }

  assert(t->rdata);
  if (t->ret) {
    if (atomic_load_explicit(&t->flags, memory_order_relaxed) &
        QTHREAD_RET_IS_SINC) {
      if (atomic_load_explicit(&t->flags, memory_order_relaxed) &
          QTHREAD_RET_IS_VOID_SINC) {
        (t->f)(t->arg);
        if (NULL != t->team) {
          qt_internal_teamfinish(
            t->team, atomic_load_explicit(&t->flags, memory_order_relaxed));
        }
        qt_sinc_submit((qt_sinc_t *)t->ret, NULL);
      } else {
        aligned_t retval = (t->f)(t->arg);
        if (NULL != t->team) {
          qt_internal_teamfinish(
            t->team, atomic_load_explicit(&t->flags, memory_order_relaxed));
        }
        qt_sinc_submit((qt_sinc_t *)t->ret, &retval);
      }
    } else if (atomic_load_explicit(&t->flags, memory_order_relaxed) &
               QTHREAD_RET_IS_SYNCVAR) {
      /* this should avoid problems with irresponsible return values */
      uint64_t retval = INT64TOINT60((t->f)(t->arg));
      if (NULL != t->team) {
        qt_internal_teamfinish(
          t->team, atomic_load_explicit(&t->flags, memory_order_relaxed));
      }
      qassert(qthread_syncvar_writeEF_const((syncvar_t *)t->ret, retval),
              QTHREAD_SUCCESS);
    } else {
      aligned_t retval = (t->f)(t->arg);
      if (NULL != t->team) {
        qt_internal_teamfinish(
          t->team, atomic_load_explicit(&t->flags, memory_order_relaxed));
      }
      qassert(qthread_writeEF_const((aligned_t *)t->ret, retval),
              QTHREAD_SUCCESS);
    }
  } else {
    assert(t->f);
    (t->f)(t->arg);
    if (NULL != t->team) {
      qt_internal_teamfinish(
        t->team, atomic_load_explicit(&t->flags, memory_order_relaxed));
    }
  }

  atomic_store_explicit(
    &t->thread_state, QTHREAD_STATE_TERMINATED, memory_order_relaxed);

#ifdef QTHREAD_COUNT_THREADS
  QTHREAD_FASTLOCK_LOCK(&effconcurrentthreads_lock);
  effconcurrentthreads--;
  QTHREAD_FASTLOCK_UNLOCK(&effconcurrentthreads_lock);
  QTHREAD_FASTLOCK_LOCK(&concurrentthreads_lock);
  assert(concurrentthreads > 0);
  concurrentthreads--;
  QTHREAD_FASTLOCK_UNLOCK(&concurrentthreads_lock);
#endif
  /* theoretically, we could rely on the uc_link pointer to bring us back to
   * the parent shepherd. HOWEVER, this doesn't work in lots of situations,
   * so we do it manually. A brief list of situations:
   *  1. if we're using the portable make/get/swapcontext
   *  2. if the context switch requires a stack-size modification
   *  3. if the thread has migrated (i.e. uc_link points to the original
   *  shepherd, not the current parent... theoretically, that could be
   *  changed, but getting a good uc_link is finicky)
   *
   * Thus, since doing it manually isn't a performance problem, we do it
   * manually.
   */
#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
  MONITOR_ASM_LABEL(qthread_fence2); // add label for HPCToolkit stack unwind
#endif
  if ((atomic_load_explicit(&t->flags, memory_order_relaxed) &
       QTHREAD_SIMPLE) == 0) {
    qthread_before_swap_from_qthread(t);
    qthread_back_to_master2(t);
    qthread_after_swap_to_qthread(t);
    qthread_before_swap_from_qthread(t);
  }
}

#ifdef QTHREAD_ALLOW_HPCTOOLKIT_STACK_UNWINDING
int in_qthread_fence(void *addr) {
  return (qthread_fence1 <= addr) && (addr >= qthread_fence2);
}

#endif

/* This function means "run thread t". The second argument (c) is a pointer
 * to the current context. */
void INTERNAL qthread_exec(qthread_t *t, qt_context_t *c) {
  assert(t != NULL);
  assert(c != NULL);

  if ((atomic_load_explicit(&t->flags, memory_order_relaxed) &
       QTHREAD_SIMPLE) == 0) {
    if (atomic_load_explicit(&t->thread_state, memory_order_relaxed) ==
        QTHREAD_STATE_NEW) {
      atomic_store_explicit(
        &t->thread_state, QTHREAD_STATE_RUNNING, memory_order_relaxed);
      qthread_makecontext(&t->rdata->context,
                          t->rdata->stack,
                          qlib->qthread_stack_size,
                          (void (*)(void))qthread_wrapper,
                          t,
                          c);
#ifdef USE_SYSTEM_SWAPCONTEXT
    } else {
      t->rdata->context.uc_link = c; /* NULL pthread_exit() */
#endif
    }

    atomic_store_explicit(&t->rdata->return_context, c, memory_order_relaxed);

    /* return_context (aka "c") is being written over with the current context
     */
#ifdef QTHREAD_USE_VALGRIND
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&t->rdata->context, sizeof(qt_context_t));
    VALGRIND_CHECK_MEM_IS_ADDRESSABLE(
      atomic_load_explicit(&t->rdata->return_context, memory_order_relaxed),
      sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(&t->rdata->context, sizeof(qt_context_t));
    VALGRIND_MAKE_MEM_DEFINED(
      atomic_load_explicit(&t->rdata->return_context, memory_order_relaxed),
      sizeof(qt_context_t));
#endif

    qthread_before_swap_to_qthread(t);

#ifdef USE_SYSTEM_SWAPCONTEXT
    qassert(swapcontext(atomic_load_explicit(&t->rdata->return_context,
                                             memory_order_relaxed),
                        &t->rdata->context),
            0);
#else
    qassert(qt_swapctxt(atomic_load_explicit(&t->rdata->return_context,
                                             memory_order_relaxed),
                        &t->rdata->context),
            0);
#endif

    qthread_after_swap_from_qthread(t);

  } else {
    assert(t->thread_state == QTHREAD_STATE_NEW);
    atomic_store_explicit(
      &t->thread_state, QTHREAD_STATE_RUNNING, memory_order_relaxed);
#ifdef QTHREAD_MAKECONTEXT_SPLIT
    unsigned int high = (((uintptr_t)t) >> 32) & 0xffffffff;
    unsigned int low = ((uintptr_t)t) & 0xffffffff;
    qthread_wrapper(high, low);
#else
    qthread_wrapper(t);
#endif
  }

  assert(t != NULL);
  assert(c != NULL);
}

/* this function yields thread t to the master kernel thread */
void API_FUNC qthread_yield_(int k) {
  assert(qthread_library_initialized);
  qthread_t *t = qthread_internal_self();

  if (t != NULL) {
    switch (k) {
      case 1: // Yield-near
        atomic_store_explicit(
          &t->thread_state, QTHREAD_STATE_YIELDED_NEAR, memory_order_relaxed);
        break;
      case 2: // direct-yield
      case 0: // general yield
        atomic_store_explicit(
          &t->thread_state, QTHREAD_STATE_YIELDED, memory_order_relaxed);
        break;
    }
    qthread_back_to_master(t);
  }
}

void API_FUNC qthread_flushsc(void) {}

/***********************************************
 * FORKING                                     *
 ***********************************************/

/**
 * Fork a thread by putting it in somebody's work queue.
 * NOTE: scheduling happens here
 *
 * These are the functions for generating a new qthread.
 *
 * Using qthread_fork() and variants:
 *
 *     The specified function will be run to completion. You can detect that a
 *     thread has finished by specifying a location to store the return value
 *     (which will be stored with a qthread_writeF call). The qthread_fork_to
 *     function spawns the thread to a specific shepherd.
 *
 *
 *
 * @param f This is a function that will be run to completion by the created
 *          qthread. (Note that f is a qthread_f and not a qthread_t.)
 * @param arg This is an argument that will be passed to the specified function.
 * @param ret This is a pointer to the location that the return value of f will
 *            be placed into.
 *
 * @return int Returns QTHREAD_SUCCESS (0) on success or a non-zero error value.
 */
#define QTHREAD_SPAWN_MASK_TEAMS                                               \
  (QTHREAD_SPAWN_NEW_TEAM | QTHREAD_SPAWN_NEW_SUBTEAM)

int API_FUNC qthread_spawn(qthread_f f,
                           void const *arg,
                           size_t arg_size,
                           void *ret,
                           size_t npreconds,
                           void *preconds,
                           qthread_shepherd_id_t target_shep,
                           unsigned int feature_flag) {
  assert(qthread_library_initialized);
  qthread_t *t;
  qthread_t *me = qthread_internal_self(); // note: cannot be myshep->current on
                                           // multithreaded shepherds
  qthread_shepherd_t *myshep;
  qthread_shepherd_id_t dest_shep;

#ifdef QTHREAD_OMP_AFFINITY
  if (target_shep == NO_SHEPHERD) {
    if (me->rdata->child_affinity != OMP_NO_CHILD_TASK_AFFINITY) {
      target_shep = me->rdata->child_affinity;
    }
  }
#endif
  if (me) {
    assert(me->rdata);
    myshep = me->rdata->shepherd_ptr;
  } else {
    myshep = NULL;
  }

  /* Step 1: Check arguments */
  assert(qlib);
  /* Step 2: Pick a destination */
  if (target_shep != NO_SHEPHERD) {
    dest_shep = target_shep % qlib->nshepherds;
  } else {
    dest_shep = qt_threadqueue_choose_dest(myshep);
  }
  /* Step 3: Allocate & init the structure */

  // Set up team
  assert(!(feature_flag & QTHREAD_SPAWN_NEW_TEAM) ||
         (feature_flag & QTHREAD_SPAWN_NEW_SUBTEAM) ||
         (feature_flag & QTHREAD_SPAWN_MASK_TEAMS));

  qt_team_t *curr_team = (me && me->team) ? me->team : NULL;
  qt_team_t *new_team = NULL;
  int team_leader = -1;

  if (!(feature_flag & QTHREAD_SPAWN_MASK_TEAMS)) {
    // Spawn into the current team
    team_leader = 0;

    if (curr_team) {
      new_team = curr_team;
      qt_sinc_expect(new_team->sinc, 1);
    }
  } else if (feature_flag & QTHREAD_SPAWN_NEW_TEAM) {
    // Spawn into a new team
    qthread_internal_incr(&(qlib->team_count), &qlib->team_count_lock, 1);
    team_leader = 1;

    // Allocate new team structure
    new_team =
      qt_internal_team_new(ret, feature_flag, NULL, QTHREAD_NON_TEAM_ID);
    assert(new_team);
  } else if (feature_flag & QTHREAD_SPAWN_NEW_SUBTEAM) {
    // Task will participate in a new subteam
    qthread_internal_incr(&(qlib->team_count), &qlib->team_count_lock, 1);
    team_leader = 1;

    // Allocate new team structure
    new_team = qt_internal_team_new(ret,
                                    feature_flag,
                                    curr_team,
                                    curr_team ? curr_team->team_id
                                              : QTHREAD_DEFAULT_TEAM_ID);
    assert(new_team);
  } else {
    // Unsupported spawn option
    assert(0);
  }

  t = qthread_thread_new(
    f, arg, arg_size, (aligned_t *)ret, new_team, team_leader);
  qassert_ret(t, QTHREAD_MALLOC_ERROR);

  if (QTHREAD_UNLIKELY(target_shep != NO_SHEPHERD)) {
    t->target_shepherd = dest_shep;
    atomic_fetch_or_explicit(
      &t->flags, QTHREAD_UNSTEALABLE, memory_order_relaxed);
  }
  if (QTHREAD_UNLIKELY(npreconds != 0)) {
    atomic_store_explicit(&t->thread_state,
                          QTHREAD_STATE_NASCENT,
                          memory_order_relaxed); // special non-executable state
    t->preconds = preconds;
    assert(((aligned_t **)preconds)[0] == (aligned_t *)(uintptr_t)npreconds);
  } else {
    t->preconds = NULL;
  }
  if (feature_flag & QTHREAD_SPAWN_SIMPLE) {
    atomic_fetch_or_explicit(&t->flags, QTHREAD_SIMPLE, memory_order_relaxed);
  }
  /* Step 4: Prepare the return value location (if necessary) */
  if (ret) {
    int test = QTHREAD_SUCCESS;
    unsigned ret_type =
      feature_flag & (QTHREAD_SPAWN_RET_SYNCVAR_T | QTHREAD_SPAWN_RET_SINC |
                      QTHREAD_SPAWN_RET_SINC_VOID);
    switch (ret_type) {
      case QTHREAD_SPAWN_RET_SYNCVAR_T:
        atomic_fetch_or_explicit(
          &t->flags, QTHREAD_RET_IS_SYNCVAR, memory_order_relaxed);
        if (qthread_syncvar_status((syncvar_t *)ret)) {
          test = qthread_syncvar_empty((syncvar_t *)ret);
        } else {
          test = QTHREAD_SUCCESS;
        }
        break;
      case QTHREAD_SPAWN_RET_SINC:
        atomic_fetch_or_explicit(
          &t->flags, QTHREAD_RET_IS_SINC, memory_order_relaxed);
        break;
      case QTHREAD_SPAWN_RET_SINC_VOID:
        atomic_fetch_or_explicit(
          &t->flags, QTHREAD_RET_IS_VOID_SINC, memory_order_relaxed);
        break;
      default:
        // QTHREAD_SPAWN_RET_ALIGNED
        test = qthread_empty(ret);
        break;
    }
    if (QTHREAD_UNLIKELY(test != QTHREAD_SUCCESS)) {
      qthread_thread_free(t);
      return test;
    }
  }
  /* Step 5: Prepare the input preconditions (if necessary) */
  if (QTHREAD_LIKELY(!preconds) || (qthread_check_feb_preconds(t) == 0)) {
    /* Step 6: Set it going */
#ifdef QTHREAD_COUNT_THREADS
    QTHREAD_FASTLOCK_LOCK(&concurrentthreads_lock);
    threadcount++;
    concurrentthreads++;
    assert(concurrentthreads <= threadcount);
    if (concurrentthreads > maxconcurrentthreads) {
      maxconcurrentthreads = concurrentthreads;
    }
    avg_concurrent_threads =
      (avg_concurrent_threads * (double)(threadcount - 1.0) / threadcount) +
      ((double)concurrentthreads / threadcount);
    QTHREAD_FASTLOCK_UNLOCK(&concurrentthreads_lock);
#endif /* ifdef QTHREAD_COUNT_THREADS */
    { qt_threadqueue_enqueue(qlib->threadqueues[dest_shep], t); }
  }

  if (feature_flag & QTHREAD_SPAWN_NETWORK)
    atomic_fetch_or_explicit(&t->flags, QTHREAD_NETWORK, memory_order_relaxed);

  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_fork(qthread_f f, void const *arg, aligned_t *ret) {
  return qthread_spawn(f, arg, 0, ret, 0, NULL, NO_SHEPHERD, 0);
}

int API_FUNC qthread_fork_net(qthread_f f, void const *arg, aligned_t *ret) {
  return qthread_spawn(
    f, arg, 0, ret, 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_NETWORK);
}

int API_FUNC qthread_fork_copyargs_precond(qthread_f f,
                                           void const *arg,
                                           size_t arg_size,
                                           syncvar_t *ret,
                                           int npreconds,
                                           ...) {
  // Collect sync info
  va_list args;
  aligned_t **preconds = NULL;

  va_start(args, npreconds);
  if (npreconds > 0) {
    preconds = MALLOC((npreconds + 1) * sizeof(aligned_t *));
    assert(preconds != NULL);
    preconds[0] = (aligned_t *)(uintptr_t)(npreconds);
    for (int i = 1; i < npreconds + 1; ++i) {
      preconds[i] = va_arg(args, aligned_t *);
    }
  } else if (npreconds < 0) {
    npreconds *= -1;
    preconds = MALLOC((npreconds + 1) * sizeof(aligned_t *));
    assert(preconds != NULL);
    preconds[0] = (aligned_t *)(uintptr_t)npreconds;
    aligned_t **tmp = va_arg(args, aligned_t **);
    memcpy(preconds + 1, tmp, sizeof(aligned_t *) * npreconds);
  }
  va_end(args);

  return qthread_spawn(f,
                       arg,
                       arg_size,
                       ret,
                       npreconds,
                       preconds,
                       NO_SHEPHERD,
                       QTHREAD_SPAWN_RET_SYNCVAR_T);
}

/*
 * Duplicate code from qthreads function qthread_fork_precond
 * Needed to add the simple function marker.
 * TODO: not happy with code duplication here
 */
int API_FUNC qthread_fork_precond_simple(
  qthread_f f, void const *arg, aligned_t *ret, int npreconds, ...) {
  // Collect sync info
  va_list args;
  aligned_t **preconds = NULL;

  va_start(args, npreconds);
  if (npreconds > 0) {
    preconds = (aligned_t **)MALLOC((npreconds + 1) * sizeof(aligned_t *));
    assert(preconds != NULL);
    preconds[0] = (aligned_t *)(uintptr_t)npreconds;
    for (int i = 1; i < npreconds + 1; ++i) {
      preconds[i] = va_arg(args, aligned_t *);
    }
  } else if (npreconds < 0) {
    npreconds *= -1;
    preconds = (aligned_t **)MALLOC((npreconds + 1) * sizeof(aligned_t *));
    assert(preconds != NULL);
    preconds[0] = (aligned_t *)(uintptr_t)(npreconds);
    aligned_t **tmp = va_arg(args, aligned_t **);
    memcpy(preconds + 1, tmp, sizeof(aligned_t *) * npreconds);
  }
  va_end(args);
  return qthread_spawn(
    f, arg, 0, ret, npreconds, preconds, NO_SHEPHERD, QTHREAD_SPAWN_SIMPLE);
}

int API_FUNC qthread_fork_precond_to(qthread_f f,
                                     void const *arg,
                                     aligned_t *ret,
                                     qthread_shepherd_id_t shepherd,
                                     int npreconds,
                                     ...) {
  // Collect sync info
  va_list args;
  aligned_t **preconds = NULL;

  va_start(args, npreconds);
  if (npreconds > 0) {
    preconds = MALLOC((npreconds + 1) * sizeof(aligned_t *));
    assert(preconds != NULL);
    preconds[0] = (aligned_t *)(uintptr_t)npreconds;
    for (int i = 1; i < npreconds + 1; ++i) {
      preconds[i] = va_arg(args, aligned_t *);
    }
  } else if (npreconds < 0) {
    npreconds *= -1;
    preconds = MALLOC((npreconds + 1) * sizeof(aligned_t *));
    assert(preconds != NULL);
    preconds[0] = (aligned_t *)(uintptr_t)npreconds;
    aligned_t **tmp = va_arg(args, aligned_t **);
    memcpy(preconds + 1, tmp, sizeof(aligned_t *) * npreconds);
  }
  va_end(args);

  if ((shepherd != NO_SHEPHERD) && (shepherd >= qlib->nshepherds)) {
    shepherd %= qlib->nshepherds;
  }
  return qthread_spawn(f, arg, 0, ret, npreconds, preconds, shepherd, 0);
}

int API_FUNC qthread_fork_precond(
  qthread_f f, void const *arg, aligned_t *ret, int npreconds, ...) {
  // Collect sync info
  va_list args;
  aligned_t **preconds = NULL;

  va_start(args, npreconds);
  if (npreconds > 0) {
    preconds = MALLOC((npreconds + 1) * sizeof(aligned_t *));
    assert(preconds != NULL);
    preconds[0] = (aligned_t *)(uintptr_t)npreconds;
    for (int i = 1; i < npreconds + 1; ++i) {
      preconds[i] = va_arg(args, aligned_t *);
    }
  } else if (npreconds < 0) {
    npreconds *= -1;
    preconds = MALLOC((npreconds + 1) * sizeof(aligned_t *));
    assert(preconds != NULL);
    preconds[0] = (aligned_t *)(uintptr_t)npreconds;
    aligned_t **tmp = va_arg(args, aligned_t **);
    memcpy(preconds + 1, tmp, sizeof(aligned_t *) * npreconds);
  }
  va_end(args);

  return qthread_spawn(f, arg, 0, ret, npreconds, preconds, NO_SHEPHERD, 0);
}

int API_FUNC qthread_fork_copyargs_to(qthread_f f,
                                      void const *arg,
                                      size_t arg_size,
                                      syncvar_t *ret,
                                      qthread_shepherd_id_t preferred_shep) {
  return qthread_spawn(f, arg, arg_size, ret, 0, NULL, preferred_shep, 0);
}

int API_FUNC qthread_fork_copyargs(qthread_f f,
                                   void const *arg,
                                   size_t arg_size,
                                   aligned_t *ret) {
  return qthread_spawn(f, arg, arg_size, ret, 0, NULL, NO_SHEPHERD, 0);
}

int API_FUNC qthread_fork_syncvar_copyargs(qthread_f f,
                                           void const *arg,
                                           size_t arg_size,
                                           syncvar_t *ret) {
  return qthread_spawn(
    f, arg, arg_size, ret, 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_RET_SYNCVAR_T);
}

int API_FUNC qthread_fork_syncvar_copyargs_simple(qthread_f f,
                                                  void const *arg,
                                                  size_t arg_size,
                                                  syncvar_t *ret) {
  return qthread_spawn(f,
                       arg,
                       arg_size,
                       ret,
                       0,
                       NULL,
                       NO_SHEPHERD,
                       QTHREAD_SPAWN_SIMPLE | QTHREAD_SPAWN_RET_SYNCVAR_T);
}

int API_FUNC
qthread_fork_syncvar(qthread_f f, void const *arg, syncvar_t *ret) {
  return qthread_spawn(
    f, arg, 0, ret, 0, NULL, NO_SHEPHERD, QTHREAD_SPAWN_RET_SYNCVAR_T);
}

int API_FUNC qthread_fork_to(qthread_f f,
                             void const *arg,
                             aligned_t *ret,
                             qthread_shepherd_id_t shepherd) {
  if ((shepherd != NO_SHEPHERD) && (shepherd >= qlib->nshepherds)) {
    shepherd %= qlib->nshepherds;
  }
  return qthread_spawn(f, arg, 0, ret, 0, NULL, shepherd, 0);
}

int API_FUNC qthread_fork_syncvar_to(qthread_f f,
                                     void const *arg,
                                     syncvar_t *ret,
                                     qthread_shepherd_id_t s) {
  if ((s != NO_SHEPHERD) && (s >= qlib->nshepherds)) { s %= qlib->nshepherds; }
  return qthread_spawn(f, arg, 0, ret, 0, NULL, s, QTHREAD_SPAWN_RET_SYNCVAR_T);
}

void INTERNAL qthread_back_to_master(qthread_t *t) {
  assert((atomic_load_explicit(&t->flags, memory_order_relaxed) &
          QTHREAD_SIMPLE) == 0);
#ifdef QTHREAD_USE_VALGRIND
  VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&t->rdata->context, sizeof(qt_context_t));
  VALGRIND_CHECK_MEM_IS_ADDRESSABLE(
    atomic_load_explicit(&t->rdata->return_context, memory_order_relaxed),
    sizeof(qt_context_t));
  VALGRIND_MAKE_MEM_DEFINED(&t->rdata->context, sizeof(qt_context_t));
  VALGRIND_MAKE_MEM_DEFINED(
    atomic_load_explicit(&t->rdata->return_context, memory_order_relaxed),
    sizeof(qt_context_t));
#endif
  qthread_before_swap_from_qthread(t);
#ifdef USE_SYSTEM_SWAPCONTEXT
  qassert(swapcontext(&t->rdata->context,
                      atomic_load_explicit(&t->rdata->return_context,
                                           memory_order_relaxed)),
          0);
#else
  qassert(qt_swapctxt(&t->rdata->context,
                      atomic_load_explicit(&t->rdata->return_context,
                                           memory_order_relaxed)),
          0);
#endif

  qthread_after_swap_to_qthread(t);
}

void INTERNAL qthread_back_to_master2(qthread_t *t) {
  assert((atomic_load_explicit(&t->flags, memory_order_relaxed) &
          QTHREAD_SIMPLE) == 0);
#ifdef QTHREAD_USE_VALGRIND
  VALGRIND_CHECK_MEM_IS_ADDRESSABLE(&t->rdata->context, sizeof(qt_context_t));
  VALGRIND_CHECK_MEM_IS_ADDRESSABLE(
    atomic_load_explicit(&t->rdata->return_context, memory_order_relaxed),
    sizeof(qt_context_t));
  VALGRIND_MAKE_MEM_DEFINED(&t->rdata->context, sizeof(qt_context_t));
  VALGRIND_MAKE_MEM_DEFINED(
    atomic_load_explicit(&t->rdata->return_context, memory_order_relaxed),
    sizeof(qt_context_t));
#endif
#ifdef USE_SYSTEM_SWAPCONTEXT
  setcontext(
    atomic_load_explicit(&t->rdata->return_context, memory_order_relaxed));
#else
  qt_setmctxt(
    &atomic_load_explicit(&t->rdata->return_context, memory_order_relaxed)->mc);
#endif
}

/* function to move a qthread from one shepherd to another */
int API_FUNC qthread_migrate_to(qthread_shepherd_id_t const shepherd) {
  assert(qthread_library_initialized);
  qthread_t *me = qthread_internal_self();

  if (atomic_load_explicit(&me->flags, memory_order_relaxed) &
      QTHREAD_REAL_MCCOY) {
    return QTHREAD_NOT_ALLOWED;
  }
  if (me->rdata->shepherd_ptr->shepherd_id == shepherd) {
    me->target_shepherd = shepherd;
    atomic_fetch_or_explicit(
      &me->flags, QTHREAD_UNSTEALABLE, memory_order_relaxed);
    return QTHREAD_SUCCESS;
  }
  if (shepherd == NO_SHEPHERD) {
    me->target_shepherd = NO_SHEPHERD;
    atomic_fetch_and_explicit(
      &me->flags, ~(uint8_t)QTHREAD_UNSTEALABLE, memory_order_relaxed);
    return QTHREAD_SUCCESS;
  }
  if (me && (shepherd < qlib->nshepherds)) {
    me->target_shepherd = shepherd;
    atomic_store_explicit(
      &me->thread_state, QTHREAD_STATE_MIGRATING, memory_order_relaxed);
    atomic_fetch_or_explicit(
      &me->flags, QTHREAD_UNSTEALABLE, memory_order_relaxed);
    qthread_back_to_master(me);

    return QTHREAD_SUCCESS;
  } else {
    return QTHREAD_BADARGS;
  }
}

void API_FUNC qthread_reset_target_shep(void) {
  assert(qthread_library_initialized);
  qthread_t *me = qthread_internal_self();

  if (me) {
    assert(me->rdata);
    me->rdata->shepherd_ptr->sched_shepherd = 0;
  } else {
    qlib->sched_shepherd = 0;
    MACHINE_FENCE;
  }
}

/* These are just accessor functions */
unsigned int API_FUNC qthread_id(void) {
  assert(qthread_library_initialized);
  qthread_t *t = qthread_internal_self();

  if (!t) { return QTHREAD_NON_TASK_ID; }
  if (t->thread_id != QTHREAD_NON_TASK_ID) { return t->thread_id; }
  t->thread_id =
    qthread_internal_incr(&(qlib->max_thread_id), &qlib->max_thread_id_lock, 1);
  if (QTHREAD_UNLIKELY(t->thread_id == QTHREAD_NULL_TASK_ID)) {
    /* yes, this is wrapping around, but... thread_id should be prevented from
     * being NULL */
    t->thread_id = qthread_internal_incr(
      &(qlib->max_thread_id), &qlib->max_thread_id_lock, 2);
  } else if (QTHREAD_UNLIKELY(t->thread_id == QTHREAD_NON_TASK_ID)) {
    /* yes, this is wrapping around, but... thread_id should be prevented from
     * being NON */
    t->thread_id = qthread_internal_incr(
      &(qlib->max_thread_id), &qlib->max_thread_id_lock, 1);
  }
  return t->thread_id;
}

void qt_set_barrier(qt_barrier_t *bar) {
  qthread_t *me = qthread_internal_self();

  me->rdata->barrier = bar;
}

qt_barrier_t *qt_get_barrier(void) {
  qthread_t *me = qthread_internal_self();

  return me->rdata->barrier;
}

/* vim:set expandtab: */
