#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */
#include <pthread.h>
#include <stdint.h>
#include <emmintrin.h>
#include <stdio.h>

/* Internal Headers */
#include "qthread/qthread.h"
#include "qt_macros.h"
#include "56reader-rwlock.h"
#include "qt_visibility.h"
#include "qthread_innards.h"           /* for qlib */
#include "qt_shepherd_innards.h"
#include "qt_qthread_struct.h"
#include "qt_asserts.h"
#include "qt_prefetch.h"
#include "qt_threadqueues.h"
#include "qt_envariables.h"

#ifndef NOINLINE
# define NOINLINE __attribute__ ((noinline))
#endif

/* Data Structures */
struct uint128 {
    uint64_t lo;
    uint64_t hi;
};

typedef struct uint128 uint128_t;

struct _qt_threadqueue_entry {
    qthread_t *value;
    uint32_t   index;
    uint32_t   counter;
};

typedef struct _qt_threadqueue_entry qt_threadqueue_entry_t;

typedef __m128i m128i;

typedef union {
    m128i sse; qt_threadqueue_entry_t entry;
} qt_threadqueue_union_t;

struct _qt_threadqueue {
    m128i                 top;
    m128i                 blanks[3];
    m128i                *base;
    uint32_t              size;
    uint32_t              bottom;
    rwlock_t             *rwlock;
    QTHREAD_FASTLOCK_TYPE spinlock;

    /* used for the work stealing queue implementation */
    m128i    flush[1];
    uint32_t empty;
    uint32_t stealing;
    uint32_t steal_disable;
} /* qt_threadqueue_t */;

// Forward declarations

void INTERNAL qt_threadqueue_enqueue_multiple(qt_threadqueue_t   *q,
                                              int                 stealcount,
                                              qthread_t         **stealbuffer,
                                              qthread_shepherd_t *shep);

INTERNAL int qt_threadqueue_dequeue_steal(qt_threadqueue_t *q,
                                          qthread_t       **nostealbuffer,
                                          qthread_t       **stealbuffer);

void INTERNAL qt_threadqueue_resize_and_enqueue(qt_threadqueue_t *q,
                                                qthread_t        *t);

int static QINLINE qt_threadqueue_stealable(qthread_t *t);

qthread_t static QINLINE *qt_threadqueue_dequeue_helper(qt_threadqueue_t *q);

void INTERNAL qt_threadqueue_enqueue_unstealable(qt_threadqueue_t *q,
                                                 qthread_t       **nostealbuffer,
                                                 int               amtNotStolen);

void INTERNAL qt_threadqueue_subsystem_init(void) {}

#ifdef CAS_STEAL_PROFILE
static void cas_profile_update(int id,
                               int retries)
{
    uint64_strip_t *cas_steal_profile = qlib->cas_steal_profile;

    if (cas_steal_profile == NULL) { return; }
    if (retries >= CAS_STEAL_PROFILE_LENGTH) {
        cas_steal_profile[id].fields[CAS_STEAL_PROFILE_LENGTH - 1]++;
    } else {
        cas_steal_profile[id].fields[retries]++;
    }
}

#else /* ifdef CAS_STEAL_PROFILE */
# define cas_profile_update(x, y) do {} while(0)
#endif /* ifdef CAS_STEAL_PROFILE */

ssize_t INTERNAL qt_threadqueue_advisory_queuelen(qt_threadqueue_t *q)
{   /*{{{*/
    return 0;
} /*}}}*/

/*****************************************/
/* functions to manage the thread queues */
/*****************************************/

static QINLINE long       qthread_steal_chunksize(void);
static QINLINE qthread_t *qthread_steal(qt_threadqueue_t *thiefq);

