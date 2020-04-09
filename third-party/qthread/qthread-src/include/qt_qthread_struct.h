#ifndef QT_QTHREAD_STRUCT_H
#define QT_QTHREAD_STRUCT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qthread/barrier.h"

#include "qt_context.h"
#include "qt_shepherd_innards.h"
#include "qt_threadstate.h"
#include "qt_blocking_structs.h"
#include "qt_teams.h"
#include "qt_queue.h"

#define ARGCOPY_DEFAULT   1024
#define TASKLOCAL_DEFAULT 8

/* flags (must be different bits) */
#define QTHREAD_FUTURE           (1 << 0)
#define QTHREAD_REAL_MCCOY       (1 << 1)
#define QTHREAD_RET_IS_SYNCVAR   (1 << 2)
#define QTHREAD_RET_IS_SINC      (1 << 3)
#define QTHREAD_RET_IS_VOID_SINC ((1 << 3) | (1 << 2))
#define QTHREAD_UNSTEALABLE      (1 << 4)
#define QTHREAD_SIMPLE           (1 << 5)
#define QTHREAD_HAS_ARGCOPY      (1 << 6)
#define QTHREAD_TEAM_LEADER      (1 << 7)
#define QTHREAD_TEAM_WATCHER     (1 << 8)
#define QTHREAD_BIG_STRUCT       (1 << 9)
#define QTHREAD_AGGREGABLE       (1 << 10)
#define QTHREAD_AGGREGATED       (1 << 11)
#define QTHREAD_NETWORK          (1 << 12)
#define QTHREAD_RESERVED_FLAG3   (1 << 13)
#define QTHREAD_RESERVED_FLAG2   (1 << 14)
#define QTHREAD_RESERVED_FLAG1   (1 << 15)

#define QTHREAD_RET_MASK (QTHREAD_RET_IS_SYNCVAR | QTHREAD_RET_IS_SINC)

struct qthread_runtime_data_s {
    void         *stack;           /* the thread's stack */
    qt_context_t  context;         /* the context switch info */
    qt_context_t *return_context;  /* context of parent shepherd */

    /* a pointer used for passing information back to the shepherd when
     * context swapping */
    union {
        qthread_addrstat_t       *addr;
        qt_blocking_queue_node_t *io;
        qthread_t                *thread;
        qthread_queue_t           queue;
    } blockedon;
    qthread_shepherd_t *shepherd_ptr;    /* the shepherd we run on */
    unsigned            tasklocal_size;
    int                 criticalsect; /* critical section depth */
    qt_barrier_t       *barrier;      /* add to allow barriers to be stacked/nested parallelism - akp 10/16/12 */

#ifdef QTHREAD_USE_VALGRIND
    unsigned int valgrind_stack_id;
#endif
# ifdef QTHREAD_OMP_AFFINITY
    /* affinity for children created by this task */
    qthread_shepherd_id_t child_affinity;
# endif
# ifdef QTHREAD_PERFORMANCE
    qtperfdata_t* performance_data;
# endif
};

/* Try very VERY hard to keep this under 1 cacheline (64 bytes) */
struct qthread_s {
    qthread_f                      f;               /* the function to call (that defines this thread) */
    void                          *arg;             /* user defined data */
    void                          *ret;             /* user defined retval location */
    struct qthread_runtime_data_s *rdata;

    qt_team_t                     *team; /* reference to task team */
    /* preconditions for data-dependent tasks */
    void                          *preconds;

    unsigned int               thread_id;
    qthread_shepherd_id_t      target_shepherd; /* the shepherd we'd rather run on; set to NO_SHEPHERD unless the thread either migrated or was spawned to a specific destination (aka the programmer expressed a desire for this thread to be somewhere) */
    uint16_t                   flags;           /* may not need all bits */
    uint8_t                    thread_state : 4;

    Q_ALIGNED(8) uint8_t data[]; /* this is where we stick argcopy and tasklocal data */
};

#endif // ifndef QT_QTHREAD_STRUCT_H
/* vim:set expandtab: */
