#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* The API */
#include "qthread/qthread.h"

/* System Headers */
#ifndef QTHREAD_NOALIGNCHECK
# include <stdio.h>
#endif

/* Internal Headers */
#include "qt_hash.h"
#include "qthread_innards.h"
#include "qt_qthread_struct.h"
#include "qt_blocking_structs.h"
#include "qt_addrstat.h"
#include "qt_threadqueues.h"
#include "qt_debug.h"
#include "qt_internal_feb.h"

/* Local Types */
typedef enum bt {
    WRITEEF,
    WRITEEF_NB,
    WRITEF,
    READFF,
    READFF_NB,
    READFE,
    READFE_NB,
    FILL,
    EMPTY
} blocker_type;
typedef struct {
    pthread_mutex_t lock;
    void           *a;
    void           *b;
    blocker_type    type;
    int             retval;
} qthread_feb_blocker_t;

/* Local Prototypes */
static QINLINE void qthread_gotlock_fill(qthread_shepherd_t *shep,
                                         qthread_addrstat_t *m,
                                         void               *maddr,
                                         const char          recursive);
static QINLINE void qthread_gotlock_empty(qthread_shepherd_t *shep,
                                          qthread_addrstat_t *m,
                                          void               *maddr,
                                          const char          recursive);

/* functions to implement FEB locking/unlocking */

#ifdef QTHREAD_COUNT_THREADS
# define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN) qthread_internal_incr(&qlib->TYPE ## _stripes[(BIN)], &qlib->TYPE ## _stripes_locks[(BIN)], 1)
#else
# define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN) do { } while(0)
#endif

static aligned_t qthread_feb_blocker_thread(void *arg)
{                                      /*{{{ */
    qthread_feb_blocker_t *const restrict a = (qthread_feb_blocker_t *)arg;

    switch (a->type) {
        case READFE:
            a->retval = qthread_readFE(a->a, a->b);
            break;
        case READFE_NB:
            a->retval = qthread_readFE_nb(a->a, a->b);
            break;
        case READFF:
            a->retval = qthread_readFF(a->a, a->b);
            break;
        case READFF_NB:
            a->retval = qthread_readFF_nb(a->a, a->b);
            break;
        case WRITEEF:
            a->retval = qthread_writeEF(a->a, a->b);
            break;
        case WRITEEF_NB:
            a->retval = qthread_writeEF_nb(a->a, a->b);
            break;
        case WRITEF:
            a->retval = qthread_writeF(a->a, a->b);
            break;
        case FILL:
            a->retval = qthread_fill(a->a);
            break;
        case EMPTY:
            a->retval = qthread_empty(a->a);
            break;
    }
    pthread_mutex_unlock(&(a->lock));
    return 0;
}                                      /*}}} */

static int qthread_feb_blocker_func(void        *dest,
                                    void        *src,
                                    blocker_type t)
{   /*{{{*/
    qthread_feb_blocker_t args = { PTHREAD_MUTEX_INITIALIZER, dest, src, t, QTHREAD_SUCCESS };

    pthread_mutex_lock(&args.lock);
    qthread_fork(qthread_feb_blocker_thread, &args, NULL);
    pthread_mutex_lock(&args.lock);
    pthread_mutex_unlock(&args.lock);
    pthread_mutex_destroy(&args.lock);
    return args.retval;
} /*}}}*/

/* The lock ordering in these functions is very particular, and is designed to
 * reduce the impact of having only one hashtable. Don't monkey with it unless
 * you REALLY know what you're doing! If one hashtable becomes a problem, we
 * may need to move to a new mechanism.
 */

/* This is just a little function that should help in debugging */
int API_FUNC qthread_feb_status(const aligned_t *addr)
{                      /*{{{ */
    const aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m;
    int                 status  = 1; /* full */
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(addr);

    QALIGN(addr, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]); {
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin],
                                                     (void *)alignedaddr);
        if (m) {
            QTHREAD_FASTLOCK_LOCK(&m->lock);
            status = m->full;
            QTHREAD_FASTLOCK_UNLOCK(&m->lock);
        }
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_BEHAVIOR, "addr %p is %i", addr,
                  status);
    return status;

