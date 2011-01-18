#ifndef _chpltasks_h_
#define _chpltasks_h_

#ifndef LAUNCHER

#include <stdint.h>
#include "arg.h"
#ifdef CHPL_TASKS_H
#include CHPL_TASKS_H
#endif


// Defined in the generated Chapel code:


// Sync variables

void      chpl_sync_lock(chpl_sync_aux_t *);
void      chpl_sync_unlock(chpl_sync_aux_t *);
void      chpl_sync_wait_full_and_lock(chpl_sync_aux_t *,
                                       int32_t, chpl_string);
void      chpl_sync_wait_empty_and_lock(chpl_sync_aux_t *,
                                        int32_t, chpl_string);
void      chpl_sync_mark_and_signal_full(chpl_sync_aux_t *);     // and unlock
void      chpl_sync_mark_and_signal_empty(chpl_sync_aux_t *);    // and unlock
chpl_bool chpl_sync_is_full(void *, chpl_sync_aux_t *, chpl_bool);
void      chpl_init_sync_aux(chpl_sync_aux_t *);
void      chpl_destroy_sync_aux(chpl_sync_aux_t *);


// Single variables

void      chpl_single_lock(chpl_single_aux_t *);
void      chpl_single_unlock(chpl_single_aux_t *);
void      chpl_single_wait_full(chpl_single_aux_t *, int32_t, chpl_string);
void      chpl_single_mark_and_signal_full(chpl_single_aux_t *); // and unlock
chpl_bool chpl_single_is_full(void *, chpl_single_aux_t *, chpl_bool);
void      chpl_init_single_aux(chpl_single_aux_t *);
void      chpl_destroy_single_aux(chpl_single_aux_t *);


// Tasks

//
// chpl_tasking_init() is called by the main task on each locale to initialize
//   the tasking layer
//   - maxThreadsPerLocale is the maximum number of threads the tasking
//     layer should use; 0 means unlimited
//   - callStackSize is the size of the callstack each task should use;
//     0 means use the system default
// These values should be checked for legality and tucked away for later use
// by the tasking layer as necessary.  This is a reasonable place to print
// out warnings about bad values.
//
void chpl_tasking_init(int32_t maxThreadsPerLocale, uint64_t callStackSize);
void chpl_tasking_exit(void);        // called by the main task

// tasking init for any threads created outside of the tasking/threading layer
void chpl_per_pthread_tasking_init(void);

//
// Have the tasking layer call the 'chpl_main' function pointer
// representing the entry point for the user's Chapel code.  This
// can either be done by invoking the function directly or by creating
// a task that evaluates the function.
//
void chpl_tasking_call_main(void (*chpl_main)(void));

typedef struct chpl_task_list* chpl_task_list_p;

void chpl_add_to_task_list(
         chpl_fn_int_t,      // function to call for task
         void*,              // argument to the function
         chpl_task_list_p*,  // task list
         int32_t,            // locale where task list resides
         chpl_bool,          // whether to call chpl_begin
         int,                // line at which function begins
         chpl_string);       // name of file containing functions
void chpl_process_task_list(chpl_task_list_p);
void chpl_execute_tasks_in_list(chpl_task_list_p);
void chpl_free_task_list(chpl_task_list_p);

// Fork one task.  Do not wait.  Used to implement Chapel's begin statement.
void chpl_begin(
         chpl_fn_p,         // function to fork
         void*,             // function arg
         chpl_bool,         // ignore_serial = force spawning task regardless
                            // of serial state; as in the case of calling
                            // for on-statement implementation
         chpl_bool,         // serial state (must be "false" except when
                            // called from a comm lib such as gasnet;
                            // otherwise, serial state is that of the
                            // task executing chpl_begin)
         chpl_task_list_p);

//
// Get ID.
//
chpl_taskID_t chpl_task_id(void);

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
chpl_bool chpl_get_serial(void);
void      chpl_set_serial(chpl_bool);

//
// returns the value of the call stack size limit being used in
// practice; the value returned may potentially differ from one locale
// to the next
//
uint64_t chpl_task_callstacksize(void);

//
// returns the number of tasks that are ready to run on the current locale,
// not including any that have already started running.
//
uint32_t chpl_numQueuedTasks(void);

//
// returns the number of tasks that are running on the current locale,
// including any that may be blocked waiting for something.
// Note that the value returned could be larger than the limit on the maximum
// number of threads, since a thread could be "suspended," particularly if it
// is waiting at the end of a cobegin, e.g.  In this case, it could be
// executing a task inside the cobegin, so in effect the same thread would be
// executing more than one task.
//
uint32_t chpl_numRunningTasks(void);

//
// returns the number of tasks that are blocked waiting on a sync or single
// variable.
// Note that this information may only available if the program is run with
// the -b switch, which enables block reporting and deadlock detection.
// If this switch is not specified, -1 may be returned.
//
int32_t chpl_numBlockedTasks(void);


// Threads

//
// returns the default maximum number of threads that can be handled by this
// threading layer (initial value of maxThreadsPerLocale); use the sentinel value 0
// if the maximum number of threads is limited only by the system's available
// resources.
//
int32_t chpl_threads_getMaxThreads(void);

//
// returns the upper limit on the maximum number of threads that can be handled
// by this threading layer; use the sentinel value 0 if the maximum number of
// threads is limited only by the system's available resources.
//
int32_t chpl_threads_maxThreadsLimit(void);

//
// returns the total number of threads that currently exist, whether running,
// blocked, or idle
//
uint32_t chpl_numThreads(void);

//
// returns the number of threads that are currently idle
//
uint32_t chpl_numIdleThreads(void);

#else // LAUNCHER

typedef void chpl_sync_aux_t;
typedef void chpl_single_aux_t;
#define chpl_tasking_exit()

#endif // LAUNCHER

#endif
