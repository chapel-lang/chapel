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

#ifndef _threads_pthreads_h_
#define _threads_pthreads_h_

#include <pthread.h>
#include <stdint.h>

//
// This is the type used to communicate thread identifiers between the
// threading layer and its users.  Thread identifiers are 64-bit ints
// and should be negative to allow distinguishing them from task
// identifiers when the latter are positive and an ID could be either
// one or the other.
//
typedef int64_t chpl_thread_id_t;
#define chpl_thread_nullThreadId INT64_MIN

typedef pthread_mutex_t chpl_thread_mutex_t;

#include "chpl-thread-local-storage.h"

extern CHPL_TLS_DECL(chpl_thread_id_t,chpl_thread_id);
extern CHPL_TLS_DECL(void*, chpl_thread_data);

#ifdef CHPL_THREAD_GETPRIVATEDATA_IMPL_DECL
#error "CHPL_THREAD_GETPRIVATEDATA_IMPL_DECL is already defined!"
#else
#define CHPL_THREAD_GETPRIVATEDATA_IMPL_DECL 1
#endif
static inline
void* chpl_thread_getPrivateData(void) {
  return CHPL_TLS_GET(chpl_thread_data);
}

#ifdef CHPL_THREAD_SETPRIVATEDATA_IMPL_DECL
#error "CHPL_THREAD_SETPRIVATEDATA_IMPL_DECL is already defined!"
#else
#define CHPL_THREAD_SETPRIVATEDATA_IMPL_DECL 1
#endif
static inline
void chpl_thread_setPrivateData(void* p) {
  CHPL_TLS_SET(chpl_thread_data, p);
}

#endif // _threads_pthreads_h_
