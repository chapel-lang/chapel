#ifndef QT_QTHREAD_STRUCT_H
#define QT_QTHREAD_STRUCT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt_context.h"
#include "qt_shepherd_innards.h"

#define ARGCOPY_DEFAULT   1024
#define TASKLOCAL_DEFAULT 64

enum threadstate {
    QTHREAD_STATE_NASCENT,              /* awaiting preconds */
    QTHREAD_STATE_NEW,                  /* first ready-to-run state */
    QTHREAD_STATE_RUNNING,              /* ready-to-run */
    QTHREAD_STATE_YIELDED,              /* reschedule, otherwise ready-to-run */
    QTHREAD_STATE_BLOCKED,              /* waiting for lock */
    QTHREAD_STATE_FEB_BLOCKED,          /* waiting for feb */
    QTHREAD_STATE_TERMINATED,           /* thread function returned */
    QTHREAD_STATE_MIGRATING,            /* thread needs to be moved, otherwise ready-to-run */
    QTHREAD_STATE_SYSCALL,              /* thread performing external blocking operation */
    QTHREAD_STATE_TERM_SHEP = UINT8_MAX /* special flag to terminate the shepherd */
};

/* flags (must be different bits) */
#define QTHREAD_FUTURE              (1 << 0)
#define QTHREAD_REAL_MCCOY          (1 << 1)
#define QTHREAD_RET_IS_SYNCVAR      (1 << 2)
#define QTHREAD_UNSTEALABLE         (1 << 3)
#define QTHREAD_MUST_BE_WORKER_ZERO (1 << 4)     /* force thread to shepherd 0 worker 0 for termination 4/1/11 akp */
#define QTHREAD_HAS_ARGCOPY         (1 << 5)
#define QTHREAD_RESERVED_1          (1 << 6)
#define QTHREAD_RESERVED_2          (1 << 7)

struct qthread_runtime_data_s {
    void         *stack;           /* the thread's stack */
    qt_context_t  context;         /* the context switch info */
    qt_context_t *return_context;  /* context of parent shepherd */

    /* a pointer used for passing information back to the shepherd when
     * becoming blocked */
    struct qthread_lock_s *blockedon;
    qthread_shepherd_t    *shepherd_ptr; /* the shepherd we run on */

#ifdef QTHREAD_USE_VALGRIND
    unsigned int valgrind_stack_id;
#endif
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    int            forCount;                     /* added akp */
    taskSyncvar_t *openmpTaskRetVar;             /* ptr to linked list if task's I started -- used in openMP taskwait */
    syncvar_t      taskWaitLock;
# ifdef QTHREAD_OMP_AFFINITY
    /* affinity for children created by this task */
    qthread_shepherd_id_t child_affinity;
# endif
#endif
};

struct qthread_s {
    struct qthread_s *volatile     next;
    qthread_shepherd_t            *creator_ptr; /* the shepherd our memory comes from */

    unsigned int                   thread_id;
    enum threadstate               thread_state;
    uint8_t                        flags;
    qthread_shepherd_t            *target_shepherd; /* the shepherd we'd rather run on */

    qthread_f                      f;               /* the function to call (that defines this thread) */
    void                          *arg;             /* user defined data */
    void                          *ret;             /* user defined retval location */
    struct qthread_runtime_data_s *rdata;
    unsigned                       tasklocal_size;

    aligned_t                      id;  /* id used in barrier and arrive_first */
    /* preconditions for data-dependent tasks */
    unsigned                       npreconds;
    void                          *preconds;
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    qthread_parallel_region_t     *currentParallelRegion; /* parallel region barrier this thread should use */
    struct qthread_s              *child;            /* pointer to list task created */
    struct qthread_s              *sibling;          /* previous task created by same parent */
    syncvar_t                      ret_value;        /* return value for task -- don't need to allocated it seperately */
    int                            task_completed;   /* counter of task completes - 1) in workhorse loop (state QTHREAD_STATE_TERMINATED) 2) parent has seen result and completes cannot free until both happen */
#endif

    Q_ALIGNED(8) uint8_t data[]; /* this is where we stick argcopy and tasklocal data */
};

#endif // ifndef QT_QTHREAD_STRUCT_H
/* vim:set expandtab: */
