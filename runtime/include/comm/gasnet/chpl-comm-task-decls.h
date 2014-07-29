#ifndef _COMM_TASK_DECLS_H_
#define _COMM_TASK_DECLS_H_

// The type of the communication handle.
typedef void* chpl_comm_nb_handle_t;

// The type of task private data.
#include "chpl-cache-task-decls.h"
#define HAS_CHPL_CACHE_FNS

typedef struct {
    chpl_cache_taskPrvData_t cache_data;
} chpl_comm_taskPrvData_t;

#endif
