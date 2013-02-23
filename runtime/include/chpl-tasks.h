#ifndef _chpl_tasks_h_
#define _chpl_tasks_h_

#ifndef LAUNCHER

#include <stdint.h>
#include "chpltypes.h"

#ifdef CHPL_TASKS_MODEL_H
#include CHPL_TASKS_MODEL_H
#endif


// Defined in the generated Chapel code:


// Sync variables

void      chpl_sync_lock(chpl_sync_aux_t *);
void      chpl_sync_unlock(chpl_sync_aux_t *);
void      chpl_sync_waitFullAndLock(chpl_sync_aux_t *,
                                       int32_t, chpl_string);
void      chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *,
                                        int32_t, chpl_string);
void      chpl_sync_markAndSignalFull(chpl_sync_aux_t *);     // and unlock
void      chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *);    // and unlock
chpl_bool chpl_sync_isFull(void *, chpl_sync_aux_t *, chpl_bool);
void      chpl_sync_initAux(chpl_sync_aux_t *);
void      chpl_sync_destroyAux(chpl_sync_aux_t *);


// Single variables (currently a synonym for syncs)

typedef chpl_sync_aux_t chpl_single_aux_t;

static ___always_inline
void chpl_single_lock(chpl_sync_aux_t * s) { chpl_sync_lock(s); }
static ___always_inline
void chpl_single_unlock(chpl_sync_aux_t * s) { chpl_sync_unlock(s); }
static ___always_inline
void chpl_single_waitFullAndLock(chpl_sync_aux_t * s,
                                 int32_t lineno, chpl_string filename) {
  chpl_sync_waitFullAndLock(s,lineno,filename);
}
static ___always_inline
void chpl_single_markAndSignalFull(chpl_sync_aux_t * s) {
  chpl_sync_markAndSignalFull(s);
}
static ___always_inline
chpl_bool chpl_single_isFull(void *val_ptr, chpl_sync_aux_t *s,
                                           chpl_bool simple_sync_var) {
  return chpl_sync_isFull(val_ptr, s, simple_sync_var);
}
static ___always_inline
void chpl_single_initAux(chpl_sync_aux_t * s) { chpl_sync_initAux(s); }
static ___always_inline
void chpl_single_destroyAux(chpl_sync_aux_t * s) { chpl_sync_destroyAux(s); }


// Tasks

//
// chpl_task_init() is called by the main task on each locale to initialize
//   the tasking layer
//
// - numThreadsPerLocale is a user-specified limit on the number of
//   threads the tasking layer should use; 0 means unlimited
// - maxThreadsPerLocale is the maximum number of threads the tasking
//   layer should use; 0 means unlimited
// - numCommTasks indicates the number of communication tasks that the
//   communication layer is going to request through chpl_task_createCommTask()
// - callStackSize is the user-specified size of the callstack each
//   task should use; 0 means use the default
//
// These values should be checked for legality and tucked away for later use
// by the tasking layer as necessary.  This is a reasonable place to print
// out warnings about bad values.
//
void chpl_task_init(int32_t numThreadsPerLocale, int32_t maxThreadsPerLocale,
                    int numCommTasks, uint64_t callStackSize);
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
// been initialized.  It gives the tasking layer the ability to make
// use of functionality in the internal modules (like the task
// tracking table) which are not yet available in
// chpl_task_callMain().
//
#ifndef CHPL_TASK_STD_MODULES_INITIALIZED
#define CHPL_TASK_STD_MODULES_INITIALIZED()
#endif


typedef struct chpl_task_list* chpl_task_list_p;

void chpl_task_addToTaskList(
         chpl_fn_int_t,      // function to call for task
         void*,              // argument to the function
         chpl_task_list_p*,  // task list
         c_nodeid_t,         // locale (node) where task list resides
         chpl_bool,          // whether to call chpl_task_begin
         int,                // line at which function begins
         chpl_string);       // name of file containing functions
void chpl_task_processTaskList(chpl_task_list_p);
void chpl_task_executeTasksInList(chpl_task_list_p);
void chpl_task_freeTaskList(chpl_task_list_p);

// Fork one task.  Do not wait.  Used to implement Chapel's begin statement.
void chpl_task_begin(
         chpl_fn_p,         // function to fork
         void*,             // function arg
         chpl_bool,         // ignore_serial = force spawning task regardless
                            // of serial state; as in the case of calling
                            // for on-statement implementation
         chpl_bool,         // serial state (must be "false" except when
                            // called from a comm lib such as gasnet;
                            // otherwise, serial state is that of the
                            // task executing chpl_task_begin)
         chpl_task_list_p);

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

//
// Get and set task-specific localeID information.
//
c_locale_t  chpl_task_getLocaleID(void);
void        chpl_task_setLocaleID(c_locale_t);

//
// returns the value of the call stack size limit being used in
// practice; the value returned may potentially differ from one locale
// to the next
//
uint64_t chpl_task_getCallStackSize(void);

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

#else // LAUNCHER

typedef void chpl_sync_aux_t;
typedef chpl_sync_aux_t chpl_single_aux_t;
#define chpl_task_exit()

#endif // LAUNCHER

#endif
