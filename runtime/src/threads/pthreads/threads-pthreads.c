//
// Pthread implementation of Chapel threading interface
//

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplcast.h"
#include "chpl-tasks.h"
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

static chpl_bool       exiting = false;         // are we shutting down?

static pthread_mutex_t thread_info_lock;        // mutual exclusion lock

static int64_t         curr_thread_id = 0;

static thread_list_p   thread_list_head = NULL; // head of thread_list
static thread_list_p   thread_list_tail = NULL; // tail of thread_list

static pthread_attr_t  thread_attributes;

static pthread_key_t   thread_id_key;
static pthread_cond_t  wakeup_cond = PTHREAD_COND_INITIALIZER;
static pthread_key_t   thread_private_key;

static int32_t         maxThreads = 0;
static uint32_t        numThreads = 0;
static pthread_mutex_t numThreadsLock;

static size_t          threadCallStackSize = 0;

static void            (*saved_threadBeginFn)(void*);
static void            (*saved_threadEndFn)(void);

static void            threadlayer_condvar_init(threadlayer_condvar_t*);
static void*           initial_pthread_func(void*);
static void*           pthread_func(void*);
static void            pool_suspend_cancel_cleanup(void*);


// Mutexes

void chpl_thread_mutexInit(chpl_thread_mutex_p mutex) {
  // WAW: how to explicitly specify blocking-type?
  if (pthread_mutex_init((pthread_mutex_t*) mutex, NULL))
    chpl_internal_error("pthread_mutex_init() failed");
}

chpl_thread_mutex_p chpl_thread_mutexNew(void) {
  chpl_thread_mutex_p m;
  m = (chpl_thread_mutex_p) chpl_mem_alloc(sizeof(chpl_thread_mutex_t),
                                           CHPL_RT_MD_MUTEX, 0, 0);
  chpl_thread_mutexInit(m);
  return m;
}

void chpl_thread_mutexLock(chpl_thread_mutex_p mutex) {
  if (pthread_mutex_lock((pthread_mutex_t*) mutex))
    chpl_internal_error("pthread_mutex_lock() failed");
}

void chpl_thread_mutexUnlock(chpl_thread_mutex_p mutex) {
  if (pthread_mutex_unlock((pthread_mutex_t*) mutex))
    chpl_internal_error("pthread_mutex_unlock() failed");
}

static void threadlayer_condvar_init(threadlayer_condvar_t* cv) {
  if (pthread_cond_init((pthread_cond_t*) cv, NULL))
    chpl_internal_error("pthread_cond_init() failed");
}

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

chpl_bool threadlayer_pool_suspend(chpl_thread_mutex_p lock,
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
  chpl_thread_mutexUnlock((threadlayer_mutex_p) void_lock);
}

