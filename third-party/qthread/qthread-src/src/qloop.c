#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <stdlib.h>

/* Installed Headers */
#include <qthread/qthread.h>
#include <qthread/qloop.h>
#include <qthread/qtimer.h>
#include <qthread/barrier.h>
#include <qthread/sinc.h>

/* Internal Headers */
#include "qt_initialized.h" // for qthread_library_initialized
#include "qloop_innards.h"
#include "qt_expect.h"
#include "qt_asserts.h"
#include "qt_debug.h"
#include "qt_alloc.h"
#include "qt_barrier.h"



typedef enum {
    ALIGNED,
    SYNCVAR_T,
    SINC_T,
    DONECOUNT,
    NO_SYNC
} synctype_t;

struct qt_loop_wrapper_args {
    qt_loop_f  func;
    size_t     startat, stopat, id;
    void      *arg;
    synctype_t sync_type;
    void      *sync;
};

struct qloop_wrapper_args {
    qt_loop_f  func;
    size_t     startat, stopat, id, level, spawnthreads;
    void      *arg;
    synctype_t sync_type;
    unsigned   spawn_flags;
    void      *sync;
};

static QINLINE void qt_loop_balance_inner(const size_t       start,
                                          const size_t       stop,
                                          const qt_loop_f    func,
                                          void              *argptr,
                                          const uint_fast8_t flags,
                                          synctype_t         sync_type);

static aligned_t qloop_wrapper(struct qloop_wrapper_args *const restrict arg)
{                                      /*{{{ */
    /* tree-based spawning (credit: AKP) */
    size_t           tot_workers = arg->spawnthreads - 1; // -1 because I already exist
    size_t           level       = arg->level;
    size_t           my_id       = arg->id;
    size_t           new_id      = my_id + (1 << level);
    const synctype_t sync_type   = arg->sync_type;
    void *const      sync        = arg->sync;

    switch (sync_type) {
        case SYNCVAR_T:
            while (new_id <= tot_workers) {      // create some children? (tot_workers zero based)
                size_t offset = new_id - my_id;  // need how much past current locations
                (arg + offset)->level = ++level; // increase depth for created thread
                qthread_spawn((qthread_f)qloop_wrapper,
                              arg + offset,
                              0,
                              ((syncvar_t *)sync) + new_id,
                              0, NULL,
                              new_id,
                              QTHREAD_SPAWN_RET_SYNCVAR_T | arg->spawn_flags);
                new_id = (1 << level) + my_id;         // level has been incremented
            }
            break;
        case ALIGNED:
            while (new_id <= tot_workers) {      // create some children? (tot_workers zero based)
                size_t offset = new_id - my_id;  // need how much past current locations
                (arg + offset)->level = ++level; // increase depth for created thread
                qthread_spawn((qthread_f)qloop_wrapper,
                              arg + offset,
                              0,
                              ((aligned_t *)sync) + new_id,
                              0, NULL,
                              new_id,
                              arg->spawn_flags);
                new_id = (1 << level) + my_id;         // level has been incremented
            }
            break;
        case SINC_T:
            while (new_id <= tot_workers) {      // create some children? (tot_workers zero based)
                size_t offset = new_id - my_id;  // need how much past current locations
                (arg + offset)->level = ++level; // increase depth for created thread
                qthread_spawn((qthread_f)qloop_wrapper,
                              arg + offset,
                              0,
                              sync,
                              0, NULL,
                              new_id,
                              arg->spawn_flags);
                new_id = (1 << level) + my_id;         // level has been incremented
            }
            break;
        default:
            while (new_id <= tot_workers) {      // create some children? (tot_workers zero based)
                size_t offset = new_id - my_id;  // need how much past current locations
                (arg + offset)->level = ++level; // increase depth for created thread
                qthread_spawn((qthread_f)qloop_wrapper,
                              arg + offset,
                              0,
                              NULL,
                              0, NULL,
                              new_id,
                              arg->spawn_flags);
                new_id = (1 << level) + my_id;         // level has been incremented
            }
    }

    // and now, we execute the function
    arg->func(arg->startat, arg->stopat, arg->arg);

    switch (sync_type) {
        default:
            break;
        case DONECOUNT:
            qthread_incr((aligned_t *)sync, 1);
            break;
    }

    return 0;
}                                      /*}}} */


struct qt_loop_spawner_arg {
    void      *argptr;
    qt_loop_f  func;
    synctype_t sync_type;
    uint8_t    flags;
};

static aligned_t qt_loop_wrapper(struct qt_loop_wrapper_args *const restrict arg)
{   /*{{{*/
    arg->func(arg->startat, arg->stopat, arg->arg);
    switch (arg->sync_type) {
        default:
            break;
        case SINC_T:
            qt_sinc_submit(arg->sync, NULL);
            break;
        case DONECOUNT:
            qthread_incr((aligned_t *)arg->sync, 1);
            break;
    }
    return 0;
} /*}}}*/

#define QT_LOOP_SPAWNER_SIMPLE (1 << 0)

static void qt_loop_spawner(const size_t start,
                            const size_t stop,
                            void        *args_)
{   /*{{{*/
    size_t                      i, threadct;
    size_t                      steps     = stop - start;
    struct qt_loop_wrapper_args qwa;
    unsigned int                flags     = 0;
    const synctype_t            sync_type = ((struct qt_loop_spawner_arg *)args_)->sync_type;
    const qt_loop_f             func      = ((struct qt_loop_spawner_arg *)args_)->func;
    void *const                 argptr    = ((struct qt_loop_spawner_arg *)args_)->argptr;
    void                       *retptr    = NULL;
    aligned_t                   dc;
    int                         yieldarg  = 2;

    assert(func);

    union {
        syncvar_t *syncvar;
        aligned_t *aligned;
        qt_sinc_t *sinc;
    } Q_ALIGNED(QTHREAD_ALIGNMENT_ALIGNED_T) sync = { NULL };
    switch (sync_type) {
        case SYNCVAR_T:
            retptr = sync.syncvar = MALLOC(steps * sizeof(syncvar_t));
            assert(sync.syncvar);
            for (i = 0; i < (stop - start); ++i) {
                sync.syncvar[i] = SYNCVAR_EMPTY_INITIALIZER;
            }
            flags |= QTHREAD_SPAWN_RET_SYNCVAR_T;
            break;
        case SINC_T:
            sync.sinc = qt_sinc_create(0, NULL, NULL, steps);
            assert(sync.sinc);
            break;
        case ALIGNED:
            retptr = sync.aligned = qt_internal_aligned_alloc(steps * sizeof(aligned_t), QTHREAD_ALIGNMENT_ALIGNED_T);
            ALLOC_SCRIBBLE(retptr, steps * sizeof(aligned_t));
            assert(sync.aligned);
            for (i = 0; i < (stop - start); ++i) {
                qthread_empty(&sync.aligned[i]);
            }
            break;
        case DONECOUNT:
            dc = 0;
            assert((((uintptr_t)&dc) & (QTHREAD_ALIGNMENT_ALIGNED_T - 1)) == 0);
            break;
        case NO_SYNC:
            abort();
    }
    switch (((struct qt_loop_spawner_arg *)args_)->flags) {
        case QT_LOOP_SPAWNER_SIMPLE:
            flags   |= QTHREAD_SPAWN_SIMPLE;
            yieldarg = 0;
            break;
    }
    for (i = start, threadct = 0; i < stop; ++i, ++threadct) {
        qwa.func      = func;
        qwa.startat   = i;
        qwa.stopat    = i + 1;
        qwa.arg       = argptr;
        qwa.id        = threadct;
        qwa.sync_type = sync_type;
        if (sync_type == DONECOUNT) {
            qwa.sync = &dc;
            qassert_aligned(dc, QTHREAD_ALIGNMENT_ALIGNED_T);
        } else {
            qwa.sync = sync.syncvar;
        }
        qassert(qthread_spawn((qthread_f)qt_loop_wrapper,
                              &qwa, sizeof(struct qt_loop_wrapper_args),
                              retptr,
                              0, NULL,
                              NO_SHEPHERD, flags), QTHREAD_SUCCESS);
        qthread_yield_(yieldarg);
    }
    switch (sync_type) {
        case SYNCVAR_T:
            for (i = 0; i < steps; i++) {
                qthread_syncvar_readFF(NULL, sync.syncvar + i);
            }
            FREE(sync.syncvar, steps * sizeof(syncvar_t));
            break;
        case ALIGNED:
            for (i = 0; i < steps; i++) {
                qthread_readFF(NULL, sync.aligned + i);
            }
            FREE_SCRIBBLE(sync.aligned, steps * sizeof(aligned_t));
            qt_internal_aligned_free(sync.aligned, QTHREAD_ALIGNMENT_ALIGNED_T);
            break;
        case SINC_T:
            qt_sinc_wait(sync.sinc, NULL);
            qt_sinc_destroy(sync.sinc);
            break;
        case DONECOUNT:
            while (dc != threadct) {
                qthread_yield();
            }
            break;
        case NO_SYNC:
            abort();
    }
} /*}}}*/

