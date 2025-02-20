#include "qthread/qthread.h"

#include "qt_alloc.h"
#include "qt_asserts.h"
#include "qt_mpool.h"
#include "qt_qthread_mgmt.h"   /* for qthread_internal_self() */
#include "qt_qthread_struct.h" /* to pass data back to worker */
#include "qt_subsystems.h"     /* for qthread_internal_cleanup() */
#include "qt_threadstate.h"
#include "qt_visibility.h"
#include "qthread_innards.h" /* for qlib */

#include "qt_queue.h"

/* Memory Management */
static qt_mpool node_pool = NULL;
#define ALLOC_TQNODE() (qthread_queue_node_t *)qt_mpool_alloc(node_pool)
#define FREE_TQNODE(n) qt_mpool_free(node_pool, (n))

static void qthread_queue_subsystem_shutdown(void) {
  qt_mpool_destroy(node_pool);
}

void INTERNAL qthread_queue_subsystem_init(void) {
  node_pool = qt_mpool_create(sizeof(qthread_queue_node_t));
  qthread_internal_cleanup(qthread_queue_subsystem_shutdown);
}

qthread_queue_t API_FUNC qthread_queue_create(uint8_t flags, aligned_t length) {
  qthread_queue_t q = qt_calloc(1, sizeof(struct qthread_queue_s));

  assert(q);
  if (flags & QTHREAD_QUEUE_MULTI_JOIN) {
    q->type = NEMESIS;
  } else if (flags & QTHREAD_QUEUE_MULTI_JOIN_LENGTH) {
    q->type = NEMESIS_LENGTH;
  } else if (flags & QTHREAD_QUEUE_CAPPED) {
    q->type = CAPPED;
    q->q.capped.maxmembers = (aligned_t)length;
    q->q.capped.membercount = 0;
    q->q.capped.busy = 0;
    q->q.capped.members = MALLOC(sizeof(qthread_t *) * length);
    assert(q->q.capped.members);
  } else {
    q->type = NOSYNC;
  }
  return q;
}

aligned_t API_FUNC qthread_queue_length(qthread_queue_t q) {
  assert(q);
  switch (q->type) {
    case NEMESIS_LENGTH:
      return atomic_load_explicit(&q->q.nemesis.length, memory_order_relaxed);
    case CAPPED: return q->q.capped.membercount;
    default: return 0;
  }
}

int API_FUNC qthread_queue_join(qthread_queue_t q) {
  assert(q);
  qthread_t *me = qthread_internal_self();
  atomic_store_explicit(
    &me->thread_state, QTHREAD_STATE_QUEUE, memory_order_relaxed);
  me->rdata->blockedon.queue = q;
  qthread_back_to_master(me);
  return QTHREAD_SUCCESS;
}

void INTERNAL qthread_queue_internal_enqueue(qthread_queue_t q, qthread_t *t) {
  switch (q->type) {
    case NOSYNC: qthread_queue_internal_nosync_enqueue(&q->q.nosync, t); break;
    case NEMESIS:
      qthread_queue_internal_NEMESIS_enqueue(&q->q.nemesis, t);
      break;
    case NEMESIS_LENGTH:
      qthread_queue_internal_NEMESIS_enqueue(&q->q.nemesis, t);
      atomic_fetch_add_explicit(
        &q->q.nemesis.length, 1ull, memory_order_relaxed);
      break;
    case CAPPED: qthread_queue_internal_capped_enqueue(&q->q.capped, t); break;
    case MTS: QTHREAD_TRAP();
  }
}

static void qthread_queue_internal_launch(qthread_t *t,
                                          qthread_shepherd_t *cur_shep) {
  assert(t);
  assert(cur_shep);
  atomic_store_explicit(
    &t->thread_state, QTHREAD_STATE_RUNNING, memory_order_relaxed);
  if ((atomic_load_explicit(&t->flags, memory_order_relaxed) &
       QTHREAD_UNSTEALABLE) &&
      (t->rdata->shepherd_ptr != cur_shep)) {
    qt_threadqueue_enqueue(t->rdata->shepherd_ptr->ready, t);
  } else {
    qt_threadqueue_enqueue(cur_shep->ready, t);
  }
}

