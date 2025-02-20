#ifndef QT_SHEPHERD_INNARDS_H
#define QT_SHEPHERD_INNARDS_H

#include <stdalign.h>

/* System Pre-requisites */
#include <pthread.h>

/* Internal Pre-requisites */
#include "qt_atomics.h"
#include "qt_hazardptrs.h"
#include "qt_macros.h"
#include "qt_mpool.h"
#include "qt_threadqueues.h"

#ifdef QTHREAD_OMP_AFFINITY
#include "omp_affinity.h"
#endif

#ifndef QTHREAD_SHEPHERD_TYPEDEF
#define QTHREAD_SHEPHERD_TYPEDEF
typedef struct qthread_shepherd_s qthread_shepherd_t;
#endif

#define QTHREAD_NO_NODE ((unsigned int)(-1))

#define STEAL_BUFFER_LENGTH 128

struct qthread_worker_s {
  uintptr_t hazard_ptrs
    [HAZARD_PTRS_PER_SHEP]; /* hazard pointers (see
                               http://portal.acm.org/citation.cfm?id=987524.987595)
                             */
  hazard_freelist_t hazard_free_list;
  pthread_t worker;
  qthread_shepherd_t *shepherd;
  struct qthread_s **nostealbuffer;
  struct qthread_s **stealbuffer;
  qthread_t *_Atomic current;
  qthread_worker_id_t unique_id;
  qthread_worker_id_t worker_id;
  qthread_worker_id_t packed_worker_id;
  _Atomic alignas(8) uint_fast8_t active;
};
typedef struct qthread_worker_s qthread_worker_t;

/* The Shepherd Struct */
struct qthread_shepherd_s {
  pthread_t shepherd;
  qthread_shepherd_id_t shepherd_id; /* whoami */
  qthread_worker_t *workers;         // dymanic length qlib->nworkerspershep
  qthread_t *current;
  qt_threadqueue_t *ready;
  /* round robin scheduler - can probably be smarter */
  aligned_t sched_shepherd;
  _Atomic uintptr_t active;
  /* affinity information */
  unsigned int node; /* whereami */
  unsigned int *shep_dists;
  qthread_shepherd_id_t *sorted_sheplist;
  _Atomic unsigned int stealing; /* True when a worker is in the steal (attempt)
                                    process OR if stealing disabled*/
#ifdef QTHREAD_OMP_AFFINITY
  unsigned int stealing_mode; /* Specifies when a shepherd may steal */
#endif
  uint32_t padding[CACHELINE_WIDTH / sizeof(uint32_t)];
};

extern TLS_DECL(qthread_shepherd_t *, shepherd_structs);

static inline qthread_shepherd_t *qthread_internal_getshep(void) {
  qthread_worker_t *w = (qthread_worker_t *)TLS_GET(shepherd_structs);
  if (w == NULL) {
    return NULL;
  } else {
    return w->shepherd;
  }
}

static inline qthread_worker_t *qthread_internal_getworker(void) {
  return (qthread_worker_t *)TLS_GET(shepherd_structs);
}

unsigned int INTERNAL
qthread_internal_shep_to_node(qthread_shepherd_id_t const shep);
qthread_shepherd_t INTERNAL *
qthread_find_active_shepherd(qthread_shepherd_id_t *l, unsigned int *d);

void qthread_back_to_master(qthread_t *t);
void qthread_back_to_master2(qthread_t *t);

#endif // ifndef QT_SHEPHERD_INNARDS_H
/* vim:set expandtab: */
