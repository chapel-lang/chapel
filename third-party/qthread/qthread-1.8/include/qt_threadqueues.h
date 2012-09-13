#ifndef QT_THREADQUEUES_H
#define QT_THREADQUEUES_H

#include <sys/types.h> /* for ssize_t (according to P90) */
#include <qthread-int.h>

#include "qt_visibility.h"
#include "qt_qthread_t.h" /* for qthread_t */
#include "qt_mpool.h"

typedef struct _qt_threadqueue qt_threadqueue_t;
typedef struct _qt_threadqueue_pools {
    qt_mpool nodes;
    qt_mpool queues;
} qt_threadqueue_pools_t;

#include "qt_spawncache.h"

void INTERNAL qt_threadqueue_subsystem_init(void);

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void);
void INTERNAL              qt_threadqueue_free(qt_threadqueue_t *q);

void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict        t);
void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict        t);
int INTERNAL qt_threadqueue_private_enqueue(qt_threadqueue_private_t *restrict pq,
                                            qt_threadqueue_t *restrict         q,
                                            qthread_t *restrict                t);
int INTERNAL qt_threadqueue_private_enqueue_yielded(qt_threadqueue_private_t *restrict q,
                                                    qthread_t *restrict                t);

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q);

qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t         *q,
                                            qt_threadqueue_private_t *qc,
                                            uint_fast8_t              active);
void INTERNAL qthread_steal_stat(void);
void INTERNAL qthread_steal_enable(void);
void INTERNAL qthread_steal_disable(void);
void INTERNAL qthread_cas_steal_stat(void);

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
/* Functions for work stealing functionality */
qthread_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                    void             *value);
#endif // ifdef QTHREAD_MULTITHREADED_SHEPHERDS

#endif // ifndef QT_THREADQUEUES_H
/* vim:set expandtab: */
