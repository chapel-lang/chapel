#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include "qthread/qthread.h"
#include "qt_visibility.h"
#include "qthread/futurelib.h"
#include "qt_atomics.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "futurelib_innards.h"
#include "qthread_innards.h"
#include "qt_debug.h"
#include "qt_macros.h"
#include "qt_qthread_struct.h"
#include "qt_qthread_mgmt.h"
#include "qt_asserts.h"

/* GLOBAL DATA (copy everywhere) */
TLS_DECL_INIT(location_t *, future_bookkeeping);
location_t *future_bookkeeping_array = NULL;

static qthread_shepherd_id_t shep_for_new_futures = 0;
static QTHREAD_FASTLOCK_TYPE sfnf_lock;

static void blocking_vp_incr(location_t *loc);

/* This function is critical to futurelib, and as such must be as fast as
 * possible.
 *
 * If the qthread is not a future, it returns NULL; otherwise, it returns
 * a pointer to the bookkeeping structure associated with that future's
 * shepherd. */
static location_t *ft_loc(void)
{
    qthread_t *t = qthread_internal_self();

    return (t->flags & QTHREAD_FUTURE) ? (location_t *)TLS_GET(future_bookkeeping) : NULL;
}

static void future_cleanup(void)
{
    int        i;
    aligned_t *rets;

    rets = (aligned_t *)calloc(qlib->nshepherds, sizeof(aligned_t));
    for (i = 0; i < qlib->nshepherds; i++) {
        location_t *ptr = &future_bookkeeping_array[i];
        qassert(pthread_mutex_destroy(&ptr->vp_count_lock), 0);
        qassert(qthread_unlock(&ptr->vp_count), QTHREAD_SUCCESS);
    }
    for (i = 0; i < qlib->nshepherds; i++) {
        qthread_readFF(NULL, rets + i);
    }
    FREE(rets, sizeof(aligned_t) * qlib->nshepherds);
    FREE(future_bookkeeping_array, qlib->nshepherds * sizeof(location_t));
    QTHREAD_FASTLOCK_DESTROY(sfnf_lock);
    TLS_DELETE(future_bookkeeping);
}

/* this function is used as a qthread; it is run by each shepherd so that each
 * shepherd will get some thread-local data associated with it. This works
 * better in the case of big machines (like massive SMP's) with intelligent
 * pthreads implementations than on PIM, but that's mostly because PIM's libc
 * doesn't support PIM-local data (yet). Better PIM support is coming. */
static aligned_t future_shep_init(void *Q_UNUSED arg)
{
    qthread_shepherd_id_t shep = qthread_shep();
    location_t           *ptr  = &(future_bookkeeping_array[shep]);

    // vp_count is *always* locked. This establishes the waiting queue.
    qthread_lock(&(ptr->vp_count));

    TLS_SET(future_bookkeeping, ptr);
    return 0;
}

void future_init(int vp_per_loc)
{
    qthread_shepherd_id_t i;
    aligned_t            *rets;

    QTHREAD_FASTLOCK_INIT(sfnf_lock);
    TLS_INIT(future_bookkeeping);
    future_bookkeeping_array = (location_t *)calloc(qlib->nshepherds, sizeof(location_t));
    rets                     = (aligned_t *)calloc(qlib->nshepherds, sizeof(aligned_t));
    for (i = 0; i < qlib->nshepherds; i++) {
        future_bookkeeping_array[i].vp_count = 0;
        future_bookkeeping_array[i].vp_max   = vp_per_loc;
        future_bookkeeping_array[i].id       = i;
        qassert(pthread_mutex_init(&(future_bookkeeping_array[i].vp_count_lock), NULL), 0);
        qthread_spawn(future_shep_init, NULL, 0, rets + i, 0, NULL, i, 0);
    }
    for (i = 0; i < qlib->nshepherds; i++) {
        qthread_readFF(NULL, rets + i);
    }
    FREE(rets, qlib->nshepherds * sizeof(aligned_t));
    atexit(future_cleanup);
}

