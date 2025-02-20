/* System Headers */
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

/* Public Headers */
#include "qthread/cacheline.h"
#include "qthread/qthread.h"

/* Internal Headers */
#include "qt_alloc.h"
#include "qt_asserts.h"
#include "qt_envariables.h"
#include "qt_expect.h"
#include "qt_prefetch.h"
#include "qt_qthread_mgmt.h"
#include "qt_qthread_struct.h"
#include "qt_shepherd_innards.h"
#include "qt_subsystems.h"
#include "qt_threadqueue_scheduler.h"
#include "qt_threadqueues.h"
#include "qt_visibility.h"
#include "qthread_innards.h" /* for qlib */

// Non portable
typedef uint8_t cacheline[CACHELINE_WIDTH];

/* Cutoff variables */
int max_backoff;
int spinloop_backoff;
int condwait_backoff;
int steal_ratio;

typedef struct qt_threadqueue_node_s qt_threadqueue_node_t;

/* Data Structures */
struct qt_threadqueue_node_s {
  struct qt_threadqueue_node_s *_Atomic next;
  struct qt_threadqueue_node_s *_Atomic prev;
  qthread_t *value;
};

typedef struct {
  qt_threadqueue_node_t *_Atomic head;
  qt_threadqueue_node_t *_Atomic tail;
  _Atomic uint64_t qlength;
  QTHREAD_TRYLOCK_TYPE qlock;
  cacheline buf; // ensure internal nodes are a cacheline apart
} qt_threadqueue_internal;

typedef struct {
  _Atomic size_t n;
  cacheline buf; // ensure
} w_ind;

struct _qt_threadqueue {
  qt_threadqueue_internal *t;
  size_t num_queues;
  w_ind *w_inds;
  QTHREAD_COND_DECL(cond);
  _Atomic uint64_t numwaiters;
};

// global cond pool
_Atomic int finalizing;

qthread_t *_Atomic mccoy = NULL;

/* Memory Management and Initialization/Shutdown */
qt_threadqueue_pools_t generic_threadqueue_pools;

#define mycounter(q)                                                           \
  (q->w_inds[qthread_worker(NULL) %                                            \
             (qlib->nshepherds * qlib->nworkerspershep)]                       \
     .n)
#define myqueue(q)                                                             \
  (q->t + atomic_load_explicit(&mycounter(q), memory_order_relaxed))

static qt_threadqueue_t *alloc_threadqueue(void) {
  qt_threadqueue_t *t =
    (qt_threadqueue_t *)qt_mpool_alloc(generic_threadqueue_pools.queues);
  t->num_queues =
    qlib->nworkerspershep; // Assumption built into api of constant number of
                           // workers per shepherd
  t->t = qt_malloc(sizeof(qt_threadqueue_internal) * t->num_queues);
  t->w_inds =
    qt_calloc(qlib->nshepherds * qlib->nworkerspershep, sizeof(w_ind));
  atomic_store_explicit(&t->numwaiters, 0ull, memory_order_relaxed);
  return t;
}

static void free_threadqueue(qt_threadqueue_t *t) {
  qt_free(t->t);
  qt_free(t->w_inds);
  qt_mpool_free(generic_threadqueue_pools.queues, t);
}

static inline qt_threadqueue_node_t *alloc_tqnode(void) {
  return (qt_threadqueue_node_t *)qt_mpool_alloc(
    generic_threadqueue_pools.nodes);
}

static inline void free_tqnode(qt_threadqueue_node_t *t) {
  qt_mpool_free(generic_threadqueue_pools.nodes, t);
}

extern qt_mpool generic_qthread_pool;

