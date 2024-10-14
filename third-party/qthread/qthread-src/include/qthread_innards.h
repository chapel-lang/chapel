#ifndef QTHREAD_INNARDS_H
#define QTHREAD_INNARDS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* System Headers */
#include <math.h>

#include <pthread.h>

#ifdef QTHREAD_HAVE_HWLOC
#include <hwloc.h>
#if (HWLOC_API_VERSION < 0x00010000)
#error HWLOC version unrecognized
#endif
#endif

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_atomics.h"
#include "qt_context.h"
#include "qt_hash.h"
#include "qt_qthread_t.h"
#include "qt_threadqueues.h"
#include "qt_visibility.h"

typedef struct qlib_s {
  unsigned int nshepherds;
  aligned_t nshepherds_active;
  aligned_t nworkers_active;
  unsigned int nworkerspershep;
  struct qthread_shepherd_s *shepherds;
  qt_threadqueue_t **threadqueues;

  unsigned qthread_stack_size;
  unsigned master_stack_size;
  unsigned max_stack_size;

  unsigned qthread_argcopy_size;
  unsigned qthread_tasklocal_size;

  qthread_t *mccoy_thread; /* free when exiting */

  void *master_stack;
  qt_context_t master_context;
#ifdef QTHREAD_USE_VALGRIND
  unsigned int valgrind_masterstack_id;
#endif
#if defined(__has_feature)
#if __has_feature(thread_sanitizer)
  void *tsan_main_fiber;
#endif
#endif

  /* assigns a unique thread_id mostly for debugging! */
  aligned_t max_thread_id;
  QTHREAD_FASTLOCK_TYPE max_thread_id_lock;

  /* assigns a unique team id */
  aligned_t max_team_id;
  QTHREAD_FASTLOCK_TYPE max_team_id_lock;

  aligned_t team_count;
  QTHREAD_FASTLOCK_TYPE team_count_lock;

  /* assigns a unique worker_id */
  aligned_t max_unique_id;
  QTHREAD_FASTLOCK_TYPE max_unique_id_lock;

  /* round robin scheduler - can probably be smarter */
  aligned_t sched_shepherd;
  QTHREAD_FASTLOCK_TYPE sched_shepherd_lock;

  /*AGG cost method, call method  and max cost
   * defined in qthreads or given by the user at qthread initialization
   */
  qthread_agg_f
    agg_f; // void(*agg_f)   (int count, qthread_f *f, void **arg, void **ret);
  int (*agg_cost)(int count, qthread_f *f, void **arg);
  int max_c;
} *qlib_t;

extern qlib_t qlib;

void INTERNAL qthread_exec(qthread_t *t, qt_context_t *c);

#endif // ifndef QTHREAD_INNARDS_H
/* vim:set expandtab: */
