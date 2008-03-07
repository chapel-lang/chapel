#ifndef _chplthreads_h_
#define _chplthreads_h_

#include "chpl_cfg_threads.h"
#include <stdint.h>

// Defined in the generated Chapel code:

extern int32_t maxThreads;


// Mutexes
// (These are only needed in chplmem.c - perhaps _chpl_sync_lock and _chpl_sync_unlock
// should be used instead?)

typedef _chpl_mutex_t  *_chpl_mutex_p;

int _chpl_mutex_lock(_chpl_mutex_p);
void _chpl_mutex_unlock(_chpl_mutex_p);


// Sync variables

int _chpl_sync_lock(_chpl_sync_aux_t *);
void _chpl_sync_unlock(_chpl_sync_aux_t *);
int _chpl_sync_wait_full_and_lock(_chpl_sync_aux_t *, int32_t, _string);
int _chpl_sync_wait_empty_and_lock(_chpl_sync_aux_t *, int32_t, _string);
void _chpl_sync_mark_and_signal_full(_chpl_sync_aux_t *);     // also unlocks
void _chpl_sync_mark_and_signal_empty(_chpl_sync_aux_t *);    // also unlocks
_chpl_bool _chpl_sync_is_full(void *, _chpl_sync_aux_t *, _chpl_bool);
void _chpl_init_sync_aux(_chpl_sync_aux_t *);


// Single variables

int _chpl_single_lock(_chpl_single_aux_t *);
int _chpl_single_wait_full(_chpl_single_aux_t *, int32_t, _string);
void _chpl_single_mark_and_signal_full(_chpl_single_aux_t *); // also unlocks
_chpl_bool _chpl_single_is_full(void *, _chpl_single_aux_t *, _chpl_bool);
void _chpl_init_single_aux(_chpl_single_aux_t *);


//
// returns the default maximum number of threads that can be handled by this
// threading layer (initial value of maxThreads); use the sentinel value 0
// if the maximum number of threads is limited only by the system's available
// resources.
//
int32_t _chpl_threads_getMaxThreads(void);

//
// returns the upper limit on the maximum number of threads that can be handled
// by this threading layer; use the sentinel value 0 if the maximum number of
// threads is limited only by the system's available resources.
//
int32_t _chpl_threads_maxThreadsLimit(void);

// Chapel system thread control
void    initChplThreads(void);             // main thread init's thread support
void    exitChplThreads(void);             // called by the main thread

uint64_t _chpl_thread_id(void);            // return caller's thread id
void    _chpl_thread_init(void);           // setup per-thread state
_chpl_bool   _chpl_get_serial(void);       // set dynamic serial state
void    _chpl_set_serial(_chpl_bool);      // set dynamic serial state true or false


typedef void* (*_chpl_threadfp_t)(void*);  // function pointer
typedef void* _chpl_threadarg_t;           // function argument

// Fork one thread.  Do not wait.  Used to implement Chapel's begin statement.
// Return thread creation error.
int
_chpl_begin (_chpl_threadfp_t,             // function to fork
             _chpl_threadarg_t,            // function arg
             _Bool);                       // serial state (must be "false" except when
                                           // called from a comm lib such as gasnet;
                                           // otherwise, serial state is that of the
                                           // thread executing _chpl_begin)

#endif  // _chplthreads_h_
