#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* System Headers */
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/* API Headers */
#include "qthread/qthread.h"

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_envariables.h"
#include "qt_macros.h"
#include "qt_prefetch.h"
#include "qt_qthread_mgmt.h" /* for qthread_thread_free() */
#include "qt_qthread_struct.h"
#include "qt_subsystems.h"
#include "qt_threadqueue_scheduler.h"
#include "qt_threadqueues.h"
#include "qt_visibility.h"
#include "qthread_innards.h" /* for qlib */

/* This thread queueing uses the NEMESIS lock-free queue protocol from
 * http://www.mcs.anl.gov/~buntinas/papers/ccgrid06-nemesis.pdf
 * Note: it is NOT SAFE to use with multiple de-queuers, it is ONLY safe to use
 * with multiple enqueuers and a single de-queuer. */

int num_spins_before_condwait;
#define DEFAULT_SPINCOUNT 300000

/* Data Structures */
struct _qt_threadqueue_node {
  struct _qt_threadqueue_node *_Atomic next;
  qthread_t *thread;
};

typedef struct {
  /* The First Cacheline */
  void *_Atomic head;
  uint8_t pad1[CACHELINE_WIDTH - sizeof(void*)];
  /* The Second Cacheline */
  void *_Atomic tail;
  uint8_t pad2[CACHELINE_WIDTH - sizeof(void*)];
  /* The Third Cacheline */
  void *shadow_head;
  /* the following is for estimating a queue's "busy" level, and is not
   * guaranteed accurate (that would be a race condition) */
  saligned_t nemesis_advisory_queuelen;
  uint8_t pad3[CACHELINE_WIDTH - sizeof(void*) - sizeof(saligned_t)];
} NEMESIS_queue;

struct _qt_threadqueue {
  alignas(CACHELINE_WIDTH) NEMESIS_queue q;
  /* the following is for estimating a queue's "busy" level, and is not
   * guaranteed accurate (that would be a race condition) */
  saligned_t advisory_queuelen;
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
  uint32_t frustration;
  QTHREAD_COND_DECL(trigger);
#endif
} /* qt_threadqueue_t */;

/* Memory Management */
qt_threadqueue_pools_t generic_threadqueue_pools = {NULL, NULL};
#define ALLOC_THREADQUEUE()                                                    \
  (qt_threadqueue_t *)qt_mpool_alloc(generic_threadqueue_pools.queues)
#define FREE_THREADQUEUE(t) qt_mpool_free(generic_threadqueue_pools.queues, t)
#define ALLOC_TQNODE()                                                         \
  (qt_threadqueue_node_t *)qt_mpool_alloc(generic_threadqueue_pools.nodes)
#define FREE_TQNODE(t) qt_mpool_free(generic_threadqueue_pools.nodes, t)

static void qt_threadqueue_subsystem_shutdown(void) { /*{{{*/
  qt_mpool_destroy(generic_threadqueue_pools.queues);
  qt_mpool_destroy(generic_threadqueue_pools.nodes);
} /*}}}*/

void INTERNAL qt_threadqueue_subsystem_init(void) { /*{{{*/

  num_spins_before_condwait =
    qt_internal_get_env_num("SPINCOUNT", DEFAULT_SPINCOUNT, 0);

  generic_threadqueue_pools.queues = qt_mpool_create_aligned(
    sizeof(qt_threadqueue_t), _Alignof(qt_threadqueue_t));
  generic_threadqueue_pools.nodes =
    qt_mpool_create_aligned(sizeof(qt_threadqueue_node_t), 8);
  qthread_internal_cleanup(qt_threadqueue_subsystem_shutdown);
} /*}}}*/

/* Thankfully, NEMESIS does not suffer from the ABA problem. */

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void) { /*{{{ */
  qt_threadqueue_t *q = ALLOC_THREADQUEUE();

  qassert_ret(q != NULL, NULL);

  atomic_init(&q->q.head, NULL);
  atomic_init(&q->q.tail, NULL);
  q->q.shadow_head = NULL;
  q->advisory_queuelen = 0;
  q->q.nemesis_advisory_queuelen = 0; // redundant
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
  q->frustration = 0;
  QTHREAD_COND_INIT(q->trigger);
#endif /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */

  return q;
} /*}}} */