static inline void free_qthread(qthread_t *t) {
  return qt_mpool_free(generic_qthread_pool, t);
}

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void) {
  qt_threadqueue_t *qe = alloc_threadqueue();
  for (int i = 0; i < qe->num_queues; i++) {
    qt_threadqueue_internal *q = qe->t + i;
    if (q != NULL) {
      atomic_store_explicit(&q->head, NULL, memory_order_relaxed);
      atomic_store_explicit(&q->tail, NULL, memory_order_relaxed);
      atomic_store_explicit(&q->qlength, 0ull, memory_order_relaxed);
      QTHREAD_TRYLOCK_INIT(q->qlock);
    }
  }
  for (size_t i = 0; i < qlib->nshepherds * qlib->nworkerspershep; i++) {
    atomic_store_explicit(
      &qe->w_inds[i].n, i % qe->num_queues, memory_order_relaxed);
  }
  QTHREAD_COND_INIT(qe->cond);
  return qe;
}

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *qe) {
  for (int i = 0; i < qe->num_queues; i++) {
    qt_threadqueue_internal *q = qe->t + i;
    if (atomic_load_explicit(&q->head, memory_order_relaxed) !=
        atomic_load_explicit(&q->tail, memory_order_relaxed)) {
      qthread_t *t;
      QTHREAD_TRYLOCK_LOCK(&q->qlock);
      while (atomic_load_explicit(&q->head, memory_order_relaxed) !=
             atomic_load_explicit(&q->tail, memory_order_relaxed)) {
        qt_threadqueue_node_t *node =
          atomic_load_explicit(&q->tail, memory_order_relaxed);
        if (node != NULL) {
          atomic_store_explicit(
            &q->tail,
            atomic_load_explicit(&node->prev, memory_order_relaxed),
            memory_order_relaxed);
          if (atomic_load_explicit(&q->tail, memory_order_relaxed) == NULL) {
            atomic_store_explicit(&q->head, NULL, memory_order_relaxed);
          } else {
            atomic_store_explicit(
              &atomic_load_explicit(&q->tail, memory_order_relaxed)->next,
              NULL,
              memory_order_relaxed);
          }
          t = node->value;
          free_tqnode(node);
          free_qthread(t);
        }
      }
      assert(atomic_load_explicit(&q->head, memory_order_relaxed) == NULL);
      assert(atomic_load_explicit(&q->tail, memory_order_relaxed) == NULL);
      atomic_store_explicit(&q->qlength, 0ull, memory_order_relaxed);
      QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
    }
    assert(atomic_load_explicit(&q->head, memory_order_relaxed) ==
           atomic_load_explicit(&q->tail, memory_order_relaxed));
    QTHREAD_TRYLOCK_DESTROY(q->qlock);
  }
  QTHREAD_COND_DESTROY(qe->cond);
  free_threadqueue(qe);
}

static void qt_threadqueue_subsystem_shutdown(void) {
  qt_mpool_destroy(generic_threadqueue_pools.nodes);
  qt_mpool_destroy(generic_threadqueue_pools.queues);
}

void INTERNAL qt_threadqueue_subsystem_init(void) {
  steal_ratio = qt_internal_get_env_num("STEAL_RATIO", 8, 0);
  condwait_backoff = qt_internal_get_env_num("CONDWAIT_BACKOFF", 2048, 0);
  atomic_store_explicit(&finalizing, 0, memory_order_relaxed);
  generic_threadqueue_pools.queues =
    qt_mpool_create_aligned(sizeof(qt_threadqueue_t), qthread_cacheline());
  generic_threadqueue_pools.nodes =
    qt_mpool_create_aligned(sizeof(qt_threadqueue_node_t), qthread_cacheline());
  qthread_internal_cleanup(qt_threadqueue_subsystem_shutdown);
}

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q) {
  return atomic_load_explicit(&myqueue(q)->qlength, memory_order_relaxed);
}

/* Threadqueue operations
 * We have 4 basic queue operations, enqueue and dequeue for head and tail */
static void qt_threadqueue_enqueue_tail(qt_threadqueue_t *restrict qe,
                                        qthread_t *restrict t) {
  if (atomic_load_explicit(&t->thread_state, memory_order_relaxed) ==
      QTHREAD_STATE_TERM_SHEP) {
    atomic_store_explicit(&finalizing, 1, memory_order_relaxed);
  }
  if (atomic_load_explicit(&t->flags, memory_order_relaxed) &
      QTHREAD_REAL_MCCOY) { // only needs to be on worker 0 for termination
    if (atomic_load_explicit(&mccoy, memory_order_relaxed)) {
      printf("mccoy thread non-null and trying to set!\n");
      exit(-1);
    }
    atomic_store_explicit(&mccoy, t, memory_order_relaxed);
  } else {
    qt_threadqueue_internal *q = myqueue(qe);
    atomic_store_explicit(
      &mycounter(qe),
      (atomic_load_explicit(&mycounter(qe), memory_order_relaxed) + 1) %
        qe->num_queues,
      memory_order_relaxed);
    qt_threadqueue_node_t *node = alloc_tqnode();
    node->value = t;
    atomic_store_explicit(&node->next, NULL, memory_order_relaxed);

    QTHREAD_TRYLOCK_LOCK(&q->qlock);
    atomic_store_explicit(&node->prev,
                          atomic_load_explicit(&q->tail, memory_order_relaxed),
                          memory_order_relaxed);
    atomic_store_explicit(&q->tail, node, memory_order_relaxed);
    if (atomic_load_explicit(&q->head, memory_order_relaxed) == NULL) {
      atomic_store_explicit(&q->head, node, memory_order_relaxed);
    } else {
      atomic_store_explicit(
        &atomic_load_explicit(&node->prev, memory_order_relaxed)->next,
        node,
        memory_order_relaxed);
    }
    atomic_fetch_add_explicit(&q->qlength, 1ull, memory_order_relaxed);
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
  }
  // we need to wake up all threads when finalizing and if pushing the mccoy
  // thread to make sure we get worker 0
  if (atomic_load_explicit(&finalizing, memory_order_relaxed) ||
      atomic_load_explicit(&t->flags, memory_order_relaxed) &
        QTHREAD_REAL_MCCOY) {
    QTHREAD_COND_LOCK(qe->cond);
    QTHREAD_COND_BCAST(qe->cond);
    QTHREAD_COND_UNLOCK(qe->cond);
  } else if (atomic_load_explicit(&qe->numwaiters, memory_order_relaxed)) {
    QTHREAD_COND_LOCK(qe->cond);
    if (atomic_load_explicit(&qe->numwaiters, memory_order_relaxed))
      QTHREAD_COND_SIGNAL(qe->cond);
    QTHREAD_COND_UNLOCK(qe->cond);
  }
}

