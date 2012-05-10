#ifndef QT_QTHREAD_STRUCT_H
#define QT_QTHREAD_STRUCT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt_context.h"
#include "qt_shepherd_innards.h"
#include "qt_threadstate.h"

#define ARGCOPY_DEFAULT   1024
#define TASKLOCAL_DEFAULT 64

/* flags (must be different bits) */
#define QTHREAD_FUTURE              (1 << 0)
#define QTHREAD_REAL_MCCOY          (1 << 1)
#define QTHREAD_RET_IS_SYNCVAR      (1 << 2)
#define QTHREAD_UNSTEALABLE         (1 << 3)
#define QTHREAD_MUST_BE_WORKER_ZERO (1 << 4)     /* force thread to shepherd 0 worker 0 for termination 4/1/11 akp */
#define QTHREAD_HAS_ARGCOPY         (1 << 5)
#define QTHREAD_TEAM_LEADER         (1 << 6)
#define QTHREAD_RESERVED_2          (1 << 7)

/* flags for teams (must be different bits) */
#define QTHREAD_TEAM_DEAD           (1 << 0)
#define QTHREAD_TEAM_RESERVED_1     (1 << 1)
#define QTHREAD_TEAM_RESERVED_2     (1 << 2)
#define QTHREAD_TEAM_RESERVED_3     (1 << 3)
#define QTHREAD_TEAM_RESERVED_4     (1 << 4)
#define QTHREAD_TEAM_RESERVED_5     (1 << 5)
#define QTHREAD_TEAM_RESERVED_6     (1 << 6)
#define QTHREAD_TEAM_RESERVED_7     (1 << 7)

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
# ifdef QTHREAD_OMP_AFFINITY
    /* affinity for children created by this task */
    qthread_shepherd_id_t child_affinity;
# endif
#endif
};

typedef struct qt_team_s {
    qt_sinc_t   *sinc;
    qt_team_id_t team_id;
    uint_fast8_t flags;
} qt_team_t;

struct qthread_s {
    struct qthread_s *volatile     next;

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

    qt_team_t                      *team; /* reference to task team */

    /* preconditions for data-dependent tasks */
    unsigned                       npreconds;
    void                          *preconds;
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    qthread_parallel_region_t     *currentParallelRegion; /* parallel region barrier this thread should use */
    aligned_t                      task_counter;
    struct qthread_s              *parent;             /* pointer to parent task */
    enum threadstate               prev_thread_state;  /* save the previous thread state */
#endif

    Q_ALIGNED(8) uint8_t data[]; /* this is where we stick argcopy and tasklocal data */
};

#endif // ifndef QT_QTHREAD_STRUCT_H
/* vim:set expandtab: */
