#ifndef QT_BLOCKING_STRUCTS_H
#define QT_BLOCKING_STRUCTS_H

#include <stdlib.h>            /* for malloc() and free() */

#include "qt_mpool.h"
#include "qt_shepherd_innards.h"
#include "qt_profiling.h"

typedef struct qthread_queue_s qthread_queue_t;

struct qthread_lock_s {
    qthread_queue_t      *waiting;
#ifdef QTHREAD_DEBUG
    unsigned              owner;
#endif
    QTHREAD_FASTLOCK_TYPE lock;
#ifdef QTHREAD_LOCK_PROFILING
    qtimer_t              hold_timer;
#endif
};

typedef struct qthread_addrres_s {
    aligned_t                *addr; /* ptr to the memory NOT being blocked on */
    qthread_t                *waiter;
    struct qthread_addrres_s *next;
} qthread_addrres_t;

typedef struct qthread_addrstat_s {
    QTHREAD_FASTLOCK_TYPE lock;
    qthread_addrres_t    *EFQ;
    qthread_addrres_t    *FEQ;
    qthread_addrres_t    *FFQ;
#ifdef QTHREAD_LOCK_PROFILING
    qtimer_t              empty_timer;
#endif
    unsigned int          full : 1;
} qthread_addrstat_t;

#ifdef UNPOOLED
# define UNPOOLED_ADDRSTAT
# define UNPOOLED_ADDRRES
#endif

#ifdef UNPOOLED_ADDRSTAT
# define ALLOC_ADDRSTAT(shep) (qthread_addrstat_t *)malloc(sizeof(qthread_addrstat_t))
# define FREE_ADDRSTAT(t)     free(t)
#else
extern qt_mpool generic_addrstat_pool;
static QINLINE qthread_addrstat_t *ALLOC_ADDRSTAT(qthread_shepherd_t *shep)
{                                      /*{{{ */
    qthread_addrstat_t *tmp = (qthread_addrstat_t *)qt_mpool_cached_alloc(generic_addrstat_pool);

    return tmp;
}                                      /*}}} */

static QINLINE void FREE_ADDRSTAT(qthread_addrstat_t *t)
{                                      /*{{{ */
    qt_mpool_cached_free(generic_addrstat_pool, t);
}                                      /*}}} */

#endif // ifdef UNPOOLED_ADDRSTAT

#ifdef UNPOOLED_ADDRRES
# define ALLOC_ADDRRES(shep) (qthread_addrres_t *)malloc(sizeof(qthread_addrres_t))
# define FREE_ADDRRES(t)     free(t)
#else
extern qt_mpool generic_addrres_pool;
static QINLINE qthread_addrres_t *ALLOC_ADDRRES(qthread_shepherd_t *shep)
{                                      /*{{{ */
    qthread_addrres_t *tmp = (qthread_addrres_t *)qt_mpool_cached_alloc(generic_addrres_pool);

    return tmp;
}                                      /*}}} */

static QINLINE void FREE_ADDRRES(qthread_addrres_t *t)
{                                      /*{{{ */
    qt_mpool_cached_free(generic_addrres_pool, t);
}                                      /*}}} */

#endif // ifdef UNPOOLED_ADDRRES

#endif // ifndef QT_BLOCKING_STRUCTS_H
/* vim:set expandtab: */
