#ifndef QT_QTHREAD_STRUCT_H
#define QT_QTHREAD_STRUCT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt_context.h"
#include "qt_shepherd_innards.h"
#include "qt_threadstate.h"
#include "qt_blocking_structs.h"

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

/* flags for teams (must be different bits) */
#define QTHREAD_TEAM_DEAD       (1 << 0)
#define QTHREAD_TEAM_RESERVED_1 (1 << 1)
#define QTHREAD_TEAM_RESERVED_2 (1 << 2)
#define QTHREAD_TEAM_RESERVED_3 (1 << 3)
#define QTHREAD_TEAM_RESERVED_4 (1 << 4)
#define QTHREAD_TEAM_RESERVED_5 (1 << 5)
#define QTHREAD_TEAM_RESERVED_6 (1 << 6)
#define QTHREAD_TEAM_RESERVED_7 (1 << 7)

struct qthread_runtime_data_s {
    void         *stack;           /* the thread's stack */
    qt_context_t  context;         /* the context switch info */
    qt_context_t *return_context;  /* context of parent shepherd */

    /* a pointer used for passing information back to the shepherd when
     * becoming blocked */
    union {
        qthread_addrstat_t       *addr;
        qt_blocking_queue_node_t *io;
    } blockedon;
    qthread_shepherd_t *shepherd_ptr;    /* the shepherd we run on */
    unsigned            tasklocal_size;

#ifdef QTHREAD_USE_VALGRIND
    unsigned int valgrind_stack_id;
#endif
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    int forCount;                                /* added akp */
# ifdef QTHREAD_OMP_AFFINITY
    /* affinity for children created by this task */
    qthread_shepherd_id_t child_affinity;
# endif
#endif
};

typedef struct qt_team_s {
    qt_team_id_t team_id;
    aligned_t    eureka;
    aligned_t    watcher_started;
    qt_sinc_t   *sinc;
    qt_sinc_t   *subteams_sinc;
    qt_team_id_t parent_id;
    aligned_t   *parent_eureka;
    qt_sinc_t   *parent_subteams_sinc;
    uint_fast8_t flags;
} qt_team_t;

/* Try very VERY hard to keep this under 1 cacheline (64 bytes) */
struct qthread_s {
    qthread_f                      f;               /* the function to call (that defines this thread) */
    void                          *arg;             /* user defined data */
    void                          *ret;             /* user defined retval location */
    struct qthread_runtime_data_s *rdata;

    qt_team_t *team;                     /* reference to task team */
    /* preconditions for data-dependent tasks */
    void *preconds;

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    // XXX: I suspect that several of these should be moved into the qthread_runtime_data_s struct
    aligned_t                  id;                    /* id used in barrier and arrive_first */
    qthread_parallel_region_t *currentParallelRegion; /* parallel region barrier this thread should use */
    aligned_t                  task_counter;
    struct qthread_s          *parent;                 /* pointer to parent task */
    enum threadstate           prev_thread_state;      /* save the previous thread state */
#endif
    unsigned int               thread_id;
    qthread_shepherd_id_t      target_shepherd;       /* the shepherd we'd rather run on */
    uint16_t                   flags;
    uint8_t                    thread_state;

    Q_ALIGNED(8) uint8_t data[]; /* this is where we stick argcopy and tasklocal data */
};

#endif // ifndef QT_QTHREAD_STRUCT_H
/* vim:set expandtab: */
