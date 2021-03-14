#ifndef QTHREAD_QLFQUEUE_H
#define QTHREAD_QLFQUEUE_H

#include "macros.h"

Q_STARTCXX /* */

typedef struct qlfqueue_s qlfqueue_t;

/* Create a new qlfqueue */
qlfqueue_t *qlfqueue_create(void);

/* destroy that queue */
int qlfqueue_destroy(qlfqueue_t *q);

/* enqueue something in the queue */
int qlfqueue_enqueue(qlfqueue_t *q,
                     void       *elem);

/* dequeue something from the queue (returns NULL for an empty queue) */
void *qlfqueue_dequeue(qlfqueue_t *q);

/* returns 1 if the queue is empty, 0 otherwise */
int qlfqueue_empty(qlfqueue_t *q);

Q_ENDCXX /* */

#endif // ifndef QTHREAD_QLFQUEUE_H
/* vim:set expandtab: */
