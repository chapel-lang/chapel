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

#ifndef _chpl_tasks_h_
#define _chpl_tasks_h_

#ifndef LAUNCHER

#include <stdint.h>
#include "chplcgfns.h"
#include "chpltypes.h"
#include "chpl-tasks-impl.h"
#include "chpl-tasks-prvdata.h"

// chpl-tasks-impl.h must define the task bundle header type,
// chpl_task_bundle_t.
typedef chpl_task_bundle_t* chpl_task_bundle_p;

#ifdef __cplusplus
extern "C" {
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


// Sync variables

void      chpl_sync_lock(chpl_sync_aux_t *);
void      chpl_sync_unlock(chpl_sync_aux_t *);
void      chpl_sync_waitFullAndLock(chpl_sync_aux_t *,
                                       int32_t, int32_t);
void      chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *,
                                        int32_t, int32_t);
void      chpl_sync_markAndSignalFull(chpl_sync_aux_t *);     // and unlock
void      chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *);    // and unlock
chpl_bool chpl_sync_isFull(void *, chpl_sync_aux_t *);
void      chpl_sync_initAux(chpl_sync_aux_t *);
void      chpl_sync_destroyAux(chpl_sync_aux_t *);


// Single variables (currently a synonym for syncs)

typedef chpl_sync_aux_t chpl_single_aux_t;

static inline
void chpl_single_lock(chpl_sync_aux_t * s) { chpl_sync_lock(s); }
static inline
void chpl_single_unlock(chpl_sync_aux_t * s) { chpl_sync_unlock(s); }
static inline
void chpl_single_waitFullAndLock(chpl_sync_aux_t * s,
                                 int32_t lineno, int32_t filename) {
  chpl_sync_waitFullAndLock(s,lineno,filename);
}
static inline
void chpl_single_markAndSignalFull(chpl_sync_aux_t * s) {
  chpl_sync_markAndSignalFull(s);
}
static inline
chpl_bool chpl_single_isFull(void *val_ptr, chpl_sync_aux_t *s) {
  return chpl_sync_isFull(val_ptr, s);
}
static inline
void chpl_single_initAux(chpl_sync_aux_t * s) { chpl_sync_initAux(s); }
static inline
void chpl_single_destroyAux(chpl_sync_aux_t * s) { chpl_sync_destroyAux(s); }


// Tasks

//
// chpl_task_init() is called by the main task on each locale to initialize
//   the tasking layer
//
void chpl_task_init(void);
void chpl_task_exit(void);        // called by the main task

//
// Have the tasking layer create a dedicated task to help the
// communication layer by running function 'fn' with argument 'arg'.
// This task should be quite dedicated (e.g., get its own system
// thread) in order to be responsive and not be held up by other
// user-level tasks. returns 0 on success, nonzero on failure.
//
// The caller of this function is responsible for ensuring that
// *arg remains available to the task as long as it is needed.
//
int chpl_task_createCommTask(chpl_fn_p fn, void* arg);

//
// Have the tasking layer call the 'chpl_main' function pointer
// representing the entry point for the user's Chapel code.  This
// can either be done by invoking the function directly or by creating
// a task that evaluates the function.
//
void chpl_task_callMain(void (*chpl_main)(void));

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

//
// Task list processing.  These are called by the compiler-emitted
// code for all parallel constructs.  addToTaskList() is called for
// each task and builds a list of all of them.  processTaskList()
// actually adds the tasks to the task pool.  executeTasksInList()
// makes sure all the tasks have at least started.  freeTaskList()
// just reclaims space associated with the list.
//
// Note that the tasking layer must generally copy the arguments
// as it cannot assume anything about the lifetime of that memory.
void chpl_task_addToTaskList(
         chpl_fn_int_t,      // function to call for task
         chpl_task_bundle_t*,// argument to the function
         size_t,             // length of the argument
         c_sublocid_t,       // desired sublocale
         void**,             // task list
         c_nodeid_t,         // locale (node) where task list resides
         chpl_bool,          // is begin{} stmt?  (vs. cobegin or coforall)
         int,                // line at which function begins
         int32_t);           // name of file containing function
void chpl_task_executeTasksInList(void**);

