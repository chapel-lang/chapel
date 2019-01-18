#ifndef QT_BLOCKING_STRUCTS_H
#define QT_BLOCKING_STRUCTS_H

#include <stdlib.h>            /* for malloc() and free() */

#include "qt_mpool.h"
#include "qt_shepherd_innards.h"
#include "qt_profiling.h"
#include "qt_debug.h"

typedef enum blocking_syscalls {
    ACCEPT,
    CONNECT,
    NANOSLEEP,
    POLL,
    READ,
    PREAD,
    /*RECV,
     * RECVFROM,*/
    SELECT,
    /*SEND,
     * SENDTO,*/
    /*SIGWAIT,*/
    SLEEP,
    SYSTEM,
    USLEEP,
    WAIT4,
    WRITE,
    PWRITE,
    USER_DEFINED
} syscall_t;

typedef struct qthread_addrres_s {
    aligned_t                *addr; /* ptr to the memory NOT being blocked on */
    qthread_t                *waiter;
    struct qthread_addrres_s *next;
} qthread_addrres_t;

typedef struct _qt_blocking_queue_node_s {
    struct _qt_blocking_queue_node_s *next;
    qthread_t                        *thread;
    syscall_t                         op;
    uintptr_t                         args[5];
    ssize_t                           ret;
} qt_blocking_queue_node_t;

typedef struct qthread_addrstat_s {
    QTHREAD_FASTLOCK_TYPE lock;
    qthread_addrres_t    *EFQ;
    qthread_addrres_t    *FEQ;
    qthread_addrres_t    *FFQ;
    qthread_addrres_t    *FFWQ;
#ifdef QTHREAD_FEB_PROFILING
    qtimer_t              empty_timer;
#endif
    uint_fast8_t          full;
    uint_fast8_t          valid;
} qthread_addrstat_t;

#ifdef UNPOOLED
# define UNPOOLED_ADDRSTAT
# define UNPOOLED_ADDRRES
#endif

#ifdef UNPOOLED_ADDRSTAT
# define ALLOC_ADDRSTAT() (qthread_addrstat_t *)MALLOC(sizeof(qthread_addrstat_t))
# define FREE_ADDRSTAT(t) FREE(t, sizeof(qthread_addrstat_t))
#else
extern qt_mpool generic_addrstat_pool;
# define ALLOC_ADDRSTAT() (qthread_addrstat_t *)qt_mpool_alloc(generic_addrstat_pool)
# define FREE_ADDRSTAT(t) qt_mpool_free(generic_addrstat_pool, t)

#endif // ifdef UNPOOLED_ADDRSTAT

#ifdef UNPOOLED_ADDRRES
# define ALLOC_ADDRRES() (qthread_addrres_t *)MALLOC(sizeof(qthread_addrres_t))
# define FREE_ADDRRES(t) FREE(t, sizeof(qthread_addrres_t))
#else
extern qt_mpool generic_addrres_pool;
static QINLINE qthread_addrres_t *ALLOC_ADDRRES(void)
{                                      /*{{{ */
    qthread_addrres_t *tmp = (qthread_addrres_t *)qt_mpool_alloc(generic_addrres_pool);

    return tmp;
}                                      /*}}} */

static QINLINE void FREE_ADDRRES(qthread_addrres_t *t)
{                                      /*{{{ */
    qt_mpool_free(generic_addrres_pool, t);
}                                      /*}}} */

#endif // ifdef UNPOOLED_ADDRRES

#endif // ifndef QT_BLOCKING_STRUCTS_H
/* vim:set expandtab: */
