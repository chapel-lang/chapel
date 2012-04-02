#include <chplrt.h>
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

/*
 * Compiler generated routine to describe the sequential codelet
 * TASK_ID = Integer ID of the codelet task
 * 
 */
#if 0
#define chpl_codelet_sequential(TASK_ID, IN, OUT, FUNC)     \
  do {                                                          \
    /* Create Codelet Task */                                   \
    _codelet_task task = starpu_task_create();                  \
    task->use_tag = 1;                                          \
    task->tag_id = TASK_ID;                                     \
    /* Initialize Codelet Dependence */                         \

  }while(0)
#endif

void cpu_func(void **, void *);
void callback_func(void *);

void cpu_func(void **buffer, void *cl) {
  printf("hello world from my fake shit\n");
}

void callback_func(void *foo) {
  printf("callback?\n");
}

/* Create a sequential codelet. There is called for sections of code with no 
 * parallel constructs */
/* TODO : Deal with registering data and its associated handles!! */
int chpl_codelet_sequential(_codelet_tag_t self_id, _codelet_tag_t incoming, 
                            _codelet_tag_t outgoing, int32_t numID, void (*function)(void **, void *), 
                            void *params, size_t paramsize) {

  _codelet cl = { };
  _codelet_task *task;
  
  int ret;
  int i, j;

  //int fakeparams = 1;
  //cl.cpu_funcs[0] =(void *)function;
  cl.cpu_funcs[0] = function;
  //cl.cpu_funcs[0] = cpu_func;
  //cl.where = CHPL_CPU;
  cl.where = STARPU_CPU;
  cl.nbuffers = 0;

    /* Create Codelet Task */
  //task = _codelet_create();
  task = starpu_task_create();

  //task->use_tag = 1;
  //task->tag_id = self_id;
  task->synchronous = 1;
  task->cl = &cl;
  task->cl_arg = params;
  task->cl_arg_size = paramsize;

  //task->callback_func = callback_func;
  //task->callback_arg = (void*) (uintptr_t) 0x42;
  //task->cl_arg = NULL;
  //task->cl_arg_size = sizeof(params);

  /* extract dependencies from bitmap */
  for (i = 0; i < 64; i++) {
    if (0x1 & incoming) {
      _codelet_declare_deps(self_id, 1, 1 << j);
      printf("just declared deps on %llu", (uint64_t)(1<<j));
    }
    incoming >>= 1;
  }

  printf("Self = %llu\n", self_id);
  printf("Outgoing = %llu\n", outgoing);
  printf("incoming = %llu\n", incoming);
  
  printf("param size = %lu\n", paramsize);

//  for (int i = 0; i < numHandles; i++) {
//    task->handles[i] = data_handles[i];
//  }

  ret = _codelet_submit_task(task);
  if (ret == -ENODEV) {
    printf("Could not submit sequential task!!");
  }
  STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit");

  starpu_shutdown();
  return 0;
}

#if 1
/* Create a parallel codelet. There is called for sections of code with 
 * parallel constructs such as (co)forall, begin, etc. */
int chpl_codelet_parallel(_codelet_tag_t self_id, _codelet_tag_t incoming, 
                            _codelet_tag_t outgoing, int index, int low, int stride, int high, int32_t numID, void (*function)(void *), void *params) {
                            //_codelet_tag_t outgoing, int32_t numID, void *params, void *function) {


#if 0
  struct starpu_codelet cl;
  cl.cpu_funcs[0] = func;
  cl.where = CHPL_CPU;
  cl.nbuffers = 0;

  /* Create Codelet Task */
  _codelet_task task = starpu_task_create();
  task->use_tag = 1;
  task->tag_id = self_id;
  task->synchronous = 0;
  task->cl = &cl;
  task->cl_arg = params;
  task->cl_arg_size = sizeof(params);
  for (int i = 0; i < numHandles; i++) {
    task->handles[i] = data_handles[i];
  }

  int ret = codelet_submit_task(task);
  if (ret == -ENODEV) {
    perror("Could not submit sequential task!!");
  }
#endif

  return 0;
}
#endif