qt_threadqueue_t INTERNAL *qt_threadqueue_new(void)
{   /*{{{*/
    qt_threadqueue_t *q;

    posix_memalign((void **)&q, 64, sizeof(qt_threadqueue_t));

    if (q != NULL) {
        qt_threadqueue_union_t top;
        top.entry.value   = NULL;
        top.entry.index   = 0;
        top.entry.counter = 0;

        q->top      = top.sse;
        q->size     = 1024;
        q->bottom   = 0;
        q->empty    = 1;
        q->stealing = 0;
        QTHREAD_FASTLOCK_INIT(q->spinlock);
        posix_memalign((void **)&(q->base), 64, q->size * sizeof(m128i));
        posix_memalign((void **)&(q->rwlock), 64, sizeof(rwlock_t));
        rwlock_init(q->rwlock);
        memset(q->base, 0, q->size * sizeof(m128i));
    }
    return q;
} /*}}}*/

void INTERNAL qt_threadqueue_free(qt_threadqueue_t *q)
{   /*{{{*/
    // mspiegel: is it necessary to drain the queue?
    /* while (q->head != q->tail) {
     *  qt_scheduler_get_thread(q, 1);
     * } */
    qt_free((void *)q->base);
    qt_free((void *)q);
} /*}}}*/

static QINLINE int qt_threadqueue_cas128(uint128_t *src,
                                         uint128_t *cmp,
                                         uint128_t *with)
{   /*{{{*/
    char result;

    // (AT&T syntax)
    __asm__ __volatile__ ("lock; cmpxchg16b (%6);"
                          "setz %7; "
                          : "=a" (cmp->lo),
                          "=d" (cmp->hi)
                          : "0" (cmp->lo),
                          "1" (cmp->hi),
                          "b" (with->lo),
                          "c" (with->hi),
                          "r" (src),
                          "m" (result)
                          : "cc", "memory");
    /* (Intel syntax)
     * __asm__ __volatile__
     * (
     *  "lock cmpxchg16b oword ptr %1\n\t"
     *  "setz %0"
     *  : "=q" ( result )
     *  , "+m" ( *src )
     *  , "+d" ( cmp->hi )
     *  , "+a" ( cmp->lo )
     *  : "c" ( with->hi )
     *  , "b" ( with->lo )
     *  : "cc"
     * );    */
    return result;
} /*}}}*/

static QINLINE void qt_threadqueue_finish(qt_threadqueue_t      *q,
                                          qt_threadqueue_entry_t top_entry)
{   /*{{{*/
    qt_threadqueue_union_t snapshot, oldnode;

    uint32_t index = top_entry.index;

    snapshot.sse = q->base[index];

    oldnode.entry.value   = snapshot.entry.value;
    oldnode.entry.counter = top_entry.counter - 1;
    oldnode.entry.index   = top_entry.index = 0;

    if(snapshot.entry.counter > oldnode.entry.counter) { return; }
    if(snapshot.entry.counter == top_entry.counter) { return; }

    qt_threadqueue_cas128((uint128_t *)q->base + index,
                          (uint128_t *)&oldnode, (uint128_t *)&top_entry);
} /*}}}*/

#ifdef QTHREAD_USE_SPAWNCACHE
qthread_t INTERNAL *qt_threadqueue_private_dequeue(qt_threadqueue_private_t *c)
{
    return NULL;
}

int INTERNAL qt_threadqueue_private_enqueue(qt_threadqueue_private_t *restrict pq,
                                            qt_threadqueue_t *restrict         q,
                                            qthread_t *restrict                t)
{
    return 0;
}

int INTERNAL qt_threadqueue_private_enqueue_yielded(qt_threadqueue_private_t *restrict q,
                                                    qthread_t *restrict                t)
{
    return 0;
}

#endif /* ifdef QTHREAD_USE_SPAWNCACHE */

