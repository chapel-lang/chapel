#include <chplrt.h>
#include <math.h>
#include <qbuffer.h>
#include "codelet.h"

/* 
 * These are the runtime related files that will be used to call into the 
 * codelet base runtime 
 *
 */

/* initialize codelet runtime : passing a NULL argument means that we use
 * default configuration for the scheduling policies and the number of
 * processors/accelerators */
//int chpl_codelet_init(_codelet_config *conf) {
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
void chpl_codelet_waitforall(void) {
  _codelet_waitforall();
}

// putting this into macro form
/* Create a sequential codelet. There is called for sections of code with no 
 * parallel constructs */
/* TODO : Deal with registering data and its associated handles!! */
int chpl_codelet_sequential(_codelet_tag_t self_id, _codelet_tag_t incoming, 
                            _codelet_tag_t outgoing, int32_t numID, void (*function)(void **, void *), 
                            void *params, size_t paramsize) {

  _codelet cl = { };
  _codelet_task *task;
  
  int ret;
  int i;

  cl.cpu_funcs[0] = function;
  cl.where = CHPL_CPU;
  cl.nbuffers = 0;

  /* Create Codelet Task */
  task = _codelet_create();
  task->use_tag = 1;
  task->tag_id = self_id;
  task->synchronous = 1;
  task->cl = &cl;
  task->cl_arg = params;
  task->cl_arg_size = paramsize;

  /* extract dependencies from bitmap */
  for (i = 0; i < 64; i++) {
    if (0x1 & incoming) {
      _codelet_declare_deps(self_id, 1, 1 << i);
      printf("SEQ: just declared deps on %llu\n", (_codelet_tag_t)(1<<i));
    }
    incoming >>= 1;
  }

  printf("SEQUENTIAL: self = %llu outgoing = %llu incoming = %llu paramsize = %lu\n", self_id, outgoing, incoming, paramsize);

//  for (int i = 0; i < numHandles; i++) {
//    task->handles[i] = data_handles[i];
//  }

  ret = _codelet_submit_task(task);
  if (ret == -ENODEV) {
    printf("Could not submit sequential task!!");
  }
  STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");
  return 0;
}

#if 0
/* Create a parallel codelet. There is called for sections of code with 
 * parallel constructs such as (co)forall, begin, etc. */
int chpl_codelet_parallel(_codelet_tag_t self_id, _codelet_tag_t incoming, 
    _codelet_tag_t outgoing, int low, int stride, int high, int index,
    int32_t numID, void (*function)(void **, void *), void *params, 
    size_t paramsize) {


  int outeridx, ret, i, numInds;
  _codelet_task *task;
  _codelet_tag_t incopy;
  _codelet_tag_t *loopInds;

  _codelet cl = { };

  printf("parallel: low = %d stride = %d high = %d\n", low, stride, high);

  numInds = ceil((high - low + 1) / stride);
  loopInds = (_codelet_tag_t *)qio_malloc(numInds * sizeof(_codelet_tag_t));

  printf("numInds = %d\n", numInds);

  cl.cpu_funcs[0] = function;
  cl.where = CHPL_CPU;
  cl.nbuffers = 0;

  for (outeridx = low; outeridx <= high; outeridx += stride) {
    
    /* Create Codelet Task */
    task = _codelet_create();

    task->use_tag = 1;
    task->tag_id = (_codelet_tag_t)(self_id | (_codelet_tag_t)(outeridx << 16));
    loopInds[outeridx] = task->tag_id;
    task->synchronous = 0;
    task->cl = &cl;
    task->cl_arg = params;
    task->cl_arg_size = paramsize;

    incopy = incoming;
    /* extract dependencies from bitmap */
    for (i = 0; i < 64; i++) {
      if (0x1 & incopy) {
        _codelet_declare_deps(task->tag_id, 1, (_codelet_tag_t)(1 << i));
        printf("PAR: %llu just declared deps on %llu\n", (_codelet_tag_t)(task->tag_id), (_codelet_tag_t)(1<<i));
      }
      incopy >>= 1;
    }

    printf("PARALLEL: self = %llu outgoing = %llu incoming = %llu paramsize = %lu\n", self_id, outgoing, incoming, paramsize);

    //  for (int i = 0; i < numHandles; i++) {
    //    task->handles[i] = data_handles[i];
    //  }

    ret = _codelet_submit_task(task);
    if (ret == -ENODEV) {
      printf("Could not submit sequential task!!");
    }
    STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

  }

  /* Add barrier here */
  starpu_tag_wait_array(numInds, loopInds);
  //starpu_tag_remove(self_id);
  starpu_tag_notify_from_apps(self_id);

  return 0;
}
#endif
