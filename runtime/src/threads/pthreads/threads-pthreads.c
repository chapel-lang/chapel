/*
 * Copyright 2004-2020 Cray Inc.
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

//
// Pthread implementation of Chapel threading interface
//

#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplcast.h"
#include "chplcgfns.h"
#include "chpl-tasks.h"
#include "config.h"
#include "error.h"
#include "chplsys.h"
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
#include <sys/mman.h>
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

// List of stack pointers
typedef struct thread_stack_list* thread_stack_list_p;
struct thread_stack_list {
  void*               stack;
  pthread_t           owner_pthread;

  thread_stack_list_p next;
  thread_stack_list_p prec;
};

chpl_bool              chpl_use_guard_page;     // we have to add guard pages?
chpl_bool              chpl_alloc_stack_in_heap;// we have to alloc the stack?

static chpl_bool       exiting = false;         // are we shutting down?

static pthread_mutex_t thread_info_lock;        // mutual exclusion lock
static pthread_mutex_t thread_stack_list_lock;  // lock for task stack list

static int64_t         curr_thread_id = 0;

static thread_list_p   thread_list_head = NULL; // head of thread_list
static thread_list_p   thread_list_tail = NULL; // tail of thread_list

static thread_stack_list_p   thread_stack_list_head = NULL;

static pthread_attr_t  thread_attributes;

CHPL_TLS_DECL(chpl_thread_id_t,chpl_thread_id);
CHPL_TLS_DECL(void*, chpl_thread_data);

static int32_t         maxThreads = 0;
static uint32_t        numThreads = 0;
static pthread_mutex_t numThreadsLock;

static size_t          threadCallStackSize = 0;

static void            (*saved_threadBeginFn)(void*);
static void            (*saved_threadEndFn)(void);

static void*           initial_pthread_func(void*);
static void*           pthread_func(void*);

/* Function prototypes */
void                   chpl_init_heap_stack(void);
void*                  chpl_alloc_pthread_stack(size_t);
void                   chpl_free_pthread_stack(void*);

/* We have to see if the thread stacks should be allocated in
 * the chapel heap and if we need to add a guard page for catch
 * a stack overflow
 */
void chpl_init_heap_stack(void){

  // We don't want a guard page, but we want the stack in the heap
  if (CHPL_STACK_CHECKS == 0) {
    chpl_alloc_stack_in_heap = true;
    chpl_use_guard_page      = false;
    return;
  }

  /* Special case: we want a guard page, but we can't add it. So
   * we use a pthread stack that provides a guard page.
   */
#if defined(CHPL_USING_CSTDLIB_MALLOC) && defined(__APPLE__)
  chpl_alloc_stack_in_heap = false;
  chpl_use_guard_page      = false;
  return;
#else
  if (chpl_getHeapPageSize() != chpl_getSysPageSize()) {
    chpl_alloc_stack_in_heap = false;
    chpl_use_guard_page      = false;
    return;
  } 
  
  // We want a guard page and we can add it
  chpl_alloc_stack_in_heap = true;
  chpl_use_guard_page      = true;
#endif
}

void* chpl_alloc_pthread_stack(size_t stack_size){
  size_t page_size, mem_size;
  void*  stack;
  void*  mem_buffer;
  int    rc;

  rc = 0;
  // Gets the system page size since we'll use it for guard pages
  // and guard pages are only enabled when not using a huge-pages heap.
  page_size = chpl_getSysPageSize();
  mem_size = (chpl_use_guard_page ? stack_size + page_size : stack_size);

  // uses memalign to align to page_size to simplify logic for the guard page
  // case but thread stacks could be allocated without such alignment
  mem_buffer = chpl_memalign(page_size, mem_size);

  if(mem_buffer == NULL){
    return NULL;
  }

  stack = mem_buffer;

  if(chpl_use_guard_page){
    /* This is architecture-dependent
    *
    * Since the stack grows downward, I have to insert the guard 
    * page at the lowest address.
    */
    stack = (unsigned char*)mem_buffer + page_size;

    rc = mprotect(mem_buffer, page_size, PROT_NONE);
    if( rc != 0 ) {
      chpl_free(mem_buffer);
      return NULL;
    }
  }

  return stack;
}