/* This is the heart and soul of the futurelib. This function has two purposes:
 * 1. it checks for (and grabs, if it exists) an available thread-execution
 *    slot
 * 2. if there is no available slot, it adds itself to the waiter
 *    queue to get one.
 */
static void blocking_vp_incr(location_t *loc)
{
    qassert(pthread_mutex_lock(&(loc->vp_count_lock)), 0);
    qthread_debug(FUTURELIB_DETAILS, "thread %i attempting a blocking increment on loc %d vps %d\n", (int)qthread_id(), loc->id, loc->vp_count);

    while (loc->vp_count >= loc->vp_max) {
        qassert(pthread_mutex_unlock(&(loc->vp_count_lock)), 0);
        qthread_debug(FUTURELIB_DETAILS, "Thread %i found too many futures in %d; waiting for vp_count\n", (int)qthread_id(), loc->id);
        qthread_lock(&(loc->vp_count));
        qassert(pthread_mutex_lock(&(loc->vp_count_lock)), 0);
    }
    loc->vp_count++;
    qthread_debug(FUTURELIB_DETAILS, "Thread %i incr loc %d to %d vps\n", (int)qthread_id(), loc->id, loc->vp_count);
    qassert(pthread_mutex_unlock(&(loc->vp_count_lock)), 0);
}

/* creates a qthread, on a location defined by the qthread library, and
 * spawns it when the # of futures on that location is below the specified
 * threshold. Thus, this function has three steps:
 * 1. Figure out where to go
 * 2. Check the # of futures on the destination
 * 3. If there are too many futures there, wait
 */
void future_fork(qthread_f  fptr,
                 void      *arg,
                 aligned_t *retval)
{
    qthread_shepherd_id_t rr;
    location_t           *ptr;

    assert(future_bookkeeping_array != NULL);
    if (future_bookkeeping_array == NULL) {
        /* futures weren't initialized properly... */
        qthread_fork(fptr, arg, retval);
        return;
    }

    ptr = (location_t *)TLS_GET(future_bookkeeping);

    qthread_debug(FUTURELIB_BEHAVIOR, "Thread %i forking a future\n", (int)qthread_id());
    assert(future_bookkeeping_array != NULL);
    /* step 1: future out where to go (fast) */
    /* XXX: should merge with qthread.c to use qthread_internal_incr_mod */
    if (ptr) {
        rr               = ptr->sched_shep++;
        ptr->sched_shep *= (ptr->sched_shep < qlib->nshepherds);
    } else {
        QTHREAD_FASTLOCK_LOCK(&sfnf_lock);
        rr                    = shep_for_new_futures++;
        shep_for_new_futures *= (shep_for_new_futures < qlib->nshepherds);
        QTHREAD_FASTLOCK_UNLOCK(&sfnf_lock);
    }
    qthread_debug(FUTURELIB_DETAILS, "Thread %i decided future will go to %i\n", (int)qthread_id(), rr);
    /* steps 2&3 (slow) */
    blocking_vp_incr(&(future_bookkeeping_array[rr]));
    qthread_spawn(fptr, arg, 0, retval, 0, NULL, rr, QTHREAD_SPAWN_FUTURE);
}

void future_fork_to(qthread_f             fptr,
                    void                 *arg,
                    aligned_t            *retval,
                    qthread_shepherd_id_t shep)
{
    assert(future_bookkeeping_array != NULL);
    if (future_bookkeeping_array == NULL) {
        /* futures weren't initialized properly... */
        qthread_spawn(fptr, arg, 0, retval, 0, NULL, shep, 0);
        return;
    }

    qthread_debug(FUTURELIB_BEHAVIOR, "Thread %i forking a future\n", (int)qthread_id());
    /* steps 2&3 (slow) */
    blocking_vp_incr(&(future_bookkeeping_array[shep]));
    qthread_spawn(fptr, arg, 0, retval, 0, NULL, shep, QTHREAD_SPAWN_FUTURE);
}

