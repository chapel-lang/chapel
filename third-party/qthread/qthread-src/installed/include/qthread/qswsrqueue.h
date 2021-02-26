#ifndef QTHREAD_QSWSRQUEUE_H
#define QTHREAD_QSWSRQUEUE_H

#include "macros.h"

Q_STARTCXX /* */

typedef struct qswsrqueue_s qswsrqueue_t;

/* Create a new qswsrqueue */
qswsrqueue_t *qswsrqueue_create(size_t elements);

/* destroy that queue */
int qswsrqueue_destroy(qswsrqueue_t *q);

/* enqueue something in the queue if there is room */
int qswsrqueue_enqueue(qswsrqueue_t *q,
                       void         *elem);

/* enqueue something in the queue */
int qswsrqueue_enqueue_blocking(qswsrqueue_t *q,
                                void         *elem);

/* dequeue something from the queue (returns NULL for an empty queue) */
void *qswsrqueue_dequeue(qswsrqueue_t *q);

/* dequeue something from the queue */
void *qswsrqueue_dequeue_blocking(qswsrqueue_t *q);

/* returns 1 if the queue is empty, 0 otherwise */
int qswsrqueue_empty(qswsrqueue_t *q);

Q_ENDCXX /* */

#endif // ifndef QTHREAD_QLFQUEUE_H
/* vim:set expandtab: */
