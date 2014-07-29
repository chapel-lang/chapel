#ifndef _chpl_cache_task_decls_h_
#define _chpl_cache_task_decls_h_

// This is the type of the task private data used by the cache
typedef struct {
  int64_t last_acquire; // cache acquire barrier sets this
} chpl_cache_taskPrvData_t;

#endif

