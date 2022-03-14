/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

//
// FIFO implementation of Chapel tasking interface
//

#include "chplrt.h"
#include "chpl_rt_utils_static.h"
#include "chplcgfns.h"
#include "chpl-arg-bundle.h"
#include "chpl-atomics.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chpl-locale-model.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpl-tasks-callbacks-internal.h"
#include "chpl-topo.h"
#include "chpltypes.h"
#include "chpl-linefile-support.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>


//
// task pool: linked list of tasks
//
typedef struct task_pool_struct* task_pool_p;

typedef struct {
  chpl_task_infoRuntime_t infoRuntime;
} chpl_task_prvDataImpl_t;

typedef struct task_pool_struct {
  task_pool_p      next;         // double-link pointers for pool
  task_pool_p      prev;

  chpl_task_prvDataImpl_t chpl_data;

  chpl_task_bundle_t* taskBundle; // addr of task bundle in bundle below
  chpl_any_arg_bundle_t bundle[0];
} task_pool_t;


// This is the data that is private to each thread.
typedef struct {
  task_pool_p   ptask;
} thread_private_data_t;


static chpl_bool        initialized = false;

static chpl_thread_mutex_t threading_lock;     // critical section lock
static chpl_thread_mutex_t task_id_lock;       // critical section lock
static volatile task_pool_p
                           task_pool_head;     // head of task pool
static volatile task_pool_p
                           task_pool_tail;     // tail of task pool

static int                 queued_task_cnt;    // number of tasks in task pool
static atomic_uint_least64_t
                           blocked_task_cnt;   // number of blocked tasks
static int                 idle_thread_cnt;    // number of threads looking
                                               //   for work

static chpl_bool do_taskReport = false;
static chpl_thread_mutex_t taskTable_lock;     // critical section lock

static chpl_fn_p comm_task_fn;

//
// Internal functions.
//
static void                    enqueue_task(task_pool_p);
static void                    dequeue_task(task_pool_p);
static void                    comm_task_wrapper(void*);
static void                    taskCallBody(chpl_fn_int_t, chpl_fn_p,
                                            void*, size_t,
                                            c_sublocid_t,
                                            int, int32_t);
static chpl_taskID_t           get_next_task_id(void);
static thread_private_data_t*  get_thread_private_data(void);
static task_pool_p             get_current_ptask(chpl_bool);
static void                    report_all_tasks(void);
static void                    SIGINT_handler(int sig);
static void                    thread_begin(void*);
static void                    thread_end(void);
static void                    maybe_add_thread(void);
static task_pool_p             add_to_task_pool(chpl_fn_int_t, chpl_fn_p,
                                                void*, size_t,
                                                chpl_bool, int, int32_t);

//
// Condition variable methods
//
static void chpl_thread_condvar_init(chpl_thread_condvar_t* cv);

//
// Sync variable methods
//
static chpl_bool chpl_thread_sync_suspend(chpl_sync_aux_t *s,
                                   struct timeval *deadline);
static void chpl_thread_sync_awaken(chpl_sync_aux_t *s);

// Sync variables

static void sync_wait_and_lock(chpl_sync_aux_t *s,
                               chpl_bool want_full,
                               int32_t lineno, int32_t filename) {
  chpl_bool suspend_using_cond;

  chpl_thread_mutexLock(&s->lock);

  // If we're oversubscribing the hardware, we wait using conditionals
  // in order to ensure fairness and thus progress.  If we're not, we
  // can spin-wait.
  suspend_using_cond = (chpl_thread_getNumThreads() >=
                        chpl_topo_getNumCPUsLogical(true));

  chpl_bool waited = false;
  while (s->is_full != want_full) {
    if (!suspend_using_cond) {
      chpl_thread_mutexUnlock(&s->lock);
    }
    do {
      if (suspend_using_cond)
        (void) chpl_thread_sync_suspend(s, NULL);
      else
        chpl_thread_yield();
    } while (s->is_full != want_full);
    if (!suspend_using_cond)
      chpl_thread_mutexLock(&s->lock);
    if (!waited) {
      waited = true;
      (void) atomic_fetch_add_uint_least64_t(&blocked_task_cnt, 1);
    }
  }

  if (waited) {
    (void) atomic_fetch_sub_uint_least64_t(&blocked_task_cnt, 1);
  }
}