static inline qt_threadqueue_node_t *
qt_internal_NEMESIS_dequeue(NEMESIS_queue *q) { /*{{{ */
  if (!q->shadow_head) {
    void *head = atomic_load_explicit(&q->head, memory_order_relaxed);
    if (!head) { return NULL; }
    q->shadow_head = head;

    atomic_store_explicit(&q->head, NULL, memory_order_relaxed);
  }

  qt_threadqueue_node_t *const retval = (void *volatile)(q->shadow_head);

  if ((retval != NULL) && (retval != (void *)1)) {
    struct _qt_threadqueue_node *next_loc =
      atomic_load_explicit(&retval->next, memory_order_acquire);
    if (next_loc != NULL) {
      q->shadow_head = next_loc;
      atomic_store_explicit(&retval->next, NULL, memory_order_relaxed);
    } else {
      qt_threadqueue_node_t *old;
      q->shadow_head = NULL;
      old = qthread_cas_ptr((void **)&(q->tail), retval, NULL);
      if (old != retval) {
        void *retval_next_tmp;
        while ((retval_next_tmp = atomic_load_explicit(
                  &retval->next, memory_order_relaxed)) == NULL)
          SPINLOCK_BODY();
        q->shadow_head = retval_next_tmp;
        atomic_store_explicit(&retval->next, NULL, memory_order_relaxed);
      }
    }
  }
  return retval;
} /*}}} */

static inline qt_threadqueue_node_t *
qt_internal_NEMESIS_dequeue_st(NEMESIS_queue *q) { /*{{{ */
  if (!q->shadow_head) {
    void *head = atomic_load_explicit(&q->head, memory_order_relaxed);
    if (!head) { return NULL; }
    q->shadow_head = head;
    atomic_store_explicit(&q->head, NULL, memory_order_relaxed);
  }

  qt_threadqueue_node_t *const retval = (void *volatile)(q->shadow_head);

  if ((retval != NULL) && (retval != (void *)1)) {
    void *retval_next_tmp =
      atomic_load_explicit(&retval->next, memory_order_relaxed);
    if (retval_next_tmp != NULL) {
      q->shadow_head = retval_next_tmp;
      atomic_store_explicit(&retval->next, NULL, memory_order_relaxed);
    } else {
      q->shadow_head = NULL;
      if (atomic_load_explicit(&q->tail, memory_order_relaxed) == retval) {
        atomic_store_explicit(&q->tail, NULL, memory_order_relaxed);
      }
    }
  }
  return retval;
} /*}}} */

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q) { /*{{{ */
  assert(q);
  while (1) {
    qt_threadqueue_node_t *node = qt_internal_NEMESIS_dequeue_st(&q->q);
    if (node) {
      qthread_t *retval = node->thread;
      assert(atomic_load_explicit(&node->next, memory_order_relaxed) == NULL);
      (void)qthread_incr(&(q->advisory_queuelen), -1);
      FREE_TQNODE(node);
      qthread_thread_free(retval);
    } else {
      break;
    }
  }
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
  QTHREAD_COND_DESTROY(q->trigger);
#endif
  FREE_THREADQUEUE(q);
} /*}}} */

void INTERNAL qthread_steal_enable(void) {}

void INTERNAL qthread_steal_disable(void) {}

#ifdef QTHREAD_PARANOIA
static void sanity_check_tq(NEMESIS_queue *q) { /*{{{*/
  qt_threadqueue_node_t *curs;

  assert(q);
  /*if (q->head != NULL) {
   *  assert(q->tail != NULL);
   * }*/
  if (q->shadow_head) { assert(q->head != q->shadow_head); }
  if (atomic_load_explicit(&q->tail, memory_order_relaxed) != NULL) {
    if (atomic_load_explicit(&q->head, memory_order_relaxed) == NULL) {
      assert(q->shadow_head != NULL);
    }
  }
  if ((atomic_load_explicit(&q->head, memory_order_relaxed) != NULL) ||
      (atomic_load_explicit(&q->tail, memory_order_relaxed) != NULL)) {
    if (q->shadow_head) {
      curs = q->shadow_head;
      assert(curs->thread);
      assert(curs->thread != (void *)0x7777777777777777);
      while (atomic_load_explicit(&curs->next, memory_order_relaxed)) {
        curs = atomic_load_explicit(&curs->next, memory_order_relaxed);
        assert(curs->thread);
        assert(curs->thread != (void *)0x7777777777777777);
      }
    }
    if (q->head) {
      curs = q->head;
      assert(curs->thread);
      assert(curs->thread != (void *)0x7777777777777777);
      while (atomic_load_explicit(&curs->next, memory_order_relaxed)) {
        curs = atomic_load_explicit(&curs->next, memory_order_relaxed);
        assert(curs->thread);
        assert(curs->thread != (void *)0x7777777777777777);
      }
    }
  }
} /*}}}*/