#else /* ifndef SST */
    QALIGN(addr, alignedaddr);
    return PIM_feb_is_full((void *)alignedaddr);
#endif /* ifndef SST */
}                      /*}}} */

/* this function removes the FEB data structure for the address maddr from the
 * hash table */
static QINLINE void qthread_FEB_remove(void *maddr)
{                      /*{{{ */
    qthread_addrstat_t *m;
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(maddr);

    qthread_debug(LOCK_DETAILS,
                  "attempting removal %p\n", maddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]); {
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], maddr);
        if (m) {
            QTHREAD_FASTLOCK_LOCK(&(m->lock));
            if ((m->FEQ == NULL) && (m->EFQ == NULL) && (m->FFQ == NULL) &&
                (m->full == 1)) {
                qthread_debug(LOCK_DETAILS,
                              "all lists are empty, and status is full\n");
                qassertnot(qt_hash_remove_locked(qlib->FEBs[lockbin], maddr), 0);
            } else {
                QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
                qthread_debug(LOCK_DETAILS,
                              "address cannot be removed; in use\n");
                m = NULL;
            }
        }
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    if (m != NULL) {
        QTHREAD_FASTLOCK_UNLOCK(&m->lock);
        qthread_addrstat_delete(m);
    }
}                      /*}}} */

static QINLINE void qthread_gotlock_empty(qthread_shepherd_t *shep,
                                          qthread_addrstat_t *m,
                                          void               *maddr,
                                          const char          recursive)
{                      /*{{{ */
    qthread_addrres_t *X = NULL;
    int                removeable;

    m->full = 0;
    QTHREAD_EMPTY_TIMER_START(m);
    if (m->EFQ != NULL) {
        /* dQ */
        X      = m->EFQ;
        m->EFQ = X->next;
        /* op */
        if (maddr && (maddr != X->addr)) {
            *(aligned_t *)maddr = *(X->addr);
        }
        /* requeue */
        X->waiter->thread_state = QTHREAD_STATE_RUNNING;
        qt_threadqueue_enqueue(X->waiter->rdata->shepherd_ptr->ready, X->waiter);
        FREE_ADDRRES(X);
        qthread_gotlock_fill(shep, m, maddr, 1);
    }
    if ((m->full == 1) && (m->EFQ == NULL) && (m->FEQ == NULL) && (m->FFQ == NULL)) {
        removeable = 1;
    } else {
        removeable = 0;
    }
    if (recursive == 0) {
        QTHREAD_FASTLOCK_UNLOCK(&m->lock);
        if (removeable) {
            qthread_FEB_remove(maddr);
        }
    }
}                      /*}}} */