//
// Call a chpl_ftable[] function in a task.
//
// This is a convenience function for use by the module code, in which
// we have function table indices rather than function pointers.
//
// Note that the tasking layer must generally copy the arguments
// as it cannot assume anything about the lifetime of that memory.
//
void chpl_task_taskCallFTable(chpl_fn_int_t fid,      // ftable[] entry to call
                              chpl_task_bundle_t* arg,// function arg
                              size_t arg_size,        // length of arg
                              c_sublocid_t subloc,    // desired sublocale
                              int lineno,             // source line
                              int32_t filename);      // source filename

// In some cases, we are not worried about the "function number" (fid)

#define FID_NONE -1

//
// Launch a task that is the logical continuation of some other task,
// but on a different locale.  This is used to invoke the body of an
// "on" statement.
//
// Note that the tasking layer must generally copy the arguments
// as it cannot assume anything about the lifetime of that memory.
//
// The chpl_fn_int_t and chpl_fn_p arguments are stored into the
// task bundle as requested_fid and requested_fn respectively. If both
// are provided, the function pointer will be used. In this way,
// the comms layer can use task-wrapper functions.
void chpl_task_startMovedTask(chpl_fn_int_t,      // ftable[] entry
                              chpl_fn_p,          // function to call
                              chpl_task_bundle_t*,// function arg
                              size_t,             // length of arg in bytes
                              c_sublocid_t,       // desired sublocale
                              chpl_taskID_t      // task identifier
                             );

//
// Get and set the current task's sublocale.  Setting the sublocale
// will actually move the task, if the specified sublocale differs
// from the current one.
//
#ifndef CHPL_TASK_GETSUBLOC_IMPL_DECL
c_sublocid_t chpl_task_getSubloc(void);
#endif

#ifndef CHPL_TASK_SETSUBLOC_IMPL_DECL
void chpl_task_setSubloc(c_sublocid_t);
#endif

//
// Get the sublocale the caller specified at the time the task was created.
//
#ifndef CHPL_TASK_GETREQUESTEDSUBLOC_IMPL_DECL
c_sublocid_t chpl_task_getRequestedSubloc(void);
#endif

// For tasking layers that support task affinity/placement, this
// resets any automatic placement order
#ifndef CHPL_TASK_IMPL_RESET_SPAWN_ORDER
#define CHPL_TASK_IMPL_RESET_SPAWN_ORDER()
#endif
static inline
void chpl_task_reset_spawn_order(void) {
  CHPL_TASK_IMPL_RESET_SPAWN_ORDER();
}


//
// Get ID.
//
chpl_taskID_t chpl_task_getId(void);

//
// Checks whether two task IDs are the same
//
chpl_bool chpl_task_idEquals(chpl_taskID_t, chpl_taskID_t);

//
// Returns the string representation of task ID
// The string returned is the same buffer passed as argument
// In case of an error NULL is returned
//
char* chpl_task_idToString(
               char *,         //buffer on which ID is written
               size_t,         //length of the buffer in bytes
               chpl_taskID_t); //Task ID

//
// Yield.
//
void chpl_task_yield(void);

//
// Suspend.
//
void chpl_task_sleep(double);

// The type for task private data, chpl_task_prvData_t,
// is defined in chpl-tasks-prvdata.h in order to support
// proper initialization order with a task model .h

// Get pointer to task private data.
#ifndef CHPL_TASK_GET_PRVDATA_IMPL_DECL
chpl_task_prvData_t* chpl_task_getPrvData(void);
#endif

#ifndef CHPL_TASK_GET_PRVBUNDLE_IMPL_DECL
chpl_task_bundle_t* chpl_task_getPrvBundle(void);
#endif

// Get the Chapel module-code managed task private data portion
// of a task bundle.
static inline
chpl_task_ChapelData_t* chpl_task_getBundleChapelData(chpl_task_bundle_t* b)
{
  // this code assumes each chpl_task_bundle_t has a state field
  // of type chpl_task_ChapelData_t.
  return &b->state;
}

//
// Get Chapel module-code managed task private data
//
static inline
chpl_task_ChapelData_t* chpl_task_getChapelData(void)
{
  chpl_task_bundle_t* prv = chpl_task_getPrvBundle();
  return chpl_task_getBundleChapelData(prv);
}


//
// Returns the maximum width of parallelism the tasking layer expects
// to be able to provide on the calling (sub)locale.  With some
// exceptions for uncommon cases, this is the greatest parallel
// speedup a program should expect to achieve.  Running more active
// tasks than this will typically add overhead but not concurrency,
// and thus increase execution wall time rather than decrease it.
//
uint32_t chpl_task_getMaxPar(void);