#endif /* ifdef QTHREAD_PARANOIA */

qthread_shepherd_id_t INTERNAL
qt_threadqueue_choose_dest(qthread_shepherd_t *curr_shep) {
  qthread_shepherd_id_t dest_shep_id = 0;

  if (curr_shep) {
    dest_shep_id = curr_shep->sched_shepherd++;
    curr_shep->sched_shepherd *= (qlib->nshepherds > (dest_shep_id + 1));
  } else {
    dest_shep_id = (qthread_shepherd_id_t)qthread_internal_incr_mod(
      &qlib->sched_shepherd, qlib->nshepherds, &qlib->sched_shepherd_lock);
  }

  return dest_shep_id;
}

void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict t) { /*{{{ */
  qt_threadqueue_node_t *node, *prev;

  assert(q);
  assert(t);

  PARANOIA(sanity_check_tq(&q->q));

  node = ALLOC_TQNODE();
  assert(node != NULL);
  node->thread = t;
  atomic_store_explicit(&node->next, NULL, memory_order_release);

  prev = qt_internal_atomic_swap_ptr((void **)&(q->q.tail), node);

  if (prev == NULL) {
    atomic_store_explicit(&q->q.head, node, memory_order_relaxed);
  } else {
    atomic_store_explicit(&prev->next, node, memory_order_relaxed);
  }
  PARANOIA(sanity_check_tq(&q->q));
  (void)qthread_incr(&(q->advisory_queuelen), 1);
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
  /* awake waiter */
  /* Yes, this needs to be here, to prevent reading frustration being hoisted
   * to before the enqueue operations. */
  MACHINE_FENCE;
  if (q->frustration) {
    QTHREAD_COND_LOCK(q->trigger);
    if (q->frustration) {
      q->frustration = 0;
      QTHREAD_COND_SIGNAL(q->trigger);
    }
    QTHREAD_COND_UNLOCK(q->trigger);
  }
#endif /* ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE */
} /*}}} */

void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict t) { /*{{{ */
  qt_threadqueue_enqueue(q, t);
} /*}}} */

ssize_t INTERNAL
qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q) { /*{{{ */
  assert(q);
  return q->advisory_queuelen;
} /*}}} */

qthread_t INTERNAL *
qt_scheduler_get_thread(qt_threadqueue_t *q,
                        qt_threadqueue_private_t *Q_UNUSED(qc),
                        uint_fast8_t Q_UNUSED(active)) { /*{{{ */
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
  int i;
#endif /* QTHREAD_CONDWAIT_BLOCKING_QUEUE */
  PARANOIA(sanity_check_tq(&q->q));
  qt_threadqueue_node_t *node = qt_internal_NEMESIS_dequeue(&q->q);
  qthread_t *retval;

  PARANOIA(sanity_check_tq(&q->q));
  if (node == NULL) {
#ifdef QTHREAD_CONDWAIT_BLOCKING_QUEUE
    i = num_spins_before_condwait;
    while (q->q.shadow_head == NULL &&
           atomic_load_explicit(&q->q.head, memory_order_relaxed) == NULL &&
           i > 0) {
      SPINLOCK_BODY();
      i--;
    }
#endif /* QTHREAD_CONDWAIT_BLOCKING_QUEUE */

    while (q->q.shadow_head == NULL &&
           atomic_load_explicit(&q->q.head, memory_order_relaxed) == NULL) {
#ifndef QTHREAD_CONDWAIT_BLOCKING_QUEUE
      SPINLOCK_BODY();
#else
      if (qthread_incr(&q->frustration, 1) > 1000) {
        QTHREAD_COND_LOCK(q->trigger);
        if (q->frustration > 1000) { QTHREAD_COND_WAIT(q->trigger); }
        QTHREAD_COND_UNLOCK(q->trigger);
      }
#endif /* ifdef USE_HARD_POLLING */
    }
    node = qt_internal_NEMESIS_dequeue(&q->q);
  }
  assert(node);
  assert(atomic_load_explicit(&node->next, memory_order_relaxed) == NULL);
  (void)qthread_incr(&(q->advisory_queuelen), -1);
  retval = node->thread;
  FREE_TQNODE(node);
  PARANOIA(sanity_check_tq(&q->q));
  return retval;
} /*}}} */