static void qt_loop_inner(const size_t     start,
                          const size_t     stop,
                          const qt_loop_f  func,
                          void            *argptr,
                          uint8_t          flags,
                          const synctype_t sync_type)
{   /*{{{*/
    struct qt_loop_spawner_arg a = { argptr, func, sync_type, flags };

    assert(qthread_library_initialized);
    flags &= ~(uint8_t)QT_LOOP_SPAWNER_SIMPLE;

    qt_loop_balance_inner(start, stop, qt_loop_spawner, &a, flags, sync_type);
} /*}}}*/

void API_FUNC qt_loop(size_t    start,
                      size_t    stop,
                      qt_loop_f func,
                      void     *argptr)
{                                      /*{{{ */
    qt_loop_inner(start, stop, func, argptr, 0, DONECOUNT);
}                                      /*}}} */

void API_FUNC qt_loop_simple(size_t    start,
                             size_t    stop,
                             qt_loop_f func,
                             void     *argptr)
{   /*{{{*/
    qt_loop_inner(start, stop, func, argptr, 2, SINC_T);
} /*}}}*/

void API_FUNC qt_loop_sv(size_t    start,
                         size_t    stop,
                         qt_loop_f func,
                         void     *argptr)
{                                      /*{{{ */
    qt_loop_inner(start, stop, func, argptr, 0, SYNCVAR_T);
}                                      /*}}} */

void API_FUNC qt_loop_dc(size_t    start,
                         size_t    stop,
                         qt_loop_f func,
                         void     *argptr)
{                                      /*{{{ */
    qt_loop_inner(start, stop, func, argptr, 0, DONECOUNT);
}                                      /*}}} */

void API_FUNC qt_loop_aligned(size_t    start,
                              size_t    stop,
                              qt_loop_f func,
                              void     *argptr)
{                                      /*{{{ */
    qt_loop_inner(start, stop, func, argptr, 0, ALIGNED);
}                                      /*}}} */

void API_FUNC qt_loop_sinc(size_t    start,
                           size_t    stop,
                           qt_loop_f func,
                           void     *argptr)
{                                      /*{{{ */
    qt_loop_inner(start, stop, func, argptr, 0, SINC_T);
}                                      /*}}} */


#define QT_LOOP_BALANCE_SIMPLE (1 << 0)

static QINLINE void qt_loop_balance_inner(const size_t       start,
                                          const size_t       stop,
                                          const qt_loop_f    func,
                                          void              *argptr,
                                          const uint_fast8_t flags,
                                          synctype_t         sync_type)
{                                      /*{{{ */
    qthread_shepherd_id_t            i;
    const qthread_shepherd_id_t      maxworkers     = ((stop - start) > qthread_num_workers()) ? qthread_num_workers() : (stop - start);
    struct qloop_wrapper_args *const qwa            = (struct qloop_wrapper_args *)MALLOC(sizeof(struct qloop_wrapper_args) * maxworkers);
    const size_t                     each           = (stop - start) / maxworkers;
    size_t                           extra          = (stop - start) - (each * maxworkers);
    size_t                           iterend        = start;
    unsigned                         internal_flags = 0;

    assert(func);
    assert(qwa);
    assert(qthread_library_initialized);

    union {
        void      *ptr;
        syncvar_t *syncvar;
        aligned_t *aligned;
        qt_sinc_t *sinc;
        aligned_t  dc;
    } Q_ALIGNED(QTHREAD_ALIGNMENT_ALIGNED_T) sync = { NULL };
    switch (sync_type) {
        case SYNCVAR_T:
            sync.syncvar    = MALLOC(maxworkers * sizeof(syncvar_t));
            assert(sync.syncvar);
            internal_flags |= QTHREAD_SPAWN_RET_SYNCVAR_T;
            break;
        case ALIGNED:
            sync.aligned = qt_internal_aligned_alloc(maxworkers * sizeof(aligned_t), QTHREAD_ALIGNMENT_ALIGNED_T);
            assert(sync.aligned);
            ALLOC_SCRIBBLE(sync.aligned, maxworkers * sizeof(aligned_t));
            break;
        case SINC_T:
            sync.sinc       = qt_sinc_create(0, NULL, NULL, maxworkers);
            assert(sync.sinc);
            internal_flags |= QTHREAD_SPAWN_RET_SINC_VOID;
            break;
        case DONECOUNT:
            break;
        case NO_SYNC:
            abort();
    }
    switch (flags) {
        case QT_LOOP_BALANCE_SIMPLE:
            internal_flags |= QTHREAD_SPAWN_SIMPLE;
            break;
    }

    /* The reason we use this big array, rather than using argcopy, is to make tree-spawning easier. */
    for (i = 0; i < maxworkers; i++) {
        qwa[i].func         = func;
        qwa[i].arg          = argptr;
        qwa[i].startat      = iterend;
        qwa[i].stopat       = iterend + each;
        qwa[i].spawn_flags  = internal_flags;
        qwa[i].id           = i;
        qwa[i].level        = 0;
        qwa[i].spawnthreads = maxworkers;
        qwa[i].sync_type    = sync_type;
        switch (sync_type) {
            case SYNCVAR_T:
                sync.syncvar[i] = SYNCVAR_EMPTY_INITIALIZER;
                qwa[i].sync     = sync.syncvar;
                break;
            case ALIGNED:
                qthread_empty(&sync.aligned[i]);
            case DONECOUNT:
                qwa[i].sync = &sync.dc;
                break;
            case SINC_T:
                qwa[i].sync = sync.sinc;
                break;
            case NO_SYNC:
                abort();
        }
        if (extra > 0) {
            qwa[i].stopat++;
            extra--;
        }
        iterend = qwa[i].stopat;
    }
    switch (sync_type) {
        case SYNCVAR_T:
        case ALIGNED:
        case SINC_T:
        case DONECOUNT:
            qassert(qthread_spawn((qthread_f)qloop_wrapper,
                                  qwa, 0,
                                  sync.ptr,
                                  0, NULL,
                                  0,
                                  internal_flags), QTHREAD_SUCCESS);
            break;
        default:
            qassert(qthread_spawn((qthread_f)qloop_wrapper,
                                  qwa, 0,
                                  NULL,
                                  0, NULL,
                                  0,
                                  0), QTHREAD_SUCCESS);
            break;
    }
    switch (sync_type) {
        case SYNCVAR_T:
            for (i = 0; i < maxworkers; i++) {
                qthread_syncvar_readFF(NULL, sync.syncvar + i);
            }
            FREE(sync.syncvar, maxworkers * sizeof(syncvar_t));
            break;
        case ALIGNED:
            for (i = 0; i < maxworkers; i++) {
                qthread_readFF(NULL, sync.aligned + i);
            }
            FREE_SCRIBBLE(sync.aligned, maxworkers * sizeof(aligned_t));
            qt_internal_aligned_free(sync.aligned, QTHREAD_ALIGNMENT_ALIGNED_T);
            break;
        case SINC_T:
            qt_sinc_wait(sync.sinc, NULL);
            qt_sinc_destroy(sync.sinc);
            break;
        case DONECOUNT:
            while (sync.dc != maxworkers) {
                qthread_yield();
            }
            break;
        case NO_SYNC:
            abort();
    }
    FREE(qwa, sizeof(struct qloop_wrapper_args) * maxworkers);
}                                      /*}}} */