/* enqueue at tail */
void INTERNAL qt_threadqueue_enqueue(qt_threadqueue_t *restrict q,
                                     qthread_t *restrict        t)
{   /*{{{*/
    qt_threadqueue_union_t oldtop, snapshot, lastchance;
    qt_threadqueue_entry_t newtop;
    uint32_t               nextindex;

#ifdef CAS_STEAL_PROFILE
    int cycles = 0;
#endif

    int id = qthread_worker_unique(NULL);

    rwlock_rdlock(q->rwlock, id);

    oldtop.sse = q->top;

    while(1) {
#ifdef CAS_STEAL_PROFILE
        cycles++;
#endif

        qt_threadqueue_finish(q, oldtop.entry);

        nextindex = (oldtop.entry.index + 1) % q->size;

        if (nextindex == q->bottom) {
            // Pthread reader-writer locks will deadlock
            // on lock promotion attempts.
            rwlock_rdunlock(q->rwlock, id);
            qt_threadqueue_resize_and_enqueue(q, t);
            cas_profile_update(id, cycles - 1);
            return;
        }

        snapshot.sse   = q->base[nextindex];
        newtop.value   = t;
        newtop.index   = nextindex;
        newtop.counter = snapshot.entry.counter + 1;

        lastchance.sse = q->top;

        if(lastchance.entry.counter != oldtop.entry.counter) {
            oldtop.entry = lastchance.entry;
            continue;
        }

        if(qt_threadqueue_cas128((uint128_t *)&(q->top),
                                 (uint128_t *)&oldtop, (uint128_t *)&newtop)) { break; }
    }

    q->empty = 0;

    rwlock_rdunlock(q->rwlock, id);

    cas_profile_update(id, cycles - 1);
} /*}}}*/

/* enqueue multiple (from steal) */
void INTERNAL qt_threadqueue_enqueue_multiple(qt_threadqueue_t   *q,
                                              int                 stealcount,
                                              qthread_t         **stealbuffer,
                                              qthread_shepherd_t *shep)
{   /*{{{*/
    /* save element 0 for the thief */
    for(int i = 1; i < stealcount; i++) {
        qthread_t *t = stealbuffer[i];
        t->target_shepherd = shep->shepherd_id;
        qt_threadqueue_enqueue(q, t);
    }
} /*}}}*/

/* This function is called when the queue is full.
 * Either a thread is enqueuing at the tail,
 * or a thread is enqueuing at the head.
 * In both cases, we need to grow the array.
 * PRECONDITION: the writer lock must be held. */
static QINLINE void qt_threadqueue_resize(qt_threadqueue_t *q)
{
    // TODO: error checking has not been performed.
    // If oldsize == UINT32_MAX, then indicate an error.
    // If memory allocation returns NULL, then indicate an error.

    uint32_t               oldsize = q->size, bottom = q->bottom;
    uint32_t               newsize = (oldsize > (UINT32_MAX / 2)) ? UINT32_MAX : oldsize * 2;
    m128i                 *newloc;
    qt_threadqueue_union_t top;

    qassert(posix_memalign((void **)&(newloc), 64, newsize * sizeof(m128i)), 0);

    assert(newsize > oldsize);
    assert(newloc != NULL);

    uint32_t len1 = oldsize - bottom;
    uint32_t len2 = bottom;

    m128i *dest1 = newloc;
    m128i *dest2 = newloc + len1;

    m128i *src1 = q->base + bottom;
    m128i *src2 = q->base;

    memcpy(dest1, src1, len1 * sizeof(m128i));
    memcpy(dest2, src2, len2 * sizeof(m128i));
    memset(newloc + oldsize, 0, (newsize - oldsize) * sizeof(m128i));

    top.sse           = q->top;
    top.entry.counter = top.entry.counter + 1;
    top.entry.index   = oldsize - 1;
    q->top            = top.sse;

    q->base   = newloc;
    q->size   = newsize;
    q->bottom = 0;
}

void INTERNAL qt_threadqueue_resize_and_enqueue(qt_threadqueue_t *q,
                                                qthread_t        *t)
{   /*{{{*/
    int id = qthread_worker_unique(NULL);

    rwlock_wrlock(q->rwlock, id);

    qt_threadqueue_union_t top, newtop, snapshot;
    uint32_t               nextindex, oldsize, bottom;

    top.sse = q->top;

    qt_threadqueue_finish(q, top.entry);

    oldsize   = q->size;
    bottom    = q->bottom;
    nextindex = (top.entry.index + 1) % oldsize;

    if (nextindex == bottom) {
        qt_threadqueue_resize(q);
        nextindex = oldsize;
    }

    snapshot.sse = q->base[nextindex];

    newtop.entry.value   = t;
    newtop.entry.index   = nextindex;
    newtop.entry.counter = snapshot.entry.counter + 1;

    snapshot.entry.value   = t;
    snapshot.entry.counter = snapshot.entry.counter + 1;

    q->top             = newtop.sse;
    q->base[nextindex] = snapshot.sse;

    q->empty = 0;

    rwlock_wrunlock(q->rwlock);
} /*}}}*/

