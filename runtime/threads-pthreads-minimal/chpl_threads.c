//
// Minimal pthread implementation of Chapel threading interface
//

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chapel_code.h"
#include "chpl_mem.h"
#include "chplrt.h"
#include "chplthreads.h"
#include "error.h"
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <inttypes.h>
#include <sys/time.h>


typedef struct {
  void* (*fn)(void*);
  void* arg;
} thread_func_t;


static pthread_key_t thread_private_key;

static void  destroy_thread_private_data(void*);
static void* pthread_func(void*);


// Mutex

void chpl_mutex_init(chpl_mutex_p mutex) {
  if (pthread_mutex_init(mutex, NULL))
    chpl_internal_error("pthread_mutex_init() failed");
}

chpl_mutex_p chpl_mutex_new(void) {
  chpl_mutex_p m;
  m = (chpl_mutex_p) chpl_alloc(sizeof(chpl_mutex_t), CHPL_RT_MD_MUTEX, 0, 0);
  chpl_mutex_init(m);
  return m;
}

void chpl_mutex_lock(chpl_mutex_p mutex) {
  if (pthread_mutex_lock(mutex))
    chpl_internal_error("pthread_mutex_lock() failed");
}

void chpl_mutex_unlock(chpl_mutex_p mutex) {
  if (pthread_mutex_unlock(mutex))
    chpl_internal_error("pthread_mutex_unlock() failed");
}


// Thread id

chpl_threadID_t chpl_thread_id(void) {
  return (chpl_threadID_t) pthread_self();
}


// Thread cancellation

void chpl_thread_cancel(chpl_threadID_t thread) {
  if (0 != pthread_cancel((pthread_t) thread)) {
    chpl_internal_error("thread cancel failed");
  }
}    


// Thread join (wait for completion)

void chpl_thread_join(chpl_threadID_t thread) {
  if (0 != pthread_join((pthread_t) thread, NULL)) {
    chpl_internal_error("thread join failed");
  }
}    


// Sync variable callbacks for the generic threading implementation

chpl_bool threadlayer_sync_suspend(chpl_sync_aux_t *s,
                                   struct timeval *deadline) {
  chpl_mutex_unlock(s->lock);
  sched_yield();
  chpl_mutex_lock(s->lock);
  return false;
}

void threadlayer_sync_awaken(chpl_sync_aux_t *s) { }

void threadlayer_init_sync(chpl_sync_aux_t *s) { }

void threadlayer_destroy_sync(chpl_sync_aux_t *s) { }


// Single variable callbacks for the generic threading implementation

chpl_bool threadlayer_single_suspend(chpl_single_aux_t *s,
                                     struct timeval *deadline) {
  chpl_mutex_unlock(s->lock);
  sched_yield();
  chpl_mutex_lock(s->lock);
  return false;
}

void threadlayer_single_awaken(chpl_single_aux_t *s) { }

void threadlayer_init_single(chpl_single_aux_t *s) { }

void threadlayer_destroy_single(chpl_single_aux_t *s) { }


// Thread callbacks for the generic threading implementation

void threadlayer_init(void) {
  if (pthread_key_create(&thread_private_key, destroy_thread_private_data))
    chpl_internal_error("pthread_key_create failed");
}

static void destroy_thread_private_data(void* p) {
  if (p)
    chpl_free(p, 0, 0);
}

void threadlayer_exit(void) {
  (void) pthread_key_delete(thread_private_key);
}

int threadlayer_thread_create(void* thread,
                              void*(*fn)(void*), void* arg) {
  thread_func_t* f;
  pthread_t pthread;

  f = (thread_func_t*) chpl_alloc(sizeof(thread_func_t),
                                  CHPL_RT_MD_THREAD_CALLEE, 0, 0);
  f->fn  = fn;
  f->arg = arg;
  if (pthread_create(&pthread, NULL, pthread_func, f))
    return -1;
  if (thread != NULL)
    *(chpl_threadID_t*) thread = (chpl_threadID_t) (intptr_t) pthread;
  return 0;
}

static void* pthread_func(void* void_f) {
  thread_func_t* f = (thread_func_t*) void_f;
  void* (*fn)(void*) = f->fn;
  void* arg = f->arg;

  chpl_free(f, 0, 0);

  // disable cancellation immediately
  // enable only while waiting for new work
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); 

  return (*(fn))(arg);
}

chpl_bool threadlayer_pool_suspend(chpl_mutex_t *lock,
                                   struct timeval *deadline) {
  int last_cancel_state;

  chpl_mutex_unlock(lock);
  sched_yield();

  // enable cancellation, check to see if we've been cancelled, and then
  // disable cancellation again
  (void) pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
  assert(last_cancel_state == PTHREAD_CANCEL_DISABLE); // sanity check
  pthread_testcancel();
  (void) pthread_setcancelstate(last_cancel_state, NULL);

  chpl_mutex_lock(lock);

  return false;
}

void threadlayer_pool_awaken(void) { }

void* threadlayer_get_thread_private_data(void) {
  return pthread_getspecific(thread_private_key);
}

void threadlayer_set_thread_private_data(void* p) {
  if (pthread_setspecific(thread_private_key, p))
    chpl_internal_error("thread private data key doesn't work");
}
