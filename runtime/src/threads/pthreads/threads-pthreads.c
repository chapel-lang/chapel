//
// Pthread implementation of Chapel threading interface
//

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chpl_mem.h"
#include "chplcast.h"
#include "chplrt.h"
#include "chpltasks.h"
#include "config.h"
#include "error.h"
#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct {
  void* (*fn)(void*);
  void* arg;
} thread_func_t;


//
// Types and variables for our list of threads.
//
typedef struct thread_list* thread_list_p;
struct thread_list {
  pthread_t     thread;
  thread_list_p next;
};

static pthread_mutex_t thread_list_lock;        // mutual exclusion lock
static thread_list_p   thread_list_head = NULL; // head of thread_list
static thread_list_p   thread_list_tail = NULL; // tail of thread_list


static pthread_attr_t thread_attributes;

static pthread_cond_t wakeup_cond = PTHREAD_COND_INITIALIZER;
static pthread_key_t  thread_private_key;

static void            threadlayer_condvar_init(threadlayer_condvar_t*);
static void*           initial_pthread_func(void*);
static void            destroy_thread_private_data(void*);
static void*           pthread_func(void*);
static void            pool_suspend_cancel_cleanup(void*);

static uint64_t threadCallStackSize = 0;


// Mutexes

void threadlayer_mutex_init(threadlayer_mutex_p mutex) {
  // WAW: how to explicitly specify blocking-type?
  if (pthread_mutex_init((pthread_mutex_t*) mutex, NULL))
    chpl_internal_error("pthread_mutex_init() failed");
}

threadlayer_mutex_p threadlayer_mutex_new(void) {
  threadlayer_mutex_p m;
  m = (threadlayer_mutex_p) chpl_alloc(sizeof(threadlayer_mutex_t),
                                       CHPL_RT_MD_MUTEX, 0, 0);
  threadlayer_mutex_init(m);
  return m;
}

void threadlayer_mutex_lock(threadlayer_mutex_p mutex) {
  if (pthread_mutex_lock((pthread_mutex_t*) mutex))
    chpl_internal_error("pthread_mutex_lock() failed");
}

void threadlayer_mutex_unlock(threadlayer_mutex_p mutex) {
  if (pthread_mutex_unlock((pthread_mutex_t*) mutex))
    chpl_internal_error("pthread_mutex_unlock() failed");
}


// Thread id

threadlayer_threadID_t threadlayer_thread_id(void) {
  return (threadlayer_threadID_t) pthread_self();
}


// Thread yield

void threadlayer_yield(void) {
  sched_yield();
}


// Condition variables

static void threadlayer_condvar_init(threadlayer_condvar_t* cv) {
  if (pthread_cond_init((pthread_cond_t*) cv, NULL))
    chpl_internal_error("pthread_cond_init() failed");
}


// Sync variable callbacks for the FIFO tasking layer

chpl_bool threadlayer_sync_suspend(chpl_sync_aux_t *s,
                                   struct timeval *deadline) {
  threadlayer_condvar_t* cond;
  cond = s->is_full ? &s->tl_aux.signal_empty : &s->tl_aux.signal_full;

  if (deadline == NULL) {
    (void) pthread_cond_wait(cond, (pthread_mutex_t*) &s->lock);
    return false;
  }
  else {
    struct timespec ts;
    ts.tv_sec  = deadline->tv_sec;
    ts.tv_nsec = deadline->tv_usec * 1000UL;
    return (pthread_cond_timedwait(cond, (pthread_mutex_t*) &s->lock, &ts)
            == ETIMEDOUT);
  }
}

void threadlayer_sync_awaken(chpl_sync_aux_t *s) {
  if (pthread_cond_signal(s->is_full ?
                          &s->tl_aux.signal_full : &s->tl_aux.signal_empty))
    chpl_internal_error("pthread_cond_signal() failed");
}

void threadlayer_sync_init(chpl_sync_aux_t *s) {
  threadlayer_condvar_init(&s->tl_aux.signal_full);
  threadlayer_condvar_init(&s->tl_aux.signal_empty);
}

void threadlayer_sync_destroy(chpl_sync_aux_t *s) { }


// Thread callbacks for the FIFO tasking layer

