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