void chpl_free_pthread_stack(void* stack){
  int free_flag;
  size_t page_size;
  
  free_flag = 0;
  page_size = 0;

  if(chpl_use_guard_page){
    free_flag = PROT_READ | PROT_WRITE | PROT_EXEC;
    page_size = chpl_getSysPageSize();
    mprotect((unsigned char*)stack - page_size, page_size, free_flag);
    chpl_free((unsigned char*)stack - page_size);
  }
  else
    chpl_free(stack);
}

static int do_pthread_create(pthread_t* thread,
                             pthread_attr_t* attr,
                             void *(*start_routine)(void *),
                             void *restrict arg) {
  thread_stack_list_p tslp;
  pthread_attr_t local_attr;
  size_t stack_size;
  void* stack;
  int rc;

  /* If we don't need to allocate the stack in the chapel heap, just call
   * pthread_create
   */
  if(!chpl_alloc_stack_in_heap)
    return pthread_create(thread, attr, start_routine, arg);

  memset(&local_attr, 0, sizeof(local_attr)); // avoid cygwin bug: see #5146
  rc = pthread_attr_init(&local_attr);
  if( rc != 0 ) {
    memset(thread, 0, sizeof(pthread_t));
    return rc;
  }

  stack_size = threadCallStackSize;
  stack = chpl_alloc_pthread_stack(stack_size);
  if( stack == NULL ){
    memset(thread, 0, sizeof(pthread_t));
    return EAGAIN;
  }
  
  rc = pthread_attr_setstack(&local_attr, stack, stack_size);
  if( rc != 0 ) {
    memset(thread, 0, sizeof(pthread_t));
    chpl_free_pthread_stack(stack);
    return rc;
  }

  rc = pthread_create(thread, &local_attr, start_routine, arg);
  if( rc != 0 ) {
    memset(thread, 0, sizeof(pthread_t));
    chpl_free_pthread_stack(stack);
    return rc;
  }

  // Add the new stack to the stack list
  tslp = (thread_stack_list_p) chpl_mem_alloc(sizeof(struct thread_stack_list),
                                       CHPL_RT_MD_THREAD_STACK_DESC, 0, 0);

  tslp->stack = stack;
  tslp->owner_pthread = *thread;
  tslp->prec = NULL;
  
  pthread_mutex_lock(&thread_stack_list_lock);

  tslp->next = thread_stack_list_head;
  if(thread_stack_list_head != NULL)
    thread_stack_list_head->prec = tslp;

  thread_stack_list_head = tslp;

  pthread_mutex_unlock(&thread_stack_list_lock);

  pthread_attr_destroy(&local_attr);
  return rc;
}

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
  int result = pthread_mutex_lock((pthread_mutex_t*) mutex);
  if (result)
    chpl_internal_error("pthread_mutex_lock() failed");
}

void chpl_thread_mutexUnlock(chpl_thread_mutex_p mutex) {
  int result = pthread_mutex_unlock((pthread_mutex_t*) mutex);
  if (result)
    chpl_internal_error("pthread_mutex_unlock() failed");
}

void chpl_thread_mutexDestroy(chpl_thread_mutex_p mutex) {
  int result = pthread_mutex_destroy((pthread_mutex_t*) mutex);
  if (result)
    chpl_internal_error("pthread_mutex_destroy() failed");
}

// Thread management