void chpl_sync_lock(chpl_sync_aux_t *s) {
  chpl_thread_mutexLock(&s->lock);
}

void chpl_sync_unlock(chpl_sync_aux_t *s) {
  chpl_thread_mutexUnlock(&s->lock);
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                                  int32_t lineno, int32_t filename) {
  sync_wait_and_lock(s, true, lineno, filename);
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                   int32_t lineno, int32_t filename) {
  sync_wait_and_lock(s, false, lineno, filename);
}

static chpl_bool chpl_thread_sync_suspend(chpl_sync_aux_t *s,
                                   struct timeval *deadline) {
  chpl_thread_condvar_t* cond;
  cond = s->is_full ? &s->signal_empty : &s->signal_full;

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

static void chpl_thread_sync_awaken(chpl_sync_aux_t *s) {
  if (pthread_cond_signal(s->is_full ?
                          &s->signal_full : &s->signal_empty))
    chpl_internal_error("pthread_cond_signal() failed");
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s) {
  s->is_full = true;
  chpl_thread_sync_awaken(s);
  chpl_sync_unlock(s);
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s) {
  s->is_full = false;
  chpl_thread_sync_awaken(s);
  chpl_sync_unlock(s);
}

chpl_bool chpl_sync_isFull(void *val_ptr,
                            chpl_sync_aux_t *s) {
  return s->is_full;
}

static void chpl_thread_condvar_init(chpl_thread_condvar_t* cv) {
  if (pthread_cond_init((pthread_cond_t*) cv, NULL))
    chpl_internal_error("pthread_cond_init() failed");
}

void chpl_sync_initAux(chpl_sync_aux_t *s) {
  s->is_full = false;
  chpl_thread_mutexInit(&s->lock);
  chpl_thread_condvar_init(&s->signal_full);
  chpl_thread_condvar_init(&s->signal_empty);
}

static void chpl_thread_condvar_destroy(chpl_thread_condvar_t* cv) {
// Leak condvars on cygwin. Some bug results from condvars still being used at
// this point on cygwin. For now, just leak them to avoid errors as a result of
// the undefined behavior of trying to destroy an in-use condvar.
#ifndef __CYGWIN__
  if (pthread_cond_destroy((pthread_cond_t*) cv))
    chpl_internal_error("pthread_cond_destroy() failed");
#endif
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s) {
  chpl_thread_condvar_destroy(&s->signal_full);
  chpl_thread_condvar_destroy(&s->signal_empty);
  chpl_thread_mutexDestroy(&s->lock);
}

static void setup_main_thread_private_data(void)
{
  thread_private_data_t* tp;

  tp = (thread_private_data_t*) chpl_mem_calloc(1, sizeof(thread_private_data_t),
                                                CHPL_RT_MD_THREAD_PRV_DATA,
                                                0, 0);

  tp->ptask = (task_pool_p) chpl_mem_calloc(1,
                                            (offsetof(task_pool_t, bundle)
                                             + sizeof(chpl_task_bundle_t)),
                                            CHPL_RT_MD_TASK_POOL_DESC,
                                            0, 0);

  tp->ptask->taskBundle = &tp->ptask->bundle[0].task;
  *tp->ptask->taskBundle = (chpl_task_bundle_t)
                           { .kind            = CHPL_ARG_BUNDLE_KIND_TASK,
                             .is_executeOn    = false,
                             .lineno          = 0,
                             .filename        = CHPL_FILE_IDX_MAIN_PROGRAM,
                             .requestedSubloc = c_sublocid_any_val,
                             .requested_fid   = FID_NONE,
                             .requested_fn    = NULL,
                             .id              = get_next_task_id(),
                           };

  chpl_thread_setPrivateData(tp);
}


// Tasks

void chpl_task_init(void) {
  chpl_thread_mutexInit(&threading_lock);
  chpl_thread_mutexInit(&task_id_lock);
  queued_task_cnt = 0;
  atomic_init_uint_least64_t(&blocked_task_cnt, 0);
  idle_thread_cnt = 0;
  task_pool_head = task_pool_tail = NULL;

  chpl_thread_init(thread_begin, thread_end);

  //
  // Set main thread private data, so that things that require access
  // to it, like chpl_task_getID() and chpl_task_setSerial(), can be
  // called early (notably during standard module initialization).
  //
  // This needs to be done after the threading layer initialization,
  // because it's based on thread layer capabilities, but before we
  // install the signal handlers, because when those are invoked they
  // may use the thread private data.
  //
  setup_main_thread_private_data();

  initialized = true;
}


void chpl_task_exit(void) {
  if (!initialized)
    return;

  chpl_thread_exit();
}


typedef void (*main_ptr_t)(void);
static void* do_callMain(void* arg) {
  main_ptr_t chpl_main = (main_ptr_t) arg;

  // make sure this thread has thread-private data.
  setup_main_thread_private_data();

  chpl_main();
  return NULL;
}

/* These extern are implemented in threads-pthreads.c
 * and they are used for allocate the stack of the main
 * task as a normal task
 */
extern chpl_bool       chpl_use_guard_page;
extern chpl_bool       chpl_alloc_stack_in_heap;

extern void            chpl_init_heap_stack(void);
extern void*           chpl_alloc_pthread_stack(size_t);
extern void            chpl_free_pthread_stack(void*);

void chpl_task_callMain(void (*chpl_main)(void)) {
  // since we want to run all work in a task with a comm-friendly stack,
  // run main in a pthread that we will wait for.
  size_t stack_size;
  void* stack;
  pthread_attr_t attr;
  pthread_t thread;
  int rc;

  stack = NULL;
  chpl_init_heap_stack();

  rc = pthread_attr_init(&attr);
  if( rc != 0 ) {
    chpl_internal_error("pthread_attr_init main failed");
  }

  stack_size  = chpl_thread_getCallStackSize();

  if(chpl_alloc_stack_in_heap){
    stack = chpl_alloc_pthread_stack(stack_size);
    if(stack == NULL)
      chpl_internal_error("chpl_alloc_pthread_stack main failed");

    rc = pthread_attr_setstack(&attr, stack, stack_size);
    if( rc != 0 ) {
      chpl_internal_error("pthread_attr_setstack main failed");
    }
  }
  else {
    rc = pthread_attr_setstacksize(&attr, stack_size);
    if( rc != 0 ) {
      chpl_internal_error("pthread_attr_setstacksize main failed");
    }
  }

  rc = pthread_create(&thread, &attr, do_callMain, chpl_main);
  if( rc != 0 ) {
    chpl_internal_error("pthread_create main failed");
  }

  rc = pthread_join(thread, NULL);
  if( rc != 0 ) {
    chpl_internal_error("pthread_join main failed");
  }

  if(chpl_alloc_stack_in_heap){
    chpl_free_pthread_stack(stack);
  }

  pthread_attr_destroy(&attr);
}

void chpl_task_stdModulesInitialized(void) {

  //
  // The task table is implemented in Chapel code in the modules, so we
  // can't use it, and thus can't support task reporting on ^C, until
  // the other modules on which it depends have been initialized.  By
  // the time this function is called that is true, so now we can set up
  // our own supporting code.
  //
  do_taskReport = chpl_task_doTaskReport();
  if (do_taskReport) {
    thread_private_data_t* tp = get_thread_private_data();

    chpldev_taskTable_add(tp->ptask->taskBundle->id,
                          tp->ptask->taskBundle->lineno,
                          tp->ptask->taskBundle->filename,
                          (uint64_t) (intptr_t) tp->ptask);
    chpldev_taskTable_set_active(tp->ptask->taskBundle->id);

    chpl_thread_mutexInit(&taskTable_lock);

    signal(SIGINT, SIGINT_handler);
  }
}


int chpl_task_createCommTask(chpl_fn_p fn, void* arg) {
  comm_task_fn = fn;
  return chpl_thread_createCommThread(comm_task_wrapper, arg);
}


static void comm_task_wrapper(void* arg) {
  thread_private_data_t* tp;

  tp = (thread_private_data_t*) chpl_mem_calloc(1, sizeof(thread_private_data_t),
                                                CHPL_RT_MD_THREAD_PRV_DATA,
                                                0, 0);

  tp->ptask = (task_pool_p) chpl_mem_calloc(1,
                                            (offsetof(task_pool_t, bundle)
                                             + sizeof(chpl_task_bundle_t)),
                                            CHPL_RT_MD_TASK_POOL_DESC,
                                            0, 0);

  tp->ptask->taskBundle = &tp->ptask->bundle[0].task;
  *tp->ptask->taskBundle = (chpl_task_bundle_t)
                           { .kind            = CHPL_ARG_BUNDLE_KIND_TASK,
                             .is_executeOn    = false,
                             .lineno          = 0,
                             .filename        = CHPL_FILE_IDX_COMM_TASK,
                             .requestedSubloc = c_sublocid_any_val,
                             .requested_fid   = FID_NONE,
                             .requested_fn    = NULL,
                             .id              = get_next_task_id(),
                           };

  chpl_thread_setPrivateData(tp);

  (*comm_task_fn)(arg);
}


//
// Enqueue and dequeue tasks from the pool.
//
static inline
void enqueue_task(task_pool_p ptask) {
  queued_task_cnt++;

  //
  // Add to pool.
  //
  if (task_pool_tail)
    task_pool_tail->next = ptask;
  else
    task_pool_head = ptask;
  ptask->prev = task_pool_tail;
  task_pool_tail = ptask;
}


static inline
void dequeue_task(task_pool_p ptask) {
  assert(queued_task_cnt > 0);
  queued_task_cnt--;

  //
  // Remove from pool.
  //
  if (ptask == task_pool_head) {
    if ((task_pool_head = task_pool_head->next) == NULL)
      task_pool_tail = NULL;
    else
      task_pool_head->prev = NULL;
  }
  else {
    if ((ptask->prev->next = ptask->next) == NULL)
      task_pool_tail = ptask->prev;
    else
      ptask->next->prev = ptask->prev;
  }
}


void chpl_task_addTask(chpl_fn_int_t fid,
                       chpl_task_bundle_t* arg, size_t arg_size,
                       c_sublocid_t subloc,
                       int lineno, int32_t filename) {
  assert(subloc == c_sublocid_any);

  arg->kind = CHPL_ARG_BUNDLE_KIND_TASK;

  // begin critical section
  chpl_thread_mutexLock(&threading_lock);

  (void) add_to_task_pool(fid, chpl_ftable[fid], arg, arg_size,
                          false, lineno, filename);

  // end critical section
  chpl_thread_mutexUnlock(&threading_lock);
}


void chpl_task_taskCallFTable(chpl_fn_int_t fid,
                        void* arg, size_t arg_size,
                        c_sublocid_t subloc,
                        int lineno, int32_t filename) {
  taskCallBody(fid, chpl_ftable[fid], arg, arg_size, subloc, lineno, filename);
}


static inline
void taskCallBody(chpl_fn_int_t fid, chpl_fn_p fp,
                  void* arg, size_t arg_size,
                  c_sublocid_t subloc,
                  int lineno, int32_t filename) {
  // begin critical section
  chpl_thread_mutexLock(&threading_lock);

  (void) add_to_task_pool(fid, fp, arg, arg_size, true,
                          lineno, filename);

  // end critical section
  chpl_thread_mutexUnlock(&threading_lock);
}



void chpl_task_startMovedTask(chpl_fn_int_t  fid, chpl_fn_p fp,
                              void* arg, size_t arg_size,
                              c_sublocid_t subloc,
                              chpl_taskID_t id) {
  //
  // For now the incoming task ID is simply dropped, though we check
  // to make sure the caller wasn't expecting us to do otherwise.  If
  // we someday make task IDs global we will need to be able to set
  // the ID of this moved task.
  //
  assert(id == chpl_nullTaskID);

  taskCallBody(fid, fp, arg, arg_size, subloc,
               0, CHPL_FILE_IDX_UNKNOWN);
}


//
// chpl_task_getSubloc() is in chpl-tasks-impl.h.
//


//
// chpl_task_setSubloc() is in chpl-tasks-impl.h.
//


//
// chpl_task_getRequestedSubloc() is in chpl-tasks-impl.h.
//


chpl_taskID_t chpl_task_getId(void) {
  task_pool_p ptask = get_current_ptask(false /*must_be_task*/);
  return ptask ? ptask->taskBundle->id : (chpl_taskID_t) -1;
}

chpl_bool chpl_task_idEquals(chpl_taskID_t id1, chpl_taskID_t id2) {
  return id1 == id2;
}

char* chpl_task_idToString(char* buff, size_t size, chpl_taskID_t id) {
  int ret = snprintf(buff, size, "%"PRIu64, id);
  if(ret>0 && ret<size)
    return buff;
  else
    return NULL;
}

void chpl_task_yield(void) {
  chpl_thread_yield();
}


void chpl_task_sleep(double secs) {
  struct timeval deadline;
  struct timeval now;

  //
  // Figure out when this task can proceed again, and until then, keep
  // yielding.
  //
  gettimeofday(&deadline, NULL);
  deadline.tv_usec += (suseconds_t) lround((secs - trunc(secs)) * 1.0e6);
  if (deadline.tv_usec > 1000000) {
    deadline.tv_sec++;
    deadline.tv_usec -= 1000000;
  }
  deadline.tv_sec += (time_t) trunc(secs);

  do {
    chpl_task_yield();
    gettimeofday(&now, NULL);
  } while (now.tv_sec < deadline.tv_sec
           || (now.tv_sec == deadline.tv_sec
               && now.tv_usec < deadline.tv_usec));
}

uint32_t chpl_task_getMaxPar(void) {
  uint32_t max;
  uint32_t maxThreads;

  //
  // We expect that even if the physical CPUs have multiple hardware
  // threads, cache and pipeline conflicts will typically prevent
  // applications from gaining by using them.  So, we just return the
  // lesser of the number of physical CPUs and whatever the threading
  // layer says it can do.
  //
  max = (uint32_t) chpl_topo_getNumCPUsPhysical(true);
  maxThreads = chpl_thread_getMaxThreads();
  if (maxThreads < max && maxThreads > 0)
    max = maxThreads;
  return max;
}

chpl_task_infoRuntime_t* chpl_task_getInfoRuntime(void) {
  task_pool_p ptask = get_current_ptask(false /*must_be_task*/);
  return ptask ? &ptask->chpl_data.infoRuntime : NULL;
}

chpl_task_infoChapel_t* chpl_task_getInfoChapel(void) {
  return & get_current_ptask(true /*must_be_task*/)->taskBundle->infoChapel;
}


size_t chpl_task_getCallStackSize(void) {
  return chpl_thread_getCallStackSize();
}

chpl_bool chpl_task_guardPagesInUse(void) {
  return chpl_use_guard_page;
}

uint32_t chpl_task_getNumQueuedTasks(void) {
  return queued_task_cnt;
}

int32_t chpl_task_getNumBlockedTasks(void) {
  return atomic_load_uint_least64_t(&blocked_task_cnt);
}


// Internal utility functions for task management

//
// Get a new task ID.
//
static chpl_taskID_t get_next_task_id(void) {
  static chpl_taskID_t       id = chpl_nullTaskID + 1;

  chpl_taskID_t              next_id;

  chpl_thread_mutexLock(&task_id_lock);
  next_id = id++;
  chpl_thread_mutexUnlock(&task_id_lock);

  return next_id;
}


//
// Get the the thread private data pointer for my thread.
//
static inline
thread_private_data_t* get_thread_private_data(void) {
  thread_private_data_t* tp;

  tp = (thread_private_data_t*) chpl_thread_getPrivateData();

  if (tp == NULL)
    chpl_internal_error("no thread private data");

  return tp;
}


//
// Get the descriptor for the task now running on my thread.
//
static inline
task_pool_p get_current_ptask(chpl_bool must_be_task) {
  thread_private_data_t* tp = chpl_thread_getPrivateData();
  if (tp == NULL) {
    if (must_be_task) {
      chpl_internal_error("no thread private data");
    }
    return NULL;
  }
  return tp->ptask;
}


//
// This signal handler prints an overall task report, containing
// pending tasks and those that are running.
//
static void report_all_tasks(void) {
  task_pool_p pendingTask = task_pool_head;

  printf("Task report\n");
  printf("--------------------------------\n");

  // print out pending tasks
  printf("Pending tasks:\n");
  while (pendingTask != NULL) {
    printf("- %s:%d\n", chpl_lookupFilename(pendingTask->taskBundle->filename),
           pendingTask->taskBundle->lineno);
    pendingTask = pendingTask->next;
  }
  printf("\n");

  // print out running tasks
  printf("Known tasks:\n");
  chpldev_taskTable_print();
}


//
// This is a signal handler that does thread and task reporting.
//
static void SIGINT_handler(int sig) {
  signal(sig, SIG_IGN);

  if (do_taskReport)
    report_all_tasks();

  chpl_exit_any(1);
}


//
// When we create a thread it runs this wrapper function, which just
// executes tasks out of the pool as they become available.
//
static void
thread_begin(void* ptask_void) {
  task_pool_p ptask;
  thread_private_data_t *tp;

  tp = (thread_private_data_t*) chpl_mem_alloc(sizeof(thread_private_data_t),
                                               CHPL_RT_MD_THREAD_PRV_DATA,
                                               0, 0);
  chpl_thread_setPrivateData(tp);

  tp->ptask = NULL;

  while (true) {
    //
    // wait for a task to be present in the task pool
    //

    // In revision 22137, we investigated whether it was beneficial to
    // implement this while loop in a hybrid style, where depending on
    // the number of tasks available, idle threads would either yield or
    // wait on a condition variable to waken them.  Through analysis, we
    // realized this could potential create a case where a thread would
    // become stranded, waiting for a condition signal that would never
    // come.  A potential solution to this was to keep a count of threads
    // that were waiting on the signal, but since there was a performance
    // impact from keeping it as a hybrid as opposed to merely yielding,
    // it was decided that we would return to the simple yield case.
    while (!task_pool_head) {
      do {
        chpl_thread_yield();
      } while (!task_pool_head);
    }

    //
    // Just now the pool had at least one task in it.  Lock and see if
    // there's something still there.
    //
    chpl_thread_mutexLock(&threading_lock);
    if (!task_pool_head) {
      chpl_thread_mutexUnlock(&threading_lock);
      continue;
    }

    //
    // We've found a task to run.
    //

    //
    // start new task; remove task from pool also add to task to task-table
    // (structure in ChapelRuntime that keeps track of currently running tasks
    // for task-reports on deadlock or Ctrl+C).
    //
    ptask = task_pool_head;
    idle_thread_cnt--;

    dequeue_task(ptask);

    // end critical section
    chpl_thread_mutexUnlock(&threading_lock);

    tp->ptask = ptask;

    if (do_taskReport) {
      chpl_thread_mutexLock(&taskTable_lock);
      chpldev_taskTable_set_active(ptask->taskBundle->id);
      chpl_thread_mutexUnlock(&taskTable_lock);
    }

    chpl_task_do_callbacks(chpl_task_cb_event_kind_begin,
                           ptask->taskBundle->requested_fid,
                           ptask->taskBundle->filename,
                           ptask->taskBundle->lineno,
                           ptask->taskBundle->id,
                           ptask->taskBundle->is_executeOn);

    (ptask->taskBundle->requested_fn)(&ptask->bundle);

    chpl_task_do_callbacks(chpl_task_cb_event_kind_end,
                           ptask->taskBundle->requested_fid,
                           ptask->taskBundle->filename,
                           ptask->taskBundle->lineno,
                           ptask->taskBundle->id,
                           ptask->taskBundle->is_executeOn);

    if (do_taskReport) {
      chpl_thread_mutexLock(&taskTable_lock);
      chpldev_taskTable_remove(ptask->taskBundle->id);
      chpl_thread_mutexUnlock(&taskTable_lock);
    }

    tp->ptask = NULL;
    chpl_mem_free(ptask, 0, 0);

    // begin critical section
    chpl_thread_mutexLock(&threading_lock);

    //
    // finished task; increment idle count
    //
    idle_thread_cnt++;

    // end critical section
    chpl_thread_mutexUnlock(&threading_lock);
  }
}


//
// When a thread is destroyed it calls this ending function.
//
static void thread_end(void)
{
  thread_private_data_t* tp;

  tp = (thread_private_data_t*) chpl_thread_getPrivateData();
  if (tp != NULL) {
    chpl_mem_free(tp, 0, 0);
    chpl_thread_setPrivateData(NULL);
  }
}


//
// Launch another thread, if it seems useful to do so and we can.
//
static void maybe_add_thread(void) {
  static chpl_bool warning_issued = false;

  if (!warning_issued && chpl_thread_canCreate()) {
    if (chpl_thread_create(NULL) == 0) {
      idle_thread_cnt++;
    }
    else {
      int32_t max_threads = chpl_thread_getMaxThreads();
      uint32_t num_threads = chpl_thread_getNumThreads();
      char msg[256];
      if (max_threads)
        sprintf(msg,
                "max threads per locale is %" PRId32
                ", but unable to create more than %d threads",
                max_threads, num_threads);
      else
        sprintf(msg,
                "max threads per locale is unbounded"
                ", but unable to create more than %d threads",
                num_threads);
      chpl_warning(msg, 0, 0);
      warning_issued = true;
    }
  }
}


// create a task from the given function pointer and arguments
// and append it to the end of the task pool
// assumes threading_lock has already been acquired!
static inline
task_pool_p add_to_task_pool(chpl_fn_int_t fid, chpl_fn_p fp,
                             void* a, size_t a_size,
                             chpl_bool is_executeOn,
                             int lineno, int32_t filename) {


  task_pool_p ptask;
  chpl_task_prvDataImpl_t pv;

  memset(&pv, 0, sizeof(pv));

  //
  // We have an arg bundle header followed by a payload, but the bundle
  // could be either a comm or a task one.
  //
  assert(a_size >= chpl_argBundleSizeofHdr(a));
  ptask = (task_pool_p) chpl_mem_alloc(offsetof(task_pool_t, bundle) + a_size,
                                       CHPL_RT_MD_TASK_ARG_AND_POOL_DESC,
                                       lineno, filename);

  memcpy(&ptask->bundle, a, a_size);
  ptask->taskBundle = chpl_argBundleTaskArgBundle(&ptask->bundle);

  ptask->next                   = NULL;
  ptask->prev                   = NULL;
  ptask->chpl_data              = pv;

  *ptask->taskBundle =
    (chpl_task_bundle_t)
    { .is_executeOn    = is_executeOn,
      .lineno          = lineno,
      .filename        = filename,
      .requestedSubloc = c_sublocid_any_val,
      .requested_fid   = fid,
      .requested_fn    = fp,
      .id              = get_next_task_id(),
      .infoChapel      = ptask->taskBundle->infoChapel,// retain; set by caller
    };

  enqueue_task(ptask);

  chpl_task_do_callbacks(chpl_task_cb_event_kind_create,
                         ptask->taskBundle->requested_fid,
                         ptask->taskBundle->filename,
                         ptask->taskBundle->lineno,
                         ptask->taskBundle->id,
                         ptask->taskBundle->is_executeOn);

  if (do_taskReport) {
    chpl_thread_mutexLock(&taskTable_lock);
    chpldev_taskTable_add(ptask->taskBundle->id,
                          ptask->taskBundle->lineno,
                          ptask->taskBundle->filename,
                          (uint64_t) (intptr_t) ptask);
    chpl_thread_mutexUnlock(&taskTable_lock);
  }

  // If we now have more tasks than threads to run them on, try to start
  // another thread
  if (queued_task_cnt > idle_thread_cnt) {
    maybe_add_thread();
  }

  return ptask;
}


// Threads

uint32_t chpl_task_getNumThreads(void) {
  return chpl_thread_getNumThreads();
}

uint32_t chpl_task_getNumIdleThreads(void) {
  return idle_thread_cnt;
}
