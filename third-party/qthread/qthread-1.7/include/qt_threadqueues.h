#ifndef QT_THREADQUEUES_H
#define QT_THREADQUEUES_H

#include <sys/types.h> /* for ssize_t (according to P90) */

#include "qt_visibility.h"

#include "qt_atomics.h"
#include "qt_mpool.h"

#ifndef QTHREAD_SHEPHERD_TYPEDEF
# define QTHREAD_SHEPHERD_TYPEDEF
typedef struct qthread_shepherd_s qthread_shepherd_t;
#endif

typedef struct _qt_threadqueue qt_threadqueue_t;
typedef struct _qt_threadqueue_pools {
    qt_mpool nodes;
    qt_mpool queues;
} qt_threadqueue_pools_t;

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
# define QMS_ARG(x) , x
#else
# define QMS_ARG(x)
#endif

#if !defined(UNPOOLED_QUEUES) && !defined(UNPOOLED)
void INTERNAL qt_threadqueue_init_pools(qt_threadqueue_pools_t *p);
void INTERNAL qt_threadqueue_destroy_pools(qt_threadqueue_pools_t *p);
#endif
void INTERNAL qt_threadqueue_subsystem_init(void);

qt_threadqueue_t INTERNAL *qt_threadqueue_new(qthread_shepherd_t *shepherd);
void INTERNAL              qt_threadqueue_free(qt_threadqueue_t *q);
void INTERNAL              qt_threadqueue_enqueue(qt_threadqueue_t   *q,
                                                  qthread_t          *t,
                                                  qthread_shepherd_t *shep);
void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t   *q,
                                             qthread_t          *t,
                                             qthread_shepherd_t *shep);
ssize_t INTERNAL    qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q);
qthread_t INTERNAL *qt_threadqueue_dequeue(qt_threadqueue_t *q);

qthread_t INTERNAL *qt_threadqueue_dequeue_blocking(qt_threadqueue_t * q QMS_ARG(size_t active));

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
/* Functions for work stealing functionality */
qthread_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                    void             *value);
void INTERNAL qthread_steal_stat(void);
#endif // ifdef QTHREAD_MULTITHREADED_SHEPHERDS

#endif // ifndef QT_THREADQUEUES_H
/* vim:set expandtab: */