void API_FUNC qt_loop_balance(const size_t    start,
                              const size_t    stop,
                              const qt_loop_f func,
                              void           *argptr)
{                                      /*{{{ */
    qt_loop_balance_inner(start, stop, func, argptr, 0, DONECOUNT);
}                                      /*}}} */

void API_FUNC qt_loop_balance_simple(const size_t    start,
                                     const size_t    stop,
                                     const qt_loop_f func,
                                     void           *argptr)
{   /*{{{*/
    qt_loop_balance_inner(start, stop, func, argptr, QT_LOOP_BALANCE_SIMPLE, DONECOUNT);
} /*}}}*/

void API_FUNC qt_loop_balance_sv(const size_t    start,
                                 const size_t    stop,
                                 const qt_loop_f func,
                                 void           *argptr)
{                                      /*{{{ */
    qt_loop_balance_inner(start, stop, func, argptr, 0, SYNCVAR_T);
}                                      /*}}} */

void API_FUNC qt_loop_balance_dc(const size_t    start,
                                 const size_t    stop,
                                 const qt_loop_f func,
                                 void           *argptr)
{                                      /*{{{ */
    qt_loop_balance_inner(start, stop, func, argptr, 0, DONECOUNT);
}                                      /*}}} */

void API_FUNC qt_loop_balance_aligned(const size_t    start,
                                      const size_t    stop,
                                      const qt_loop_f func,
                                      void           *argptr)
{                                      /*{{{ */
    qt_loop_balance_inner(start, stop, func, argptr, 0, ALIGNED);
}                                      /*}}} */

void API_FUNC qt_loop_balance_sinc(const size_t    start,
                                   const size_t    stop,
                                   const qt_loop_f func,
                                   void           *argptr)
{                                      /*{{{ */
    qt_loop_balance_inner(start, stop, func, argptr, 0, SINC_T);
}                                      /*}}} */

struct qloopaccum_wrapper_args {
    qt_loopr_f     func;
    size_t         startat, stopat, id, level, spawnthreads;
    void *restrict arg;
    void *restrict ret;
    synctype_t     sync_type;
    void          *sync;
};

static aligned_t qloopaccum_wrapper(struct qloopaccum_wrapper_args *const restrict arg)
{                                      /*{{{ */
    /* tree-based spawning (credit: AKP) */
    size_t           tot_workers = arg->spawnthreads - 1; // -1 because I already exist
    size_t           level       = arg->level;
    size_t           my_id       = arg->id;
    size_t           new_id      = my_id + (1 << level);
    const synctype_t sync_type   = arg->sync_type;
    void            *sync        = arg->sync;

    switch (sync_type) {
        case SYNCVAR_T:
            while (new_id <= tot_workers) {      // create some children? (tot_workers zero based)
                size_t offset = new_id - my_id;  // need how much past current locations
                (arg + offset)->level = ++level; // increase depth for created thread
                qthread_fork_syncvar_to((qthread_f)qloopaccum_wrapper,
                                        arg + offset,
                                        (syncvar_t *)sync + new_id,
                                        new_id);
                new_id = (1 << level) + my_id; // level has been incremented
            }
            break;
        case ALIGNED:
            while (new_id <= tot_workers) {      // create some children? (tot_workers zero based)
                size_t offset = new_id - my_id;  // need how much past current locations
                (arg + offset)->level = ++level; // increase depth for created thread
                qthread_fork_to((qthread_f)qloopaccum_wrapper,
                                arg + offset,
                                (aligned_t *)sync + new_id,
                                new_id);
                new_id = (1 << level) + my_id; // level has been incremented
            }
            break;
        default:
            while (new_id <= tot_workers) {      // create some children? (tot_workers zero based)
                size_t offset = new_id - my_id;  // need how much past current locations
                (arg + offset)->level = ++level; // increase depth for created thread
                qthread_fork_syncvar_to((qthread_f)qloopaccum_wrapper,
                                        arg + offset,
                                        NULL,
                                        new_id);
                new_id = (1 << level) + my_id; // level has been incremented
            }
    }

    // and now, we execute the function
    arg->func(arg->startat, arg->stopat, arg->arg, arg->ret);

    switch (sync_type) {
        default:
            break;
        case SINC_T:
            qt_sinc_submit(arg->sync, arg->ret);
            break;
        case DONECOUNT:
            qthread_incr((aligned_t *)arg->sync, 1);
            break;
    }
    return 0;
}                                      /*}}} */

