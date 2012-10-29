#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h>
#include <stdio.h>

/* System Compatibility Header */
#include "qthread-int.h"

/* Installed Headers */
#include <qthread/qthread.h>
#include <qthread/feb_barrier.h>

/* Internal Headers */
#include "qt_atomics.h"
#include "qt_mpool.h"
#include "qthread_innards.h"
#include "qt_visibility.h"
#include "qt_debug.h"
#include "qthread_asserts.h"

struct qt_feb_barrier_s {
    syncvar_t in_gate;
    syncvar_t out_gate;
    aligned_t blockers;
    size_t    max_blockers;
};

static union {
    qt_mpool pool;
    void    *vp;
} fbp;
QTHREAD_CASLOCK_EXPLICIT_DECL(fbp_caslock)
static Q_UNUSED qt_feb_barrier_t *global_barrier = NULL;

#ifndef UNPOOLED
static void cleanup_feb_barrier(void)
{
    if (fbp.pool) {
        qt_mpool_destroy(fbp.pool);
    }
    fbp.pool = NULL;
}

#endif

void INTERNAL qt_feb_barrier_internal_init(void)
{
    fbp.pool = NULL;
    QTHREAD_CASLOCK_EXPLICIT_INIT(fbp_caslock);
}

qt_feb_barrier_t API_FUNC *qt_feb_barrier_create(size_t max_threads)
{
    qt_feb_barrier_t *b;

#ifndef UNPOOLED
    if (fbp.pool == NULL) {
        qt_mpool bp = qt_mpool_create(sizeof(struct qt_feb_barrier_s));
        if (QT_CAS_(fbp.vp, NULL, bp, fbp_caslock) != NULL) {
            /* someone else created an mpool first */
            qt_mpool_destroy(bp);
        } else {
            qthread_internal_cleanup(cleanup_feb_barrier);
        }
    }
    b = qt_mpool_alloc(fbp.pool);
#else /* ifndef UNPOOLED */
    b = MALLOC(sizeof(struct qt_feb_barrier_s));
#endif /* ifndef UNPOOLED */
    b->blockers     = 0;
    b->max_blockers = max_threads;
    b->in_gate.u.w  = 0;
    b->out_gate.u.w = 0;
    qthread_syncvar_fill(&b->in_gate);
    qthread_syncvar_empty(&b->out_gate);
    return b;
}

void API_FUNC qt_feb_barrier_enter(qt_feb_barrier_t *b)
{
    aligned_t waiters;

    assert(qthread_library_initialized);
    qassert_retvoid(b);
    /* pass through the in_gate */
    qthread_syncvar_readFF(NULL, &b->in_gate);
    /* increment the blocker count */
    waiters = qthread_incr(&b->blockers, 1) + 1;
    assert(waiters <= b->max_blockers);
    if (waiters == b->max_blockers) {
        /* last guy into the barrier, close the in_gate, open the out_gate */
        qthread_syncvar_empty(&b->in_gate);
        qthread_syncvar_fill(&b->out_gate);
    } else {
        /* pass through the out_gate */
        qthread_syncvar_readFF(NULL, &b->out_gate);
    }
    /* I'm on the way out, so decrement the blocker count */
    waiters = qthread_incr(&b->blockers, -1) - 1;
    if (waiters == 0) {
        /* last guy out of the barrier, close the out_gate, open the in_gate */
        qthread_syncvar_empty(&b->out_gate);
        qthread_syncvar_fill(&b->in_gate);
    }
}

void API_FUNC qt_feb_barrier_destroy(qt_feb_barrier_t *b)
{
#ifndef UNPOOLED
    assert(fbp.pool != NULL);
#endif
    while (b->blockers > 0) qthread_yield();
    qthread_syncvar_fill(&b->out_gate);
    qthread_syncvar_fill(&b->in_gate);
#ifndef UNPOOLED
    qt_mpool_free(fbp.pool, b);
#else
    FREE(b, sizeof(struct qt_feb_barrier_s));
#endif
}

#ifdef QTHREAD_GLOBAL_FEB_BARRIER
void INTERNAL qt_global_barrier(qthread_t *me)
{
    assert(global_barrier);
    qt_feb_barrier_enter(me, global_barrier);
}

// allow barrer initization from C
void INTERNAL qt_global_barrier_init(int size,
                            int debug)
{
    if (global_barrier == NULL) {
        global_barrier = qt_feb_barrier_create(NULL, size);
        assert(global_barrier);
    }
}

void INTERNAL qt_global_barrier_destroy(void)
{
    if (global_barrier) {
        qt_feb_barrier_destroy(global_barrier);
        global_barrier = NULL;
    }
}

#endif /* ifdef QTHREAD_GLOBAL_FEB_BARRIER */
/* vim:set expandtab: */
