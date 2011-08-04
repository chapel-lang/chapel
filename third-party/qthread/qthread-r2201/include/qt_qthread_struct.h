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
    QTHREAD_STATE_NASCENT,
    QTHREAD_STATE_NEW,
    QTHREAD_STATE_RUNNING,
    QTHREAD_STATE_YIELDED,
    QTHREAD_STATE_BLOCKED,
    QTHREAD_STATE_FEB_BLOCKED,
    QTHREAD_STATE_TERMINATED,
    QTHREAD_STATE_DONE,
    QTHREAD_STATE_MIGRATING,
    QTHREAD_STATE_SYSCALL,
    QTHREAD_STATE_TERM_SHEP = UINT8_MAX
};

/* flags (must be different bits) */
#define QTHREAD_FUTURE              (1 << 0)
#define QTHREAD_REAL_MCCOY          (1 << 1)
#define QTHREAD_RET_IS_SYNCVAR      (1 << 2)
#define QTHREAD_UNSTEALABLE         (1 << 3)
#define QTHREAD_MUST_BE_WORKER_ZERO (1 << 4)     /* force thread to shepherd 0 worker 0 for termination 4/1/11 akp */
#define QTHREAD_HAS_ARGCOPY         (1 << 5)

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
    int                        forCount;         /* added akp */
    taskSyncvar_t             *openmpTaskRetVar; /* ptr to linked list if task's I started -- used in openMP taskwait */
    syncvar_t                  taskWaitLock;
    /* parallel region barrier this thread should use */
    qthread_parallel_region_t *currentParallelRegion;
# ifdef QTHREAD_OMP_AFFINITY
    /* affinity for children created by this task */
    qthread_shepherd_id_t child_affinity;
# endif
#endif
};

struct qthread_s {
    struct qthread_s *volatile next;
    /* the shepherd our memory comes from */
    qthread_shepherd_t        *creator_ptr;

    unsigned int               thread_id;
    enum threadstate           thread_state;
    unsigned char              flags;

    /* the shepherd we'd rather run on */
    qthread_shepherd_t *target_shepherd;

    /* input preconds for lazy tasks */
    void * preconds;
    unsigned npreconds;

    /* the function to call (that defines this thread) */
    qthread_f                      f;
    aligned_t                      id;  /* id used in barrier and arrive_first */
    void                          *arg; /* user defined data */
    void                          *ret; /* user defined retval location */
    struct qthread_runtime_data_s *rdata;
    unsigned                       tasklocal_size;
    Q_ALIGNED(8) uint8_t           data[];
};

#endif // ifndef QT_QTHREAD_STRUCT_H
/* vim:set expandtab: */
