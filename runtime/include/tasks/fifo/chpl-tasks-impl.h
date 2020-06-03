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

//
// Fifo tasking layer type declarations.
//

#ifndef _chpl_tasks_impl_h_
#define _chpl_tasks_impl_h_

#include <pthread.h>
#include <stdint.h>

#include "chpl-threads.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
} // end extern "C"
#endif

#endif