void future_fork_syncvar_to(qthread_f             fptr,
                            void                 *arg,
                            syncvar_t            *retval,
                            qthread_shepherd_id_t shep)
{
    assert(future_bookkeeping_array != NULL);
    if (future_bookkeeping_array == NULL) {
        /* futures weren't initialized properly... */
        qthread_spawn(fptr, arg, 0, retval, 0, NULL, shep, QTHREAD_SPAWN_RET_SYNCVAR_T);
        return;
    }

    qthread_debug(FUTURELIB_BEHAVIOR, "Thread %i forking a future\n", (int)qthread_id());
    /* steps 2&3 (slow) */
    blocking_vp_incr(&(future_bookkeeping_array[shep]));
    qthread_spawn(fptr, arg, 0, retval, 0, NULL, shep, QTHREAD_SPAWN_RET_SYNCVAR_T | QTHREAD_SPAWN_FUTURE);
}

/* This says: "I do not count toward future resource limits, temporarily." */
int future_yield(void)
{
    location_t *loc;

    assert(future_bookkeeping_array != NULL);
    loc = ft_loc();
    qthread_debug(FUTURELIB_BEHAVIOR, "Thread %i yield on loc %p\n", (int)qthread_id(), (void *)loc);
    // Non-futures do not have a vproc to yield
    if (loc != NULL) {
        int unlockit = 0;

        // yield vproc
        qthread_debug(FUTURELIB_DETAILS, "Thread %i yield loc %d vps %d\n", (int)qthread_id(), loc->id, loc->vp_count);
        qassert(pthread_mutex_lock(&(loc->vp_count_lock)), 0);
        unlockit = (loc->vp_count-- == loc->vp_max);
        qassert(pthread_mutex_unlock(&(loc->vp_count_lock)), 0);
        if (unlockit) {
            qthread_unlock(&(loc->vp_count));
        }
        return 1;
    }
    return 0;
}

/* This says: "I count as a future again.", or, more accurately:
 * "I am now a thread that should be limited by the resource limitations."
 */
void future_acquire(void)
{
    location_t *loc;

    assert(future_bookkeeping_array != NULL);
    loc = ft_loc();
    qthread_debug(FUTURELIB_BEHAVIOR, "Thread %i acquire on loc %p\n", (int)qthread_id(),
                  (void *)loc);
    // Non-futures need not acquire a v proc
    if (loc != NULL) {
        blocking_vp_incr(loc);
    }
}

/* this is pretty obvious: wait for a thread to finish (ft is supposed
 * to be a thread/future's return value. */
static void future_join(aligned_t *ft)
{
    assert(future_bookkeeping_array != NULL);
    qthread_debug(FUTURELIB_BEHAVIOR, "Thread %i join to future %p\n", (int)qthread_id(), (void *)ft);
    qthread_readFF(NULL, ft);
}

/* this makes me not a future. Once a future exits, the thread may not
 * terminate, but there's no way for it to become a future again. */
void future_exit(void)
{
    qthread_t *t = qthread_internal_self();

    assert(future_bookkeeping_array != NULL);
    qthread_debug(FUTURELIB_BEHAVIOR, "Thread %i exit on loc %d\n", (int)qthread_id(), qthread_shep());
    assert(t->flags & QTHREAD_FUTURE);
    future_yield();
    t->flags ^= QTHREAD_FUTURE;
}

/* a more fun version of future_join */
void future_join_all(aligned_t *fta,
                     int        ftc)
{
    int i;

    assert(future_bookkeeping_array != NULL);
    assert(fta != NULL);
    assert(ftc > 0);
    qthread_debug(FUTURELIB_BEHAVIOR, "Qthread %i join all to %d futures\n", (int)qthread_id(), ftc);
    for (i = 0; i < ftc; i++) future_join(fta + i);
}

/* vim:set expandtab: */
