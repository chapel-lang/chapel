#ifndef _chplthreads_h_
#define _chplthreads_h_

// Pthread version of the Chapel thread interface
#include <pthread.h>
#include "chpltypes.h"

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
int _chpl_condvar_broadcast( _chpl_condvar_p cond);
int _chpl_condvar_wait( _chpl_condvar_p cond, _chpl_mutex_p mutex);

// thread-related
typedef void* (*_chpl_threadfp_t)(void*);  // function pointer
typedef void* _chpl_threadarg_t;           // function argument
typedef pthread_attr_t _chpl_thread_attr_t;// thread attributes
typedef pthread_t _chpl_thread_t;          // thread handle
typedef struct {                           // temporary work space
  _chpl_thread_t thread;                   // thread handle for join/wait
  int            error;                    // to store fork error code
} _chpl_cobegin_wkspace_t;


// Chapel system thread control
void    initChplThreads(void);             // main thread init's thread support
void    exitChplThreads(void);             // called by the main thread

_uint64 _chpl_thread_id(void);             // return caller's thread id
void    _chpl_thread_init(void);           // setup per-thread state
_bool   _chpl_get_serial(void);            // set dynamic serial state
void    _chpl_set_serial(_bool state);     // set dynamic serial state true or false
void    _chpl_serial_delete(_bool *p);


// macros to init, touch, and free reference-counted Chapel variables
#define _CHPL_REFC_INIT(_var, _refc, _mutex)            \
  *_refc = 0;                                           \
  _chpl_mutex_init(_mutex)

#define _CHPL_REFC_TOUCH(_var, _refc, _mutex)           \
  _chpl_mutex_lock(_mutex);                             \
  (*_refc)++;                                           \
  _chpl_mutex_unlock(_mutex)

#define _CHPL_REFC_FREE(_var, _refc, _mutex, ln, fn)    \
  _chpl_mutex_lock(_mutex);                             \
  (*_refc)--;                                           \
  if ((_refc) == 0) {                                   \
    _chpl_free( &(_var), ln, fn);                       \
    _chpl_free(_refc, ln, fn);                          \
    _chpl_mutex_unlock(_mutex);                         \
    _chpl_mutex_destroy(_mutex);                        \
    _chpl_free(_mutex, ln, fn);                         \
  } else {                                              \
    _chpl_mutex_unlock(_mutex);                         \
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


#endif  // _chplthreads_h_