static void qt_threadqueue_enqueue_head(qt_threadqueue_t *restrict qe,
                                        qthread_t *restrict t) {
  if (atomic_load_explicit(&t->flags, memory_order_relaxed) &
      QTHREAD_REAL_MCCOY) { // only needs to be on worker 0 for termination
    if (atomic_load_explicit(&mccoy, memory_order_relaxed)) {
      printf("mccoy thread non-null and trying to set!\n");
      exit(-1);
    }
    atomic_store_explicit(&mccoy, t, memory_order_relaxed);
    return;
  }

  qt_threadqueue_internal *q = myqueue(qe);
  atomic_store_explicit(
    &mycounter(qe),
    (atomic_load_explicit(&mycounter(qe), memory_order_relaxed) + 1) %
      qe->num_queues,
    memory_order_relaxed);
  qt_threadqueue_node_t *node = alloc_tqnode();
  node->value = t;
  atomic_store_explicit(&node->prev, NULL, memory_order_relaxed);

  QTHREAD_TRYLOCK_LOCK(&q->qlock);
  atomic_store_explicit(&node->next,
                        atomic_load_explicit(&q->head, memory_order_relaxed),
                        memory_order_relaxed);
  atomic_store_explicit(&q->head, node, memory_order_relaxed);
  if (atomic_load_explicit(&q->tail, memory_order_relaxed) == NULL) {
    atomic_store_explicit(&q->tail, node, memory_order_relaxed);
  } else {
    atomic_store_explicit(
      &atomic_load_explicit(&node->next, memory_order_relaxed)->prev,
      node,
      memory_order_relaxed);
  }
  atomic_fetch_add_explicit(&q->qlength, 1ull, memory_order_relaxed);
  QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
  if (atomic_load_explicit(&qe->numwaiters, memory_order_relaxed)) {
    QTHREAD_COND_LOCK(qe->cond);
    if (atomic_load_explicit(&qe->numwaiters, memory_order_relaxed)) {
      QTHREAD_COND_SIGNAL(qe->cond);
    }
    QTHREAD_COND_UNLOCK(qe->cond);
  }
}

static qt_threadqueue_node_t *
qt_threadqueue_dequeue_tail(qt_threadqueue_t *qe) {
  qt_threadqueue_internal *q = myqueue(qe);
  atomic_store_explicit(
    &mycounter(qe),
    (atomic_load_explicit(&mycounter(qe), memory_order_relaxed) + 1) %
      qe->num_queues,
    memory_order_relaxed);
  qt_threadqueue_node_t *node;

  // If there is no work or we can't get the lock, fail
  if (atomic_load_explicit(&q->qlength, memory_order_relaxed) == 0) return NULL;
  if (!QTHREAD_TRYLOCK_TRY(&q->qlock)) return NULL;
  if (atomic_load_explicit(&q->qlength, memory_order_relaxed) == 0) {
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
    return NULL;
  }

  node = (qt_threadqueue_node_t *)atomic_load_explicit(&q->tail,
                                                       memory_order_relaxed);
  atomic_store_explicit(&q->tail,
                        atomic_load_explicit(&node->prev, memory_order_relaxed),
                        memory_order_relaxed);
  if (atomic_load_explicit(&q->tail, memory_order_relaxed))
    atomic_store_explicit(
      &atomic_load_explicit(&q->tail, memory_order_relaxed)->next,
      NULL,
      memory_order_relaxed);
  if (atomic_load_explicit(&q->head, memory_order_relaxed) == node)
    atomic_store_explicit(&q->head, NULL, memory_order_relaxed);
  atomic_fetch_sub_explicit(&q->qlength, 1ull, memory_order_relaxed);
  QTHREAD_TRYLOCK_UNLOCK(&q->qlock);

  return node;
}