static QINLINE void qthread_gotlock_fill(qthread_shepherd_t *shep,
                                         qthread_addrstat_t *m,
                                         void               *maddr,
                                         const char          recursive)
{                      /*{{{ */
    qthread_addrres_t *X = NULL;
    int                removeable;

    qthread_debug(LOCK_DETAILS, "m(%p), addr(%p)\n", m, maddr);
    m->full = 1;
    QTHREAD_EMPTY_TIMER_STOP(m);
    /* dequeue all FFQ, do their operation, and schedule them */
    qthread_debug(LOCK_DETAILS, "dQ all FFQ\n");
    while (m->FFQ != NULL) {
        /* dQ */
        X      = m->FFQ;
        m->FFQ = X->next;
        /* op */
        if (X->addr && (X->addr != maddr)) {
            memcpy(X->addr, maddr, sizeof(aligned_t));
        }
        /* schedule */
        if (QTHREAD_STATE_NASCENT == X->waiter->thread_state) {
            /* Note: the nascent thread is being tossed into a real live ready
             * queue for one big fat reason: the alternative involves
             * potentially locking multiple parts of the FEB hash table, not to
             * mention needing to avoid re-locking the portion we've already
             * got locked. By allowing the precond thread to have its
             * conditions checked by a shepherd, we can ensure that we don't
             * have this problem. Also, this allows the "work" of checking
             * preconds to be load-balanced by workstealing schedulers, if
             * that's important or useful. */
            if (X->waiter->target_shepherd == NULL) {
                qt_threadqueue_enqueue(shep->ready, X->waiter);
            } else {
                qt_threadqueue_enqueue(X->waiter->target_shepherd->ready, X->waiter);
            }
        } else {
            X->waiter->thread_state = QTHREAD_STATE_RUNNING;
            qt_threadqueue_enqueue(X->waiter->rdata->shepherd_ptr->ready, X->waiter);
        }
        FREE_ADDRRES(X);
    }
    if (m->FEQ != NULL) {
        /* dequeue one FEQ, do their operation, and schedule them */
        qthread_t *waiter;

        qthread_debug(LOCK_DETAILS, "dQ 1 FEQ\n");
        X      = m->FEQ;
        m->FEQ = X->next;
        /* op */
        if (X->addr && (X->addr != maddr)) {
            memcpy(X->addr, maddr, sizeof(aligned_t));
        }
        waiter               = X->waiter;
        waiter->thread_state = QTHREAD_STATE_RUNNING;
        qt_threadqueue_enqueue(waiter->rdata->shepherd_ptr->ready, waiter);
        FREE_ADDRRES(X);
        qthread_gotlock_empty(shep, m, maddr, 1);
    }
    if ((m->EFQ == NULL) && (m->FEQ == NULL) && (m->full == 1)) {
        removeable = 1;
    } else {
        removeable = 0;
    }
    if (recursive == 0) {
        QTHREAD_FASTLOCK_UNLOCK(&m->lock);
        /* now, remove it if it needs to be removed */
        if (removeable) {
            qthread_FEB_remove(maddr);
        }
    }
}                      /*}}} */

int API_FUNC qthread_empty(const aligned_t *dest)
{                      /*{{{ */
    const aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m;
    qt_hash             FEBbin;
    qthread_shepherd_t *shep = qthread_internal_getshep();

    if (!shep) {
        return qthread_feb_blocker_func((void *)dest, NULL, EMPTY);
    }
    {
        const int lockbin = QTHREAD_CHOOSE_STRIPE(dest);
        FEBbin = qlib->FEBs[lockbin];

        QALIGN(dest, alignedaddr);
        QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    }
    qt_hash_lock(FEBbin);
    {                      /* BEGIN CRITICAL SECTION */
        m = (qthread_addrstat_t *)qt_hash_get_locked(FEBbin, (void *)alignedaddr);
        if (!m) {
            /* currently full, and must be added to the hash to empty */
            m = qthread_addrstat_new(shep);
            if (!m) {
                qt_hash_unlock(FEBbin);
                return QTHREAD_MALLOC_ERROR;
            }
            m->full = 0;
            QTHREAD_EMPTY_TIMER_START(m);
            qassertnot(qt_hash_put_locked(FEBbin, (void *)alignedaddr, m), 0);
            m = NULL;
        } else {
            /* it could be either full or not, don't know */
            QTHREAD_FASTLOCK_LOCK(&m->lock);
        }
    }                      /* END CRITICAL SECTION */
    qt_hash_unlock(FEBbin);
    qthread_debug(LOCK_BEHAVIOR, "%p is now empty\n", dest);
    if (m) {
        qthread_gotlock_empty(shep, m, (void *)alignedaddr, 0);
    }
#else /* ifndef SST */
    QALIGN(dest, alignedaddr);
    PIM_feb_empty((void *)alignedaddr);
#endif /* ifndef SST */
    return QTHREAD_SUCCESS;
}                      /*}}} */

