#ifndef _CODELET_H_
#define _CODELET_H_

#include "codelet_starpu.h"

#define CODELET_PRIVATE_ID(tag) ((_codelet_tag_t)(tag << 24))
#define CODELET_INNER_LOOP_ID(level, tag) ((_codelet_tag_t)(1 << (16 + level) | tag))
#define CODELET_BARRIER_ID(tag) ((_codelet_tag_t)(tag << 30))

int chpl_codelet_init(void);
void chpl_codelet_shutdown(void);
void chpl_codelet_wait_for_id(_codelet_tag_t);
void chpl_codelet_signal(_codelet_tag_t);
void _fork_tasks(void **, void *);
void inner_barrier(void **, void *);
void _barrier_cb(void *);

extern _codelet barrier_cl;
extern _codelet forktask_cl;
extern int64_t ILHigh, ILLow, ILStride;

typedef struct {
    int iters;
    _codelet_tag_t *tags;
} barrier_cb_t;

typedef struct {
  int64_t _1_ILIndex;
} codelet_param_t;

typedef struct {
  void (*forall_func)(void **, void *);  
  _codelet_tag_t barrier_id;
  _codelet_tag_t successor_id;
} _codelet_fork_param_t;

/* Create a parallel codelet. There is called for sections of code with 
 * parallel constructs such as (co)forall, begin, etc. */
#define chpl_codelet_parallel(self_id, incoming, outgoing, low, stride, high, index, numID, function, params, paramtype) \
  do {                                                                          \
    int ret;                                                                    \
    _codelet_task *task;                                                        \
    _codelet_fork_param_t *fparam;                                              \
    fparam = qio_malloc(sizeof(_codelet_fork_param_t));                         \
    fparam->forall_func = function;                                             \
    fparam->successor_id = (_codelet_tag_t)outgoing;                            \
    fparam->barrier_id = (_codelet_tag_t)self_id;                               \
    task = _codelet_create();                                                   \
    task->cl_arg = fparam;                                                      \
    task->cl_arg_size = sizeof(_codelet_fork_param_t);                          \
    task->use_tag = 1;                                                          \
    task->tag_id = CODELET_PRIVATE_ID(self_id);                                 \
    task->cl = &forktask_cl;                                                    \
    task->synchronous = 0;                                                      \
    task->regenerate = 1;                                                       \
    task->destroy = 0;                                                          \
    _codelet_declare_deps(task->tag_id, 1, (_codelet_tag_t)self_id);            \
    ret = _codelet_submit_task(task);                                           \
    if (ret == -ENODEV) printf("PROBLEM\n");                                    \
    STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");                       \
  } while (0)

/* Create a sequential codelet. There is called for sections of code with no 
 * parallel constructs
 * TODO : Deal with registering data and its associated handles!! */
#define chpl_codelet_sequential(self_id, incoming, outgoing, numID, function, params, paramsize) \
  do {                                                                          \
    int ret;                                                                    \
    _codelet_task *task;                                                        \
    static _codelet cl = { };                                                   \
    cl.cpu_funcs[0] = function; \
    cl.where = CHPL_CPU; \
    cl.nbuffers = 0;\
    task = _codelet_create();                                                   \
    task->cl = &cl;                                                    \
    task->cl_arg = params;                                                      \
    task->cl_arg_size = paramsize;      \
    task->use_tag = 1;                                                          \
    task->tag_id = CODELET_PRIVATE_ID(self_id);                                 \
    task->synchronous = 0;                                                      \
    task->regenerate = 1;                                                       \
    task->destroy = 0;                                                          \
    _codelet_declare_deps(task->tag_id, 1, (_codelet_tag_t)self_id);            \
    ret = _codelet_submit_task(task);                                           \
    if (ret == -ENODEV) printf("PROBLEM\n");                                    \
    STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");                       \
  } while (0)

#endif