static qt_threadqueue_node_t *
qt_threadqueue_dequeue_head(qt_threadqueue_t *qe) {
  qt_threadqueue_internal *q = myqueue(qe);
  atomic_store_explicit(
    &mycounter(qe),
    (atomic_load_explicit(&mycounter(qe), memory_order_relaxed) + 1) %
      qe->num_queues,
    memory_order_relaxed);
  qt_threadqueue_node_t *node;

  // If there is no work or we can't get the lock, fail
  if (atomic_load_explicit(&q->qlength, memory_order_relaxed) == 0) return NULL;
  if (!QTHREAD_TRYLOCK_TRY(&q->qlock)) return NULL;
  if (atomic_load_explicit(&q->qlength, memory_order_relaxed) == 0) {
    QTHREAD_TRYLOCK_UNLOCK(&q->qlock);
    return NULL;
  }

  node = (qt_threadqueue_node_t *)atomic_load_explicit(&q->head,
                                                       memory_order_relaxed);
  atomic_store_explicit(&q->head,
                        atomic_load_explicit(&node->next, memory_order_relaxed),
                        memory_order_relaxed);
  if (atomic_load_explicit(&q->head, memory_order_relaxed))
    atomic_store_explicit(
      &atomic_load_explicit(&q->head, memory_order_relaxed)->prev,
      NULL,
      memory_order_relaxed);
  if (atomic_load_explicit(&q->tail, memory_order_relaxed) == node)
    atomic_store_explicit(&q->tail, NULL, memory_order_relaxed);
  atomic_fetch_sub_explicit(&q->qlength, 1ull, memory_order_relaxed);
  QTHREAD_TRYLOCK_UNLOCK(&q->qlock);

  return node;
}

void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict t) {
  return qt_threadqueue_enqueue_tail(q, t);
}

void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict t) {
  return qt_threadqueue_enqueue_head(q, t);
}

/* Unsupported operations */
qthread_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                    void *value) {
  return NULL;
}

inline int square(int x) { return x * x; }

// We try and dequeue locally, if that fails we should do some stealing
qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t *qe,
                                            uint_fast8_t active) {
  qt_threadqueue_node_t *node = NULL;
  qthread_t *t;

  for (int numwaits = 0; !node; numwaits++) {
    node = qt_threadqueue_dequeue_tail(qe);

    // If we've done QT_STEAL_RATIO waits on local queue, try to steal
    if (!node && steal_ratio > 0 && numwaits % steal_ratio == 0) {
      for (int i = 0; i < qlib->nshepherds; i++) {
        qt_threadqueue_t *victim_queue = qlib->shepherds[i].ready;
        node = qt_threadqueue_dequeue_head(victim_queue);
        if (node) {
          t = node->value;
          free_tqnode(node);
          return t;
        }
      }
    }

    if (!node && qthread_worker(NULL) == 0 &&
        atomic_load_explicit(&mccoy, memory_order_relaxed)) {
      qthread_t *t = atomic_load_explicit(&mccoy, memory_order_relaxed);
      atomic_store_explicit(&mccoy, NULL, memory_order_relaxed);
      return t;
    } else if (!node) {
      if (numwaits > condwait_backoff &&
          !atomic_load_explicit(&finalizing, memory_order_relaxed)) {
        QTHREAD_COND_LOCK(qe->cond);
        atomic_fetch_add_explicit(&qe->numwaiters, 1ull, memory_order_relaxed);
        MACHINE_FENCE;
        if (!atomic_load_explicit(&finalizing, memory_order_relaxed))
          QTHREAD_COND_WAIT(qe->cond);
        atomic_fetch_sub_explicit(&qe->numwaiters, 1ull, memory_order_relaxed);
        QTHREAD_COND_UNLOCK(qe->cond);
        numwaits = 0;
      } else {
        SPINLOCK_BODY();
      }
    }
  }
  t = node->value;
  free_tqnode(node);
  return t;
}

void INTERNAL qthread_steal_enable(void) {}

void INTERNAL qthread_steal_disable(void) {}

qthread_shepherd_id_t INTERNAL
qt_threadqueue_choose_dest(qthread_shepherd_t *curr_shep) {
  qthread_shepherd_id_t dest_shep_id = 0;
  if (curr_shep) {
    dest_shep_id = curr_shep->sched_shepherd++ % qlib->nshepherds;
    curr_shep->sched_shepherd *= (qlib->nshepherds > (dest_shep_id + 1));
  } else {
    dest_shep_id = (qthread_shepherd_id_t)qthread_internal_incr_mod(
      &qlib->sched_shepherd, qlib->nshepherds, &qlib->sched_shepherd_lock);
  }

  return dest_shep_id;
}

size_t INTERNAL qt_threadqueue_policy(const enum threadqueue_policy policy) {
  switch (policy) {
    default: return THREADQUEUE_POLICY_UNSUPPORTED;
  }
}

/* vim:set expandtab: */
