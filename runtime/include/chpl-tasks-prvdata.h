#ifndef _chpl_tasks_prvdata_h_
#define _chpl_tasks_prvdata_h_

// This header file provides chpl_comm_taskPrvData_t
#include "chpl-comm-task-decls.h"

// The type for task private data
typedef struct {
  chpl_bool serial_state;      // true: serialize execution
  chpl_comm_taskPrvData_t comm_data;
} chpl_task_prvData_t;

#endif

