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

#ifndef _chpl_tasks_prvdata_h_
#define _chpl_tasks_prvdata_h_

// This header file provides chpl_comm_taskPrvData_t
#include "chpl-comm-task-decls.h"

// Type for Chapel-managed task private data
// to be copied to new tasks.
// Includes 'serial_state'
typedef struct {
  unsigned char data[32];
} chpl_task_ChapelData_t;

// The type for runtime-managed task private data
typedef struct {
  chpl_comm_taskPrvData_t comm_data;
} chpl_task_prvData_t;

#endif