//
// returns the value of the call stack size limit being used in
// practice; the value returned may potentially differ from one locale
// to the next
//
size_t chpl_task_getCallStackSize(void);

//
// This returns whether guard pages (stack checks) are in use
//
chpl_bool chpl_task_guardPagesInUse(void);

//
// returns the number of tasks that are ready to run on the current locale,
// not including any that have already started running.
//
uint32_t chpl_task_getNumQueuedTasks(void);

//
// returns the number of tasks that are blocked waiting on a sync or single
// variable.
// Note that this information may only available if the program is run with
// the -b switch, which enables block reporting and deadlock detection.
// If this switch is not specified, -1 may be returned.
//
int32_t chpl_task_getNumBlockedTasks(void);


// Threads

//
// If the tasking layer runs tasks on a fixed number of threads, this
// returns the number of such threads.  Otherwise it returns 0 (zero).
// As examples, for CHPL_TASKS=qthreads it returns the number of worker
// threads, while for CHPL_TASKS=fifo it returns 0.  If this is called
// prior to tasking layer initialization the result is unpredictable.
//
#ifndef CHPL_TASK_IMPL_GET_FIXED_NUM_THREADS
#define CHPL_TASK_IMPL_GET_FIXED_NUM_THREADS() 0
#endif
static inline
uint32_t chpl_task_getFixedNumThreads(void) {
  return CHPL_TASK_IMPL_GET_FIXED_NUM_THREADS();
}

//
// If the tasking layer runs tasks on a fixed number of threads and
// the calling thread is one of those, this returns true.  Otherwise,
// it returns false.
//
#ifndef CHPL_TASK_IMPL_IS_FIXED_THREAD
#define CHPL_TASK_IMPL_IS_FIXED_THREAD() 0
#endif
static inline
uint32_t chpl_task_isFixedThread(void) {
  return CHPL_TASK_IMPL_IS_FIXED_THREAD();
}

//
// If the tasking layer will always execute tasks on the same thread
// they started on, this returns true.  Otherwise, it returns false.
// For example CHPL_TASKS=fifo a task is a thread, so tasks can't
// migrate.  For CHPL_TASKS=qthreads some schedulers support
// work-stealing where tasks can be stolen and moved to a different
// thread than they started on.
//
#ifndef CHPL_TASK_IMPL_CAN_MIGRATE_THREADS
#define CHPL_TASK_IMPL_CAN_MIGRATE_THREADS() 1
#endif
static inline
uint32_t chpl_task_canMigrateThreads(void) {
  return CHPL_TASK_IMPL_CAN_MIGRATE_THREADS();
}

//
// returns the total number of threads that currently exist, whether running,
// blocked, or idle
//
uint32_t chpl_task_getNumThreads(void);

//
// returns the number of threads that are currently idle
//
uint32_t chpl_task_getNumIdleThreads(void);


//
// This gets any per-locale thread count specified in the environment.
// It is common to all tasking implementations and so is implemented
// in runtime/src/chpl-tasks.c.
//
int32_t chpl_task_getenvNumThreadsPerLocale(void);

//
// This returns any task call stack size specified in the environment.
// If the environment doesn't specify a call stack size, it returns 0.
// It is common to all tasking implementations and so is implemented
// in runtime/src/chpl-tasks.c.
//
size_t chpl_task_getEnvCallStackSize(void);

//
// This returns the default task call stack size.
// It is common to all tasking implementations and so is implemented
// in runtime/src/chpl-tasks.c.
//
size_t chpl_task_getDefaultCallStackSize(void);

//
// These are service functions provided to the runtime by the module
// code.
//
extern void chpl_taskRunningCntInc(int64_t _ln, int32_t _fn);
extern void chpl_taskRunningCntDec(int64_t _ln, int32_t _fn);
extern void chpl_taskRunningCntReset(int64_t _ln, int32_t _fn);

#ifdef __cplusplus
} // end extern "C"
#endif

#include "chpl-tasks-callbacks.h"

#else // LAUNCHER

#ifdef __cplusplus
extern "C" {
#endif

typedef void chpl_sync_aux_t;
typedef chpl_sync_aux_t chpl_single_aux_t;
#define chpl_task_exit()

#ifdef __cplusplus
} // end extern "C"
#endif

#endif // LAUNCHER

#endif
