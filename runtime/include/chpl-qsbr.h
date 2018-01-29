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

// Invoked periodically by tasks to indicate that it is no
// longer using QSBR-Protected data. Note that this must never
// be called while accessing the data itself. This function
// will update TLS data. 
void chpl_qsbr_checkpoint();

// Broadcasts a state change, returning the previous epoch that can be waited on.
// Note that this can be used to allow non-blocking deferred deletion.
uint64_t chpl_qsbr_broadcast();

// Wait for all threads with unblocked tasks to pass through a checkpoint.
void chpl_qsbr_wait(uint64_t epoch);

// Informs that a task is about to block and will be unable to pass checkpoints.
// A 'blocked' task will not be waited on. Note that this can be called if you know
// that a task will not use the QSBR-Protected data for a prolonged duration of time. 
void chpl_qsbr_blocked();

// Informs that a task is no longer blocked and should be accounted for.
void chpl_qsbr_unblocked();

#endif // _chpl_qsbr_h_