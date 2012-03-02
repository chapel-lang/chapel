// This lock-free algorithm borrowed from
// http://www.research.ibm.com/people/m/michael/podc-1996.pdf

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdlib.h>                    /* for malloc() */
#include <qthread/qthread.h>
#include <qthread/qlfqueue.h>

#include <qthread/qpool.h>
#include "qthread_asserts.h"

/* queue declarations */
typedef struct _qlfqueue_node {
    void *value;
    void *next;
} qlfqueue_node_t;

struct qlfqueue_s {             /* typedef'd to qlfqueue_t */
    qlfqueue_node_t *head;
    qlfqueue_node_t *tail;
};

static qpool *qlfqueue_node_pool = NULL;

/* to avoid ABA reinsertion trouble, each pointer in the queue needs to have a
 * monotonically increasing counter associated with it. The counter doesn't
 * need to be huge, just big enough to avoid trouble. We'll
 * just claim 4, to be conservative. Thus, a qlfqueue_node_t must be aligned to
 * 16 bytes. */
#if defined(QTHREAD_USE_VALGRIND) && NO_ABA_PROTECTION
# define QPTR(x)        (x)
# define QCTR(x)        0
# define QCOMPOSE(x, y) (x)
#else
# define QCTR_MASK (15)
# define QPTR(x)        ((qlfqueue_node_t *)(((uintptr_t)(x))& ~(uintptr_t)QCTR_MASK))
# define QCTR(x)        (((uintptr_t)(x))&QCTR_MASK)
# define QCOMPOSE(x, y) (void *)(((uintptr_t)QPTR(x)) | ((QCTR(y) + 1)&QCTR_MASK))
#endif

qlfqueue_t *qlfqueue_create(void)
{                                      /*{{{ */
    qlfqueue_t *q;

    if (qlfqueue_node_pool == NULL) {
        qpool *newpool = qpool_create_aligned(sizeof(qlfqueue_node_t), 16);
        qpool *oldpool = qthread_cas_ptr(&qlfqueue_node_pool, NULL, newpool);
        if (oldpool != NULL) {
            qpool_destroy(newpool);
        }
    }
    qassert_ret((qlfqueue_node_pool != NULL), NULL);

    q = malloc(sizeof(struct qlfqueue_s));
    if (q != NULL) {
        q->head = (qlfqueue_node_t *)qpool_alloc(qlfqueue_node_pool);
        assert(QPTR(q->head) != NULL);
        if (QPTR(q->head) == NULL) {   // if we're not using asserts, fail nicely
            free(q);
            return NULL;
        }
        q->tail             = q->head;
        QPTR(q->tail)->next = NULL;
    }
    return q;
}                                      /*}}} */

int qlfqueue_destroy(qlfqueue_t *q)
{                                      /*{{{ */
    qassert_ret((q != NULL), QTHREAD_BADARGS);
    while (QPTR(q->head) != QPTR(q->tail)) {
        qlfqueue_dequeue(q);
        COMPILER_FENCE;
    }
    COMPILER_FENCE;
    qpool_free(qlfqueue_node_pool, (void *)(QPTR(q->head)));
    free(q);
    return QTHREAD_SUCCESS;
}                                      /*}}} */

int qlfqueue_enqueue(qlfqueue_t *q,
                     void       *elem)
{                                      /*{{{ */
    qlfqueue_node_t *tail;
    qlfqueue_node_t *next;
    qlfqueue_node_t *node;

    qassert_ret((elem != NULL), QTHREAD_BADARGS);
    qassert_ret((q != NULL), QTHREAD_BADARGS);

    node = (qlfqueue_node_t *)qpool_alloc(qlfqueue_node_pool);
    // these asserts should be redundant
    qassert_ret((node != NULL), QTHREAD_MALLOC_ERROR);
    assert(QCTR(node) == 0);           // node MUST be aligned

    memset((void *)node, 0, sizeof(qlfqueue_node_t));
    node->value = elem;

    while (1) {
        tail = q->tail;
        next = QPTR(tail)->next;
        MACHINE_FENCE;
        if (tail == q->tail) {      // are tail and next consistent?
            if (QPTR(next) == NULL) {  // was tail pointing to the last node?
                if (qthread_cas_ptr((void **)&(QPTR(tail)->next),
                                    (void *)next,
                                    QCOMPOSE(node, next)) == next) {
                    break;             // success!
                }
            } else {                   // tail not pointing to last node
                (void)qthread_cas_ptr((void **)&(q->tail),
                                      (void *)tail,
                                      QCOMPOSE(next, tail));
            }
        }
    }
    (void)qthread_cas_ptr((void **)&(q->tail),
                          (void *)tail,
                          QCOMPOSE(node, tail));
    return QTHREAD_SUCCESS;
}                                      /*}}} */

void *qlfqueue_dequeue(qlfqueue_t *q)
{                                      /*{{{ */
    void            *p = NULL;
    qlfqueue_node_t *head;
    qlfqueue_node_t *tail;
    qlfqueue_node_t *next_ptr;

    qassert_ret((q != NULL), NULL);
    while (1) {
        head     = q->head;
        tail     = q->tail;
        next_ptr = QPTR(QPTR(head)->next);
        MACHINE_FENCE;
        if (head == q->head) {           // are head, tail, and next consistent?
            if (QPTR(head) == QPTR(tail)) { // is queue empty or tail falling behind?
                if (next_ptr == NULL) {     // is queue empty?
                    return NULL;
                }
                (void)qthread_cas_ptr((void **)&(q->tail),
                                      (void *)tail,
                                      QCOMPOSE(next_ptr, tail)); // advance tail ptr
            } else if (next_ptr != NULL) {                       // no need to deal with tail
                // read value before CAS, otherwise another dequeue might free the next node
                p = next_ptr->value;
                if (qthread_cas_ptr((void **)&(q->head),
                                    (void *)head,
                                    QCOMPOSE(next_ptr, head)) == head) {
                    break;             // success!
                }
            }
        }
    }
    qpool_free(qlfqueue_node_pool, (void *)(QPTR(head)));
    return p;
}                                      /*}}} */

int qlfqueue_empty(qlfqueue_t *q)
{                                      /*{{{ */
    qlfqueue_node_t *head;
    qlfqueue_node_t *tail;
    qlfqueue_node_t *next;

    qassert_ret((q != NULL), QTHREAD_BADARGS);

    while (1) {
        head = q->head;
        tail = q->tail;
        next = QPTR(head)->next;
        MACHINE_FENCE;
        if (head == q->head) {           // are head, tail, and next consistent?
            if (QPTR(head) == QPTR(tail)) { // is queue empty or tail falling behind?
                if (QPTR(next) == NULL) {   // queue is empty!
                    return 1;
                } else {               // tail falling behind (queue NOT empty)
                    return 0;
                }
            } else {                   // queue is NOT empty and tail is NOT falling behind
                return 0;
            }
        }
    }
}                                      /*}}} */

/* vim:set expandtab: */
