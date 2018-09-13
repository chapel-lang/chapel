#ifndef QT_SHEPHERD_INNARDS_H
#define QT_SHEPHERD_INNARDS_H

#ifdef QTHREAD_PERFORMANCE
#include <qthread/performance.h>
#endif

/* System Pre-requisites */
#include <pthread.h>

/* Internal Pre-requisites */
#include "qt_mpool.h"
#include "qt_atomics.h"
#include "qt_threadqueues.h"
#include "qt_hazardptrs.h"
#include "qt_macros.h"

#ifdef QTHREAD_SHEPHERD_PROFILING
# include "qthread/qtimer.h"
#endif

#ifdef QTHREAD_FEB_PROFILING
# include "qt_hash.h"
#endif

#ifdef QTHREAD_OMP_AFFINITY
# include "omp_affinity.h"
#endif

#ifndef QTHREAD_SHEPHERD_TYPEDEF
# define QTHREAD_SHEPHERD_TYPEDEF
typedef struct qthread_shepherd_s qthread_shepherd_t;
#endif

#define QTHREAD_NO_NODE ((unsigned int)(-1))

# define STEAL_BUFFER_LENGTH 128

struct qthread_worker_s {
    uintptr_t                 hazard_ptrs[HAZARD_PTRS_PER_SHEP]; /* hazard pointers (see http://portal.acm.org/citation.cfm?id=987524.987595) */
    hazard_freelist_t         hazard_free_list;
    pthread_t                 worker;
    qthread_shepherd_t       *shepherd;
    struct qthread_s        **nostealbuffer;
    struct qthread_s        **stealbuffer;
    qthread_t                *current;
    qthread_worker_id_t       unique_id;
    qthread_worker_id_t       worker_id;
    qthread_worker_id_t       packed_worker_id;
#ifdef QTHREAD_PERFORMANCE
    struct qtperfdata_s*             performance_data;
#endif
    Q_ALIGNED(8) uint_fast8_t QTHREAD_CASLOCK(active);
};
typedef struct qthread_worker_s qthread_worker_t;

/* The Shepherd Struct */
struct qthread_shepherd_s {
    pthread_t             shepherd;
    qthread_shepherd_id_t shepherd_id;  /* whoami */
    qthread_worker_t     *workers;  // dymanic length qlib->nworkerspershep
    qthread_t            *current;
    qt_threadqueue_t     *ready;
#ifdef QTHREAD_LOCAL_PRIORITY
    qt_threadqueue_t     *local_priority_queue;
#endif /* ifdef QTHREAD_LOCAL_PRIORITY */
    /* round robin scheduler - can probably be smarter */
    aligned_t             sched_shepherd;
    uintptr_t             QTHREAD_CASLOCK(active);
    /* affinity information */
    unsigned int          node;  /* whereami */
#ifdef QTHREAD_HAVE_LGRP
    unsigned int          lgrp;
#endif
    unsigned int          *shep_dists;
    qthread_shepherd_id_t *sorted_sheplist;
    unsigned int           stealing; /* True when a worker is in the steal (attempt) process OR if stealing disabled*/
#ifdef QTHREAD_OMP_AFFINITY
    unsigned int           stealing_mode; /* Specifies when a shepherd may steal */
#endif
#ifdef STEAL_PROFILE // should give mechanism to make steal profiling optional
    size_t steal_called;
    size_t steal_elected;
    size_t steal_attempted;
    size_t steal_failed;
#endif
#ifdef QTHREAD_SHEPHERD_PROFILING
    qtimer_t total_time;        /* how much time the shepherd spent running */
    double   idle_maxtime;      /* max time the shepherd spent waiting for new threads */
    double   idle_time;         /* how much time the shepherd spent waiting for new threads */
    size_t   idle_count;        /* how many times the shepherd did a blocking dequeue */
    size_t   num_threads;       /* number of threads handled */
#endif
#ifdef QTHREAD_FEB_PROFILING
# ifdef QTHREAD_MUTEX_INCREMENT
    qt_hash uniqueincraddrs;    /* the unique addresses that are incremented */
    double  incr_maxtime;       /* maximum time spent in a single increment */
    double  incr_time;          /* total time spent incrementing */
    size_t  incr_count;         /* number of increments */
# endif

    qt_hash  uniquelockaddrs;    /* the unique addresses that are locked */
    double   aquirelock_maxtime; /* max time spent aquiring locks */
    double   aquirelock_time;    /* total time spent aquiring locks */
    size_t   aquirelock_count;   /* num locks aquired */
    double   lockwait_maxtime;   /* max time spent blocked on a lock */
    double   lockwait_time;      /* total time spent blocked on a lock */
    size_t   lockwait_count;     /* num times blocked on a lock */
    double   hold_maxtime;       /* max time spent holding locks */
    double   hold_time;          /* total time spent holding locks (use aquirelock_count) */

    qt_hash  uniquefebaddrs;    /* unique addresses that are associated with febs */
    double   febblock_maxtime;  /* max time spent aquiring FEB words */
    double   febblock_time;     /* total time spent aquiring FEB words */
    size_t   febblock_count;    /* num FEB words aquired */
    double   febwait_maxtime;   /* max time spent blocking on FEBs */
    double   febwait_time;      /* total time spent blocking on FEBs */
    size_t   febwait_count;     /* num FEB blocking waits required */
    double   empty_maxtime;     /* max time addresses spent empty */
    double   empty_time;        /* total time addresses spent empty */
    size_t   empty_count;       /* num times addresses were empty */
#endif // ifdef QTHREAD_FEB_PROFILING
    uint32_t padding[CACHELINE_WIDTH / sizeof(uint32_t)];
};

extern TLS_DECL(qthread_shepherd_t *, shepherd_structs);

static QINLINE qthread_shepherd_t *qthread_internal_getshep(void)
{
    qthread_worker_t *w = (qthread_worker_t *)TLS_GET(shepherd_structs);
    if (w == NULL) {
        return NULL;
    } else {
        return w->shepherd;
    }
}

static QINLINE qthread_worker_t *qthread_internal_getworker(void)
{
    return (qthread_worker_t *)TLS_GET(shepherd_structs);
}

unsigned int INTERNAL qthread_internal_shep_to_node(const qthread_shepherd_id_t shep);
qthread_shepherd_t INTERNAL *qthread_find_active_shepherd(qthread_shepherd_id_t *l,
                                                          unsigned int          *d);

void qthread_back_to_master(qthread_t *t);
void qthread_back_to_master2(qthread_t *t);

#endif // ifndef QT_SHEPHERD_INNARDS_H
/* vim:set expandtab: */