int API_FUNC qthread_fill(const aligned_t *dest)
{                      /*{{{ */
    const aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m;
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_shepherd_t *shep    = qthread_internal_getshep();

    if (!shep) {
        return qthread_feb_blocker_func((void *)dest, NULL, FILL);
    }
    QALIGN(dest, alignedaddr);
    /* lock hash */
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {                      /* BEGIN CRITICAL SECTION */
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], (void *)alignedaddr);
        if (m) {
            QTHREAD_FASTLOCK_LOCK(&m->lock);
        }
    }                                    /* END CRITICAL SECTION */
    qt_hash_unlock(qlib->FEBs[lockbin]); /* unlock hash */
    qthread_debug(LOCK_BEHAVIOR, "%p is now full\n", dest);
    if (m) {
        /* if dest wasn't in the hash, it was already full. Since it was,
         * we need to fill it. */
        qthread_gotlock_fill(shep, m, (void *)alignedaddr, 0);
    }
#else /* ifndef SST */
    QALIGN(dest, alignedaddr);
    PIM_feb_fill((unsigned int *)alignedaddr);
#endif /* ifndef SST */
    return QTHREAD_SUCCESS;
}                      /*}}} */

/* the way this works is that:
 * 1 - data is copies from src to destination
 * 2 - the destination's FEB state gets changed from empty to full
 */

int API_FUNC qthread_writeF(aligned_t *restrict const       dest,
                            const aligned_t *restrict const src)
{                      /*{{{ */
    aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m;
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_shepherd_t *shep    = qthread_internal_getshep();

    if (!shep) {
        return qthread_feb_blocker_func(dest, (void *)src, WRITEF);
    }
    qthread_debug(LOCK_BEHAVIOR, "tid %u dest=%p src=%p...\n", (shep->current)?(shep->current->thread_id):UINT_MAX, dest, src);
    QALIGN(dest, alignedaddr);
    QTHREAD_LOCK_UNIQUERECORD2(feb, dest, shep);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]); {    /* lock hash */
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], (void *)alignedaddr);
        if (!m) {
            m = qthread_addrstat_new(shep);
            if (!m) {
                qt_hash_unlock(qlib->FEBs[lockbin]);
                return QTHREAD_MALLOC_ERROR;
            }
            qassertnot(qt_hash_put_locked(qlib->FEBs[lockbin], alignedaddr, m), 0);
        }
        QTHREAD_FASTLOCK_LOCK(&m->lock);
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);    /* unlock hash */
    /* we have the lock on m, so... */
    if (dest && (dest != src)) {
        memcpy(dest, src, sizeof(aligned_t));
    }
    qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p\n", (shep->current)?(shep->current->thread_id):UINT_MAX, dest, src);
    qthread_gotlock_fill(shep, m, alignedaddr, 0);
#else /* ifndef SST */
    QALIGN(dest, alignedaddr);
    PIM_feb_empty((void *)alignedaddr);
#endif /* ifndef SST */
    return QTHREAD_SUCCESS;
}                      /*}}} */

int API_FUNC qthread_writeF_const(aligned_t *const dest,
                                  const aligned_t  src)
{                      /*{{{ */
    return qthread_writeF(dest, &src);
}                      /*}}} */

/* the way this works is that:
 * 1 - destination's FEB state must be "empty"
 * 2 - data is copied from src to destination
 * 3 - the destination's FEB state gets changed from empty to full
 */

