#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* API */
#include <qthread/qthread.h>
#include <qthread/qswsrqueue.h>

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_alloc.h"          /* for aligned alloc */

/* queue declarations */
struct qswsrqueue_s {             /* typedef'd to qswsrqueue_t */
    uint32_t head;
    uint32_t size;
    uint8_t  pad[CACHELINE_WIDTH - (2 * sizeof(uint32_t))];
    uint32_t tail;
    uint32_t size2;
    uint8_t  pad2[CACHELINE_WIDTH - (2 * sizeof(uint32_t))];
    void    *elements[];
};

qswsrqueue_t *qswsrqueue_create(size_t elements)
{                                      /*{{{ */
    qswsrqueue_t *q;

    if ((elements * sizeof(void *)) < CACHELINE_WIDTH) {
        elements = CACHELINE_WIDTH / sizeof(void *);
    }
    if (elements % CACHELINE_WIDTH) {
        elements += CACHELINE_WIDTH - (elements % CACHELINE_WIDTH);
    }
    if (elements > UINT32_MAX) {
        return NULL;
    }
    q = qt_internal_aligned_alloc(sizeof(struct qswsrqueue_s) + (elements * sizeof(void *)), CACHELINE_WIDTH);
    if (q != NULL) {
        q->head  = 0;
        q->size  = elements;
        q->tail  = 0;
        q->size2 = elements;
    }
    return q;
}                                      /*}}} */

int qswsrqueue_destroy(qswsrqueue_t *q)
{                                      /*{{{ */
    qassert_ret((q != NULL), QTHREAD_BADARGS);
    qt_internal_aligned_free(q, sizeof(struct qswsrqueue_s) + (q->size * sizeof(void *)));
    return QTHREAD_SUCCESS;
}                                      /*}}} */

int qswsrqueue_enqueue(qswsrqueue_t *q,
                       void         *elem)
{                                      /*{{{ */
    uint32_t cur_tail  = q->tail;
    uint32_t next_tail = (cur_tail + 1) % q->size;
    COMPILER_FENCE;
    if (next_tail != q->head) {
        q->elements[cur_tail] = elem;
        MACHINE_FENCE;
        q->tail = next_tail;
        return QTHREAD_SUCCESS;
    } else {
        return QTHREAD_OPFAIL;
    }
}                                      /*}}} */

int qswsrqueue_enqueue_blocking(qswsrqueue_t *q,
                       void         *elem)
{                                      /*{{{ */
    uint32_t cur_tail  = q->tail;
    uint32_t next_tail = (cur_tail + 1) % q->size2;
    do {
        while (next_tail == q->head) qthread_yield();
        COMPILER_FENCE;
        if (next_tail != q->head) {
            q->elements[cur_tail] = elem;
            MACHINE_FENCE;
            q->tail = next_tail;
            return QTHREAD_SUCCESS;
        }
    } while (1);
}                                      /*}}} */

void *qswsrqueue_dequeue(qswsrqueue_t *q)
{                                      /*{{{ */
    void *item;
    uint32_t cur_head = q->head;
    COMPILER_FENCE;
    if (cur_head == q->tail)
        return NULL;
    item = q->elements[cur_head];
    COMPILER_FENCE; // MIGHT need to be a MACHINE_FENCE, but I don't think so
    q->head = (cur_head + 1) % q->size;
    return item;
}                                      /*}}} */

void *qswsrqueue_dequeue_blocking(qswsrqueue_t *q)
{                                      /*{{{ */
    void *item;
    uint32_t cur_head = q->head;
    uint32_t next_head = (cur_head + 1) % q->size;
    do {
        while (next_head == q->tail) qthread_yield();
        COMPILER_FENCE;
        if (next_head != q->tail) {
            item = q->elements[cur_head];
            COMPILER_FENCE;
            q->head = next_head;
            return item;
        }
    } while (1);
}                                      /*}}} */

/* returns 1 if the queue is empty, 0 otherwise */
int qswsrqueue_empty(qswsrqueue_t *q)
{                                      /*{{{ */
    return (q->head == q->tail);
}                                      /*}}} */

/* vim:set expandtab: */
