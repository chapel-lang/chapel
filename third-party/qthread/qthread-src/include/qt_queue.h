#ifndef QT_QUEUE_H
#define QT_QUEUE_H

#include "qt_qthread_t.h"
#include "qt_visibility.h"

/* This queue can use the NEMESIS lock-free queue protocol from
 * http://www.mcs.anl.gov/~buntinas/papers/ccgrid06-nemesis.pdf
 */

typedef struct qthread_queue_node_s {
    struct qthread_queue_node_s *next;
    qthread_t                   *thread;
} qthread_queue_node_t;

typedef struct qthread_queue_NEMESIS_s {
    /* The First Cacheline */
    void     *head;
    void     *tail;
    uint8_t   pad1[CACHELINE_WIDTH - (2 * sizeof(void *))];
    /* The Second Cacheline */
    aligned_t length;
    void     *shadow_head;
    uint8_t   pad2[CACHELINE_WIDTH - sizeof(void *) - sizeof(aligned_t)];
} qthread_queue_NEMESIS_t;

typedef struct qthread_queue_nosync_s {
    qthread_queue_node_t *head;
    qthread_queue_node_t *tail;
} qthread_queue_nosync_t;

typedef struct qthread_queue_capped_s {
    qthread_t **members;
    aligned_t   membercount;
    aligned_t   maxmembers;
    aligned_t   busy; /* a flag to say whether someone is busy modifying this datastructure */
} qthread_queue_capped_t;

enum qthread_queue_synctype {
    NOSYNC,
    NEMESIS,        /* multi-join, emptying is user's synch */
    MTS,            /* multi-join, multi-empty (UNIMPLEMENTED) */
    NEMESIS_LENGTH, /* multi-join, w/ atomic length */
    CAPPED
};

struct qthread_queue_s {
    enum qthread_queue_synctype type;
    union {
        qthread_queue_nosync_t  nosync;
        qthread_queue_NEMESIS_t nemesis;
        qthread_queue_capped_t  capped;
    } q;
};

void INTERNAL qthread_queue_subsystem_init(void);
void INTERNAL qthread_queue_internal_enqueue(qthread_queue_t q,
                                             qthread_t      *t);
void INTERNAL qthread_queue_internal_nosync_enqueue(qthread_queue_nosync_t *q,
                                                    qthread_t              *t);
qthread_t INTERNAL *qthread_queue_internal_nosync_dequeue(qthread_queue_nosync_t *q);
void INTERNAL       qthread_queue_internal_NEMESIS_enqueue(qthread_queue_NEMESIS_t *q,
                                                           qthread_t               *t);
qthread_t INTERNAL *qthread_queue_internal_NEMESIS_dequeue(qthread_queue_NEMESIS_t *q);
void INTERNAL       qthread_queue_internal_capped_enqueue(qthread_queue_capped_t *q,
                                                          qthread_t              *t);
qthread_t INTERNAL *qthread_queue_internal_capped_dequeue(qthread_queue_capped_t *q);

#endif // ifndef QT_QUEUE_H
/* vim:set expandtab: */
