/*
 * Copyright 2004-2015 Cray Inc.
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


// Sync variables

void      chpl_sync_lock(chpl_sync_aux_t *);
void      chpl_sync_unlock(chpl_sync_aux_t *);
void      chpl_sync_waitFullAndLock(chpl_sync_aux_t *,
                                       int32_t, c_string);
void      chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *,
                                        int32_t, c_string);
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
                                 int32_t lineno, c_string filename) {
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
void chpl_task_addToTaskList(
         chpl_fn_int_t,      // function to call for task
         void*,              // argument to the function
         c_sublocid_t,       // desired sublocale
         chpl_task_list_p*,  // task list
         c_nodeid_t,         // locale (node) where task list resides
         chpl_bool,          // is begin{} stmt?  (vs. cobegin or coforall)
         int,                // line at which function begins
         c_string);          // name of file containing functions
void chpl_task_processTaskList(chpl_task_list_p);
void chpl_task_executeTasksInList(chpl_task_list_p);
void chpl_task_freeTaskList(chpl_task_list_p);

//
// Launch a task that is the logical continuation of some other task,
// but on a different locale.  This is used to invoke the body of an
// "on" statement.
//
void chpl_task_startMovedTask(chpl_fn_p,          // function to call
                              void*,              // function arg
                              c_sublocid_t,       // desired sublocale
                              chpl_taskID_t,      // task identifier
                              chpl_bool);         // serial state

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

//
// Get ID.
//
chpl_taskID_t chpl_task_getId(void);

//
// Yield.
//
void chpl_task_yield(void);

//
// Suspend.
//
void chpl_task_sleep(int);

//
// Get and set dynamic serial state.
//
chpl_bool chpl_task_getSerial(void);
void      chpl_task_setSerial(chpl_bool);

// The type for task private data, chpl_task_prvData_t,
// is defined in chpl-tasks-prvdata.h in order to support
// proper initialization order with a task model .h

// Get pointer to task private data.
#ifndef CHPL_TASK_GET_PRVDATA_IMPL_DECL
chpl_task_prvData_t* chpl_task_getPrvData(void);
#endif

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
// Returns the number of sublocales the tasking layer knows about,
// within the span of hardware it is managing tasks on.
//
c_sublocid_t chpl_task_getNumSublocales(void);

//
// returns the value of the call stack size limit being used in
// practice; the value returned may potentially differ from one locale
// to the next
//
size_t chpl_task_getCallStackSize(void);

//
// returns the number of tasks that are ready to run on the current locale,
// not including any that have already started running.
//
uint32_t chpl_task_getNumQueuedTasks(void);

//
// returns the number of tasks that are running on the current locale,
// including any that may be blocked waiting for something.
// Note that the value returned could be larger than the limit on the maximum
// number of threads, since a thread could be "suspended," particularly if it
// is waiting at the end of a cobegin, e.g.  In this case, it could be
// executing a task inside the cobegin, so in effect the same thread would be
// executing more than one task.
//
uint32_t chpl_task_getNumRunningTasks(void);

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
extern void chpl_taskRunningCntInc(int64_t _ln, c_string _fn);
extern void chpl_taskRunningCntDec(int64_t _ln, c_string _fn);

//
// Tasking callback support.
//
// NAME
//
//   chpl_task_install_callback   - install tasking callback function
//   chpl_task_uninstall_callback - remove tasking callback function
//
//
// SYNOPSIS
//
//     #include "chpl-tasks.h"
//
//     int chpl_task_install_callback(chpl_task_cb_kind_t kind,
//                                    chpl_task_cb_fn_t cb_fn);
//     int chpl_task_uninstall_callback(chpl_task_cb_kind_t kind,
//                                      chpl_task_cb_fn_t cb_fn);
//
//
// DESCRIPTION
//
//   These runtime functions are used to install and remove functions
//   called when certain events occur in the runtime tasking layer.
//
//   The 'kind' argument specifies the tasking layer event that is of
//   interest.  The 'cb_fn' argument is a pointer to a callback function
//   for that event.  The callback functions installed for each event
//   will be called, in the order in which they were installed, each
//   time the corresponding event occurs in the tasking layer of the
//   runtime.
//
//   The type of 'kind', chpl_task_cb_kind_t, is defined something like
//   this:
//
//     typedef enum {
//       chpl_task_cb_kind_task_begin,
//       chpl_task_cb_kind_task_end
//     } chpl_task_cb_kind_t;
//
//   The type of a tasking layer callback function pointer is:
//
//     typedef void (*chpl_task_cb_fn_t)(chpl_task_cb_kind_t kind,
//                                       chpl_task_cb_info_t* info);
//
//   The first argument tells the kind of event the callback represents.
//   The second is a pointer to more information about the event, held in a
//   value of this type:
//
//     typedef union {
//       struct {                  // for chpl_task_cb_kind_task_begin:
//         int version;            //   version of this info: 1
//         char* filename;         //   task is defined in this source file
//         int lineno;             //   task is defined at this source line
//         chpl_taskID_t id;       //   unique (within top-level locale) ID
//         bool is_executeOn;      //   true: task is for executeOn body
//       } begin_info;
//       struct {                  // for chpl_task_cb_kind_task_end:
//         int version;            //   version of this info: 1
//         chpl_taskID_t id;       //   unique (within top-level locale) ID
//       } end_info;
//     } chpl_task_cb_info_t;
//
//
// RETURN VALUE
//
//   The returned value is 0 if no errors occurred and some other value
//   (with errno set) if any errors did occur.
//
//
// ERRORS
//
//   The following errors can occur with chpl_task_install_callback():
//
//     ENOMEM:  No room to install another callback function for this
//              event.  At present there is a static limit of 10 installed
//              callback functions for each event.
//     ERANGE:  The specified 'kind' is too large.
//
//   The following errors can occur with chpl_task_uninstall_callback():
//
//     ENOENT:  The given pointer was not found in the list of installed
//              callback functions for the given event.
//     ERANGE:  The specified 'kind' is too large.
//
//
// NOTES
//
//   The callback function should not depend upon the allocation status of
//   the pointed-to info or any member within it, past the point when it
//   returns.  In particular, it should not store a copy of the pointer
//   passed to it when kind==chpl_task_cb_kind_task_begin, for use when
//   the corresponding chpl_task_cb_kind_task_end call is made.  It should
//   also not store a copy of the filename pointer from a 'begin' call, to
//   be used in the 'end' call.  If the info as a whole or the filename are
//   wanted later, the callback function must allocate memory to hold a
//   copy of the pointed-to data and duplicate it itself.
//
//   These functions are not thread safe.  Calling them simultaneously from
//   more than one thread can corrupt internal data structures and lead to
//   chaos.
//
typedef enum {
  chpl_task_cb_kind_task_begin,
  chpl_task_cb_kind_task_end,
  chpl_task_num_cb_kinds
} chpl_task_cb_kind_t;

typedef union {
  struct {                  // for chpl_task_cb_kind_task_begin:
    int version;            //   version of this info: 1
    const char* filename;   //   task is defined in this source file
    int lineno;             //   task is defined at this source line
    chpl_taskID_t id;       //   unique (within top-level locale) ID
    chpl_bool is_executeOn; //   true: task is for executeOn body
  } begin_info;
  struct {                  // for chpl_task_cb_kind_task_end:
    int version;            //   version of this info: 1
    chpl_taskID_t id;       //   unique (within top-level locale) ID
  } end_info;
} chpl_task_cb_info_t;

typedef void (*chpl_task_cb_fn_t)(chpl_task_cb_kind_t,
                                  const chpl_task_cb_info_t*);

int chpl_task_install_callback(chpl_task_cb_kind_t,
                               chpl_task_cb_fn_t);
int chpl_task_uninstall_callback(chpl_task_cb_kind_t,
                                 chpl_task_cb_fn_t);

#else // LAUNCHER

typedef void chpl_sync_aux_t;
typedef chpl_sync_aux_t chpl_single_aux_t;
#define chpl_task_exit()

#endif // LAUNCHER

#endif