/* walk queue removing all tasks matching this description */
void INTERNAL qt_threadqueue_filter(qt_threadqueue_t *q,
                                    qt_threadqueue_filter_f f) { /*{{{*/
  NEMESIS_queue tmp;
  qt_threadqueue_node_t *curs, *prev;

  assert(q != NULL);

  atomic_init(&tmp.head, NULL);
  atomic_init(&tmp.tail, NULL);
  tmp.shadow_head = NULL;
  tmp.nemesis_advisory_queuelen = 0;
  PARANOIA(sanity_check_tq(&q->q));
  while ((curs = qt_internal_NEMESIS_dequeue_st(&q->q))) {
    qthread_t *t = curs->thread;
    PARANOIA(sanity_check_tq(&tmp));
    PARANOIA(sanity_check_tq(&q->q));
    switch (f(t)) {
      case IGNORE_AND_CONTINUE: // ignore, move on
        prev = qt_internal_atomic_swap_ptr((void **)&(tmp.tail), curs);
        if (prev == NULL) {
          atomic_store_explicit(&tmp.head, curs, memory_order_relaxed);
        } else {
          atomic_store_explicit(&prev->next, curs, memory_order_relaxed);
        }
        tmp.nemesis_advisory_queuelen++;
        break;
      case IGNORE_AND_STOP: // ignore, stop looking
        prev = qt_internal_atomic_swap_ptr((void **)&(tmp.tail), curs);
        if (prev == NULL) {
          atomic_store_explicit(&tmp.head, curs, memory_order_relaxed);
        } else {
          atomic_store_explicit(&prev->next, curs, memory_order_relaxed);
        }
        tmp.nemesis_advisory_queuelen++;
        goto pushback;
      case REMOVE_AND_CONTINUE: // remove, move on
        FREE_TQNODE(curs);
        break;
      case REMOVE_AND_STOP: // remove, stop looking
        FREE_TQNODE(curs);
        goto pushback;
    }
  }
pushback:
  /* dequeue the rest of the queue */
  PARANOIA(sanity_check_tq(&tmp));
  if (atomic_load_explicit(&q->q.head, memory_order_relaxed)) {
    prev = qt_internal_atomic_swap_ptr((void **)&(tmp.tail), q->q.head);
    if (prev == NULL) {
      atomic_store_explicit(
        &tmp.head,
        atomic_load_explicit(&q->q.head, memory_order_relaxed),
        memory_order_relaxed);
    } else {
      atomic_store_explicit(
        &prev->next,
        atomic_load_explicit(&q->q.head, memory_order_relaxed),
        memory_order_relaxed);
    }
    tmp.nemesis_advisory_queuelen += q->advisory_queuelen;
    atomic_store_explicit(
      &tmp.tail,
      atomic_load_explicit(&q->q.tail, memory_order_relaxed),
      memory_order_relaxed);
  }
  atomic_store_explicit(&q->q.head,
                        atomic_load_explicit(&tmp.head, memory_order_relaxed),
                        memory_order_relaxed);
  atomic_store_explicit(&q->q.tail,
                        atomic_load_explicit(&tmp.tail, memory_order_relaxed),
                        memory_order_relaxed);
  q->q.shadow_head = NULL;
  q->advisory_queuelen = tmp.nemesis_advisory_queuelen;
  PARANOIA(sanity_check_tq(&q->q));
} /*}}}*/

/* some place-holder functions */
void INTERNAL qthread_steal_stat(void) {}

void INTERNAL qthread_cas_steal_stat(void) {}

qthread_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                    void *value) {
  return NULL;
}

size_t INTERNAL qt_threadqueue_policy(const enum threadqueue_policy policy) {
  switch (policy) {
    case SINGLE_WORKER: return THREADQUEUE_POLICY_TRUE;
    default: return THREADQUEUE_POLICY_UNSUPPORTED;
  }
}

/* vim:set expandtab: */