/* yielded threads enqueue at head */
void INTERNAL qt_threadqueue_enqueue_yielded(qt_threadqueue_t *restrict q,
                                             qthread_t *restrict        t)
{   /*{{{*/
    int id = qthread_worker_unique(NULL);

    rwlock_wrlock(q->rwlock, id);

    qt_threadqueue_union_t top;

    top.sse = q->top;

    qt_threadqueue_finish(q, top.entry);

    /* Three cases to consider:
     * (a) The queue is empty. Move the
     *     new thread into q->top and return.
     * (b) The queue is full. Resize the
     *     queue and then continue to part (c).
     * (c) otherwise.
     */
    if (top.entry.index == q->bottom) {
        qt_threadqueue_union_t snapshot, newtop;

        uint32_t nextindex = (top.entry.index + 1) % q->size;
        snapshot.sse = q->base[nextindex];
        uint32_t nextcounter = snapshot.entry.counter + 1;

        newtop.entry.index   = nextindex;
        newtop.entry.value   = t;
        newtop.entry.counter = nextcounter;

        snapshot.entry.value   = t;
        snapshot.entry.counter = nextcounter;

        q->top             = newtop.sse;
        q->base[nextindex] = snapshot.sse;
        rwlock_wrunlock(q->rwlock);
        return;
    } else if ((top.entry.index + 1) % q->size == q->bottom) {
        qt_threadqueue_resize(q);
    }

    uint32_t bot    = q->bottom, size = q->size;
    uint32_t newbot = (bot - 1) % size;

    qt_threadqueue_union_t bottom, newbottom;

    bottom.sse               = q->base[bot];
    newbottom.sse            = q->base[newbot];
    bottom.entry.counter    += 1;
    newbottom.entry.counter += 1;
    bottom.entry.value       = t;
    newbottom.entry.value    = NULL;

    q->base[bot]    = bottom.sse;
    q->base[newbot] = newbottom.sse;
    q->bottom       = newbot;

    q->empty = 0;

    rwlock_wrunlock(q->rwlock);
} /*}}}*/

qthread_t static QINLINE *qt_threadqueue_dequeue_helper(qt_threadqueue_t *q)
{
    qthread_t *t = NULL;

    q->stealing = 1;

    QTHREAD_FASTLOCK_LOCK(&q->spinlock);
    if ( !(q->steal_disable) && (q->stealing)) {
        t = qthread_steal(q);
    }
    QTHREAD_FASTLOCK_UNLOCK(&q->spinlock);

    return(t);
}