static QINLINE void qt_loopaccum_balance_inner(const size_t       start,
                                               const size_t       stop,
                                               const size_t       size,
                                               void *restrict     out,
                                               const qt_loopr_f   func,
                                               void *restrict     argptr,
                                               const qt_accum_f   acc,
                                               const uint_fast8_t flags,
                                               synctype_t         sync_type)
{                                      /*{{{ */
    const qthread_shepherd_id_t           maxworkers  = qthread_num_workers();
    struct qloopaccum_wrapper_args *const qwa         = (struct qloopaccum_wrapper_args *)MALLOC(sizeof(struct qloopaccum_wrapper_args) * maxworkers);
    uint8_t                              *realrets    = NULL;
    const size_t                          each        = (stop - start) / maxworkers;
    size_t                                extra       = (stop - start) - (each * maxworkers);
    size_t                                iterend     = start;
    unsigned                              spawn_flags = 0;

    assert(qwa);
    assert(func);
    assert(acc);
    assert(qthread_library_initialized);

    union {
        void      *ptr;
        syncvar_t *syncvar;
        aligned_t *aligned;
        qt_sinc_t *sinc;
        aligned_t  dc;
    } Q_ALIGNED(QTHREAD_ALIGNMENT_ALIGNED_T) sync = { NULL };
    switch (sync_type) {
        case SYNCVAR_T:
            sync.syncvar = MALLOC(maxworkers * sizeof(syncvar_t));
            assert(sync.syncvar);
            spawn_flags |= QTHREAD_SPAWN_RET_SYNCVAR_T;
            break;
        case SINC_T:
            sync.sinc = qt_sinc_create(size, out, acc, maxworkers);
            assert(sync.sinc);
            break;
        case DONECOUNT:
            break;
        case ALIGNED:
        case NO_SYNC:
            abort();
    }
    switch (flags) {
        case QT_LOOP_BALANCE_SIMPLE:
            spawn_flags |= QTHREAD_SPAWN_SIMPLE;
            break;
    }

    if (maxworkers > 1) {
        realrets = (uint8_t *)MALLOC(size * (maxworkers - 1));
        assert(realrets);
    }
    for (qthread_shepherd_id_t i = 0; i < maxworkers; i++) {
        qwa[i].func = func;
        qwa[i].arg  = argptr;
        if (i == 0) {
            qwa[0].ret = out;
        } else {
            qwa[i].ret = realrets + ((i - 1) * size);
        }
        qwa[i].startat      = iterend;
        qwa[i].stopat       = iterend + each;
        qwa[i].id           = i;
        qwa[i].level        = 0;
        qwa[i].spawnthreads = maxworkers;
        qwa[i].sync_type    = sync_type;
        switch (sync_type) {
            case SYNCVAR_T:
                sync.syncvar[i] = SYNCVAR_EMPTY_INITIALIZER;
                qwa[i].sync     = sync.syncvar;
                break;
            case SINC_T:
                qwa[i].sync = sync.sinc;
                break;
            case DONECOUNT:
                qwa[i].sync = &sync.dc;
                break;
            case ALIGNED:
            case NO_SYNC:
                abort();
        }
        if (extra > 0) {
            qwa[i].stopat++;
            extra--;
        }
        iterend = qwa[i].stopat;
    }
    switch (sync_type) {
        case SYNCVAR_T:
        case ALIGNED:
            qassert(qthread_spawn((qthread_f)qloopaccum_wrapper,
                                  qwa, 0,
                                  sync.ptr,
                                  0, NULL,
                                  0,
                                  spawn_flags), QTHREAD_SUCCESS);
            break;
        default:
            qassert(qthread_spawn((qthread_f)qloopaccum_wrapper,
                                  qwa, 0,
                                  NULL,
                                  0, NULL,
                                  0,
                                  spawn_flags), QTHREAD_SUCCESS);
            break;
    }
    switch (sync_type) {
        case SYNCVAR_T:
            for (qthread_shepherd_id_t i = 0; i < maxworkers; i++) {
                qthread_syncvar_readFF(NULL, sync.syncvar + i);
                if (i > 0) {
                    acc(out, realrets + ((i - 1) * size));
                }
            }
            FREE(sync.syncvar, maxworkers * sizeof(syncvar_t));
            break;
        case SINC_T:
            qt_sinc_wait(sync.sinc, out);
            qt_sinc_destroy(sync.sinc);
            break;
        case DONECOUNT:
            while (sync.dc != maxworkers) {
                qthread_yield();
            }
            for (qthread_shepherd_id_t i = 0; i < maxworkers; i++) {
                if (i > 0) {
                    acc(out, realrets + ((i - 1) * size));
                }
            }
            qt_internal_aligned_free(sync.aligned, QTHREAD_ALIGNMENT_ALIGNED_T);
            break;
        case ALIGNED:
        case NO_SYNC:
            abort();
    }
    if (realrets) {
        FREE(realrets, size * (maxworkers - 1));
    }
    FREE(qwa, sizeof(struct qloopaccum_wrapper_args) * maxworkers);
}                                      /*}}} */

void API_FUNC qt_loopaccum_balance(const size_t     start,
                                   const size_t     stop,
                                   const size_t     size,
                                   void *restrict   out,
                                   const qt_loopr_f func,
                                   void *restrict   argptr,
                                   const qt_accum_f acc)
{                                      /*{{{ */
    qt_loopaccum_balance_inner(start, stop, size, out, func, argptr, acc, 0, SYNCVAR_T);
}                                      /*}}} */

void API_FUNC qt_loopaccum_balance_sinc(const size_t     start,
                                        const size_t     stop,
                                        const size_t     size,
                                        void *restrict   out,
                                        const qt_loopr_f func,
                                        void *restrict   argptr,
                                        const qt_accum_f acc)
{                                      /*{{{ */
    qt_loopaccum_balance_inner(start, stop, size, out, func, argptr, acc, 0, SINC_T);
}                                      /*}}} */

void API_FUNC qt_loopaccum_balance_sv(const size_t     start,
                                      const size_t     stop,
                                      const size_t     size,
                                      void *restrict   out,
                                      const qt_loopr_f func,
                                      void *restrict   argptr,
                                      const qt_accum_f acc)
{                                      /*{{{ */
    qt_loopaccum_balance_inner(start, stop, size, out, func, argptr, acc, 0, SYNCVAR_T);
}                                      /*}}} */

void API_FUNC qt_loopaccum_balance_dc(const size_t     start,
                                      const size_t     stop,
                                      const size_t     size,
                                      void *restrict   out,
                                      const qt_loopr_f func,
                                      void *restrict   argptr,
                                      const qt_accum_f acc)
{                                      /*{{{ */
    qt_loopaccum_balance_inner(start, stop, size, out, func, argptr, acc, 0, DONECOUNT);
}                                      /*}}} */

/* Now, the easy option for qt_loop_balance() is... effective, but has a major
 * drawback: if some iterations take longer than others, we will have a laggard
 * thread holding everyone up. Even worse, imagine if a shepherd is disabled
 * during loop processing: with qt_loop_balance, the thread responsible for a
 * 1/n chunk of the iteration space will be reassigned to another shepherd,
 * thereby guaranteeing that one thread doesn't keep up with the rest (and that
 * we will have idle shepherds).
 *
 * To handle this, we can use a slightly more complicated (and thus,
 * less-efficient) method: a shared iteration "queue" (probably the wrong word,
 * but gives you the right idea) that each thread can pull from. This allows
 * for a certain degree of self-scheduling, and adapts better when shepherds
 * are disabled.
 */

