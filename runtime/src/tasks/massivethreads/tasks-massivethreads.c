/*
 * Copyright 2004-2018 Cray Inc.
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
// MassiveThreads implementation of Chapel tasking interface
//

#include "chplrt.h"
#include "chpl_rt_utils_static.h"
#include "chplcgfns.h"
#include "chpl-comm.h"
#include "chplexit.h"
#include "chpl-locale-model.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chpl-tasks-callbacks-internal.h"
#include "chplsys.h"
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

/* MYTH_IS_PTHREAD defined in tasks-massivethreads.h */

#include <sys/syscall.h>
#if MYTH_IS_PTHREAD
#include <pthread.h>
#else
#include <myth/myth.h>
#endif

#ifndef LAUNCHER

#include <stdint.h>
#include "chplcgfns.h"
#include "chpltypes.h"
#include "chpl-tasks-prvdata.h"

#ifdef CHPL_TASKS_MODEL_H
#include CHPL_TASKS_MODEL_H
#endif


//
// Some function declarations here may be protected like this:
//   #ifdef CHPL_TASK_func_IMPL_DECL
//   declaration of func() here
//   #endif
// In these cases the implementation-specific .h file can provide the
// declaration of func() itself and then set CHPL_TASK_func_IMPL_DECL
// to say that it has done so.  Typically this is done for functions
// that benefit by being declared static inline, for performance.  At
// present we only support this for a few functions.  We can add more
// as needed.
//

static int chpl_myth_debug_set(void) {
  static int chpl_myth_debug = -1;
  if (chpl_myth_debug == -1) {
    char * s = getenv("CHPL_MYTH_DEBUG");
    if (s) {
      chpl_myth_debug = atoi(s);
    } else {
      chpl_myth_debug = 0;
    }
  }
  return chpl_myth_debug;
}

static pid_t gettid(void) {
  return syscall(SYS_gettid);
}

static FILE * log_fp = 0;
static FILE * ensure_log_fp(void) {
  if (log_fp == 0) {
    char filename[100];
    sprintf(filename, "log.%d", chpl_nodeID);
    log_fp = fopen(filename, "wb");
    assert(log_fp);
  }
  return log_fp;
}

static int enter__(const char * function_name) {
  if (chpl_myth_debug_set()) {
    fprintf(ensure_log_fp(), "%d:%d: enter       %s:\n", getpid(), gettid(), function_name);
    fflush(ensure_log_fp());
  }
  return 0;
}
#define enter_() enter__(__func__)

static int return_from__(const char * function_name) {
  if (chpl_myth_debug_set()) {
    fprintf(ensure_log_fp(), "%d:%d: return_from %s:\n", getpid(), gettid(), function_name);
    fflush(ensure_log_fp());
  }
  return 0;
}
#define return_from_() return_from__(__func__)

// Sync variables

#if MYTH_IS_PTHREAD
#include <sched.h>

typedef pthread_key_t myth_key_t;

typedef struct {
  size_t default_stack_size;
} myth_globalattr_t;

int myth_globalattr_init(myth_globalattr_t * attr) {
  attr->default_stack_size = 1 << 17;
  return 0;
}

int myth_globalattr_set_stacksize(myth_globalattr_t * attr, size_t css) {
  attr->default_stack_size = css;
  return 0;
}

int myth_globalattr_get_stacksize(myth_globalattr_t * attr, size_t * css) {
  *css = (1 << 17);
  return 0;
}

void myth_init_ex(myth_globalattr_t * a) {
  (void)a;
}

int myth_key_create(myth_key_t *key, void (*destructor)(void*)) {
  return pthread_key_create(key, destructor);
}

void myth_fini(void) {
}

typedef void * (*myth_func_t)(void *);
typedef pthread_t myth_thread_t;

myth_thread_t myth_create(myth_func_t fn, void * arg) {
  myth_thread_t tid;
  pthread_create(&tid, 0, fn, arg);
  return tid;
}

myth_thread_t myth_self(void) {
  return pthread_self();
}

void myth_yield() {
  sched_yield();
}

void * myth_getspecific(myth_key_t key) {
  return pthread_getspecific(key);
}

int myth_setspecific(myth_key_t key, void * state) {
  return pthread_setspecific(key, state);
}

uint32_t myth_get_num_workers() {
  return 10;
}

typedef struct myth_felockattr { } myth_felockattr_t;