/* dequeue at tail, unlike original qthreads implementation */
qthread_t INTERNAL *qt_scheduler_get_thread(qt_threadqueue_t         *q,
                                            qt_threadqueue_private_t *QUNUSED(qc),
                                            uint_fast8_t              active)
{   /*{{{*/
    qthread_t             *t      = NULL;
    rwlock_t              *rwlock = q->rwlock;
    qt_threadqueue_union_t oldtop, lastchance;

#ifdef CAS_STEAL_PROFILE
    int cycles = 0;
#endif

    int id = qthread_worker_unique(NULL);

    assert(q != NULL);

    rwlock_rdlock(rwlock, id);

    oldtop.sse = q->top;

    while(1) {
#ifdef CAS_STEAL_PROFILE
        cycles++;
#endif

        if (oldtop.entry.index == q->bottom) {
            rwlock_rdunlock(rwlock, id);
            if (active) {
                t = qt_threadqueue_dequeue_helper(q);
                if (t != NULL) {
                    cas_profile_update(id, cycles - 1);
                    return(t);
                }
            }
            rwlock_rdlock(rwlock, id);
            oldtop.sse = q->top;
        } else {
            t = oldtop.entry.value;

            if ((t->flags & QTHREAD_REAL_MCCOY)) { // only needs to be on worker 0 for termination
                switch(qthread_worker(NULL)) {
                    case NO_WORKER:                  // only happens during termination -- keep trying
                        rwlock_rdunlock(rwlock, id); // release lock and get new value
                        rwlock_rdlock(rwlock, id);
                        oldtop.sse = q->top;
                        continue;
                    case 0:
                        break;
                    default:
                        /* McCoy thread can only run on worker 0 */
                        rwlock_rdunlock(rwlock, id);
                        if (active) {
                            t = qt_threadqueue_dequeue_helper(q);
                            if (t != NULL) {
                                cas_profile_update(id, cycles - 1);
                                return(t);
                            }
                        }
                        rwlock_rdlock(rwlock, id);
                        oldtop.sse = q->top;
                        continue;
                }
            }

            qt_threadqueue_finish(q, oldtop.entry);

            uint32_t previndex = (oldtop.entry.index - 1) % q->size;

            qt_threadqueue_entry_t newtop;
            qt_threadqueue_union_t belowtop;
            belowtop.sse   = q->base[previndex];
            newtop.index   = previndex;
            newtop.value   = belowtop.entry.value;
            newtop.counter = belowtop.entry.counter + 1;

            lastchance.sse = q->top;

            if(lastchance.entry.counter != oldtop.entry.counter) {
                oldtop.entry = lastchance.entry;
                continue;
            }

            if(qt_threadqueue_cas128((uint128_t *)&(q->top),
                                     (uint128_t *)&oldtop, (uint128_t *)&newtop)) {
                rwlock_rdunlock(rwlock, id);
                assert(t != NULL);
                cas_profile_update(id, cycles - 1);
                return (t);
            }
        }
    }
} /*}}}*/

int static QINLINE qt_threadqueue_stealable(qthread_t *t)
{
    return(t->thread_state != QTHREAD_STATE_YIELDED &&
           t->thread_state != QTHREAD_STATE_TERM_SHEP &&
           !(t->flags & QTHREAD_UNSTEALABLE));
}

void INTERNAL qt_threadqueue_enqueue_unstealable(qt_threadqueue_t *q,
                                                 qthread_t       **nostealbuffer,
                                                 int               amtNotStolen)
{
    if (amtNotStolen == 0) { return; }

    uint32_t bottom = q->bottom;

    qt_threadqueue_union_t top;
    top.sse = q->top;

    if(top.entry.index == bottom) {
        top.entry.value = nostealbuffer[amtNotStolen - 1];
        q->top          = top.sse;
    }

    qt_threadqueue_union_t snapshot;
    for(int i = amtNotStolen - 1; i >= 0; i--) {
        snapshot.sse         = q->base[bottom];
        snapshot.entry.value = nostealbuffer[i];
        q->base[bottom]      = snapshot.sse;
        bottom               = (bottom - 1) % q->size;
    }
    q->bottom = bottom;
}