static Q_UNUSED QINLINE int qqloop_get_iterations_guided(qqloop_iteration_queue_t *const restrict    iq,
                                                         struct qqloop_static_args *const restrict   sa,
                                                         struct qqloop_wrapper_range *const restrict range)
{                                      /*{{{ */
    saligned_t                  ret        = iq->start;
    saligned_t                  ret2       = iq->stop;
    saligned_t                  iterations = 0;
    const saligned_t            stop       = iq->stop;
    const qthread_shepherd_id_t sheps      = sa->activesheps;

    if (qthread_num_workers() == 1) {
        if (ret < iq->stop) {
            range->startat = iq->start;
            range->stopat  = iq->stop;
            range->step    = iq->step;
            iq->start      = iq->stop;
            return 1;
        } else {
            range->startat = range->stopat = range->step = 0;
            return 0;
        }
    }

    /* this loop ensure atomicity in figuring out the number of iterations to
     * process */
    if (ret != ret2) {
        while (ret < iq->stop) {
            ret        = iq->start;
            iterations = (stop - ret) / sheps;
            if (iterations == 0) {
                iterations = 1;
            }
            ret2 = qthread_cas(&(iq->start), ret, ret + iterations);
            if (ret == ret2) { break; }
        }
    }
    if (ret < iq->stop) {
        assert(iterations > 0);
        range->startat = ret;
        range->stopat  = ret + iterations;
        range->step    = iq->step;
        return 1;
    } else {
        range->startat = 0;
        range->stopat  = 0;
        range->step    = 0;
        return 0;
    }
}                                      /*}}} */

static QINLINE int qqloop_get_iterations_factored(qqloop_iteration_queue_t *const restrict    iq,
                                                  struct qqloop_static_args *const restrict   sa,
                                                  struct qqloop_wrapper_range *const restrict range)
{                                      /*{{{ */
    saligned_t                  ret        = iq->start;
    saligned_t                  ret2       = iq->stop;
    const saligned_t            stop       = iq->stop;
    saligned_t                  iterations = 0;
    saligned_t                  phase      = iq->type_specific_data.phase;
    const qthread_shepherd_id_t sheps      = sa->activesheps;

    if (qthread_num_workers() == 1) {
        if (ret < iq->stop) {
            range->startat = iq->start;
            range->stopat  = iq->stop;
            range->step    = iq->step;
            iq->start      = iq->stop;
            return 1;
        } else {
            range->startat = range->stopat = range->step = 0;
            return 0;
        }
    }

    /* this loop ensures atomicity in figuring out the number of iterations to
     * process */
    while (ret < iq->stop && ret != ret2) {
        ret = iq->start;
        while (ret >= phase && ret < iq->stop) {
            /* set a new phase */
            saligned_t newphase  = (stop + ret) / 2;
            saligned_t chunksize = (stop - newphase) / sheps;
            newphase = ret + (chunksize * sheps);
            if (newphase != phase) {
                phase = qthread_cas(&(iq->type_specific_data.phase), phase, newphase);
            } else {
                phase = qthread_cas(&(iq->type_specific_data.phase), phase, stop);
            }
        }
        iterations = (stop - phase) / sheps;
        if (iterations == 0) {
            iterations = 1;
        }
        ret2 = qthread_cas(&(iq->start), ret, ret + iterations);
    }
    if (ret < iq->stop) {
        assert(iterations > 0);
        range->startat = ret;
        range->stopat  = ret + iterations;
        return 1;
    } else {
        range->startat = 0;
        range->stopat  = 0;
        return 0;
    }
}                                      /*}}} */

static QINLINE int qqloop_get_iterations_chunked(qqloop_iteration_queue_t *const restrict    iq,
                                                 struct qqloop_static_args *const restrict   sa,
                                                 struct qqloop_wrapper_range *const restrict range)
{                                      /*{{{ */
    saligned_t ret        = iq->start;
    saligned_t chunk_size = sa->chunksize;
    int        retval     = 1;

    if (ret < iq->stop) {
        ret = qthread_incr(&(iq->start), chunk_size);
    }
    if (ret < iq->stop) {
        if (ret + chunk_size > iq->stop) {
            chunk_size = iq->stop - ret;
        }
        range->startat = ret;
        range->stopat  = ret + chunk_size;
        range->step    = iq->step;
    } else {
        range->startat = range->stopat = range->step = 0;
        retval         = 0;
    }
    return retval;
}                                      /*}}} */

static QINLINE int qqloop_get_iterations_timed(qqloop_iteration_queue_t *const restrict    iq,
                                               struct qqloop_static_args *const restrict   sa,
                                               struct qqloop_wrapper_range *const restrict range)
{                                      /*{{{ */
    const qthread_shepherd_id_t workerCount = sa->activesheps;
    const qthread_shepherd_id_t shep        = qthread_shep();
    const saligned_t            localstop   = iq->stop;
    const saligned_t            localstep   = iq->step;

    ssize_t    dynamicBlock;
    double     loop_time;
    saligned_t localstart;

    assert(iq->type_specific_data.timed.timers != NULL);
    assert(iq->type_specific_data.timed.lastblocks != NULL);
    if (range->step == 0) {
        loop_time    = 1.0;
        dynamicBlock = 1;
    } else {
        loop_time    = qtimer_secs(iq->type_specific_data.timed.timers[shep]);
        dynamicBlock = iq->type_specific_data.timed.lastblocks[shep];
    }

    /* this loop ensures atomicity while figuring out iterations */
    localstart = iq->start;
    while (localstart < localstop) {
        saligned_t tmp;
        if (loop_time >= 7.5e-7) {     /* KBW: XXX: arbitrary constant */
            dynamicBlock = ((localstop - localstart) / ((workerCount << 1) * (localstep))) + 1;
        }
        if ((localstart + dynamicBlock) > localstop) {
            dynamicBlock = (localstop - localstart);
        }
        tmp = qthread_cas(&iq->start, localstart, localstart + dynamicBlock);
        if (tmp == localstart) {
            iq->type_specific_data.timed.lastblocks[shep] = dynamicBlock;
            break;
        } else {
            localstart = tmp;
        }
    }

    if ((localstart < localstop) && (dynamicBlock > 0)) {
        assert((localstart + dynamicBlock) <= localstop);
        range->startat = localstart;
        range->stopat  = localstart + dynamicBlock;
        range->step    = localstep;
        return 1;
    } else {
        range->startat = 0;
        range->stopat  = 0;
        range->step    = 0;
        return 0;
    }
}                                      /*}}} */

