/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _chpl_tasks_impl_h_
#define _chpl_tasks_impl_h_

#include <stdint.h>

#include "chpl-tasks-prvdata.h"
#include "chpl-threads.h"

#ifdef __cplusplus
extern "C" {
#endif


//
// Because we use the task tracking table for fifo tasking, this gives
// us the ability to register the main task in the task table and
// block report.
//
#define CHPL_TASK_STD_MODULES_INITIALIZED chpl_task_stdModulesInitialized

void chpl_task_stdModulesInitialized(void);

//
// The FIFO implementation of tasking is a least-common-denominator
// version designed for general usefulness, simplicity and limited
// demands on the threading layer.
//

//
// Type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
//
typedef uint64_t chpl_taskID_t;
#define chpl_nullTaskID 0
#ifndef CHPL_TASK_ID_STRING_MAX_LEN
#define CHPL_TASK_ID_STRING_MAX_LEN 21
#endif

//
// Task layer private area argument bundle header
//
typedef struct {
  chpl_bool is_executeOn;
  int lineno;
  int filename;
  c_sublocid_t requestedSubloc;  
  chpl_fn_int_t requested_fid;
  chpl_fn_p requested_fn;
  chpl_taskID_t id;
  chpl_task_ChapelData_t state;
} chpl_task_bundle_t;


//
// Condition variables
//
typedef pthread_cond_t chpl_thread_condvar_t;


//
// Sync variables
//
typedef struct {
  volatile chpl_bool  is_full;
  chpl_thread_mutex_t lock;
  chpl_thread_condvar_t signal_full;  // wait for full; signal this when full
  chpl_thread_condvar_t signal_empty; // wait for empty; signal this when empty
  //  threadlayer_sync_aux_t tl_aux;
} chpl_sync_aux_t;


//
// The fifo tasking layer doesn't really support sublocales.
//
// Putting these interface function definitions here and marking them
// for inlining makes them cost-free at execution time.
//
#ifdef CHPL_TASK_GETSUBLOC_IMPL_DECL
#error "CHPL_TASK_GETSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GETSUBLOC_IMPL_DECL 1
#endif
static inline
c_sublocid_t chpl_task_getSubloc(void) {
  return 0;
}


#ifdef CHPL_TASK_SETSUBLOC_IMPL_DECL
#error "CHPL_TASK_SETSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_SETSUBLOC_IMPL_DECL 1
#endif
static inline
void chpl_task_setSubloc(c_sublocid_t subloc) {
  // nothing to do
}


#ifdef CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL
#error "CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL is already defined!"
#else
#define CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL 1
#endif
static inline
c_sublocid_t chpl_task_getRequestedSubloc(void) {
  return c_sublocid_any;
}

#define CHPL_TASK_IMPL_CAN_MIGRATE_THREADS() 0

#ifdef __cplusplus
} // end extern "C"
#endif

#endif