/* dequeue stolen threads at head, skip yielded threads */
INTERNAL int qt_threadqueue_dequeue_steal(qt_threadqueue_t *q,
                                          qthread_t       **nostealbuffer,
                                          qthread_t       **stealbuffer)
{                                      /*{{{ */
    assert(q != NULL);

    int amtStolen = 0, amtNotStolen = 0;

    int id = qthread_worker_unique(NULL);

    if (q->empty) { return(0); }

    rwlock_wrlock(q->rwlock, id);

    qt_threadqueue_union_t top;

    top.sse = q->top;
    qt_threadqueue_finish(q, top.entry);

    uint32_t bottom  = q->bottom;
    uint32_t current = (bottom + 1) % q->size;

    qt_threadqueue_union_t snapshot;

    while (amtStolen < qthread_steal_chunksize()) {
        snapshot.sse = q->base[current];

        /* Three cases to consider:
         * (a) The queue is empty.
         * (b) The queue contains a single element.
         * (c) Otherwise.
         */
        if (bottom == top.entry.index) {
            q->empty = 1;
            break;
        } else if (current == top.entry.index) {
            qthread_t *candidate = top.entry.value;
            if (qt_threadqueue_stealable(candidate)) {
                stealbuffer[amtStolen++] = candidate;
                snapshot.entry.value     = NULL;
                snapshot.entry.counter   = snapshot.entry.counter + 1;
                top.entry.value          = NULL;
                top.entry.counter        = snapshot.entry.counter;
                q->base[current]         = snapshot.sse;
                q->top                   = top.sse;
                bottom                   = current;
            }
            q->empty = 1;
            break;
        } else {
            qthread_t *candidate = snapshot.entry.value;
            if (qt_threadqueue_stealable(candidate)) {
                stealbuffer[amtStolen++] = candidate;
            } else if (amtNotStolen == STEAL_BUFFER_LENGTH) {
                abort(); // should never happen
            } else {
                nostealbuffer[amtNotStolen++] = candidate;
            }
            snapshot.entry.value   = NULL;
            snapshot.entry.counter = snapshot.entry.counter + 1;
            q->base[current]       = snapshot.sse;
            bottom                 = current;
            current                = (current + 1) % q->size;
        }
    }
    q->bottom = bottom;

    qt_threadqueue_enqueue_unstealable(q, nostealbuffer, amtNotStolen);

    rwlock_wrunlock(q->rwlock);

#ifdef STEAL_PROFILE                   // should give mechanism to make steal profiling optional
    qthread_incr(&q->steal_amount_stolen, amtStolen);
#endif

    return(amtStolen);
}                                      /*}}} */

/* Returns the number of tasks to steal per steal operation (chunk size) */
static QINLINE long qthread_steal_chunksize(void)
{   /*{{{*/
    static long chunksize = 0;

    if (chunksize == 0) {
        chunksize = qt_internal_get_env_num("STEAL_CHUNKSIZE", qlib->nworkerspershep, 1);
    }

    return chunksize;
} /*}}}*/

/*  Steal work from another shepherd's queue
 *    Returns the amount of work stolen
 *  PRECONDITION: the readlock must be aquired.
 */
static QINLINE qthread_t *qthread_steal(qt_threadqueue_t *thiefq)
{   /*{{{*/
    int i;

    extern TLS_DECL(qthread_shepherd_t *, shepherd_structs);
    qthread_shepherd_t *victim_shepherd;
    qthread_worker_t   *worker =
        (qthread_worker_t *)TLS_GET(shepherd_structs);
    qthread_shepherd_t *thief_shepherd =
        (qthread_shepherd_t *)worker->shepherd;
    qthread_t **nostealbuffer = worker->nostealbuffer;
    qthread_t **stealbuffer   = worker->stealbuffer;

#ifdef STEAL_PROFILE                   // should give mechanism to make steal profiling optional
    qthread_incr(&thief_shepherd->steal_called, 1);
#endif
#ifdef QTHREAD_OMP_AFFINITY
    if (thief_shepherd->stealing_mode == QTHREAD_STEAL_ON_ALL_IDLE) {
        for (i = 0; i < qlib->nworkerspershep; i++)
            if (thief_shepherd->workers[i].current != NULL) {
                thiefq->stealing = 0;
                return(NULL);
            }
        thief_shepherd->stealing_mode = QTHREAD_STEAL_ON_ANY_IDLE;
    }
#endif
#ifdef STEAL_PROFILE                   // should give mechanism to make steal profiling optional
    qthread_incr(&thief_shepherd->steal_attempted, 1);
#endif
    int shepherd_offset = qthread_worker(NULL) % qlib->nshepherds;
    for (i = 1; i < qlib->nshepherds; i++) {
        shepherd_offset = (shepherd_offset + 1) % qlib->nshepherds;
        if (shepherd_offset == thief_shepherd->shepherd_id) {
            shepherd_offset = (shepherd_offset + 1) % qlib->nshepherds;
        }
        victim_shepherd = &qlib->shepherds[shepherd_offset];
        if (victim_shepherd->ready->empty) { continue; }
        int amtStolen = qt_threadqueue_dequeue_steal(victim_shepherd->ready,
                                                     nostealbuffer, stealbuffer);
        if (amtStolen > 0) {
#ifdef STEAL_PROFILE                   // should give mechanism to make steal profiling optional
            qthread_incr(&thief_shepherd->steal_successful, 1);
#endif
            qt_threadqueue_enqueue_multiple(thiefq, amtStolen, stealbuffer, thief_shepherd);
            thiefq->stealing = 0;
            return(stealbuffer[0]);
        }
#ifdef STEAL_PROFILE                   // should give mechanism to make steal profiling optional
        else {
            qthread_incr(&thief_shepherd->steal_failed, 1);
        }
#endif
    }
    thiefq->stealing = 0;
    return(NULL);
} /*}}}*/