chpl_thread_id_t chpl_thread_getId(void) {
  void* val = (void*) (intptr_t) CHPL_TLS_GET(chpl_thread_id);

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

  //
  // We expect sched_yield to yield the processor and move the thread to the
  // end of the scheduling queue. However, on cygwin it seems there is no
  // guarantee that the same thread won't be immediately rescheduled even if
  // there are others threads waiting to run. Sleeping should yield the rest of
  // the time slice and allow other threads to actually run.
  //
#ifdef __CYGWIN__
  usleep(1);
#endif

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
    size_t        css;
    size_t        pagesize = (size_t) sysconf(_SC_PAGESIZE);
    struct rlimit rlim;

    if ((css = chpl_task_getEnvCallStackSize()) == 0)
      css = chpl_task_getDefaultCallStackSize();
    assert(css > 0);

    css = (css + pagesize - 1) & ~(pagesize - 1);

    if (getrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("getrlimit() failed");

    if (rlim.rlim_max != RLIM_INFINITY && css > rlim.rlim_max) {
      char warning[128];
      sprintf(warning, "call stack size capped at %lu\n", 
              (unsigned long)rlim.rlim_max);
      chpl_warning(warning, 0, 0);

      css = rlim.rlim_max;
    }

    rlim.rlim_cur = css;

#ifndef __CYGWIN__
    //
    // Cygwin can't do setrlimit(RLIMIT_STACK).
    //
    if (setrlimit(RLIMIT_STACK, &rlim) != 0)
      chpl_internal_error("setrlimit() failed");
#endif

    if (pthread_attr_setstacksize(&thread_attributes, css) != 0)
      chpl_internal_error("pthread_attr_setstacksize() failed");
  }

  if (pthread_attr_getstacksize(&thread_attributes, &threadCallStackSize) != 0)
      chpl_internal_error("pthread_attr_getstacksize() failed");

  saved_threadBeginFn = threadBeginFn;
  saved_threadEndFn   = threadEndFn;

  CHPL_TLS_INIT(chpl_thread_id);
  CHPL_TLS_SET(chpl_thread_id, (intptr_t) --curr_thread_id);
  CHPL_TLS_INIT(chpl_thread_data);

  pthread_mutex_init(&thread_info_lock, NULL);
  pthread_mutex_init(&numThreadsLock, NULL);
  pthread_mutex_init(&thread_stack_list_lock, NULL);

  chpl_init_heap_stack();

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

    // see comment in chpl_thread_yield()
#ifdef __CYGWIN__
    usleep(1);
#endif

  }
  return NULL;
}

int chpl_thread_createCommThread(chpl_fn_p fn, void* arg) {
  pthread_t polling_thread;
  return do_pthread_create(&polling_thread, &thread_attributes, (void*(*)(void*))fn, arg);
}

void chpl_thread_exit(void) {
  chpl_bool debug = false;
  thread_list_p tlp;
  thread_stack_list_p tslp;

  pthread_mutex_lock(&thread_info_lock);
  exiting = true;

  // shut down all threads
  for (tlp = thread_list_head; tlp != NULL; tlp = tlp->next) {
    if (pthread_cancel(tlp->thread) != 0)
      chpl_internal_error("thread cancel failed");
  }

  pthread_mutex_unlock(&thread_info_lock);

  pthread_mutex_lock(&thread_stack_list_lock);

  while (thread_list_head != NULL) {
    tlp = thread_list_head;
    if (pthread_join(tlp->thread, NULL) != 0)
      chpl_internal_error("thread join failed");

    /* We have joined with tlp->thread, so now we search for its stack
     * in the thread_stack_list_head list and free it. This is for be
     * sure that we free stacks used only by terminated threads.
     */
    for (tslp = thread_stack_list_head; tslp != NULL; tslp = tslp->next) {
      if(pthread_equal(tlp->thread, tslp->owner_pthread)){
          chpl_free_pthread_stack(tslp->stack);
          if(tslp->prec == NULL)
            thread_stack_list_head = tslp->next;
          else
            tslp->prec->next = tslp->next;
          
          if(tslp->next != NULL)
            tslp->next->prec = tslp->prec;
          
          chpl_mem_free(tslp, 0, 0);
          break;
      }
    }

    thread_list_head = thread_list_head->next;
    chpl_mem_free(tlp, 0, 0);
  }

  pthread_mutex_unlock(&thread_stack_list_lock);

  CHPL_TLS_DELETE(chpl_thread_id);
  CHPL_TLS_DELETE(chpl_thread_data);

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

  if (do_pthread_create(&pthread, &thread_attributes, pthread_func, arg)) {
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
                                       CHPL_RT_MD_THREAD_LIST_DESC, 0, 0);

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

  CHPL_TLS_SET(chpl_thread_id, (intptr_t) my_thread_id);

  if (saved_threadEndFn == NULL)
    (*saved_threadBeginFn)(arg);
  else {
    pthread_cleanup_push((void (*)(void*)) saved_threadEndFn, NULL);
    (*saved_threadBeginFn)(arg);
    pthread_cleanup_pop(1); // Shouldn't we run the thread
                            // end function even if not cancelled?
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

  // see comment in chpl_thread_yield()
#ifdef __CYGWIN__
  usleep(1);
#endif

  (void) pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &last_cancel_state);
  pthread_testcancel();
  (void) pthread_setcancelstate(last_cancel_state, NULL);
}

uint32_t chpl_thread_getMaxThreads(void) {
  return maxThreads;
}

uint32_t chpl_thread_getNumThreads(void) {
  return numThreads;
}

size_t chpl_thread_getCallStackSize(void) {
    return threadCallStackSize;
}