static QINLINE qqloop_iteration_queue_t *qqloop_create_iq(const size_t             startat,
                                                          const size_t             stopat,
                                                          const size_t             step,
                                                          const qt_loop_queue_type type)
{      /*{{{ */
    qqloop_iteration_queue_t *iq = MALLOC(sizeof(qqloop_iteration_queue_t));

    iq->start = startat;
    iq->stop  = stopat;
    iq->step  = step;
    iq->type  = type;
    switch (type) {
        case FACTORED:
            iq->type_specific_data.phase = (startat + stopat) / 2;
            break;
        case TIMED:
        {
            const qthread_shepherd_id_t max    = qthread_num_workers();
            assert(max != 0);
            qthread_shepherd_id_t       i;
            qtimer_t                   *timers = MALLOC(sizeof(qtimer_t) * max);
            assert(timers);
            for (i = 0; i < max; i++) {
                timers[i] = qtimer_create();
            }
            iq->type_specific_data.timed.timers     = timers;
            iq->type_specific_data.timed.lastblocks = MALLOC(sizeof(saligned_t) * max);
            assert(iq->type_specific_data.timed.lastblocks);
            break;
        }
        default:
            break;
    }
    return iq;
}                                      /*}}} */

static QINLINE void qqloop_destroy_iq(qqloop_iteration_queue_t *iq)
{                                      /*{{{ */
    assert(iq);
    switch (iq->type) {
        case TIMED:
        {
            const qthread_shepherd_id_t max = qthread_num_workers();
            if (iq->type_specific_data.timed.timers) {
                qthread_shepherd_id_t i;
                for (i = 0; i < max; i++) {
                    qtimer_destroy(iq->type_specific_data.timed.timers[i]);
                }
                FREE(iq->type_specific_data.timed.timers, sizeof(qtimer_t) * max);
            }
            if (iq->type_specific_data.timed.lastblocks) {
                FREE(iq->type_specific_data.timed.lastblocks, sizeof(saligned_t) * max);
            }
            break;
        }
        default:
            break;
    }
    FREE(iq, sizeof(qqloop_iteration_queue_t));
}                                      /*}}} */

static aligned_t qqloop_wrapper(const struct qqloop_wrapper_args *arg)
{   /*{{{*/
    struct qqloop_static_args *const restrict stat      = arg->stat;
    qqloop_iteration_queue_t *const restrict  iq        = stat->iq;
    const qt_loop_f                           func      = stat->func;
    void *const restrict                      a         = stat->arg;
    aligned_t *const                          dc        = &(stat->donecount);
    const qq_getiter_f                        get_iters = stat->get;
    const qthread_shepherd_id_t               shep      = arg->shep;

    /* non-consts */
    struct qqloop_wrapper_range range    = { 0, 0, 0 };
    int                         safeexit = 1;

    assert(get_iters != NULL);
    if (get_iters(iq, stat, &range)) {
        assert(range.startat != range.stopat);
        do {
            if (iq->type == TIMED) {
                qtimer_start(iq->type_specific_data.timed.timers[shep]);
            }
            func(range.startat, range.stopat, a);
            if (iq->type == TIMED) {
                qtimer_stop(iq->type_specific_data.timed.timers[shep]);
            }
            if (!qthread_shep_ok()) {
                /* my shepherd has been disabled while I was running */
                qthread_debug(LOOP_DETAILS, "my shepherd (%i) has been disabled!\n", (int)shep);
                safeexit = 0;
                qthread_incr(&(stat->activesheps), -1);
                break;
            }
        } while (get_iters(iq, stat, &range));
    }
    if (safeexit) {
        qthread_incr(dc, 1);
    }
    return 0;
} /*}}}*/

qqloop_handle_t *qt_loop_queue_create(const qt_loop_queue_type type,
                                      const size_t             start,
                                      const size_t             stop,
                                      const size_t             incr,
                                      const qt_loop_f          func,
                                      void *const restrict     argptr)
{   /*{{{*/
    qassert_ret(func, NULL);
    {
        qqloop_handle_t *const restrict h = MALLOC(sizeof(qqloop_handle_t));
        if (h) {
            const qthread_shepherd_id_t maxsheps = qthread_num_workers();
            qthread_shepherd_id_t       i;

            h->qwa              = MALLOC(sizeof(struct qqloop_wrapper_args) * maxsheps);
            h->stat.donecount   = 0;
            h->stat.activesheps = 0;
            h->stat.iq          = qqloop_create_iq(start, stop, incr, type);
            h->stat.func        = func;
            h->stat.arg         = argptr;
            h->stat.chunksize   = (stop - start) / qthread_num_workers() / 10; // completely arbitrary
            if (h->stat.chunksize == 0) {
                h->stat.chunksize = 1;
            }
            switch (type) {
                case FACTORED:
                    h->stat.get = qqloop_get_iterations_factored; break;
                case TIMED:
                    h->stat.get = qqloop_get_iterations_timed; break;
                case GUIDED:
                    h->stat.get = qqloop_get_iterations_guided; break;
                case CHUNK:
                    h->stat.get = qqloop_get_iterations_chunked; break;
            }
            for (i = 0; i < maxsheps; i++) {
                h->qwa[i].stat = &(h->stat);
                h->qwa[i].shep = i;    // this is the only thread-specific piece of information...
            }
        }
        return h;
    }
} /*}}}*/

void API_FUNC qt_loop_queue_setchunk(qqloop_handle_t *l,
                                     size_t           chunk)
{   /*{{{*/
    assert(l->stat.get == qqloop_get_iterations_chunked);
    l->stat.chunksize = chunk;
} /*}}}*/


void API_FUNC qt_loop_queue_run(qqloop_handle_t *loop)
{   /*{{{*/
    qassert_retvoid(loop);
    assert(qthread_library_initialized);
    {
        qthread_shepherd_id_t       i;
        const qthread_shepherd_id_t maxwkrs = qthread_num_workers();
        aligned_t *const            dc      = &(loop->stat.donecount);
        aligned_t *const            as      = &(loop->stat.activesheps);

        loop->stat.activesheps = maxwkrs;
        for (i = 0; i < maxwkrs; i++) {
            qthread_fork_to((qthread_f)qqloop_wrapper, loop->qwa + i, NULL, i);
        }
        /* turning this into a spinlock :P
         * I *would* do readFF, except shepherds can join and leave
         * during the loop */
        while (*dc < *as) {
            qthread_yield();
        }
        qqloop_destroy_iq(loop->stat.iq);
        FREE(loop->qwa, sizeof(struct qqloop_wrapper_args) * maxwkrs);
        FREE(loop, sizeof(qqloop_handle_t));
    }
} /*}}}*/

void API_FUNC qt_loop_queue_run_there(qqloop_handle_t      *loop,
                                      qthread_shepherd_id_t shep)
{   /*{{{*/
    assert(qthread_library_initialized);
    qassert_retvoid(loop);
    qassert_retvoid(shep < qthread_num_shepherds());
    {
        aligned_t *const dc = &(loop->stat.donecount);
        aligned_t *const as = &(loop->stat.activesheps);

        qthread_incr(as, 1);
        MACHINE_FENCE;
        qthread_fork_to((qthread_f)qqloop_wrapper, loop->qwa + shep, NULL, shep);
        /* turning this into a spinlock :P
         * I *would* do readFF, except shepherds can join and leave
         * during the loop */
        while (*dc < *as) {
            qthread_yield();
        }
        qqloop_destroy_iq(loop->stat.iq);
        FREE(loop->qwa, sizeof(struct qqloop_wrapper_args) * qthread_num_shepherds());
        FREE(loop, sizeof(qqloop_handle_t));
    }
} /*}}}*/