int API_FUNC qthread_queue_release_one(qthread_queue_t q) {
  assert(q);
  qthread_t *t;
  switch (q->type) {
    case NOSYNC: t = qthread_queue_internal_nosync_dequeue(&q->q.nosync); break;
    case NEMESIS:
      t = qthread_queue_internal_NEMESIS_dequeue(&q->q.nemesis);
      break;
    case NEMESIS_LENGTH:
      t = qthread_queue_internal_NEMESIS_dequeue(&q->q.nemesis);
      atomic_fetch_add_explicit(
        &q->q.nemesis.length, (aligned_t)-1, memory_order_relaxed);
      break;
    case CAPPED: t = qthread_queue_internal_capped_dequeue(&q->q.capped); break;
    default: QTHREAD_TRAP();
  }
  qthread_shepherd_id_t destination = t->target_shepherd;
  if (destination == NO_SHEPHERD) {
    qthread_queue_internal_launch(t, qthread_internal_getshep());
  } else {
    qthread_queue_internal_launch(t, &qlib->shepherds[destination]);
  }
  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_queue_release_all(qthread_queue_t q) {
  assert(q);
  qthread_t *t;
  qthread_shepherd_t *shep = qthread_internal_getshep();
  switch (q->type) {
    case NOSYNC:
      while ((t = qthread_queue_internal_nosync_dequeue(&q->q.nosync)) !=
             NULL) {
        qthread_queue_internal_launch(t, shep);
      }
      break;
    case NEMESIS:
      while ((t = qthread_queue_internal_NEMESIS_dequeue(&q->q.nemesis)) !=
             NULL) {
        qthread_queue_internal_launch(t, shep);
      }
      break;
    case NEMESIS_LENGTH: {
      aligned_t const count =
        atomic_load_explicit(&q->q.nemesis.length, memory_order_relaxed);
      for (aligned_t c = 0; c < count; c++) {
        t = qthread_queue_internal_NEMESIS_dequeue(&q->q.nemesis);
        assert(t);
        if (t) { qthread_queue_internal_launch(t, shep); }
      }
      atomic_fetch_add_explicit(
        &q->q.nemesis.length, -count, memory_order_relaxed);
      break;
    }
    case CAPPED: {
      size_t const membercount = q->q.capped.membercount;
      qthread_t **members_copy = MALLOC(sizeof(qthread_t *) * membercount);
      assert(members_copy);
      while (q->q.capped.busy != 0) SPINLOCK_BODY();
      memcpy(
        members_copy, q->q.capped.members, sizeof(qthread_t *) * membercount);
      memset(q->q.capped.members, 0, sizeof(qthread_t *) * membercount);
      if (membercount == q->q.capped.maxmembers) {
        q->q.capped.membercount = 0;
      }
      for (size_t c = 0; c < q->q.capped.membercount; c++) {
        if (members_copy[c] != NULL) {
          qthread_queue_internal_launch(members_copy[c], shep);
        }
      }
      qt_free(members_copy);
      break;
    }
    default: QTHREAD_TRAP();
  }
  return QTHREAD_SUCCESS;
}

int API_FUNC qthread_queue_destroy(qthread_queue_t q) {
  assert(q);
  switch (q->type) {
    case NOSYNC:
    case NEMESIS:
    case NEMESIS_LENGTH: break;
    case CAPPED:
      FREE(q->q.capped.members, sizeof(qthread_t *) * q->q.capped.maxmembers);
      break;
    default: QTHREAD_TRAP();
  }
  FREE(q, sizeof(struct qthread_queue_s));
  return QTHREAD_SUCCESS;
}

void INTERNAL qthread_queue_internal_nosync_enqueue(qthread_queue_nosync_t *q,
                                                    qthread_t *t) {
  qthread_queue_node_t *node = ALLOC_TQNODE();

  assert(node);
  assert(q);
  assert(t);

  atomic_store_explicit(&node->thread, t, memory_order_relaxed);
  atomic_store_explicit(&node->next, NULL, memory_order_relaxed);
  if (atomic_load_explicit(&q->tail, memory_order_relaxed) == NULL) {
    atomic_store_explicit(&q->head, node, memory_order_relaxed);
  } else {
    atomic_store_explicit(
      &atomic_load_explicit(&q->tail, memory_order_relaxed)->next,
      node,
      memory_order_relaxed);
  }
  atomic_store_explicit(&q->tail, node, memory_order_relaxed);
}

qthread_t INTERNAL *
qthread_queue_internal_nosync_dequeue(qthread_queue_nosync_t *q) {
  qthread_queue_node_t *node;
  qthread_t *t = NULL;

  assert(q);

  node = atomic_load_explicit(&q->head, memory_order_relaxed);
  if (node) {
    atomic_store_explicit(
      &q->head,
      atomic_load_explicit(&node->next, memory_order_relaxed),
      memory_order_relaxed);
    t = atomic_load_explicit(&node->thread, memory_order_relaxed);
    FREE_TQNODE(node);
  }
  return t;
}

void INTERNAL qthread_queue_internal_NEMESIS_enqueue(qthread_queue_NEMESIS_t *q,
                                                     qthread_t *t) {
  qthread_queue_node_t *node, *prev;

  node = ALLOC_TQNODE();
  assert(node != NULL);
  atomic_store_explicit(&node->thread, t, memory_order_relaxed);
  atomic_store_explicit(&node->next, NULL, memory_order_relaxed);

  prev = qt_internal_atomic_swap_ptr((void **)&(q->tail), node);
  if (prev == NULL) {
    atomic_store_explicit(&q->head, node, memory_order_relaxed);
  } else {
    atomic_store_explicit(&prev->next, node, memory_order_relaxed);
  }
}

qthread_t INTERNAL *
qthread_queue_internal_NEMESIS_dequeue(qthread_queue_NEMESIS_t *q) {
  if (!q->shadow_head) {
    if (!atomic_load_explicit(&q->head, memory_order_relaxed)) { return NULL; }
    q->shadow_head = atomic_load_explicit(&q->head, memory_order_relaxed);
    atomic_store_explicit(&q->head, NULL, memory_order_relaxed);
  }

  qthread_queue_node_t *const dequeued = q->shadow_head;
  if (dequeued != NULL) {
    if (atomic_load_explicit(&dequeued->next, memory_order_relaxed) != NULL) {
      q->shadow_head =
        atomic_load_explicit(&dequeued->next, memory_order_relaxed);
      atomic_store_explicit(&dequeued->next, NULL, memory_order_relaxed);
    } else {
      qthread_queue_node_t *old;
      q->shadow_head = NULL;
      old = qthread_cas_ptr((void **)&(q->tail), dequeued, NULL);
      if (old != dequeued) {
        while (atomic_load_explicit(&dequeued->next, memory_order_relaxed) ==
               NULL)
          SPINLOCK_BODY();
        q->shadow_head =
          atomic_load_explicit(&dequeued->next, memory_order_relaxed);
        atomic_store_explicit(&dequeued->next, NULL, memory_order_relaxed);
      }
    }
    qthread_t *retval =
      atomic_load_explicit(&dequeued->thread, memory_order_relaxed);
    FREE_TQNODE(dequeued);
    return retval;
  } else {
    return NULL;
  }
}

void INTERNAL qthread_queue_internal_capped_enqueue(qthread_queue_capped_t *q,
                                                    qthread_t *t) {
  aligned_t offset;

  assert(q);
  assert(t);

  if (q->membercount >= q->maxmembers) { return; }
  qthread_incr(&q->busy, 1);
  offset = qthread_incr(&q->membercount, 1);
  qassert_retvoid(offset >= q->maxmembers);
  q->members[offset] = t;
  qthread_incr(&q->busy, (aligned_t)-1);
}

qthread_t INTERNAL *
qthread_queue_internal_capped_dequeue(qthread_queue_capped_t *q) {
  assert(q);
  qthread_t *t = NULL;
  size_t i = 0;
  while (q->busy != 0) SPINLOCK_BODY();
  for (; i < q->membercount && q->members[i] == NULL; i++);
  if (i < q->membercount) {
    assert(q->members[i]);
    t = q->members[i];
    q->members[i] = NULL;
  }
  return t;
}

/* vim:set expandtab: */
