#ifndef _CODELET_H_
#define _CODELET_H_

#include "codelet_starpu.h"

int chpl_codelet_init(void);
void chpl_codelet_shutdown(void);
void chpl_codelet_waitforall(void);

#define chpl_codelet_parallel(self_id, incoming, outgoing, low, stride, high, index, numID, function, params, paramtype) \
  { \
    int outeridx, ret, i, numInds; \
    _codelet_task *task; \
    _codelet_tag_t incopy; \
    _codelet_tag_t *loopInds; \
    paramtype **myparam; \
    _codelet cl = { }; \
    numInds = ceil((high - low + 1) / stride);\
    loopInds = (_codelet_tag_t *)qio_malloc(numInds * sizeof(_codelet_tag_t));\
    cl.cpu_funcs[0] = function;\
    cl.where = CHPL_CPU;\
    cl.nbuffers = 0;\
    myparam = qio_malloc(sizeof(paramtype *) * numInds); \
    for (outeridx = low; outeridx <= high; outeridx += stride) { \
      myparam[outeridx] = qio_malloc(sizeof(paramtype)); \
      myparam[outeridx]->_1_ILIndex = outeridx; \
      /*printf("myparam = %p index = %d\n", &myparam, outeridx);*/ \
      /*params->_1_ILIndex = outeridx;*/  \
      task = _codelet_create(); \
      task->use_tag = 1;\
      task->tag_id = (_codelet_tag_t)(self_id | (_codelet_tag_t)(outeridx << 16)); \
      loopInds[outeridx] = task->tag_id; \
      task->synchronous = 0;    \
      task->cl = &cl;   \
      /*task->cl_arg = params;*/    \
      task->cl_arg = myparam[outeridx];    \
      /*task->cl_arg_size = sizeof(params);*/ \
      task->cl_arg_size = sizeof(paramtype);\
      incopy = incoming;        \
      /* extract dependencies from bitmap */ \
      for (i = 0; i < 64; i++) { \
        if (0x1 & incopy) { \
          _codelet_declare_deps(task->tag_id, 1, (_codelet_tag_t)(1 << i)); \
          /*printf("PAR: %llu just declared deps on %llu\n", (_codelet_tag_t)(task->tag_id), (_codelet_tag_t)(1<<i));*/  \
        } \
        incopy >>= 1; \
      } \
      /*printf("PARALLEL: self = %llu outgoing = %llu incoming = %llu paramsize = %llu\n", (_codelet_tag_t)self_id, (_codelet_tag_t)outgoing, (_codelet_tag_t)incoming, (_codelet_tag_t)sizeof(params)); */ \
      ret = _codelet_submit_task(task); \
      if (ret == -ENODEV) { \
        printf("Could not submit sequential task!!"); \
      } \
      STARPU_CHECK_RETURN_VALUE(ret, "starpu_task_submit"); \
    } \
    _codelet_wait_all_array(numInds, loopInds); \
    _codelet_resolve_dep(self_id); \
  }


int chpl_codelet_sequential(_codelet_tag_t, _codelet_tag_t, _codelet_tag_t, int32_t, void (* function)(void **, void *), void *, size_t size);
//int chpl_codelet_parallel(_codelet_tag_t, _codelet_tag_t, _codelet_tag_t, int, int, int, int, int32_t, void (* function)(void **, void *), void *, size_t size);

#endif
