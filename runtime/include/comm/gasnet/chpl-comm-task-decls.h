#ifndef _COMM_TASK_DECLS_H_
#define _COMM_TASK_DECLS_H_

// The type of the communication handle.
typedef void* chpl_comm_nb_handle_t;

// The type of task private data.
//#include "chpl-cache-task-decls.h"

typedef struct {
  int dummy;
  // Dummy element so that compiler doesn't complain about an empty struct
  // Please remove when another element will be present.

//    chpl_cache_taskPrvData_t cache_data;
} chpl_comm_taskPrvData_t;

#define HAS_CHPL_CACHE_FNS

#endif
