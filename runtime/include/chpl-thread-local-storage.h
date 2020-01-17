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

#ifndef _chpl_thread_local_storage_h_
#define _chpl_thread_local_storage_h_

#include <pthread.h>

#ifdef __GNUC__
// For GCC and CLANG, where we know there is __thread, use it instead
// of pthread_get/setspecific since __thread appears to be 2x faster.
#ifndef __APPLE__ // But at least in Darwin, __thread is not supported.
#ifndef __CYGWIN__ // Nor cygwin
#define CHPL_TLS __thread
#endif
#endif
#elif __STDC_VERSION >= 201112L && !defined __STDC_NO_THREADS__
// For the C11 standard, _Thread_local is the name of it
#define CHPL_TLS _Thread_local
#elif defined(_MSC_VER)
#define CHPL_TLS __declspec(thread)
#endif

// Inspired by qthreads and taken with permission
#ifdef CHPL_TLS
# define CHPL_TLS_DECL(type, name)      CHPL_TLS type name
# define CHPL_TLS_DECL_INIT(type, name) CHPL_TLS type name = 0
# define CHPL_TLS_GET(name)             name
# define CHPL_TLS_SET(name, val)        name = (val)
# define CHPL_TLS_INIT(name)
# define CHPL_TLS_INIT2(name, func)
# define CHPL_TLS_DELETE(name)          name = 0
#else

extern void chpl_internal_error(const char* message);
static inline void chpl_tlschk(int got, int expect)
{
  if( got != expect ) {
    chpl_internal_error("failure in thread private data");
  }
}

# define CHPL_TLS_DECL(type, name)      pthread_key_t name
# define CHPL_TLS_DECL_INIT(type, name) pthread_key_t name
# define CHPL_TLS_GET(name)             pthread_getspecific(name)
# define CHPL_TLS_SET(name, val)        chpl_tlschk(pthread_setspecific((name), (void *)(val)), 0)
# define CHPL_TLS_INIT(name)            chpl_tlschk(pthread_key_create(&(name), NULL), 0)
# define CHPL_TLS_INIT2(name, func)     chpl_tlschk(pthread_key_create(&(name), (func)), 0)
# define CHPL_TLS_DELETE(name)          chpl_tlschk(pthread_key_delete(name), 0)
#endif // ifdef CHPL_TLS

#endif