static inline int myth_felock_init_body(myth_felock_t * fe,
                                        const myth_felockattr_t * attr) {
  (void)attr;
  pthread_mutex_init(fe->mutex, 0);
  pthread_cond_init(&fe->cond[0], 0);
  pthread_cond_init(&fe->cond[1], 0);
  fe->status = 0;
  return 0;
}

static inline int myth_felock_destroy_body(myth_felock_t * fe) {
  pthread_mutex_destroy(fe->mutex);
  pthread_cond_destroy(&fe->cond[0]);
  pthread_cond_destroy(&fe->cond[1]);
  return 0;
}

static inline int myth_felock_lock_body(myth_felock_t * fe) {
  return pthread_mutex_lock(fe->mutex);
}

static inline int myth_felock_unlock_body(myth_felock_t * fe) {
  return pthread_mutex_unlock(fe->mutex);
}

static inline int myth_felock_wait_and_lock_body(myth_felock_t * fe, 
                                                 int status_to_wait) {
  pthread_mutex_lock(fe->mutex);
  while (fe->status != status_to_wait) {
    pthread_cond_wait(&fe->cond[status_to_wait], fe->mutex);
  }
  return 0;
}

static inline int myth_felock_mark_and_signal_body(myth_felock_t * fe,
                                                   int status_to_signal) {
  fe->status = status_to_signal;
  pthread_cond_signal(&fe->cond[status_to_signal]);
  return pthread_mutex_unlock(fe->mutex);
}

static inline int myth_felock_status_body(myth_felock_t * fe) {
  return fe->status;
}

int myth_felock_init(myth_felock_t * fe, const myth_felockattr_t * attr) {
  return myth_felock_init_body(fe, attr);
}

int myth_felock_destroy(myth_felock_t * fe) {
  return myth_felock_destroy_body(fe);
}

int myth_felock_lock(myth_felock_t * fe) {
  return myth_felock_lock_body(fe);
}

int myth_felock_unlock(myth_felock_t * fe) {
  return myth_felock_unlock_body(fe);
}

int myth_felock_wait_and_lock(myth_felock_t * fe, int status_to_wait) {
  return myth_felock_wait_and_lock_body(fe, status_to_wait);
}

int myth_felock_mark_and_signal(myth_felock_t * fe, int status_to_signal) {
  return myth_felock_mark_and_signal_body(fe, status_to_signal);
}

int myth_felock_status(myth_felock_t * fe) {
  return myth_felock_status_body(fe);
}

#else  /* MYTH_IS_PTHREAD */

#if 0
pthread_t pthread_self(void) {
  //printf("pthread_self\n");
  pthread_t x = (pthread_t)myth_self();
  //printf("pthread_self -> %lx\n", x);
  return x;
}
#endif

#endif

void chpl_sync_lock(chpl_sync_aux_t * s) {
  enter_();
  myth_felock_lock(s->felock);
  return_from_();
}

void chpl_sync_unlock(chpl_sync_aux_t * s) {
  enter_();
  myth_felock_unlock(s->felock);
  return_from_();
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t * s,
                               int32_t lineno, int32_t filename) {
  enter_();
  myth_felock_wait_and_lock(s->felock, 1);
  return_from_();
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t * s,
                                int32_t lineno, int32_t filename) {
  enter_();
  myth_felock_wait_and_lock(s->felock, 0);
  return_from_();
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t * s) {
  enter_();
  myth_felock_mark_and_signal(s->felock, 1);
  return_from_();
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t * s) {
  enter_();
  myth_felock_mark_and_signal(s->felock, 0);
  return_from_();
}

chpl_bool chpl_sync_isFull(void * val_ptr, chpl_sync_aux_t * s) {
  chpl_bool x;
  enter_();
  x = myth_felock_status(s->felock);
  return_from_();
  return x;
}

void chpl_sync_initAux(chpl_sync_aux_t * s) {
  enter_();
  myth_felock_init(s->felock, 0);
  return_from_();
}

void chpl_sync_destroyAux(chpl_sync_aux_t * s) {
  enter_();
  myth_felock_destroy(s->felock);
  return_from_();
}


// Single variables (currently a synonym for syncs)

typedef chpl_sync_aux_t chpl_single_aux_t;


// Tasks

//
// chpl_task_init() is called by the main task on each locale to initialize
//   the tasking layer
//

