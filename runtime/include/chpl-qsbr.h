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

#include <stdint.h>
#include <stdbool.h>

#ifndef _chpl_qsbr_h_
#define _chpl_qsbr_h_

/**
* A Quiescent State-Based Reclamation style of Read-Copy-Update and
* pseudo garbage-collector. We generalize and coarsen the granularity
* of scope by keeping a single global epoch describing the state of 
* the entire system. Updates to protected data can be 'broadcast' to the system,
* incrementing the global epoch and moving the system's state forward.
* Threads must periodically pass through checkpoints which indicate that they
* are not currently accessing protected data by updating their thread-local epochs
* to the current global epoch. Once all threads have passed some epoch, it is
* safe for the updater to delete the older version of data. Multiple writers are
* allowed.
*/

void chpl_qsbr_init(void);

// Invoked periodically by tasks to indicate that it is no
// longer using QSBR-Protected data. Note that this must never
// be called while accessing the data itself. This function
// will update TLS data. 
void chpl_qsbr_checkpoint(void);

// Broadcasts a global state change and deletes 'args' if all threads pass a
// checkpoint. In the case that the user passes a stack-allocated array for 'args'
// whether or not it gets deleted is optional. If a thread does not pass the checkpoint
// the task of deletion is deferred to that thread.
void chpl_qsbr_defer_deletion(void **data, int numData, bool deleteData);

// Keeps track of number of tasks that a thread has. Necessary so that tasks do not wait on
// threads without any tasks, as threads without tasks will never actually execute deferred deletion.
void chpl_qsbr_onTaskCreation(void); 
void chpl_qsbr_onTaskDestruction(void); 

void chpl_qsbr_exit(void);

#endif // _chpl_qsbr_h_
