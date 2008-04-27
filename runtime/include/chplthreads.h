#ifndef _chplthreads_h_
#define _chplthreads_h_

#include <stdint.h>
#include "arg.h"
#include "chpl_cfg_threads.h"

// Defined in the generated Chapel code:

extern int32_t maxThreads;

// Mutexes
// (These are only needed in chplmem.c - perhaps chpl_sync_lock and chpl_sync_unlock
// should be used instead?)

typedef chpl_mutex_t  *chpl_mutex_p;

int chpl_mutex_lock(chpl_mutex_p);
void chpl_mutex_unlock(chpl_mutex_p);


// Sync variables

int chpl_sync_lock(chpl_sync_aux_t *);
void chpl_sync_unlock(chpl_sync_aux_t *);
int chpl_sync_wait_full_and_lock(chpl_sync_aux_t *, int32_t, _string);
int chpl_sync_wait_empty_and_lock(chpl_sync_aux_t *, int32_t, _string);
void chpl_sync_mark_and_signal_full(chpl_sync_aux_t *);     // also unlocks
void chpl_sync_mark_and_signal_empty(chpl_sync_aux_t *);    // also unlocks
chpl_bool chpl_sync_is_full(void *, chpl_sync_aux_t *, chpl_bool);
void chpl_init_sync_aux(chpl_sync_aux_t *);


// Single variables

int chpl_single_lock(chpl_single_aux_t *);
void chpl_single_unlock(chpl_single_aux_t *);
int chpl_single_wait_full(chpl_single_aux_t *, int32_t, _string);
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

// Chapel system thread control
void      initChplThreads(void);           // main thread init's thread support
void      exitChplThreads(void);           // called by the main thread

uint64_t  chpl_thread_id(void);            // return caller's thread id
void      chpl_thread_init(void);          // setup per-thread state
chpl_bool chpl_get_serial(void);           // set dynamic serial state
void      chpl_set_serial(chpl_bool);      // set dynamic serial state true or false


typedef void* (*chpl_threadfp_t)(void*);   // function pointer
typedef void* chpl_threadarg_t;            // function argument

typedef struct chpl_task_list *chpl_task_list_p;

void chpl_add_to_task_list (chpl_threadfp_t, chpl_threadarg_t, chpl_task_list_p *);
void chpl_process_task_list (chpl_task_list_p);

// Fork one thread.  Do not wait.  Used to implement Chapel's begin statement.
// Return thread creation error.
int
chpl_begin (chpl_threadfp_t,  // function to fork
            chpl_threadarg_t, // function arg
            chpl_bool,        // ignore_serial = force spawning task
                              // regardless of serial state; as in the
                              // case of calling for on-statement
                              // implementation
            chpl_bool);       // serial state (must be "false" except
                              // when called from a comm lib such as
                              // gasnet; otherwise, serial state is
                              // that of the thread executing
                              // chpl_begin)

#endif  // _chplthreads_h_