void chpl_task_init(void) {
  size_t css;
  int _ = enter_();
  myth_globalattr_t attr[1];
  int r = myth_globalattr_init(attr);
  (void)_; (void)r;
  assert(r == 0);
  /* set call stack size */
  css = chpl_task_getEnvCallStackSize();
  if (css) {
    myth_globalattr_set_stacksize(attr, css);
  } else {
    /* chpl_task_getDefaultCallStackSize() generally returns
       a too larger value. we do not use it.
       this essentially uses MassiveThreads default stack size */
#if 0
    css = chpl_task_getDefaultCallStackSize();
    myth_globalattr_set_stacksize(attr, css);
#endif
  }
  myth_init_ex(attr);
  return_from_();
}

void chpl_task_exit(void) {
  // called by the main task
  enter_();
  myth_fini();
  return_from_();
}

//
// Have the tasking layer create a dedicated task to help the
// communication layer by running function 'fn' with argument 'arg'.
// This task should be quite dedicated (e.g., get its own system
// thread) in order to be responsive and not be held up by other
// user-level tasks. returns 0 on success, nonzero on failure.
//
int chpl_task_createCommTask(chpl_fn_p fn, void* arg) {
  // return chpl_thread_createCommThread(fn, arg);
  myth_create((myth_func_t)fn, arg);
  return 0;
}

static chpl_task_bundle_t main_bundle;

//
// Have the tasking layer call the 'chpl_main' function pointer
// representing the entry point for the user's Chapel code.  This
// can either be done by invoking the function directly or by creating
// a task that evaluates the function.
//
void chpl_task_callMain(void (*chpl_main)(void)) {
  enter_();
  chpl_main();
  return_from_();
}

//
// The following is an optional callback into the tasking layer from
// the main task indicating that the standard internal modules have
// been initialized.  It gives the tasking layer the ability to wait
// to make use of functionality in the internal modules (like the task
// tracking table) which are not yet available at the time of the call
// to chpl_task_callMain().
//
#ifndef CHPL_TASK_STD_MODULES_INITIALIZED
#define CHPL_TASK_STD_MODULES_INITIALIZED()
#endif


typedef struct chpl_task_list* chpl_task_list_p;

/* TODO: eliminate global task ID */
static chpl_taskID_t g_chpl_task_id = 0;
static chpl_taskID_t get_next_task_id(void) {
  return __sync_fetch_and_add(&g_chpl_task_id, 1);
}

void chpl_task_setSerial(chpl_bool state);

static void * myth_chpl_wrap(void * a_) {
  chpl_task_bundle_t * arg = myth_wsapi_get_hint_ptr(0);

  chpl_fn_int_t fid = arg->requested_fid;
  int32_t filename = arg->filename;
  int lineno = arg->lineno;
  chpl_taskID_t id = arg->id;
  chpl_bool is_executeOn = arg->is_executeOn;

  chpl_task_do_callbacks(chpl_task_cb_event_kind_begin,
                         fid,
                         filename,
                         lineno,
                         id,
                         is_executeOn);
  chpl_ftable[fid](arg);
  chpl_task_do_callbacks(chpl_task_cb_event_kind_end,
                         fid,
                         filename,
                         lineno,
                         id,
                         is_executeOn);
  return 0;
}

void myth_chpl_create(chpl_bool is_executeOn,
                      int lineno,
                      int32_t filename,
                      c_sublocid_t subloc,
                      chpl_fn_int_t fid,
                      chpl_taskID_t id,
                      chpl_task_bundle_t* arg, size_t arg_size);

void myth_chpl_create(chpl_bool is_executeOn,
                      int lineno,
                      int32_t filename,
                      c_sublocid_t subloc,
                      chpl_fn_int_t fid,
                      chpl_taskID_t id,
                      chpl_task_bundle_t* arg, size_t arg_size) {
  myth_thread_t th = 0;
  myth_thread_attr_t attr[1];
  chpl_task_do_callbacks(chpl_task_cb_event_kind_create,
                         fid,
                         filename,
                         lineno,
                         id,
                         is_executeOn);
  arg->is_executeOn = is_executeOn;
  arg->lineno = lineno;
  arg->filename = filename;
  arg->requestedSubloc = subloc;
  arg->requested_fid = fid;
  arg->requested_fn = chpl_ftable[fid];
  arg->id = id;
  //arg->arg_size = arg_size;
  assert(arg_size > 0);

  myth_thread_attr_init(attr);
  attr->custom_data_size = arg_size;
  attr->custom_data = arg;
  myth_create_ex(&th, attr, (myth_func_t)myth_chpl_wrap, 0);
}