void threadlayer_init(uint64_t callStackSize) {
  //
  // If a value was specified for the call stack size config const, use
  // that (rounded up to a whole number of pages) to set the system and
  // pthread stack limits.
  //
  if (pthread_attr_init(&thread_attributes) != 0)
    chpl_internal_error("pthread_attr_init() failed");

  //
  // If a value was specified for the call stack size config const, use
  // that (rounded up to a whole number of pages) to set the system
  // stack limit.
  //
  if (callStackSize != 0) {
    uint64_t      pagesize = (uint64_t) sysconf(_SC_PAGESIZE);
    struct rlimit rlim;

    callStackSize = (callStackSize + pagesize - 1) & ~(pagesize - 1);

    if (getrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("getrlimit() failed");

    if (rlim.rlim_max != RLIM_INFINITY && callStackSize > rlim.rlim_max) {
      char warning[128];
      sprintf(warning, "callStackSize capped at %lu\n", 
              (unsigned long)rlim.rlim_max);
      chpl_warning(warning, 0, NULL);

      callStackSize = rlim.rlim_max;
    }

    rlim.rlim_cur = callStackSize;

    if (setrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("setrlimit() failed");

    if (pthread_attr_setstacksize(&thread_attributes, callStackSize) != 0)
      chpl_internal_error("pthread_attr_setstacksize() failed");
  }
  threadCallStackSize = callStackSize;

  if (pthread_key_create(&thread_private_key, destroy_thread_private_data))
    chpl_internal_error("pthread_key_create failed");

  pthread_mutex_init(&thread_list_lock, NULL);

  //
  // This is something of a hack, but it makes us a bit more resilient
  // if we're out of memory or near to it at shutdown time.  Launch,
  // cancel, and join with an initial pthread, forcing initialization
  // needed by any of those activities.  (In particular we have found
  // that cancellation needs to dlopen(3) a shared object, which fails
  // if we are out of memory.  Doing it now means that shared object is
  // already available when we need it later.)
  //
  {
    pthread_t initial_pthread;

    if (!pthread_create(&initial_pthread, NULL, initial_pthread_func, NULL)) {
      (void) pthread_cancel(initial_pthread);
      (void) pthread_join(initial_pthread, NULL);
    }
  }
}

//
// The initial pthread just waits to be canceled.  See the comment in
// threadlayer_init() for the purpose of this.
//
static void* initial_pthread_func(void* ignore) {
  while (1) {
    pthread_testcancel();
    sched_yield();
  }
  return NULL;
}

static void destroy_thread_private_data(void* p) {
  if (p)
    chpl_free(p, 0, 0);
}

void threadlayer_exit(void) {
  thread_list_p tlp;

  // shut down all threads
  for (tlp = thread_list_head; tlp != NULL; tlp = tlp->next) {
    if (pthread_cancel(tlp->thread) != 0)
      chpl_internal_error("thread cancel failed");
  }
  while (thread_list_head != NULL) {
    if (pthread_join(thread_list_head->thread, NULL) != 0)
      chpl_internal_error("thread join failed");
    tlp = thread_list_head;
    thread_list_head = thread_list_head->next;
    chpl_free(tlp, 0, 0);
  }

  if (pthread_key_delete(thread_private_key) != 0)
    chpl_internal_error("pthread_key_delete() failed");

  if (pthread_attr_destroy(&thread_attributes) != 0)
    chpl_internal_error("pthread_attr_destroy() failed");
}

int threadlayer_thread_create(threadlayer_threadID_t* thread,
                              void*(*fn)(void*), void* arg) {
  thread_func_t* f;
  pthread_t pthread;

  f = (thread_func_t*) chpl_alloc(sizeof(thread_func_t),
                                  CHPL_RT_MD_THREAD_CALLEE, 0, 0);
  f->fn  = fn;
  f->arg = arg;
  if (pthread_create(&pthread, &thread_attributes, pthread_func, f))
    return -1;
  if (thread != NULL)
    *thread = (threadlayer_threadID_t) pthread;
  return 0;
}

static void* pthread_func(void* void_f) {
  thread_func_t* f = (thread_func_t*) void_f;
  void*          (*fn)(void*) = f->fn;
  void*          arg = f->arg;
  thread_list_p  tlp;

  chpl_free(f, 0, 0);

  // disable cancellation immediately
  // enable only while waiting for new work
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); 

  // add us to the list of threads
  tlp = (thread_list_p) chpl_alloc(sizeof(struct thread_list),
                                   CHPL_RT_MD_THREAD_LIST_DESCRIPTOR, 0, 0);

  tlp->thread = pthread_self();
  tlp->next   = NULL;

  pthread_mutex_lock(&thread_list_lock);
  if (thread_list_head == NULL)
    thread_list_head = tlp;
  else
    thread_list_tail->next = tlp;
  thread_list_tail = tlp;
  pthread_mutex_unlock(&thread_list_lock);

  return (*(fn))(arg);
}

chpl_bool threadlayer_pool_suspend(threadlayer_mutex_p lock,
                                   struct timeval *deadline) {
  int last_cancel_state;
  chpl_bool res;

  // enable cancellation with cleanup handler before waiting for wakeup signal
  pthread_cleanup_push(pool_suspend_cancel_cleanup, lock);
  (void) pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
  assert(last_cancel_state == PTHREAD_CANCEL_DISABLE); // sanity check

  if (deadline == NULL) {
    (void) pthread_cond_wait(&wakeup_cond, (pthread_mutex_t*) lock);
    res = false;
  }
  else {
    struct timespec ts;
    ts.tv_sec  = deadline->tv_sec;
    ts.tv_nsec = deadline->tv_usec * 1000UL;
    res = (pthread_cond_timedwait(&wakeup_cond, (pthread_mutex_t*) lock, &ts)
           == ETIMEDOUT);
  }

  // disable cancellation again
  (void) pthread_setcancelstate(last_cancel_state, NULL);
  pthread_cleanup_pop(0);

  return res;
}

static void pool_suspend_cancel_cleanup(void* void_lock) {
  threadlayer_mutex_unlock((threadlayer_mutex_p) void_lock);
}

void threadlayer_pool_awaken(void) {
  if (pthread_cond_signal(&wakeup_cond))
    chpl_internal_error("pthread_cond_signal() failed");
}

void* threadlayer_get_thread_private_data(void) {
  return pthread_getspecific(thread_private_key);
}

void threadlayer_set_thread_private_data(void* p) {
  if (pthread_setspecific(thread_private_key, p))
    chpl_internal_error("thread private data key doesn't work");
}

uint64_t threadlayer_call_stack_size(void) {
  return threadCallStackSize;
}
