/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _COMM_TASK_DECLS_H_
#define _COMM_TASK_DECLS_H_

// The type of task private data.
#include "chpl-cache-task-decls.h"
#define HAS_CHPL_CACHE_FNS

typedef struct {
    chpl_cache_taskPrvData_t cache_data;
} chpl_comm_taskPrvData_t;

//
// Comm layer private area within executeOn argument bundles
// (bundle.comm)
typedef struct {
  int caller;

  void* ack; // address on caller to post acknowledgement
} chpl_comm_bundleData_t;

// The type of the communication handle.
typedef void* chpl_comm_nb_handle_t;

#endif