#ifdef CAS_STEAL_PROFILE
void INTERNAL qthread_cas_steal_stat(void)
{
    int            i, j;
    uint64_strip_t accum;
    uint64_t       total        = 0;
    double         weighted_sum = 0.0;

    for(j = 0; j < CAS_STEAL_PROFILE_LENGTH; j++) {
        accum.fields[j] = 0;
    }
    for (i = 0; i < qlib->nshepherds * qlib->nworkerspershep; i++) {
        for(j = 0; j < CAS_STEAL_PROFILE_LENGTH; j++) {
            accum.fields[j] += qlib->cas_steal_profile[i].fields[j];
        }
    }
    for(j = 0; j < CAS_STEAL_PROFILE_LENGTH; j++) {
        total        += accum.fields[j];
        weighted_sum += (accum.fields[j] * j);
    }

    fprintf(stdout, "threadqueue distribution of CAS retries\n");
    for(j = 0; j < (CAS_STEAL_PROFILE_LENGTH - 1); j++) {
        fprintf(stdout, "%d  - %4.2f%%\n", j, ((double)accum.fields[j]) / total * 100.0);
    }
    fprintf(stdout, "%d+ - %4.2f%%\n", j, ((double)accum.fields[j]) / total * 100.0);
    fprintf(stdout, "approximate mean is %4.2f \n", weighted_sum / total);
    fprintf(stdout, "\n");
}

#endif /* ifdef CAS_STEAL_PROFILE */

#ifdef STEAL_PROFILE                   // should give mechanism to make steal profiling optional
void INTERNAL qthread_steal_stat(void)
{
    int i;

    assert(qlib);
    for (i = 0; i < qlib->nshepherds; i++) {
        fprintf(stdout,
                "shepherd %d - steals called %ld attempted %ld failed %ld successful %ld work stolen %ld\n",
                qlib->shepherds[i].shepherd_id,
                qlib->shepherds[i].steal_called,
                qlib->shepherds[i].steal_attempted,
                qlib->shepherds[i].steal_failed,
                qlib->shepherds[i].steal_successful,
                qlib->shepherds[i].steal_amount_stolen);
    }
}

#endif  /* ifdef STEAL_PROFILE */

/* walk queue looking for a specific value  -- if found remove it (and start
 * it running)  -- if not return NULL
 */
qthread_t INTERNAL *qt_threadqueue_dequeue_specific(qt_threadqueue_t *q,
                                                    void             *value)
{   /*{{{*/
    int id = qthread_worker_unique(NULL);

    assert(q != NULL);

    rwlock_wrlock(q->rwlock, id);

    qt_threadqueue_union_t top;

    top.sse = q->top;
    qt_threadqueue_finish(q, top.entry);

    if (q->bottom == top.entry.index) {
        rwlock_wrunlock(q->rwlock);
        return(NULL);
    }

    if (top.entry.value->ret == value) {
        qt_threadqueue_union_t snapshot;

        qthread_t *retval    = top.entry.value;
        uint32_t   previndex = (top.entry.index - 1) % q->size;
        snapshot.sse = q->base[previndex];

        top.entry.index   = previndex;
        top.entry.value   = snapshot.entry.value;
        top.entry.counter = snapshot.entry.counter;

        q->top = top.sse;

        rwlock_wrunlock(q->rwlock);
        return(retval);
    } else {
        uint32_t current = (q->bottom + 1) % q->size;
        uint32_t size    = q->size;
        uint32_t bottom  = q->bottom;
        while (current != top.entry.index) {
            qt_threadqueue_union_t snapshot;

            snapshot.sse = q->base[current];
            qthread_t *t = snapshot.entry.value;
            if (t->ret == value) {
                /* Two cases:
                 * (i)  The current index is below the top
                 *    index in the array. It is easier to
                 *    slide the elements above current, and
                 *    decrement the top index.
                 * (ii) The current index is above the top
                 *    index in the array. It is easier to
                 *    slide the elements below current, and
                 *    increment the bottom index.
                 */
                if (current < top.entry.index) {
                    memmove((void *)(q->base + current), (const void *)(q->base + current + 1),
                            (top.entry.index - current - 1) * sizeof(qt_threadqueue_entry_t));
                    top.entry.index = (top.entry.index - 1) % q->size;
                    q->top          = top.sse;
                } else {
                    memmove((void *)(q->base + bottom + 1), (const void *)(q->base + bottom),
                            (current - bottom) * sizeof(qt_threadqueue_entry_t));
                    q->bottom = (bottom + 1) % q->size;
                }
                rwlock_wrunlock(q->rwlock);
                return(t);
            }
            current = (current + 1) % size;
        }
    }

    rwlock_wrunlock(q->rwlock);
    return (NULL);
} /*}}}*/

