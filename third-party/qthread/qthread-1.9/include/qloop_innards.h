#ifndef QLOOP_INNARDS_H
#define QLOOP_INNARDS_H

#include "qthread/qtimer.h"

typedef struct qqloop_iteration_queue {
    saligned_t         start;
    saligned_t         stop;
    saligned_t         step;
    qt_loop_queue_type type;
    union {
        saligned_t phase;
        struct {
            qtimer_t   *timers;
            saligned_t *lastblocks;
        } timed;
    } type_specific_data;
} qqloop_iteration_queue_t;
struct qqloop_static_args;
struct qqloop_wrapper_range;
typedef int (*qq_getiter_f)(qqloop_iteration_queue_t *const restrict,
                            struct qqloop_static_args *const restrict,
                            struct qqloop_wrapper_range *const restrict);
struct qqloop_static_args {
    qt_loop_f                 func;
    void                     *arg;
    aligned_t                 donecount;
    aligned_t                 activesheps;
    qqloop_iteration_queue_t *iq;
    qq_getiter_f              get;
    size_t                    chunksize;
};
struct qqloop_step_static_args {
    qt_loop_step_f            func;
    void                     *arg;
    aligned_t                 donecount;
    aligned_t                 activesheps;
    qqloop_iteration_queue_t *iq;
    qq_getiter_f              get;
};
struct qqloop_wrapper_args {
    qthread_shepherd_id_t      shep;
    struct qqloop_static_args *stat;
};
struct qqloop_step_wrapper_args {
    qthread_shepherd_id_t           shep;
    struct qqloop_step_static_args *stat;
};
struct qqloop_wrapper_range {
    size_t startat, stopat, step;
};
struct qqloop_handle_s {
    struct qqloop_wrapper_args *qwa;
    struct qqloop_static_args   stat;
};

enum qloop_handle_type {
    QLOOP_NONE = 0,
    STATIC_SCHED,
    GUIDED_SCHED,
    DYNAMIC_SCHED,
    RUNTIME_SCHED
};

struct qqloop_step_handle_s {
    struct qqloop_step_wrapper_args *qwa;
    struct qqloop_step_static_args   stat;
#ifdef QTHREAD_USE_ROSE_EXTENSIONS
    aligned_t                        workers;
    aligned_t                        departed_workers;
    aligned_t                        expected_workers; // used to decide when the data structure can be reused (expected_workers == departed_workers) AKP 1/26/12
    aligned_t                       *current_workers;  // actually allocated at end of work_array
    aligned_t                        allowed_workers;
    struct qqloop_step_handle_s     *next;
    enum qloop_handle_type           type;
    aligned_t                        assignNext;
    aligned_t                        assignStart;
    aligned_t                        assignStop;
    aligned_t                        assignStep;
    aligned_t                        chunkSize;
    aligned_t                        assignDone; // start+offset
    aligned_t                        iterations;
    aligned_t                        whichLoop; // value used to pair workers with correct loop structure
    aligned_t                        ready;     // what is the last whichLoop that has been initialized (usable)
#endif /* QTHREAD_USE_ROSE_EXTENSIONS */
    aligned_t                        work_array_size;
    aligned_t                        work_array; // really an address to reach an array
                                                 // with one aligned_t per worker/shepherd
};

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
#include "qt_visibility.h"

int qloop_internal_computeNextBlock(qqloop_step_handle_t *loop);

double *cnbTimeMin_(void);
int *   cnbWorkers_(void);

qqloop_step_handle_t *qt_loop_rose_queue_create(int64_t start,
                                                int64_t stop,
                                                int64_t incr);
void qt_loop_rose_queue_free(qqloop_step_handle_t *);

/* functions added by akp to handle OpenMP task completion
 */
extern int __qthreads_temp;
void INTERNAL qthread_reset_forCount(void);

int INTERNAL qthread_forCount(int inc);

#endif /* QTHREAD_USE_ROSE_EXTENSIONS */

#endif // ifndef QLOOP_INNARDS_H
/* vim:set expandtab: */
