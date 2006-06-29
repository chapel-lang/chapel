#ifndef _chplthreads_h_
#define _chplthreads_h_

// Pthread version of the Chapel thread interface
#include <pthread.h>

// mutexes
typedef pthread_mutex_t _chpl_mutex_t;

int _chpl_mutex_init( _chpl_mutex_t *mutex);
int _chpl_mutex_lock( _chpl_mutex_t *mutex);
int _chpl_mutex_trylock( _chpl_mutex_t *mutex);
int _chpl_mutex_unlock( _chpl_mutex_t *mutex);
int _chpl_mutex_destroy( _chpl_mutex_t *mutex);

// condition variables
typedef pthread_cond_t _chpl_condvar_t;

int _chpl_condvar_init( _chpl_condvar_t *cond);
int _chpl_condvar_destroy( _chpl_condvar_t *cond);
int _chpl_condvar_signal( _chpl_condvar_t *cond);
int _chpl_condvar_wait( _chpl_condvar_t *cond, _chpl_mutex_t *mutex);


// thread-related
typedef void* (*_chpl_threadfp_t)(void*);  // function pointer
typedef void* _chpl_threadarg_t;           // function argument
typedef pthread_attr_t _chpl_thread_attr_t;// thread attributes
typedef pthread_t _chpl_thread_t;          // pthread handle
typedef struct {                           // temporary work space
  _chpl_thread_t thread;                   // thread handle for join/wait
  int            error;                    // to store fork error code
} _chpl_cobegin_wkspace_t;


// Chapel system thread control
void initChplThreads( void);               // main thread init's thread support
void exitChplThreads( void);               // called by the main thread

// Fork and wait on nthreads. Used to implement Chapel's cobegin statement.
// Return aggregate thread join error.
int
_chpl_cobegin (int                      nthreads, // number of threads
               _chpl_threadfp_t        *fp,       // ptrs to functions to fork
               _chpl_threadarg_t       *a,        // function args
               _chpl_cobegin_wkspace_t *twrk);    // temporary work space

// Fork one thread.  Do not wait. Used to implement Chapel's begin statement.
// Return thread creation error.
int
_chpl_begin (_chpl_threadfp_t  fp,                // function to fork
             _chpl_threadarg_t a);                // function arg

#endif  // _chplthreads_h_