int API_FUNC qthread_writeEF(aligned_t *restrict const       dest,
                             const aligned_t *restrict const src)
{                      /*{{{ */
    aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m;
    qthread_addrres_t  *X       = NULL;
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_t          *me      = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(febblock);

    if (!me) {
        return qthread_feb_blocker_func(dest, (void *)src, WRITEEF);
    }
    qthread_debug(LOCK_BEHAVIOR, "tid %u dest=%p src=%p...\n", me->thread_id, dest, src);
    QTHREAD_LOCK_UNIQUERECORD(feb, dest, me);
    QTHREAD_LOCK_TIMER_START(febblock);
    QALIGN(dest, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], (void *)alignedaddr);
        if (!m) {
            m = qthread_addrstat_new(me->rdata->shepherd_ptr);
            if (!m) {
                qt_hash_unlock(qlib->FEBs[lockbin]);
                return QTHREAD_MALLOC_ERROR;
            }
            qassertnot(qt_hash_put_locked(qlib->FEBs[lockbin], alignedaddr, m), 0);
        }
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_DETAILS, "data structure locked\n");
    /* by this point m is locked */
    qthread_debug(LOCK_DETAILS, "m->full == %i\n", m->full);
    if (m->full == 1) {            /* full, thus, we must block */
        QTHREAD_WAIT_TIMER_DECLARATION;
        X = ALLOC_ADDRRES(me->rdata->shepherd_ptr);
        if (X == NULL) {
            QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
            return QTHREAD_MALLOC_ERROR;
        }
        X->addr   = (aligned_t *)src;
        X->waiter = me;
        X->next   = m->EFQ;
        m->EFQ    = X;
        qthread_debug(LOCK_DETAILS, "back to parent\n");
        me->thread_state     = QTHREAD_STATE_FEB_BLOCKED;
        me->rdata->blockedon = (struct qthread_lock_s *)m;
        QTHREAD_WAIT_TIMER_START();
        qthread_back_to_master(me);
        QTHREAD_WAIT_TIMER_STOP(me, febwait);
        qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p after waiting\n", me->thread_id, dest, src);
    } else {
        if (dest && (dest != src)) {
            *(aligned_t *)dest = *(aligned_t *)src;
        }
        qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p\n", me->thread_id, dest, src);
        qthread_gotlock_fill(me->rdata->shepherd_ptr, m, alignedaddr, 0);
    }
    QTHREAD_LOCK_TIMER_STOP(febblock, me);
#else /* ifndef SST */
    QALIGN(dest, alignedaddr);
    while (PIM_feb_try_writeef(alignedaddr, src) == 1) {
        qthread_yield(me);
    }
#endif /* ifndef SST */
    return QTHREAD_SUCCESS;
}                      /*}}} */

int API_FUNC qthread_writeEF_const(aligned_t *const dest,
                                   const aligned_t  src)
{                      /*{{{ */
    return qthread_writeEF(dest, &src);
}                      /*}}} */

int INTERNAL qthread_writeEF_nb(aligned_t *restrict const       dest,
                                const aligned_t *restrict const src)
{                      /*{{{ */
    aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m;
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(dest);
    qthread_t          *me      = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(febblock);

    if (!me) {
        return qthread_feb_blocker_func(dest, (void *)src, WRITEEF);
    }
    qthread_debug(LOCK_BEHAVIOR, "tid %u dest=%p src=%p...\n", me->thread_id, dest, src);
    QTHREAD_LOCK_UNIQUERECORD(feb, dest, me);
    QTHREAD_LOCK_TIMER_START(febblock);
    QALIGN(dest, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], (void *)alignedaddr);
        if (!m) {
            m = qthread_addrstat_new(me->rdata->shepherd_ptr);
            if (!m) {
                qt_hash_unlock(qlib->FEBs[lockbin]);
                return QTHREAD_MALLOC_ERROR;
            }
            qassertnot(qt_hash_put_locked(qlib->FEBs[lockbin], alignedaddr, m), 0);
        }
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_DETAILS, "data structure locked\n");
    /* by this point m is locked */
    qthread_debug(LOCK_DETAILS, "m->full == %i\n", m->full);
    if (m->full == 1) {            /* full, thus, we must block */
        qthread_debug(LOCK_BEHAVIOR, "tid %u non-blocking fail\n", me->thread_id);
        QTHREAD_FASTLOCK_UNLOCK(&(m->lock));
        return QTHREAD_OPFAIL;
    } else {
        if (dest && (dest != src)) {
            *(aligned_t *)dest = *(aligned_t *)src;
        }
        qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p\n", me->thread_id, dest, src);
        qthread_gotlock_fill(me->rdata->shepherd_ptr, m, alignedaddr, 0);
    }
    QTHREAD_LOCK_TIMER_STOP(febblock, me);