void INTERNAL qthread_steal_enable()
{   /*{{{*/
    qt_threadqueue_t *q;
    size_t            i;
    size_t            numSheps = qthread_num_shepherds();

    for (i = 0; i < numSheps; i++) {
        q                = qlib->threadqueues[i];
        q->steal_disable = 0;
    }
}   /*}}}*/

void INTERNAL qthread_steal_disable()
{   /*{{{*/
    qt_threadqueue_t *q;
    size_t            i;
    size_t            numSheps = qthread_num_shepherds();

    for (i = 0; i < numSheps; i++) {
        q                = qlib->threadqueues[i];
        q->steal_disable = 1;
    }
}   /*}}}*/

#if 0 // begin test code, because this function
      // can't go in the test suite as it calls internal functions

# include <stdio.h>

int qt_threadqueue_test()
{
    printf("Initializing test\n");
    qthread_initialize();
    qt_threadqueue_t *threadqueue = qt_threadqueue_new(&(qlib->shepherds[0]));

    assert(threadqueue->empty == 1);
    if(threadqueue->empty != 1) {
        fprintf(stderr, "Threadqueue was initialized with empty bit set to FALSE\n");
        return -1;
    }

    qthread_t *task = qthread_thread_new(NULL, NULL, 0, NULL, 0);

    printf("Enqueueing task.\n");
    qt_threadqueue_enqueue(threadqueue, task);

    assert(threadqueue->empty != 1);

    if(threadqueue->empty == 1) {
        fprintf(stderr, "Threadqueue empty bit set after enqueue operation\n");
        return -1;
    }

    printf("Dequeueing task\n");
    qthread_t *result = qt_scheduler_get_thread(threadqueue, 1);

    assert(result == task);

    if(result != task) {
        fprintf(stderr, "Task enqueued to stack is not identical to task dequeued from stack\n");
        return -1;
    }

    uint32_t size = threadqueue->size;
    printf("Queue size is %d\n", size);
    printf("Enqueueing the same task %d times\n", size);

    for(int i = 0; i < size; i++) {
        qt_threadqueue_enqueue(threadqueue, task);
    }

    uint32_t newsize = threadqueue->size;
    printf("Queue size is %d\n", newsize);

    assert(newsize == 2 * size);
    if (newsize != 2 * size) {
        fprintf(stderr, "New size is not equal to twice the old size\n");
        return -1;
    }

    return 0;
}

// end test code
#endif /* if 0 */

qthread_shepherd_id_t INTERNAL qt_threadqueue_choose_dest(qthread_shepherd_t * curr_shep)
{
    if (curr_shep) {
        return curr_shep->shepherd_id;
    } else {
        return (qthread_shepherd_id_t)0;
    }
}

size_t INTERNAL qt_threadqueue_policy(const enum threadqueue_policy policy)
{
    switch (policy) {
        default:
            return THREADQUEUE_POLICY_UNSUPPORTED;
    }
}

/* vim:set expandtab: */
