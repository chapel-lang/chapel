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

#ifdef __GNUC__
// For GCC and CLANG, where we know there is __thread, use it instead
// of pthread_get/setspecific since __thread appears to be 2x faster.
#ifndef __APPLE__ // But at least in Darwin, __thread is not supported.
#define CHPL_TLS __thread
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

extern CHPL_TLS_DECL(chpl_thread_id_t,chpl_thread_id);
extern CHPL_TLS_DECL(void*, chpl_thread_data);

static inline
void* chpl_thread_getPrivateData(void) {
  return CHPL_TLS_GET(chpl_thread_data);
}

static inline
void chpl_thread_setPrivateData(void* p) {
  CHPL_TLS_SET(chpl_thread_data, p);
}

#endif // _threads_pthreads_h_