#else /* ifndef SST */
    QALIGN(dest, alignedaddr);
    while (PIM_feb_try_writeef(alignedaddr, src) == 1) {
        return QTHREAD_OPFAIL;
    }
#endif /* ifndef SST */
    return QTHREAD_SUCCESS;
}                      /*}}} */

int INTERNAL qthread_writeEF_const_nb(aligned_t *const dest,
                                      const aligned_t  src)
{                      /*{{{ */
    return qthread_writeEF_nb(dest, &src);
}                      /*}}} */

/* the way this works is that:
 * 1 - src's FEB state must be "full"
 * 2 - data is copied from src to destination
 */

int API_FUNC qthread_readFF(aligned_t *restrict const       dest,
                            const aligned_t *restrict const src)
{                      /*{{{ */
    const aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m       = NULL;
    qthread_addrres_t  *X       = NULL;
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(src);
    qthread_t          *me      = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(febblock);

    if (!me) {
        return qthread_feb_blocker_func(dest, (void *)src, READFF);
    }
    qthread_debug(LOCK_BEHAVIOR, "tid %u dest=%p src=%p...\n", me->thread_id, dest, src);
    QTHREAD_LOCK_UNIQUERECORD(feb, src, me);
    QTHREAD_LOCK_TIMER_START(febblock);
    QALIGN(src, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], (void *)alignedaddr);
        if (!m) {
            if (dest && (dest != src)) {
                memcpy(dest, src, sizeof(aligned_t));
            }
        } else {
            QTHREAD_FASTLOCK_LOCK(&m->lock);
        }
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_DETAILS, "data structure locked\n");
    /* now m, if it exists, is locked - if m is NULL, then we're done! */
    if (m == NULL) {               /* already full! */
        if (dest && (dest != src)) {
            memcpy(dest, src, sizeof(aligned_t));
        }
    } else if (m->full != 1) {         /* not full... so we must block */
        QTHREAD_WAIT_TIMER_DECLARATION;
        X = ALLOC_ADDRRES(me->rdata->shepherd_ptr);
        if (X == NULL) {
            QTHREAD_FASTLOCK_UNLOCK(&m->lock);
            return QTHREAD_MALLOC_ERROR;
        }
        X->addr   = (aligned_t *)dest;
        X->waiter = me;
        X->next   = m->FFQ;
        m->FFQ    = X;
        qthread_debug(LOCK_DETAILS, "back to parent\n");
        me->thread_state     = QTHREAD_STATE_FEB_BLOCKED;
        me->rdata->blockedon = (struct qthread_lock_s *)m;
        QTHREAD_WAIT_TIMER_START();
        qthread_back_to_master(me);
        QTHREAD_WAIT_TIMER_STOP(me, febwait);
        qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p after waiting\n", me->thread_id, dest, src);
    } else {                   /* exists AND is empty... weird, but that's life */
        if (dest && (dest != src)) {
            memcpy(dest, src, sizeof(aligned_t));
        }
        qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p\n", me->thread_id, dest, src);
        QTHREAD_FASTLOCK_UNLOCK(&m->lock);
    }
    QTHREAD_LOCK_TIMER_STOP(febblock, me);
#else /* ifndef SST */
    QALIGN(src, alignedaddr);
    while (PIM_feb_try_readff(dest, alignedaddr) == 1) {
        qthread_yield(me);
    }
#endif /* ifndef SST */
    return QTHREAD_SUCCESS;
}                      /*}}} */

