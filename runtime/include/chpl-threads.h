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

#ifndef _chpl_threads_h_
#define _chpl_threads_h_

#include <stdint.h>

#include "chpl-threads-impl.h"

//
// Threads are the execution vehicles for tasks.  This file declares the
// interface by which the tasking layer obtains thread services.
//

//
// Initialize the threading layer.
//
void chpl_thread_init(void(*)(void*), void(*)(void));

//
// Create a thread for a communication task to run function 'fn' with
// argument 'arg'.  This thread should be quite dedicated (e.g., get
// its own system thread) in order to be responsive and not be held up
// by other user-level tasks. returns 0 on success, nonzero on
// failure.

//
int chpl_thread_createCommThread(chpl_fn_p fn, void* arg);

//
// Shut down the threading layer.
//
void chpl_thread_exit(void);

//
// Can the thread layer create another thread?
//
chpl_bool chpl_thread_canCreate(void);

//
// Create a new thread.
//
int chpl_thread_create(void*);

//
// Destroy the calling thread.  The threading layer is allowed to return
// without destroying the thread, so this function is really an advisory
// one, to say that the thread is no longer needed.  Also, the threading
// layer is actually prohibited from destroying the thread if doing so
// would leave no threads running on the processor.
//
void chpl_thread_destroy(void);

//
// Get the calling thread's unique identifier.
//
chpl_thread_id_t chpl_thread_getId(void);

//
// Yield the processor, so that some other thread can run on it.
//
void chpl_thread_yield(void);

//
// Thread private data
//
// These set and get a pointer to thread private data associated with
// each thread.  This is for the use of the tasking layer itself.  If
// the threading layer also needs to store some data private to each
// thread, it must make other arrangements to do so.
//

#ifndef CHPL_THREAD_SETPRIVATEDATA_IMPL_DECL
void  chpl_thread_setPrivateData(void*);
#endif

#ifndef CHPL_THREAD_GETPRIVATEDATA_IMPL_DECL
void* chpl_thread_getPrivateData(void);
#endif

//
// Get the maximum number of threads that can exist.
//
uint32_t chpl_thread_getMaxThreads(void);

//
// Get the number of threads currently in existence.
//
uint32_t chpl_thread_getNumThreads(void);

//
// Get the current thread stack size.
//
size_t chpl_thread_getCallStackSize(void);

//
// Mutexes, and operations upon them.
//
typedef chpl_thread_mutex_t* chpl_thread_mutex_p;

void chpl_thread_mutexInit(chpl_thread_mutex_p);
chpl_thread_mutex_p chpl_thread_mutexNew(void);
void chpl_thread_mutexLock(chpl_thread_mutex_p);
void chpl_thread_mutexUnlock(chpl_thread_mutex_p);
void chpl_thread_mutexDestroy(chpl_thread_mutex_p);

#endif
