#include <chplrt.h>
#include <math.h>
#include <qbuffer.h>
#include "codelet.h"

//#define CODELET_DEBUG

/* 
 * These are the runtime related files that will be used to call into the 
 * codelet base runtime 
 *
 */

/* initialize codelet runtime : passing a NULL argument means that we use
 * default configuration for the scheduling policies and the number of
 * processors/accelerators */
int chpl_codelet_init(void) {
  printf("INITIALIZING CODELET R/T...\n");
  return _codelet_init(NULL);
}

/* shutdown codelet runtime */
void chpl_codelet_shutdown(void) {
  printf("SHUTTING DOWN CODELET R/T...\n");
  _codelet_shutdown();
}

/* shutdown codelet runtime */
void chpl_codelet_wait_for_id(_codelet_tag_t id) {
  _codelet_wait_for_id(id);
}

void chpl_codelet_signal(_codelet_tag_t id) {
  _codelet_tag_restart(id);
  _codelet_signal(id);
}

void inner_barrier(void **buffers, void *cl_arg) { 
  /* Send a signal to waiting codelet that the barrier is complete */
  chpl_codelet_signal(*((_codelet_tag_t *)cl_arg));
}

void _fork_tasks(void **buffers, void *cl_arg) { 
  _codelet_task *barrier_task;
  _codelet_fork_param_t *fparam;
  codelet_param_t **myparam;
  barrier_cb_t *barrier_data;
  _codelet_tag_t *loopInds;
  static _codelet cl;
  int ret;
  unsigned outeridx, numInds;
  numInds = ceil((ILHigh - ILLow + 1) / ILStride);
  loopInds = (_codelet_tag_t *)qio_malloc(numInds * sizeof(_codelet_tag_t));
  fparam = cl_arg;
  cl.cpu_funcs[0] = fparam->forall_func;
  cl.where = CHPL_CPU;
  cl.nbuffers = 0;
  myparam = qio_malloc(sizeof(codelet_param_t *) * numInds);
  for (outeridx = ILLow; outeridx <= ILHigh; outeridx += ILStride) {
    _codelet_task *task = _codelet_create();
    myparam[outeridx] = qio_malloc(sizeof(codelet_param_t));
    myparam[outeridx]->_1_ILIndex = outeridx;
    task->use_tag = 1;
    task->tag_id = CODELET_INNER_LOOP_ID(0, outeridx);
    task->synchronous = 0;
    task->cl = &cl;
    task->regenerate = 0;
    task->cl_arg = myparam[outeridx];
    task->cl_arg_size = sizeof(codelet_param_t);
    loopInds[outeridx] = task->tag_id;
    ret = _codelet_submit_task(task);
    if (ret == -ENODEV) {
      printf("Could not submit sequential task!!");
    }
    STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit"); 
  }
  barrier_data = (barrier_cb_t *)qio_malloc(sizeof(barrier_cb_t));
  barrier_data->iters = numInds;
  barrier_data->tags = loopInds;
  barrier_task = _codelet_create();
  barrier_task->cl = &barrier_cl;
  barrier_task->cl_arg = &(fparam->successor_id);
  barrier_task->synchronous = 0;
  barrier_task->use_tag = 1;
  barrier_task->tag_id = CODELET_BARRIER_ID(fparam->barrier_id);
  barrier_task->regenerate = 0;
  barrier_task->callback_func = _barrier_cb;
  barrier_task->callback_arg = barrier_data;
  _codelet_declare_deps_array(barrier_task->tag_id, numInds, loopInds);
  ret = _codelet_submit_task(barrier_task);
  if (ret == -ENODEV) printf("PROBLEM\n");
  STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
}

_codelet forktask_cl = {
  .where = CHPL_CPU,
  .cpu_funcs = { _fork_tasks , 0 },
  .nbuffers = 0
};

_codelet barrier_cl = {
  .where = STARPU_CPU,
  .cpu_funcs = { inner_barrier, 0 },
  .nbuffers = 0
};

void _barrier_cb(void *id) {
  int i;
  barrier_cb_t *input = (barrier_cb_t *)id;

  int numIters = input->iters;
  _codelet_tag_t *tags = input->tags;

  for (i = 0; i < numIters; i++) {
    _codelet_tag_restart(tags[i]);
  }
}
