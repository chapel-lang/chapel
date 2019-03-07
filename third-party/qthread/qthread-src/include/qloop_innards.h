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
    aligned_t                        work_array_size;
    aligned_t                        work_array; // really an address to reach an array
                                                 // with one aligned_t per worker/shepherd
};


#endif // ifndef QLOOP_INNARDS_H
/* vim:set expandtab: */
