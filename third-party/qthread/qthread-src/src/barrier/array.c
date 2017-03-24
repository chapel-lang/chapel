#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */

/* System Compatibility Header */
#include "qthread-int.h"

/* Installed Headers */
#include "qthread/qthread.h"
#include "qthread/barrier.h"

/* Internal Headers */
#include "qt_atomics.h"
#include "qt_visibility.h"
#include "qt_debug.h"
#include "qt_asserts.h"
#include "qt_barrier.h"

#if 0
# define WTYPE syncvar_t
# define INIT(x)   x = SYNCVAR_EMPTY_INITIALIZER
# define EMPTY(x)  qthread_syncvar_empty(x)
# define WAIT(x)   qthread_syncvar_readFE(NULL, (x))
# define SIGNAL(x) qthread_syncvar_writeEF_const((x), 0)
# define CLEAN(x)  do { } while(0)
#else
# define WTYPE aligned_t
# define INIT(x)   qthread_empty(&(x))
# define EMPTY(x)  qthread_empty(x)
# define WAIT(x)   qthread_readFE(NULL, (x))
# define SIGNAL(x) qthread_writeEF_const((x), 0)
# define CLEAN(x)  qthread_fill(&(x))
#endif /* if 1 */

/* The Datatype */
struct qt_barrier_s {
    size_t    maxParticipants;
    size_t    numParticipants;
    WTYPE    *up;
    WTYPE    *down;
    aligned_t participant;
};

qt_barrier_t API_FUNC *qt_barrier_create(size_t           max_threads,
                                         qt_barrier_btype type)
{
    qt_barrier_t *b    = MALLOC(sizeof(qt_barrier_t));
    uint64_t      pow2 = max_threads - 1;

    pow2 |= pow2 >> 1;
    pow2 |= pow2 >> 2;
    pow2 |= pow2 >> 4;
    pow2 |= pow2 >> 8;
    pow2 |= pow2 >> 16;
    pow2 |= pow2 >> 32;
    pow2++;
    b->maxParticipants = pow2;
    b->numParticipants = max_threads;
    b->up              = MALLOC(sizeof(WTYPE) * pow2);
    b->down            = MALLOC(sizeof(WTYPE) * pow2);
    b->participant     = 0;
    for (uint64_t i = 0; i < pow2; i++) {
        INIT(b->up[i]);
        INIT(b->down[i]);
    }
    return b;
}

void API_FUNC qt_barrier_enter(qt_barrier_t *b)
{
    qt_barrier_enter_id(b, qthread_incr(&(b->participant), 1) % b->numParticipants);
}

void API_FUNC qt_barrier_enter_id(qt_barrier_t *b,
                                  size_t        id)
{
    size_t   parent, leftchild, rightchild;
    size_t   test;
    uint64_t value;

    parent     = ((id + 1) >> 1) - 1;
    leftchild  = ((id + 1) << 1) - 1;
    rightchild = leftchild + 1;

    if (leftchild < b->numParticipants) {
        /* Wait for my children to enter the barrier */
        // printf("id %u wait for leftchild (%u:%u)\n", (unsigned)id, (unsigned)leftchild, qthread_feb_status(&(b->up[leftchild])));
        WAIT(&(b->up[leftchild]));
        if (rightchild < b->numParticipants) {
            // printf("id %u wait for rightchild (%u)\n", (unsigned)id, (unsigned)rightchild);
            WAIT(&(b->up[rightchild]));
        }
    }

    if (id > 0) {
        /* Tell my parent that I have entered the barrier */
        // printf("id %u signal parent\n", (unsigned)id);
        SIGNAL(&(b->up[id]));

        /* Wait for my parent to wake me up */
        // printf("id %u wait for parent (%u)\n", (unsigned)id, (unsigned)parent);
        WAIT(&(b->down[id]));
    }

    /* Wake my children */
    if (leftchild < b->numParticipants) {
        SIGNAL(&(b->down[leftchild]));
        if (rightchild < b->numParticipants) {
            SIGNAL(&(b->down[rightchild]));
        }
    }
}

void API_FUNC qt_barrier_destroy(qt_barrier_t *b)
{
    assert(b);
    assert(b->up);
    assert(b->down);
    for (uint64_t i = 0; i < b->maxParticipants; i++) {
        CLEAN(b->up[i]);
        CLEAN(b->down[i]);
    }
    FREE(b->up, sizeof(WTYPE) * b->maxParticipants);
    FREE(b->down, sizeof(WTYPE) * b->maxParticipants);
    FREE(b, sizeof(qt_barrier_t));
}

void API_FUNC qt_barrier_resize(qt_barrier_t *b,
                                size_t        size)
{
    assert(b);
    if (size <= b->maxParticipants) {
        b->numParticipants = size;
    } else {
        uint64_t pow2    = size - 1;
        size_t   oldsize = b->maxParticipants;

        for (size_t i = 0; i < oldsize; ++i) {
            CLEAN(b->up[i]);
            CLEAN(b->down[i]);
        }
        pow2 |= pow2 >> 1;
        pow2 |= pow2 >> 2;
        pow2 |= pow2 >> 4;
        pow2 |= pow2 >> 8;
        pow2 |= pow2 >> 16;
        pow2 |= pow2 >> 32;
        pow2++;
        b->maxParticipants = pow2;
        b->numParticipants = size;
        FREE(b->up, sizeof(WTYPE) * b->maxParticipants);
        FREE(b->down, sizeof(WTYPE) * b->maxParticipants);
        b->up   = MALLOC(sizeof(WTYPE) * pow2);
        b->down = MALLOC(sizeof(WTYPE) * pow2);
        for (size_t i = 0; i < pow2; ++i) {
            INIT(b->up[i]);
            INIT(b->down[i]);
        }
    }
    b->participant = 0;
}

/* debugging... */
void API_FUNC qt_barrier_dump(qt_barrier_t    *b,
                              qt_barrier_dtype dt)
{}

void INTERNAL qt_barrier_internal_init(void)
{}

/* vim:set expandtab: */
