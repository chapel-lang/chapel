#ifndef QT_THREADQUEUES_H
#define QT_THREADQUEUES_H
#include <stdint.h>

#include <sys/types.h> /* for ssize_t (according to P90) */

#include "qt_filters.h"
#include "qt_mpool.h"
#include "qt_qthread_t.h" /* for qthread_t */
#include "qt_visibility.h"

typedef filter_code (*qt_threadqueue_filter_f)(qthread_t *);

typedef struct _qt_threadqueue qt_threadqueue_t;

typedef struct _qt_threadqueue_pools {
  qt_mpool nodes;
  qt_mpool queues;
} qt_threadqueue_pools_t;

void INTERNAL qt_threadqueue_subsystem_init(void);

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void);
void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q);

void INTERNAL qt_threadqueue_filter(qt_threadqueue_t *q,
                                    qt_threadqueue_filter_f f);
void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict t);
void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict t);

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q);

qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t *q,
                                            uint_fast8_t active);
void INTERNAL qthread_steal_stat(void);
void INTERNAL qthread_steal_enable(void);
void INTERNAL qthread_steal_disable(void);
void INTERNAL qthread_cas_steal_stat(void);

/* Functions for work stealing functionality */
qthread_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                    void *value);

enum threadqueue_policy {
  THREADQUEUE_POLICY_FALSE = 0,
  THREADQUEUE_POLICY_TRUE = 1,
  THREADQUEUE_POLICY_UNSUPPORTED = 2,
  SINGLE_WORKER
};

size_t qt_threadqueue_policy(const enum threadqueue_policy policy);

#endif // ifndef QT_THREADQUEUES_H
/* vim:set expandtab: */
