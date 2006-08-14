// Pthread implementation

#include <stdint.h>
#include <pthread.h>
#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"

typedef struct {
  _chpl_threadfp_t  fun;                   // function to fork as a new thread
  _chpl_threadarg_t arg;                   // argument for the created thread
} _chpl_newthread_t;


// The global vars are to synchronize with threads created with 
// begin's which are not joined.  We need to wait on them before the
// main thread can call exit.
static _chpl_mutex_t   _chpl_begin_cnt_lock;   // protect _chpl_begin_cnt
static int             _chpl_begin_cnt;        // number of unjoined threads 
static _chpl_condvar_t _chpl_can_exit;         // can main thread exit?

// Mutex
_chpl_mutex_p 
_chpl_mutex_new( void) {
  _chpl_mutex_p m;
  m = (_chpl_mutex_p) _chpl_alloc( sizeof( _chpl_mutex_t), "mutex");
  _chpl_mutex_init( m);
  return m;
}


int 
_chpl_mutex_init( _chpl_mutex_p mutex) {
  // WAW: how to explicitly specify blocking-type?
  return pthread_mutex_init( mutex, NULL);
}


int
_chpl_mutex_lock( _chpl_mutex_p mutex) {
  return pthread_mutex_lock( mutex);
}


int 
_chpl_mutex_trylock( _chpl_mutex_p mutex) {
  return pthread_mutex_trylock( mutex);
}


int
_chpl_mutex_unlock( _chpl_mutex_p mutex) {
  return pthread_mutex_unlock( mutex);
}


int  
_chpl_mutex_destroy( _chpl_mutex_p mutex) {
  return pthread_mutex_destroy( mutex);
}


// Condition variables
_chpl_condvar_p 
_chpl_condvar_new( void) {
  _chpl_condvar_p cv;
  cv = (_chpl_condvar_p) _chpl_alloc( sizeof( _chpl_condvar_t), "condition var");
  _chpl_condvar_init( cv);
  return cv;
}

int
_chpl_condvar_init( _chpl_condvar_p cond) {
  // WAW: attributes?
  return pthread_cond_init( cond, NULL);
}


int
_chpl_condvar_destroy( _chpl_condvar_p cond) {
  return pthread_cond_destroy( cond);
}


int
_chpl_condvar_signal( _chpl_condvar_p cond) {
  return pthread_cond_signal( cond);
}


int
_chpl_condvar_wait( _chpl_condvar_p cond, _chpl_mutex_p mutex) {
  return pthread_cond_wait( cond, mutex);
}


// Threads
void 
initChplThreads() {
  _chpl_mutex_init( &_chpl_begin_cnt_lock);
  _chpl_begin_cnt = 0;                          // only main thread running

  _chpl_condvar_init( &_chpl_can_exit);
}


void 
exitChplThreads() {
  _chpl_mutex_lock( &_chpl_begin_cnt_lock);
  if (_chpl_begin_cnt > 0) {
    // block until everyone else is finished
    _chpl_condvar_wait( &_chpl_can_exit, &_chpl_begin_cnt_lock);
  }
  _chpl_mutex_unlock( &_chpl_begin_cnt_lock);
}


_uint64
_chpl_thread_id( void) {
  return (intptr_t) pthread_self();
}


int
_chpl_cobegin (int                      nthreads, 
               _chpl_threadfp_t        *fp, 
               _chpl_threadarg_t       *a, 
               _chpl_cobegin_wkspace_t *twrk) {
  int   t, retv;
  void *fn_retv;                         // drop?
  retv = 0;

  // fork pthreads
  for (t=0; t<nthreads; t++) {
    if ((twrk[t].error = pthread_create ( &(twrk[t].thread), 
                                          NULL, 
                                          fp[t], 
                                          a[t]))) {
      // break; // do not abort for now?
    }
    retv |= twrk[t].error;
  }

  // wait on those fork'd
  for (t=0; t<nthreads; t++) {
    if (!twrk[t].error) {                // if we fork'd successfully
      twrk[t].error = pthread_join ( twrk[t].thread, (void*) &fn_retv);
      retv |= twrk[t].error;
     }
  }
  return retv;
}


// Will fork the real begin statement function. Only purpose of this
// helper thread is to wait on that thread and coordinate the exiting
// of the main Chapel thread.
static void
_chpl_begin_helper( _chpl_newthread_t *nt) {
  _chpl_thread_t thread;
  void          *fn_retv;

  pthread_create( &thread, NULL, nt->fun, nt->arg);
  pthread_join( thread, (void*) &fn_retv);

  // decrement begin thread count and see if we can signal Chapel exit
  _chpl_mutex_lock( &_chpl_begin_cnt_lock);
  _chpl_begin_cnt--;
  if (_chpl_begin_cnt == 0) {
    _chpl_condvar_signal( &_chpl_can_exit);
  }
  _chpl_mutex_unlock( &_chpl_begin_cnt_lock);

  _chpl_free( nt);

}


// Similar to _chpl_cobegin above, be we do not wait on the forked
// thread.  Also we only expect one thread to fork with a
// begin block.
//
// Because we will need to indirectly join with the forked thread,
// we will actually create two threads.  The first will be the
// intended Chapel-level begin.  The second will join with the first
// and then call the Chapel thread exit coordination code.  
int
_chpl_begin( _chpl_threadfp_t fp, _chpl_threadarg_t a) {
  int                 error;
  _chpl_thread_t      thread;
  _chpl_thread_attr_t attr;
  _chpl_newthread_t  *nt;

  error = 0;

  _chpl_mutex_lock( &_chpl_begin_cnt_lock);
  _chpl_begin_cnt++;                     // assume begin will succeed
  _chpl_mutex_unlock( &_chpl_begin_cnt_lock);

  nt = (_chpl_newthread_t*) _chpl_malloc( 1, 
                                          sizeof( _chpl_newthread_t), 
                                          "_chpl_begin helper arg");
  nt->fun = fp;
  nt->arg = a;

  // fork pthread
  error  = pthread_attr_init( &attr);
  error |= pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED);
  error |= pthread_create ( &thread, 
                            &attr, 
                            (_chpl_threadfp_t) _chpl_begin_helper, 
                            nt);
  error |= pthread_attr_destroy( &attr);

  return error;
}
