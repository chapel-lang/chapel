#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* System Headers */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Public Headers */
#include "qthread/barrier.h"
#include "qthread/qthread.h"

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_atomics.h"
#include "qt_barrier.h"
#include "qt_initialized.h" // for qthread_library_initialized
#include "qt_macros.h"
#include "qt_mpool.h"
#include "qt_subsystems.h"
#include "qt_visibility.h"

struct qt_barrier_s {
  aligned_t in_gate;
  aligned_t out_gate;
  aligned_t blockers;
  size_t max_blockers;
};

static union {
  qt_mpool pool;
  void *vp;
} fbp;
QTHREAD_CASLOCK_EXPLICIT_DECL(fbp_caslock)

static qt_barrier_t *global_barrier = NULL;

static void cleanup_barrier(void) {
  if (fbp.pool) { qt_mpool_destroy(fbp.pool); }
  fbp.pool = NULL;
}

void INTERNAL qt_barrier_internal_init(void) {
  fbp.pool = NULL;
  QTHREAD_CASLOCK_EXPLICIT_INIT(fbp_caslock);
}

qt_barrier_t API_FUNC *qt_barrier_create(size_t max_threads,
                                         qt_barrier_btype Q_UNUSED(type)) {
  qt_barrier_t *b;

  if (fbp.pool == NULL) {
    qt_mpool bp = qt_mpool_create(sizeof(struct qt_barrier_s));
    if (QT_CAS_(fbp.vp, NULL, bp, fbp_caslock) != NULL) {
      /* someone else created an mpool first */
      qt_mpool_destroy(bp);
    } else {
      qthread_internal_cleanup(cleanup_barrier);
    }
  }
  b = qt_mpool_alloc(fbp.pool);
  b->blockers = 0;
  b->max_blockers = max_threads;
  b->in_gate = 0;
  b->out_gate = 0;
  qthread_empty(&b->out_gate);
  return b;
}

void API_FUNC qt_barrier_enter_id(qt_barrier_t *b, size_t Q_UNUSED(shep)) {
  qt_barrier_enter(b);
}

void API_FUNC qt_barrier_enter(qt_barrier_t *b) {
  aligned_t waiters;

  assert(qthread_library_initialized);
  qassert_retvoid(b);
  /* pass through the in_gate */
  qthread_readFF(NULL, &b->in_gate);
  /* increment the blocker count */
  waiters = qthread_incr(&b->blockers, 1) + 1;
  assert(waiters <= b->max_blockers);
  if (waiters == b->max_blockers) {
    /* last guy into the barrier, close the in_gate, open the out_gate */
    qthread_empty(&b->in_gate);
    qthread_fill(&b->out_gate);
  } else {
    /* pass through the out_gate */
    qthread_readFF(NULL, &b->out_gate);
  }
  /* I'm on the way out, so decrement the blocker count */
  waiters = qthread_incr(&b->blockers, -1) - 1;
  if (waiters == 0) {
    /* last guy out of the barrier, close the out_gate, open the in_gate */
    qthread_empty(&b->out_gate);
    qthread_fill(&b->in_gate);
  }
}

void API_FUNC qt_barrier_resize(qt_barrier_t *restrict b, size_t new_size) {
  assert(b->blockers == 0);
  b->max_blockers = new_size;
}

void API_FUNC qt_barrier_destroy(qt_barrier_t *b) {
  assert(fbp.pool != NULL);
  while (b->blockers > 0) qthread_yield();
  qthread_fill(&b->out_gate);
  qthread_fill(&b->in_gate);
  qt_mpool_free(fbp.pool, b);
}

void qt_global_barrier(void) {
  assert(global_barrier);
  qt_barrier_enter(global_barrier);
}

void qt_global_barrier_init(size_t size, int debug) {
  if (global_barrier == NULL) {
    global_barrier = qt_barrier_create(size, 0);
    assert(global_barrier);
  }
}

void qt_global_barrier_destroy(void) {
  if (global_barrier) {
    qt_barrier_destroy(global_barrier);
    global_barrier = NULL;
  }
}

void qt_global_barrier_resize(size_t size) {
  qt_barrier_resize(global_barrier, size);
}

/* vim:set expandtab: */