/* The easiest way to get shepherds/workers to REJOIN when/if shepherds are
 * re-enabled is to make the user do it. */
void API_FUNC qt_loop_queue_addworker(qqloop_handle_t            *loop,
                                      const qthread_shepherd_id_t shep)
{   /*{{{*/
    assert(qthread_library_initialized);
    qthread_incr(&(loop->stat.activesheps), 1);
    MACHINE_FENCE;
    if (loop->stat.donecount == 0) {
        qthread_fork_to((qthread_f)qqloop_wrapper, loop->qwa + shep, NULL, shep);
    } else {
        qthread_incr(&(loop->stat.activesheps), -1);
    }
} /*}}}*/

#define PARALLEL_FUNC(category, initials, _op_, type, shorttype)                               \
    static void qt ## initials ## _febworker(const size_t startat, const size_t stopat,        \
                                             void *restrict arg, void *restrict ret)           \
    {                                                                                          \
        size_t i;                                                                              \
        type   acc;                                                                            \
        qthread_readFF(NULL, (aligned_t *)(((type *)arg) + startat));                          \
        acc = ((type *)arg)[startat];                                                          \
        for (i = startat + 1; i < stopat; i++) {                                               \
            qthread_readFF(NULL, (aligned_t *)(((type *)arg) + i));                            \
            acc = _op_(acc, ((type *)arg)[i]);                                                 \
        }                                                                                      \
        *(type *)ret = acc;                                                                    \
    }                                                                                          \
    static void qt ## initials ## _worker(const size_t startat, const size_t stopat,           \
                                          void *restrict arg, void *restrict ret)              \
    {                                                                                          \
        size_t i;                                                                              \
        type   acc = ((type *)arg)[startat];                                                   \
        for (i = startat + 1; i < stopat; i++) {                                               \
            acc = _op_(acc, ((type *)arg)[i]);                                                 \
        }                                                                                      \
        *(type *)ret = acc;                                                                    \
    }                                                                                          \
    static void qt ## initials ## _acc(void *restrict a, const void *restrict b)               \
    {                                                                                          \
        *(type *)a = _op_(*(type *)a, *(type *)b);                                             \
    }                                                                                          \
    type API_FUNC qt_ ## shorttype ## _ ## category(type * array, size_t length, int checkfeb) \
    {                                                                                          \
        type ret;                                                                              \
        assert(qthread_library_initialized);                                                   \
        if (checkfeb) {                                                                        \
            if (sizeof(type) != sizeof(aligned_t)) { return 0; }                               \
            qt_loopaccum_balance_inner(0, length, sizeof(type), &ret,                          \
                                       qt ## initials ## _febworker,                           \
                                       array, qt ## initials ## _acc, 0, SYNCVAR_T);           \
        } else {                                                                               \
            qt_loopaccum_balance_inner(0, length, sizeof(type), &ret,                          \
                                       qt ## initials ## _worker,                              \
                                       array, qt ## initials ## _acc, 0, SYNCVAR_T);           \
        }                                                                                      \
        return ret;                                                                            \
    }

#define ADD(a, b)  a + b
#define MULT(a, b) a * b
#define MAX(a, b)  (a > b) ? a : b
#define MIN(a, b)  (a < b) ? a : b

PARALLEL_FUNC(sum, uis, ADD, aligned_t, uint)
PARALLEL_FUNC(prod, uip, MULT, aligned_t, uint)
PARALLEL_FUNC(max, uimax, MAX, aligned_t, uint)
PARALLEL_FUNC(min, uimin, MIN, aligned_t, uint)

PARALLEL_FUNC(sum, is, ADD, saligned_t, int)
PARALLEL_FUNC(prod, ip, MULT, saligned_t, int)
PARALLEL_FUNC(max, imax, MAX, saligned_t, int)
PARALLEL_FUNC(min, imin, MIN, saligned_t, int)
PARALLEL_FUNC(sum, ds, ADD, double, double)
PARALLEL_FUNC(prod, dp, MULT, double, double)
PARALLEL_FUNC(max, dmax, MAX, double, double)
PARALLEL_FUNC(min, dmin, MIN, double, double)

/* The next idea is to implement it in a memory-bound kind of way. And I don't
 * mean memory-bound in that it spends its time waiting for memory; I mean in
 * the kind of "that memory belongs to shepherd Y, so therefore iteration X
 * should be on shepherd Y".
 *
 * Of course, in terms of giving each processor a contiguous chunk to work on,
 * that's what qt_loop_balance() does. The really interesting bit is to
 * "register" address ranges with the library, and then have it decide where to
 * spawn threads based on the array you've handed it. HOWEVER, this can't be
 * quite so generic, unfortunately, because we don't know what memory we're
 * working with (the qt_loop_balance interface is too generic).
 *
 * The more I think about that, though, the more I'm convinced that it's almost
 * impossible to make particularly generic, because an *arbitrary* function may
 * use two or more arrays that are on different processors, and there's no way
 * qthreads can know that (or even do very much to help if that memory has been
 * assigned to different processors). The best way to achieve this sort of
 * behavior is through premade utility functions, like qutil... but even then,
 * binding given memory to given shepherds won't last through multiple calls
 * unless it's done explicitly. That said, given the way this works, doing
 * repeated operations on the same array will divide the array in the same
 * fashion every time.
 */
#define SWAP(a, m, n) do { double temp = a[m]; a[m] = a[n]; a[n] = temp; } while (0)
static int dcmp(const void *restrict a,
                const void *restrict b)
{
    if ((*(double *)a) < (*(double *)b)) {
        return -1;
    }
    if ((*(double *)a) > (*(double *)b)) {
        return 1;
    }
    return 0;
}

struct qt_qsort_args {
    double             *array;
    double              pivot;
    size_t              length, chunksize, jump, offset;
    aligned_t *restrict furthest_leftwall;
    aligned_t *restrict furthest_rightwall;
};

static aligned_t qt_qsort_partition(struct qt_qsort_args *args)
{   /*{{{*/
    double      *a         = args->array;
    const double pivot     = args->pivot;
    const size_t chunksize = args->chunksize;
    const size_t length    = args->length;
    const size_t jump      = args->jump;
    size_t       leftwall, rightwall;

    leftwall  = 0;
    rightwall = length - 1;
    /* adjust the edges; this is critical for this algorithm */
    while (a[leftwall] <= pivot) {
        if ((leftwall + 1) % chunksize != 0) {
            leftwall++;
        } else {
            leftwall += jump;
        }
        if (rightwall < leftwall) {
            goto quickexit;
        }
    }
    while (a[rightwall] > pivot) {
        if (rightwall % chunksize != 0) {
            if (rightwall == 0) {
                goto quickexit;
            }
            rightwall--;
        } else {
            if (rightwall < jump) {
                goto quickexit;
            }
            rightwall -= jump;
        }
        if (rightwall < leftwall) {
            goto quickexit;
        }
    }
    SWAP(a, leftwall, rightwall);
    while (1) {
        do {
            leftwall += ((leftwall + 1) % chunksize != 0) ? 1 : jump;
            if (rightwall < leftwall) {
                goto quickexit;
            }
        } while (a[leftwall] <= pivot);
        if (rightwall <= leftwall) {
            break;
        }
        do {
            if (rightwall % chunksize != 0) {
                if (rightwall == 0) {
                    goto quickexit;
                }
                rightwall--;
            } else {
                if (rightwall < jump) {
                    goto quickexit;
                }
                rightwall -= jump;
            }
        } while (a[rightwall] > pivot);
        if (rightwall <= leftwall) {
            break;
        }
        SWAP(a, leftwall, rightwall);
    }
quickexit:
    qthread_lock(args->furthest_leftwall);
    if (leftwall + args->offset < *args->furthest_leftwall) {
        *args->furthest_leftwall = leftwall + args->offset;
    }
    if (rightwall + args->offset > *args->furthest_rightwall) {
        *args->furthest_rightwall = rightwall + args->offset;
    }
    qthread_unlock(args->furthest_leftwall);
    return 0;
} /*}}}*/

struct qt_qsort_iargs {
    double *array;
    size_t  length;
};

typedef struct qt_qsort_iprets {
    aligned_t leftwall, rightwall;
} qt_qsort_iprets_t;

static qt_qsort_iprets_t qt_qsort_inner_partitioner(double      *array,
                                                    const size_t length,
                                                    const double pivot)
{      /*{{{ */
    const size_t chunksize = 10;

    /* choose the number of threads to use */
    const size_t numthreads = qthread_num_shepherds();

    /* a "megachunk" is a set of numthreads chunks.
     * calculate the megachunk information for determining the array lengths
     * each thread will be fed. */
    const size_t megachunk_size = chunksize * numthreads;

    /* just used as a boolean test */
    const size_t extra_chunks = length % megachunk_size;

    /* non-consts */
    size_t                megachunks = length / (chunksize * numthreads);
    qt_qsort_iprets_t     retval     = { ((aligned_t)-1), 0 };
    syncvar_t            *rets;
    struct qt_qsort_args *args;
    size_t                i;

    rets = (syncvar_t *) qt_calloc(numthreads, sizeof(syncvar_t));
    args = (struct qt_qsort_args *)MALLOC(sizeof(struct qt_qsort_args) * numthreads);
    /* spawn threads to do the partitioning */
    for (i = 0; i < numthreads; i++) {
        args[i].array              = array + (i * chunksize);
        args[i].offset             = i * chunksize;
        args[i].pivot              = pivot;
        args[i].chunksize          = chunksize;
        args[i].jump               = (numthreads - 1) * chunksize + 1;
        args[i].furthest_leftwall  = &retval.leftwall;
        args[i].furthest_rightwall = &retval.rightwall;
        if (extra_chunks != 0) {
            args[i].length = megachunks * megachunk_size + chunksize;
            if (args[i].length + args[i].offset >= length) {
                args[i].length = length - args[i].offset;
                megachunks--;
            }
        } else {
            args[i].length = length - megachunk_size + chunksize;
        }
        /* qt_qsort_partition(args+i); */
        qthread_fork_syncvar((qthread_f)qt_qsort_partition, args + i, rets + i);
    }
    for (i = 0; i < numthreads; i++) {
        qthread_syncvar_readFF(NULL, rets + i);
    }
    FREE(args, sizeof(struct qt_qsort_args) * numthreads);
    FREE(rets, numthreads * sizeof(syncvar_t));

    return retval;
}                                      /*}}} */

static aligned_t qt_qsort_inner(const struct qt_qsort_iargs *a)
{                                      /*{{{ */
    const size_t      len   = a->length;
    double           *array = a->array;
    size_t            i;
    qt_qsort_iprets_t furthest;
    const size_t      thread_chunk = len / qthread_num_shepherds();

    /* choose the number of threads to use */
    if ((qthread_num_shepherds() == 1) || (len <= 10000)) { /* shortcut */
        qsort(array, len, sizeof(double), dcmp);
        return 0;
    }
    furthest.leftwall  = 0;
    furthest.rightwall = len - 1;
    /* tri-median pivot selection */
    i                  = len / 2;
    if (array[0] > array[i]) {
        SWAP(array, 0, i);
    }
    if (array[0] > array[len - 1]) {
        SWAP(array, 0, len - 1);
    }
    if (array[i] > array[len - 1]) {
        SWAP(array, i, len - 1);
    }
    {
        const double pivot = array[i];

        while (furthest.rightwall > furthest.leftwall &&
               furthest.rightwall - furthest.leftwall > (2 * thread_chunk)) {
            const size_t offset = furthest.leftwall;

            furthest =
                qt_qsort_inner_partitioner(array + furthest.leftwall,
                                           furthest.rightwall -
                                           furthest.leftwall + 1, pivot);
            furthest.leftwall  += offset;
            furthest.rightwall += offset;
        }
        /* data between furthest.leftwall and furthest.rightwall is unlikely to be partitioned correctly */
        {
            size_t leftwall  = furthest.leftwall;
            size_t rightwall = furthest.rightwall;

            while ((leftwall < rightwall) && (array[leftwall] <= pivot)) leftwall++;
            while ((leftwall < rightwall) && (array[rightwall] > pivot)) rightwall--;
            if (leftwall < rightwall) {
                SWAP(array, leftwall, rightwall);
                for (;;) {
                    while ((++leftwall < rightwall) && (array[leftwall] <= pivot)) ;
                    if (rightwall < leftwall) {
                        break;
                    }
                    while ((leftwall < --rightwall) && (array[rightwall] > pivot)) ;
                    if (rightwall < leftwall) {
                        break;
                    }
                    SWAP(array, leftwall, rightwall);
                }
            }
            if (array[rightwall] <= pivot) {
                rightwall++;
            }
            /* now, spawn the next two iterations */
            {
                struct qt_qsort_iargs na[2];
                syncvar_t             rets[2] = { SYNCVAR_INITIALIZER, SYNCVAR_INITIALIZER };
                na[0].array  = array;
                na[0].length = rightwall;
                na[1].array  = array + rightwall;
                na[1].length = len - rightwall;
                if (na[0].length > 0) {
                    /* qt_qsort_inner(na); */
                    qthread_fork_syncvar((qthread_f)qt_qsort_inner, na, rets);
                }
                if ((na[1].length > 0) && (len > rightwall)) {
                    /* qt_qsort_inner(na+1); */
                    qthread_fork_syncvar((qthread_f)qt_qsort_inner, na + 1, rets + 1);
                }
                qthread_syncvar_readFF(NULL, rets);
                qthread_syncvar_readFF(NULL, rets + 1);
            }
        }
    }
    return 0;
}                                      /*}}} */

void API_FUNC qt_qsort(double      *array,
                       const size_t length)
{                                      /*{{{ */
    struct qt_qsort_iargs arg;

    arg.array  = array;
    arg.length = length;

    qt_qsort_inner(&arg);
}                                      /*}}} */

/* vim:set expandtab: */
