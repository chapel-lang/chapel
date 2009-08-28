#ifndef _chplthreads_h_
#define _chplthreads_h_

#ifndef LAUNCHER

#include <stdint.h>
#include "arg.h"
#include "chpl_cfg_threads.h"

// Defined in the generated Chapel code:

extern int32_t maxThreads;

// Mutexes
// (These are only needed in chpl_mem.c - perhaps chpl_sync_lock and chpl_sync_unlock
// should be used instead?)

typedef chpl_mutex_t* chpl_mutex_p;

void chpl_mutex_init(chpl_mutex_p mutex);
int chpl_mutex_lock(chpl_mutex_p);
void chpl_mutex_unlock(chpl_mutex_p);


// Sync variables

int chpl_sync_lock(chpl_sync_aux_t *);
void chpl_sync_unlock(chpl_sync_aux_t *);
int chpl_sync_wait_full_and_lock(chpl_sync_aux_t *, int32_t, chpl_string);
int chpl_sync_wait_empty_and_lock(chpl_sync_aux_t *, int32_t, chpl_string);
void chpl_sync_mark_and_signal_full(chpl_sync_aux_t *);     // also unlocks
void chpl_sync_mark_and_signal_empty(chpl_sync_aux_t *);    // also unlocks
chpl_bool chpl_sync_is_full(void *, chpl_sync_aux_t *, chpl_bool);
void chpl_init_sync_aux(chpl_sync_aux_t *);
void chpl_destroy_sync_aux(chpl_sync_aux_t *);


// Single variables

int chpl_single_lock(chpl_single_aux_t *);
void chpl_single_unlock(chpl_single_aux_t *);
int chpl_single_wait_full(chpl_single_aux_t *, int32_t, chpl_string);
void chpl_single_mark_and_signal_full(chpl_single_aux_t *); // also unlocks
chpl_bool chpl_single_is_full(void *, chpl_single_aux_t *, chpl_bool);
void chpl_init_single_aux(chpl_single_aux_t *);


//
// returns the default maximum number of threads that can be handled by this
// threading layer (initial value of maxThreads); use the sentinel value 0
// if the maximum number of threads is limited only by the system's available
// resources.
//
int32_t   chpl_threads_getMaxThreads(void);

//
// returns the upper limit on the maximum number of threads that can be handled
// by this threading layer; use the sentinel value 0 if the maximum number of
// threads is limited only by the system's available resources.
//
int32_t   chpl_threads_maxThreadsLimit(void);

//
// returns the total number of threads that currently exist, whether running,
// blocked, or idle
//
uint32_t  chpl_numThreads(void);

//
// returns the number of threads that are currently idle
//
uint32_t  chpl_numIdleThreads(void);

//
// returns the number of tasks that are ready to run on the current locale,
// not including any that have already started running.
//
uint32_t  chpl_numQueuedTasks(void);

//
// returns the number of tasks that are running on the current locale,
// including any that may be blocked waiting for something.
// Note that the value returned could be larger than the limit on the maximum
// number of threads, since a thread could be "suspended," particularly if it
// is waiting at the end of a cobegin, e.g.  In this case, it could be
// executing a task inside the cobegin, so in effect the same thread would be
// executing more than one task.
//
uint32_t  chpl_numRunningTasks(void);

//
// returns the number of tasks that are blocked waiting on a sync or single
// variable.
// Note that this information may only available if the program is run with
// the -b switch, which enables block reporting and deadlock detection.
// If this switch is not specified, -1 may be returned.
//
int32_t chpl_numBlockedTasks(void);


// Chapel system thread control
void      initChplThreads(void);           // main thread init's thread support
void      exitChplThreads(void);           // called by the main thread

chpl_threadID_t  chpl_thread_id(void);       // return caller's thread id
void             chpl_thread_init(void);     // setup per-thread state
chpl_bool        chpl_get_serial(void);      // get dynamic serial state
void             chpl_set_serial(chpl_bool); // set dynamic serial state true or false

void             chpl_thread_cancel(chpl_threadID_t); // ask thread to terminate
void             chpl_thread_join(chpl_threadID_t);   // wait for thread termination

typedef struct chpl_task_list*   chpl_task_list_p;
typedef struct chpl_pool_struct* chpl_task_pool_p;

void chpl_add_to_task_list(
    chpl_fn_int_t fid,           // function to call for task
    void* arg,                   // argument to the function
    chpl_task_list_p *task_list,
    int32_t task_list_locale,    // locale where task list resides
    chpl_bool call_chpl_begin,   // whether to call chpl_begin
    int lineno,
    chpl_string filename);
void chpl_process_task_list (chpl_task_list_p);
void chpl_execute_tasks_in_list (chpl_task_list_p);
void chpl_free_task_list (chpl_task_list_p);

// Fork one thread.  Do not wait.  Used to implement Chapel's begin statement.
// Returns a pointer to the task pool entry.
void
chpl_begin(chpl_fn_p,         // function to fork
           void*,             // function arg
           chpl_bool,         // ignore_serial = force spawning task regardless
                              // of serial state; as in the case of calling
                              // for on-statement implementation
           chpl_bool,         // serial state (must be "false" except when
                              // called from a comm lib such as gasnet;
                              // otherwise, serial state is that of the
                              // thread executing chpl_begin)
           chpl_task_list_p);

#else // LAUNCHER

#define chpl_mutex_init(x)
#define chpl_mutex_lock(x)
#define chpl_mutex_unlock(x)
typedef void chpl_sync_aux_t;
typedef void chpl_single_aux_t;
typedef int chpl_mutex_t;
#define exitChplThreads()

#endif // LAUNCHER

#endif