void threadlayer_pool_awaken(void) {
  if (pthread_cond_signal(&wakeup_cond))
    chpl_internal_error("pthread_cond_signal() failed");
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


// Thread management

chpl_thread_id_t chpl_thread_getId(void) {
  void* val = pthread_getspecific(thread_id_key);

  if (val == NULL)
    return chpl_thread_nullThreadId;
  return (chpl_thread_id_t) (intptr_t) val;
}


void chpl_thread_yield(void) {
  int last_cancel_state;

  //
  // Yield the processor.  To support orderly shutdown, we check
  // for cancellation once the thread regains the processor.
  //
  sched_yield();

  (void) pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
  pthread_testcancel();
  (void) pthread_setcancelstate(last_cancel_state, NULL);
}


void chpl_thread_init(void(*threadBeginFn)(void*),
                      void(*threadEndFn)(void)) {
  //
  // This threading layer does not have any inherent limit on the number
  // of threads.  Its limit is the lesser of any limits imposed by the
  // comm layer and the user.
  //
  {
    uint32_t lim;

    if ((lim = chpl_task_getenvNumThreadsPerLocale()) > 0)
      maxThreads = lim;
    else if ((lim = chpl_comm_getMaxThreads()) > 0)
      maxThreads = lim;
  }

  //
  // Count the main thread on locale 0 as already existing, since it
  // is (or soon will be) running the main program.
  //
  if (chpl_nodeID == 0)
    numThreads = 1;

  //
  // If a value was specified for the call stack size config const, use
  // that (rounded up to a whole number of pages) to set the system and
  // pthread stack limits.
  //
  if (pthread_attr_init(&thread_attributes) != 0)
    chpl_internal_error("pthread_attr_init() failed");

  //
  // If a value was specified for the call stack size, use that (rounded
  // up to a whole number of pages) to set the system and pthread stack
  // limits.  This will in turn limit the stack for any task hosted by
  // either the main process or a pthread.
  //
  {
    size_t css;

    if ((css = chpl_task_getenvCallStackSize()) != 0) {
      uint64_t      pagesize = (uint64_t) sysconf(_SC_PAGESIZE);
      struct rlimit rlim;

      css = (css + pagesize - 1) & ~(pagesize - 1);

      if (getrlimit(RLIMIT_STACK, &rlim) != 0)
        chpl_internal_error("getrlimit() failed");

      if (rlim.rlim_max != RLIM_INFINITY && css > rlim.rlim_max) {
        char warning[128];
        sprintf(warning, "call stack size capped at %lu\n", 
                (unsigned long)rlim.rlim_max);
        chpl_warning(warning, 0, NULL);

        css = rlim.rlim_max;
      }

      rlim.rlim_cur = css;

      if (setrlimit(RLIMIT_STACK, &rlim) != 0)
        chpl_internal_error("setrlimit() failed");

      if (pthread_attr_setstacksize(&thread_attributes, css) != 0)
        chpl_internal_error("pthread_attr_setstacksize() failed");
    }
  }

  if (pthread_attr_getstacksize(&thread_attributes, &threadCallStackSize) != 0)
      chpl_internal_error("pthread_attr_getstacksize() failed");

  saved_threadBeginFn = threadBeginFn;
  saved_threadEndFn   = threadEndFn;

  if (pthread_key_create(&thread_id_key, NULL))
    chpl_internal_error("pthread_key_create(thread_id_key) failed");

  if (pthread_setspecific(thread_id_key, (void*) (intptr_t) --curr_thread_id))
    chpl_internal_error("thread id data key doesn't work");

  if (pthread_key_create(&thread_private_key, NULL))
    chpl_internal_error("pthread_key_create(thread_private_key) failed");

  pthread_mutex_init(&thread_info_lock, NULL);
  pthread_mutex_init(&numThreadsLock, NULL);

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
// chpl_thread_init() for the purpose of this.
//
static void* initial_pthread_func(void* ignore) {
  while (1) {
    pthread_testcancel();
    sched_yield();
  }
  return NULL;
}

int chpl_thread_createCommThread(chpl_fn_p fn, void* arg) {
  pthread_t polling_thread;
  return pthread_create(&polling_thread, NULL, (void*(*)(void*))fn, arg);
}

void chpl_thread_exit(void) {
  chpl_bool debug = false;
  thread_list_p tlp;

  pthread_mutex_lock(&thread_info_lock);
  exiting = true;

  // shut down all threads
  for (tlp = thread_list_head; tlp != NULL; tlp = tlp->next) {
    if (pthread_cancel(tlp->thread) != 0)
      chpl_internal_error("thread cancel failed");
  }

  pthread_mutex_unlock(&thread_info_lock);

  while (thread_list_head != NULL) {
    if (pthread_join(thread_list_head->thread, NULL) != 0)
      chpl_internal_error("thread join failed");
    tlp = thread_list_head;
    thread_list_head = thread_list_head->next;
    chpl_mem_free(tlp, 0, 0);
  }

  if (pthread_key_delete(thread_id_key) != 0)
    chpl_internal_error("pthread_key_delete(thread_id_key) failed");

  if (pthread_attr_destroy(&thread_attributes) != 0)
    chpl_internal_error("pthread_attr_destroy() failed");

  if (debug)
    fprintf(stderr, "A total of %u threads were created\n", numThreads);
}

chpl_bool chpl_thread_canCreate(void) {
  return (maxThreads == 0 ||
          numThreads < (uint32_t) maxThreads);
}

int chpl_thread_create(void* arg)
{
  //
  // An implementation note:
  //
  // It's important to keep the thread counter as accurate as possible,
  // because it's used to throttle thread creation so that we don't go
  // over the user's specified limit.  We could count the new thread
  // when it starts executing, in pthread_func().  But if the kernel
  // executed parent threads in preference to children, the resulting
  // delay in updating the counter could cause us to create many more
  // threads than the limit.  Or we could count them after creating
  // them, here.  But if grabbing the mutex that protects the counter
  // stalled the parent and led the kernel to schedule other threads,
  // updates to the counter could again be delayed and too many threads
  // created.  The solution adopted is to update the counter in the
  // parent before creating the new thread, and then decrement it if
  // thread creation fails.  The idea is that if the only thing that
  // separates the counter update from the thread creation is a mutex
  // unlock which won't cause the parent to be rescheduled, we maximize
  // the likelihood that everyone will see accurate counter values.
  //

  pthread_t pthread;

  pthread_mutex_lock(&numThreadsLock);
  numThreads++;
  pthread_mutex_unlock(&numThreadsLock);

  if (pthread_create(&pthread, &thread_attributes, pthread_func, arg)) {
    pthread_mutex_lock(&numThreadsLock);
    numThreads--;
    pthread_mutex_unlock(&numThreadsLock);

    return -1;
  }

  return 0;
}

static void* pthread_func(void* arg) {
  chpl_thread_id_t my_thread_id;
  thread_list_p          tlp;

  // disable cancellation immediately
  // enable only while waiting for new work
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); 

  // add us to the list of threads
  tlp = (thread_list_p) chpl_mem_alloc(sizeof(struct thread_list),
                                       CHPL_RT_MD_THREAD_LIST_DESCRIPTOR, 0, 0);

  tlp->thread = pthread_self();
  tlp->next   = NULL;

  pthread_mutex_lock(&thread_info_lock);

  if (exiting) {
    pthread_mutex_unlock(&thread_info_lock);
    chpl_mem_free(tlp, 0, 0);
    return NULL;
  }

  my_thread_id = --curr_thread_id;

  if (thread_list_head == NULL)
    thread_list_head = tlp;
  else
    thread_list_tail->next = tlp;
  thread_list_tail = tlp;

  pthread_mutex_unlock(&thread_info_lock);

  if (pthread_setspecific(thread_id_key, (void*) (intptr_t) my_thread_id))
    chpl_internal_error("thread id data key doesn't work");

  if (saved_threadEndFn == NULL)
    (*saved_threadBeginFn)(arg);
  else {
    pthread_cleanup_push((void (*)(void*)) saved_threadEndFn, NULL);
    (*saved_threadBeginFn)(arg);
    pthread_cleanup_pop(0);
  }

  return NULL;
}

void chpl_thread_destroy(void) {
  int last_cancel_state;

  //
  // Creating threads is expensive, so we don't destroy them once we
  // have them.  Instead we just yield the processor.  Eventually, the
  // thread will return to its caller, which is allowed behavior under
  // the threadlayer API.
  //
  // To support orderly shutdown, we check for cancellation once the
  // thread regains the processor.
  //
  sched_yield();

  (void) pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
  pthread_testcancel();
  (void) pthread_setcancelstate(last_cancel_state, NULL);
}

void* chpl_thread_getPrivateData(void) {
  return pthread_getspecific(thread_private_key);
}

void chpl_thread_setPrivateData(void* p) {
  if (pthread_setspecific(thread_private_key, p))
    chpl_internal_error("thread private data key doesn't work");
}

uint32_t chpl_thread_getMaxThreads(void) {
  return maxThreads;
}

uint32_t chpl_thread_getNumThreads(void) {
  return numThreads;
}

uint64_t chpl_thread_getCallStackSize(void) {
    return (uint64_t) threadCallStackSize;
}