int INTERNAL qthread_readFF_nb(aligned_t *restrict const       dest,
                               const aligned_t *restrict const src)
{                      /*{{{ */
    const aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m       = NULL;
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(src);
    qthread_t          *me      = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(febblock);

    if (!me) {
        return qthread_feb_blocker_func(dest, (void *)src, READFF_NB);
    }
    qthread_debug(LOCK_BEHAVIOR, "tid %u dest=%p src=%p...\n", me->thread_id, dest, src);
    QTHREAD_LOCK_UNIQUERECORD(feb, src, me);
    QTHREAD_LOCK_TIMER_START(febblock);
    QALIGN(src, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], (void *)alignedaddr);
        if (!m) {
            if (dest && (dest != src)) {
                memcpy(dest, src, sizeof(aligned_t));
            }
        } else {
            QTHREAD_FASTLOCK_LOCK(&m->lock);
        }
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_DETAILS, "data structure locked\n");
    /* now m, if it exists, is locked - if m is NULL, then we're done! */
    if (m == NULL) {               /* already full! */
        if (dest && (dest != src)) {
            memcpy(dest, src, sizeof(aligned_t));
        }
    } else if (m->full != 1) {         /* not full... so we must block */
        qthread_debug(LOCK_BEHAVIOR, "tid %u non-blocking fail\n", me->thread_id);
        QTHREAD_FASTLOCK_UNLOCK(&m->lock);
        return QTHREAD_OPFAIL;
    } else {                   /* exists AND is empty... weird, but that's life */
        if (dest && (dest != src)) {
            memcpy(dest, src, sizeof(aligned_t));
        }
        qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p\n", me->thread_id, dest, src);
        QTHREAD_FASTLOCK_UNLOCK(&m->lock);
    }
    QTHREAD_LOCK_TIMER_STOP(febblock, me);
#else /* ifndef SST */
    QALIGN(src, alignedaddr);
    while (PIM_feb_try_readff(dest, alignedaddr) == 1) {
        return QTHREAD_OPFAIL;
    }
#endif /* ifndef SST */
    return QTHREAD_SUCCESS;
}                      /*}}} */

/* the way this works is that:
 * 1 - src's FEB state must be "full"
 * 2 - data is copied from src to destination
 * 3 - the src's FEB bits get changed from full to empty
 */

int API_FUNC qthread_readFE(aligned_t *restrict const       dest,
                            const aligned_t *restrict const src)
{                      /*{{{ */
    const aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m;
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(src);
    qthread_t          *me      = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(febblock);

    if (!me) {
        return qthread_feb_blocker_func(dest, (void *)src, READFE);
    }
    qthread_debug(LOCK_BEHAVIOR, "tid %u dest=%p src=%p...\n", me->thread_id, dest, src);
    QTHREAD_LOCK_UNIQUERECORD(feb, src, me);
    QTHREAD_LOCK_TIMER_START(febblock);
    QALIGN(src, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], alignedaddr);
        if (!m) {
            m = qthread_addrstat_new(me->rdata->shepherd_ptr);
            if (!m) {
                qt_hash_unlock(qlib->FEBs[lockbin]);
                return QTHREAD_MALLOC_ERROR;
            }
            qassertnot(qt_hash_put_locked(qlib->FEBs[lockbin], alignedaddr, m), 0);
        }
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_DETAILS, "data structure locked\n");
    /* by this point m is locked */
    if (m->full == 0) {            /* empty, thus, we must block */
        QTHREAD_WAIT_TIMER_DECLARATION;
        qthread_addrres_t *X = ALLOC_ADDRRES(me->rdata->shepherd_ptr);

        if (X == NULL) {
            QTHREAD_FASTLOCK_UNLOCK(&m->lock);
            return QTHREAD_MALLOC_ERROR;
        }
        X->addr   = (aligned_t *)dest;
        X->waiter = me;
        X->next   = m->FEQ;
        m->FEQ    = X;
        qthread_debug(LOCK_DETAILS, "back to parent\n");
        me->thread_state = QTHREAD_STATE_FEB_BLOCKED;
        /* so that the shepherd will unlock it */
        me->rdata->blockedon = (struct qthread_lock_s *)m;
        QTHREAD_WAIT_TIMER_START();
        qthread_back_to_master(me);
        QTHREAD_WAIT_TIMER_STOP(me, febwait);
        qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p after waiting\n", me->thread_id, dest, src);
    } else {                   /* full, thus IT IS OURS! MUAHAHAHA! */
        if (dest && (dest != src)) {
            memcpy(dest, src, sizeof(aligned_t));
        }
        qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p\n", me->thread_id, dest, src);
        qthread_gotlock_empty(me->rdata->shepherd_ptr, m, (void *)alignedaddr, 0);
    }
    QTHREAD_LOCK_TIMER_STOP(febblock, me);
