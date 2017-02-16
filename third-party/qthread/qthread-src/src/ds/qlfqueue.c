#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdlib.h>                    /* for malloc() */
#include <qthread/qthread.h>
#include <qthread/qlfqueue.h>

#include <qthread/qpool.h>
#include "qt_hazardptrs.h"
#include "qt_atomics.h"
#include "qt_asserts.h"
#include "qt_debug.h"                  /* for malloc debug wrappers */
#include "qt_subsystems.h"             /* for qthread_internal_cleanup_late() */

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

static void qlfqueue_internal_cleanup(void)
{
    assert(qlfqueue_node_pool);
    qpool_destroy(qlfqueue_node_pool);
}

/*
 * This lock-free algorithm borrowed from
 * http://www.research.ibm.com/people/m/michael/podc-1996.pdf
 * ... and modified to use hazard ptrs according to
 * http://www.research.ibm.com/people/m/michael/ieeetpds-2004.pdf
 */

qlfqueue_t *qlfqueue_create(void)
{                                      /*{{{ */
    qlfqueue_t *q;

    if (qlfqueue_node_pool == NULL) {
        switch ((uintptr_t)qthread_cas_ptr(&qlfqueue_node_pool, NULL, (void *)1)) {
            case 0: /* I won, I will allocate */
                qlfqueue_node_pool = qpool_create_aligned(sizeof(qlfqueue_node_t), 0);
                qthread_internal_cleanup_late(qlfqueue_internal_cleanup);
                break;
            case 1:
                while (qlfqueue_node_pool == (void *)1) {
                    SPINLOCK_BODY();
                }
                break;
        }
    }
    qassert_ret((qlfqueue_node_pool != NULL), NULL);

    q = MALLOC(sizeof(struct qlfqueue_s));
    if (q != NULL) {
        q->head = (qlfqueue_node_t *)qpool_alloc(qlfqueue_node_pool);
        assert(q->head != NULL);
        if (q->head == NULL) {   /* if we're not using asserts, fail nicely */
            FREE(q, sizeof(struct qlfqueue_s));
            return NULL;
        }
        q->tail       = q->head;
        q->tail->next = NULL;
    }
    return q;
}                                      /*}}} */

int qlfqueue_destroy(qlfqueue_t *q)
{                                      /*{{{ */
    qassert_ret((q != NULL), QTHREAD_BADARGS);
    while (q->head != q->tail) {
        qlfqueue_dequeue(q);
        COMPILER_FENCE;
    }
    COMPILER_FENCE;
    qpool_free(qlfqueue_node_pool, (void *)(q->head));
    FREE(q, sizeof(struct qlfqueue_s));
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
    /* these asserts should be redundant */
    qassert_ret((node != NULL), QTHREAD_MALLOC_ERROR);

    memset((void *)node, 0, sizeof(qlfqueue_node_t));
    node->value = elem;

    while (1) {
        tail = q->tail;

        hazardous_ptr(0, tail);
        if (tail != q->tail) { continue; }

        next = tail->next;
        if (next != NULL) { /* tail not pointing to last node */
            (void)qthread_cas_ptr((void **)&(q->tail), (void *)tail, next);
            continue;
        }
        /* tail must be pointing to the last node */
        if (qthread_cas_ptr((void **)&(tail->next), (void *)next, node) == next) {
            break;             /* success! */
        }
    }
    (void)qthread_cas_ptr((void **)&(q->tail), (void *)tail, node);
    hazardous_ptr(0, NULL); // release the ptr (avoid hazardptr resource exhaustion)
    return QTHREAD_SUCCESS;
}                                      /*}}} */

static void qlfqueue_pool_free_wrapper(void *p)
{   /*{{{*/
    qpool_free(qlfqueue_node_pool, p);
} /*}}}*/

void *qlfqueue_dequeue(qlfqueue_t *q)
{                                      /*{{{ */
    void            *p = NULL;
    qlfqueue_node_t *head;
    qlfqueue_node_t *tail;
    qlfqueue_node_t *next_ptr;

    qassert_ret((q != NULL), NULL);
    while (1) {
        head = q->head;

        hazardous_ptr(0, head);
        if (head != q->head) { continue; }

        tail     = q->tail;
        next_ptr = head->next;

        hazardous_ptr(1, next_ptr);

        if (next_ptr == NULL) { return NULL; } /* queue is empty */
        if (head == tail) { /* tail is falling behind! */
            /* advance tail ptr... */
            (void)qthread_cas_ptr((void **)&(q->tail), (void *)tail, next_ptr);
            continue;
        }
        /* read value before CAS, otherwise another dequeue might free the next node */
        p = next_ptr->value;
        if (qthread_cas_ptr((void **)&(q->head), (void *)head, next_ptr) == head) {
            break;             /* success! */
        }
    }
    hazardous_release_node(qlfqueue_pool_free_wrapper, head);
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
        next = head->next;
        MACHINE_FENCE;
        if (head == q->head) {      /* are head, tail, and next consistent? */
            if (head == tail) {     /* is queue empty or tail falling behind? */
                if (next == NULL) { /* queue is empty! */
                    return 1;
                } else {               /* tail falling behind (queue NOT empty) */
                    return 0;
                }
            } else {                   /* queue is NOT empty and tail is NOT falling behind */
                return 0;
            }
        }
    }
}                                      /*}}} */

/* vim:set expandtab: */
