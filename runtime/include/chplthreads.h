#ifndef _chplthreads_h_
#define _chplthreads_h_

// Pthread version of the Chapel thread interface
#include <pthread.h>

#define LOCK_NAME    "_mutex"
#define REFC_NAME    "_refc"

// mutexes
typedef pthread_mutex_t _chpl_mutex_t;
typedef _chpl_mutex_t  *_chpl_mutex_p;

_chpl_mutex_p _chpl_mutex_new( void);      // malloc and init a mutex
int _chpl_mutex_init( _chpl_mutex_p mutex);
int _chpl_mutex_lock( _chpl_mutex_p mutex);
int _chpl_mutex_trylock( _chpl_mutex_p mutex);
int _chpl_mutex_unlock( _chpl_mutex_p mutex);
int _chpl_mutex_destroy( _chpl_mutex_p mutex);

// condition variables
typedef pthread_cond_t    _chpl_condvar_t;
typedef _chpl_condvar_t  *_chpl_condvar_p;

_chpl_condvar_p _chpl_condvar_new( void);  // malloc and init a condvar
int _chpl_condvar_init( _chpl_condvar_p cond);
int _chpl_condvar_destroy( _chpl_condvar_p cond);
int _chpl_condvar_signal( _chpl_condvar_p cond);
int _chpl_condvar_wait( _chpl_condvar_p cond, _chpl_mutex_p mutex);


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


// macros to init, touch, and free reference-counted Chapel variables
#define _CHPL_REFC_TOUCH(_var, _refc, _mutex)           \
  _chpl_mutex_lock( &(_mutex));                         \
  (_refc)++;                                            \
  _chpl_mutex_unlock( &(_mutex))

#define _CHPL_REFC_FREE(_var, _refc, _mutex)            \
  _chpl_mutex_lock( &(_mutex));                         \
  (_refc)--;                                            \
  if ((_refc) == 0) {                                   \
    _chpl_free( &(_var));                               \
    _chpl_free( &(_refc));                              \
    _chpl_mutex_unlock( &(_mutex));                     \
    _chpl_mutex_destroy( &(_mutex));                    \
    _chpl_free( &(_mutex));                             \
  } else {                                              \
    _chpl_mutex_unlock( &(_mutex));                     \
  }



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



/*
  _var##_cv_full  = _chpl_alloc( sizeof( _chpl_condvar_t), "cv full");
  _var##_cv_empty = _chpl_alloc( sizeof( _chpl_condvar_t), "cv empty");
*/

#ifdef _PIGS_FLY_
// Synch variable sttribute support
// WAW: stuff broken include
//  - records (need to do aggregate read/assign
//  - function parameter passing

// initialize metadata needed for synch vars
#define _CHPL_SYNC_INIT(_svar)                                          \
  _chpl_mutex_init( &(_svar ## _lock));                                 \
  _chpl_condvar_init( &(_svar ## _cv_full));                            \
  _chpl_condvar_init( &(_svar ## _cv_empty));                           \
  _svar ## _is_empty = false;


// do not wait, set empty=true
// WAW: is this correct?  what about the value of _svar?
#define _CHPL_SYNC_PURGE(_svar, _expr)                                  \
  _chpl_mutex_lock( &(_svar ## _lock));                                 \
  _svar ## _is_empty = true;                                            \
  _chpl_condvar_signal( &(_svar ## _cv_empty));                         \
  _chpl_mutex_unlock( &(_svar ## _lock))


// wait for full, assign to _expr the value of _svar, set empty=true
#define _CHPL_SYNC_READ_LEAVE_EMPTY(_svar, _expr)                       \
  _chpl_mutex_lock( &(_svar ## _lock));                                 \
  if (_svar ## _is_empty) {                                             \
    _chpl_condvar_wait( &(_svar ## _cv_full) , &(_svar ## _lock));      \
  }                                                                     \
  _svar ## _is_empty = true;                                            \
  _chpl_condvar_signal( &(_svar ## _cv_empty));                         \
  (_expr) = (_svar);                                                    \
  _chpl_mutex_unlock( &(_svar ## _lock))


// wait for full, assign to _expr the value of _svar, leave full (empty=false)
#define _CHPL_SYNC_READ_LEAVE_FULL(_svar, _expr)                        \
  _chpl_mutex_lock( &(_svar ## _lock));                                 \
  if (_svar ## _is_empty) {                                             \
    _chpl_condvar_wait( &(_svar ## _cv_full) , &(_svar ## _lock));      \
  }                                                                     \
  _svar ## _is_empty = false;                                           \
  _chpl_condvar_signal( &(_svar ## _cv_full));                          \
  (_expr) = (_svar);                                                    \
  _chpl_mutex_unlock( &(_svar ## _lock))


// do not wait, assign to _svar the value of _expr, leave full (empty=false)
#define _CHPL_SYNC_WRITE(_svar, _expr)                                  \
  _chpl_mutex_lock( &(_svar ## _lock));                                 \
  _svar ## _is_empty = false;                                           \
  _chpl_condvar_signal( &(_svar ## _cv_full));                          \
  (_svar) = (_expr);                                                    \
  _chpl_mutex_unlock( &(_svar ## _lock))


// wait for empty, assign to _svar the value of _expr, leave full (empty=false)
#define _CHPL_SYNC_WRITE_LEAVE_FULL(_svar, _expr)                       \
  _chpl_mutex_lock( &(_svar ## _lock));                                 \
  if (!(_svar ## _is_empty)) {                                          \
    _chpl_condvar_wait( &(_svar ## _cv_empty) , &(_svar ## _lock));     \
  }                                                                     \
  _svar ## _is_empty = false;                                           \
  _chpl_condvar_signal( &(_svar ## _cv_full));                          \
  (_svar) = (_expr);                                                    \
  _chpl_mutex_unlock( &(_svar ## _lock))

#endif  /* _PIGS_FLY_ */

#endif  // _chplthreads_h_