#else /* ifndef SST */
    QALIGN(src, alignedaddr);
    while (PIM_feb_try_readfe(dest, alignedaddr) == 1) {
        qthread_yield(me);
    }
#endif /* ifndef SST */
    return QTHREAD_SUCCESS;
}                      /*}}} */

/* This is the non-blocking version of the previous one */
int INTERNAL qthread_readFE_nb(aligned_t *restrict const       dest,
                               const aligned_t *restrict const src)
{                      /*{{{ */
    const aligned_t *alignedaddr;

#ifndef SST
    qthread_addrstat_t *m;
    const int           lockbin = QTHREAD_CHOOSE_STRIPE(src);
    qthread_t          *me      = qthread_internal_self();

    QTHREAD_LOCK_TIMER_DECLARATION(febblock);

    if (!me) {
        return qthread_feb_blocker_func(dest, (void *)src, READFE_NB);
    }
    qthread_debug(LOCK_BEHAVIOR, "tid %u dest=%p src=%p...\n", me->thread_id, dest, src);
    QTHREAD_LOCK_UNIQUERECORD(feb, src, me);
    QTHREAD_LOCK_TIMER_START(febblock);
    QALIGN(src, alignedaddr);
    QTHREAD_COUNT_THREADS_BINCOUNTER(febs, lockbin);
    qt_hash_lock(qlib->FEBs[lockbin]);
    {
        m = (qthread_addrstat_t *)qt_hash_get_locked(qlib->FEBs[lockbin], alignedaddr);
        if (!m) {
            m = qthread_addrstat_new(me->rdata->shepherd_ptr);
            if (!m) {
                qt_hash_unlock(qlib->FEBs[lockbin]);
                return QTHREAD_MALLOC_ERROR;
            }
            qassertnot(qt_hash_put_locked(qlib->FEBs[lockbin], alignedaddr, m), 0);
        }
        QTHREAD_FASTLOCK_LOCK(&(m->lock));
    }
    qt_hash_unlock(qlib->FEBs[lockbin]);
    qthread_debug(LOCK_DETAILS, "data structure locked\n");
    /* by this point m is locked */
    if (m->full == 0) {            /* empty, thus, we must fail */
        qthread_debug(LOCK_BEHAVIOR, "tid %u non-blocking fail\n", me->thread_id);
        QTHREAD_FASTLOCK_UNLOCK(&m->lock);
        return QTHREAD_OPFAIL;
    } else {                   /* full, thus IT IS OURS! MUAHAHAHA! */
        if (dest && (dest != src)) {
            memcpy(dest, src, sizeof(aligned_t));
        }
        qthread_debug(LOCK_BEHAVIOR, "tid %u succeeded on %p=%p\n", me->thread_id, dest, src);
        qthread_gotlock_empty(me->rdata->shepherd_ptr, m, (void *)alignedaddr, 0);
    }
    QTHREAD_LOCK_TIMER_STOP(febblock, me);
#else /* ifndef SST */
    QALIGN(src, alignedaddr);
    if (PIM_feb_try_readfe(dest, alignedaddr) == 1) {
        return QTHREAD_OPFAIL;
    }
#endif /* ifndef SST */
    return QTHREAD_SUCCESS;
}                      /*}}} */

/* vim:set expandtab: */