//
// Task list processing.  These are called by the compiler-emitted
// code for all parallel constructs.  addToTaskList() is called for
// each task and builds a list of all of them.  processTaskList()
// actually adds the tasks to the task pool.  executeTasksInList()
// makes sure all the tasks have at least started.  freeTaskList()
// just reclaims space associated with the list.
//
void chpl_task_addToTaskList(
         chpl_fn_int_t fid,      // function to call for task
         //void* arg,              // argument to the function
         chpl_task_bundle_t* arg, size_t arg_size,
         c_sublocid_t subloc,       // desired sublocale
         void** p_task_list_void,             // task list
         c_nodeid_t task_list_locale,         // locale (node) where task list resides
         chpl_bool is_begin_stmt,          // is begin{} stmt?  (vs. cobegin or coforall)
         int lineno,                // line at which function begins
         int32_t filename) { // name of file containing function
  enter_();
  myth_chpl_create(/* is_executeOn = */ false,
                   lineno, filename,
                   subloc, fid, get_next_task_id(), arg, arg_size);
  return_from_();
}

void chpl_task_executeTasksInList(void** p_task_list_void) {
  enter_();
  return_from_();
}

//
// Call a function in a task.
//
void chpl_task_taskCallFTable(chpl_fn_int_t fid,          // function to call
                              //void* arg,              // function arg
                              chpl_task_bundle_t* arg,// function arg
                              size_t arg_size,        // length of arg
                              c_sublocid_t subloc,       // desired sublocale
                              int lineno,                // line at which function begins
                              int32_t filename) {           // name of file containing function
  enter_();
  myth_chpl_create(/* is_executeOn = */ false,
                   lineno, filename,
                   subloc, fid, get_next_task_id(), arg, arg_size);
  return_from_();
}

//
// Launch a task that is the logical continuation of some other task,
// but on a different locale.  This is used to invoke the body of an
// "on" statement.
//
void chpl_task_startMovedTask(chpl_fn_int_t fid,          // function to call
                              chpl_fn_p fp,
                              //void* arg,              // function arg
                              chpl_task_bundle_t* arg,// function arg
                              size_t arg_size, // length of arg in bytes
                              c_sublocid_t subloc,       // desired sublocale
                              chpl_taskID_t id      // task identifier
                             ) {
  enter_();
  myth_chpl_create(/* is_executeOn = */ true,
                   0, CHPL_FILE_IDX_UNKNOWN,
                   subloc, fid, id, arg, arg_size);
  return_from_();
}

//
// Get and set the current task's sublocale.  Setting the sublocale
// will actually move the task, if the specified sublocale differs
// from the current one.
//
#ifndef CHPL_TASK_GETSUBLOC_IMPL_DECL
c_sublocid_t chpl_task_getSubloc(void) {
  enter_();
  return_from_();
  return 0;
}
#endif

#ifndef CHPL_TASK_SETSUBLOC_IMPL_DECL
void chpl_task_setSubloc(c_sublocid_t subloc) {
  enter_();
  return_from_();
}
#endif

//
// Get the sublocale the caller specified at the time the task was created.
//
#ifndef CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL
c_sublocid_t chpl_task_getRequestedSubloc(void) {
  enter_();
  return_from_();
  return c_sublocid_any;
}
#endif

//
// Get ID.
//
chpl_taskID_t chpl_task_getId(void) {
  chpl_taskID_t tid;
  enter_();
  tid = (chpl_taskID_t)myth_self();
  return_from_();
  return tid;
}

chpl_bool chpl_task_idEquals(chpl_taskID_t id1, chpl_taskID_t id2) {
  return id1 == id2;
}

char* chpl_task_idToString(char* buff, size_t size, chpl_taskID_t id) {
  int ret = snprintf(buff, size, "%"PRIiPTR, id);
  if(ret>0 && ret<size)
    return buff;
  else
    return NULL;
}

//
// Yield.
//
void chpl_task_yield(void) {
  enter_();
  myth_yield();
  return_from_();
}

//
// Suspend.
//
static double cur_time(void) {
  struct timespec tp[1];
  clock_gettime(CLOCK_REALTIME, tp);
  return tp->tv_sec + tp->tv_nsec * 1.0e-9;
}

void chpl_task_sleep(double secs) {
  double t, end_t;
  enter_();
  t = cur_time();
  end_t = t + secs;
  while (t < end_t) {
    myth_yield();
    t = cur_time();
  }
  return_from_();
}

