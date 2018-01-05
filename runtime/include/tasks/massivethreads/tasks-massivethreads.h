/*
 * Copyright 2004-2018 Cray Inc.
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

#include "chpl-threads.h"

#define MYTH_IS_PTHREAD 0

#if MYTH_IS_PTHREAD
typedef struct myth_felock {
  pthread_mutex_t mutex[1];
  pthread_cond_t cond[2];
  int status;
} myth_felock_t;
#else
#include "myth/myth.h"
#endif

typedef struct {
  myth_felock_t felock[1];
  volatile int is_full;
} chpl_sync_aux_t;

// type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
typedef intptr_t chpl_taskID_t;
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
  //chpl_task_prvData_t prv;
} chpl_task_bundle_t;

#endif
