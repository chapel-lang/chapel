/**************************************************************************
*  Copyright 2011 Sandia Corporation. Under the terms of Contract
*  DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
*  or on behalf of the U.S. Government. Export of this program may require a
*  license from the United States Government
**************************************************************************/

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
// Qthreads tasking layer type declarations.
//

#ifndef _chpl_tasks_impl_h_
#define _chpl_tasks_impl_h_

#include "chpltypes.h"

#include "qthread.h"
#include "qthread-chapel.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CHPL_COMM_YIELD_TASK_WHILE_POLLING
void chpl_task_yield(void);

//
// Type (and default value) used to communicate task identifiers
// between C code and Chapel code in the runtime.
//
typedef unsigned int chpl_taskID_t;
#define chpl_nullTaskID QTHREAD_NULL_TASK_ID
#ifndef CHPL_TASK_ID_STRING_MAX_LEN
#define CHPL_TASK_ID_STRING_MAX_LEN 21
#endif

//
// Sync variables
//
typedef struct {
    aligned_t lock;
    int       is_full;
    aligned_t signal_full;
    aligned_t signal_empty;
} chpl_sync_aux_t;

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // ifndef _chpl_tasks_impl_h_
/* vim:set expandtab: */