// The type for task private data, chpl_task_prvData_t,
// is defined in chpl-tasks-prvdata.h in order to support
// proper initialization order with a task model .h

// Get pointer to task private data.
#ifndef CHPL_TASK_GET_PRVDATA_IMPL_DECL
#if 0
chpl_task_prvData_t* chpl_task_getPrvData(void) {
  chpl_task_bundle_t * arg = myth_wsapi_get_hint_ptr(0);
  static chpl_task_prvData_t prvData[1] = { { .serial_state = false } };
  enter_();
  return_from_();
  return prvData;
}
#endif
#endif


chpl_task_bundle_t* chpl_task_getPrvBundle(void) {
  chpl_task_bundle_t * arg;
  enter_();
  arg = myth_wsapi_get_hint_ptr(0);
  if (arg == NULL)
    arg = &main_bundle;
  return_from_();
  return arg;
}


//
// Can this tasking layer support remote caching?
//
// (In practice this answers: "Are tasks bound to specific pthreads
// or, if not, does the tasking layer make memory consistency calls
// whenever it might move a task from one pthread to another?"  Remote
// caching uses pthread-specific data (TLS) extensively, so it turns
// itself off when it's used with a tasking layer that can't support
// that.)
//
#ifndef CHPL_TASK_SUPPORTS_REMOTE_CACHE_IMPL_DECL
int chpl_task_supportsRemoteCache(void) {
  enter_();
  return_from_();
  return 0;
}
#endif

//
// Returns the maximum width of parallelism the tasking layer expects
// to be able to provide on the calling (sub)locale.  With some
// exceptions for uncommon cases, this is the greatest parallel
// speedup a program should expect to achieve.  Running more active
// tasks than this will typically add overhead but not concurrency,
// and thus increase execution wall time rather than decrease it.
//
uint32_t chpl_task_getMaxPar(void) {
  uint32_t n;
  enter_();
  n = (uint32_t) chpl_getNumPhysicalCpus(true);
  return_from_();
  return n;
}

//
// Returns the number of sublocales the tasking layer knows about,
// within the span of hardware it is managing tasks on.
//
c_sublocid_t chpl_task_getNumSublocales(void) {
  return 0;
}

//
// returns the value of the call stack size limit being used in
// practice; the value returned may potentially differ from one locale
// to the next
//
size_t chpl_task_getCallStackSize(void) {
  size_t size;
  int r = myth_globalattr_get_stacksize(0, &size);
  (void)r;
  assert(r == 0);
  return size;
}

//
// returns the number of tasks that are ready to run on the current locale,
// not including any that have already started running.
//
uint32_t chpl_task_getNumQueuedTasks(void) {
  return 0;
}

//
// returns the number of tasks that are blocked waiting on a sync or single
// variable.
// Note that this information may only available if the program is run with
// the -b switch, which enables block reporting and deadlock detection.
// If this switch is not specified, -1 may be returned.
//
int32_t chpl_task_getNumBlockedTasks(void) {
  return 0;
}


// Threads

//
// returns the total number of threads that currently exist, whether running,
// blocked, or idle
//
uint32_t chpl_task_getNumThreads(void) {
  uint32_t n;
  enter_();
  n = myth_get_num_workers();
  return_from_();
  return n;
}

//
// returns the number of threads that are currently idle
//
uint32_t chpl_task_getNumIdleThreads(void) {
  return 0;
}


//
// This gets any per-locale thread count specified in the environment.
// It is common to all tasking implementations and so is implemented
// in runtime/src/chpl-tasks.c.
//
//int32_t chpl_task_getenvNumThreadsPerLocale(void) {
//
//}

//
// This returns any task call stack size specified in the environment.
// If the environment doesn't specify a call stack size, it returns 0.
// It is common to all tasking implementations and so is implemented
// in runtime/src/chpl-tasks.c.
//
//size_t chpl_task_getEnvCallStackSize(void) {
//
//}

//
// This returns the default task call stack size.
// It is common to all tasking implementations and so is implemented
// in runtime/src/chpl-tasks.c.
//
//size_t chpl_task_getDefaultCallStackSize(void) {
//
//}

//
// These are service functions provided to the runtime by the module
// code.
//

#include "chpl-tasks-callbacks.h"

#else // LAUNCHER

typedef void chpl_sync_aux_t;
typedef chpl_sync_aux_t chpl_single_aux_t;
#define chpl_task_exit()

#endif // LAUNCHER
