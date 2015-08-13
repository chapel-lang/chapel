/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _tasks_myth_h_
#define _tasks_myth_h_

#define CHPL_COMM_YIELD_TASK_WHILE_POLLING

#include "myth.h"

typedef struct{
        myth_felock_t lock;
        volatile int is_full;
}chpl_sync_aux_t;

// type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
typedef intptr_t chpl_taskID_t;
#define chpl_nullTaskID 0

#define CHPL_TASK_STD_MODULES_INITIALIZED chpl_task_stdModulesInitialized
void chpl_task_stdModulesInitialized(void);

#ifdef CHPL_TASK_TASKS_BOUND_TO_PTHREADS_IMPL_DECL
#error "CHPL_TASK_TASKS_BOUND_TO_PTHREADS_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_TASKS_BOUND_TO_PTHREADS_IMPL_DECL 1
#endif
static inline
int chpl_task_tasksBoundToPthreads(void) {
  return 0;    // not sure; return safe answer
}

#endif
