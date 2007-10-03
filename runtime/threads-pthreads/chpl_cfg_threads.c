// Pthread implementation

#include <stdint.h>
#include <pthread.h>
#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"


typedef struct {
  _chpl_threadfp_t  fun;                   // function to fork as a new thread
  _chpl_threadarg_t arg;                   // argument for the created thread
} _chpl_createarg_t;


// The global vars are to synchronize with threads created with 
// begin's which are not joined.  We need to wait on them before the
// main thread can call exit for the process.
static _chpl_mutex_t   _chpl_begin_cnt_lock; // protect _chpl_begin_cnt
static int             _chpl_begin_cnt;      // number of unjoined threads 
static _chpl_condvar_t _chpl_can_exit;       // can main thread exit?
static pthread_key_t   _chpl_serial;         // per-thread serial state


// Mutex

_chpl_mutex_p _chpl_mutex_new(void) {
  _chpl_mutex_p m;
  m = (_chpl_mutex_p) _chpl_alloc(sizeof(_chpl_mutex_t), "mutex", 0, 0);
  _chpl_mutex_init(m);
  return m;
}

int _chpl_mutex_init(_chpl_mutex_p mutex) {
  // WAW: how to explicitly specify blocking-type?
  return pthread_mutex_init(mutex, NULL);
}

int _chpl_mutex_lock(_chpl_mutex_p mutex) {
  return pthread_mutex_lock(mutex);
}

int _chpl_mutex_trylock(_chpl_mutex_p mutex) {
  return pthread_mutex_trylock(mutex);
}

int _chpl_mutex_unlock(_chpl_mutex_p mutex) {
  return pthread_mutex_unlock(mutex);
}

int _chpl_mutex_destroy(_chpl_mutex_p mutex) {
  return pthread_mutex_destroy(mutex);
}


// Condition variables

_chpl_condvar_p _chpl_condvar_new(void) {
  _chpl_condvar_p cv;
  cv = (_chpl_condvar_p) _chpl_alloc(sizeof(_chpl_condvar_t), "condition var", 0, 0);
  _chpl_condvar_init(cv);
  return cv;
}

int _chpl_condvar_init (_chpl_condvar_p cond) {
  // WAW: attributes?
  return pthread_cond_init(cond, NULL);
}

int _chpl_condvar_destroy(_chpl_condvar_p cond) {
  return pthread_cond_destroy(cond);
}

int _chpl_condvar_signal(_chpl_condvar_p cond) {
  return pthread_cond_signal(cond);
}

int _chpl_condvar_broadcast(_chpl_condvar_p cond) {
  return pthread_cond_broadcast(cond);
}

int _chpl_condvar_wait(_chpl_condvar_p cond, _chpl_mutex_p mutex) {
  return pthread_cond_wait(cond, mutex);
}



void _chpl_serial_delete(_bool *p) {
  if (NULL != p) {
    _chpl_free(p, 0, 0);
  }
}


// Threads

void initChplThreads() {
  _chpl_mutex_init(&_chpl_begin_cnt_lock);
  _chpl_begin_cnt = 0;                          // only main thread running
  _chpl_condvar_init(&_chpl_can_exit);

  _chpl_mutex_init(&_memtrack_lock);
  _chpl_mutex_init(&_memstat_lock);
  _chpl_mutex_init(&_memtrace_lock);

  pthread_key_create(&_chpl_serial, (void(*)(void*))_chpl_serial_delete);
  _chpl_thread_init();
}


void exitChplThreads() {
  _chpl_mutex_lock(&_chpl_begin_cnt_lock);
  if (_chpl_begin_cnt > 0) {
    // block until everyone else is finished
    _chpl_condvar_wait(&_chpl_can_exit, &_chpl_begin_cnt_lock);
  }
  _chpl_mutex_unlock(&_chpl_begin_cnt_lock);

  pthread_key_delete(_chpl_serial);
}


void _chpl_thread_init(void) {
  _bool *p;
  p = pthread_getspecific(_chpl_serial);
  if (NULL == p) {
    p = (_bool*) _chpl_alloc(sizeof(_bool), "serial flag", 0, 0);
    *p = false;
    pthread_setspecific(_chpl_serial, p);
  }
}


_uint64 _chpl_thread_id(void) {
  return (intptr_t) pthread_self();
}


_bool _chpl_get_serial(void) {
  _bool *p;
  p = (_bool*) pthread_getspecific(_chpl_serial);
  if (NULL == p) {
    _printInternalError("serial state not created");
  }
  return *p;
}


void _chpl_set_serial(_bool state) {
  _bool *p;
  p = (_bool*) pthread_getspecific(_chpl_serial);
  if (NULL != p) {
    *p = state;
  }
}


int _chpl_cobegin (int                      nthreads, 
                   _chpl_threadfp_t        *fp, 
                   _chpl_threadarg_t       *a, 
                   _chpl_cobegin_wkspace_t *twrk) {
  int               t, retv;
  void             *fn_retv;                         // drop?
  retv = 0;

  if (_chpl_get_serial()) {
    for (t=0; t<nthreads; t++) {
      (*fp[t])(a[t]);
    }
  } else {
    // fork pthreads
    for (t=0; t<nthreads; t++) {
      if ((twrk[t].error = pthread_create(&(twrk[t].thread), 
                                          NULL, 
                                          fp[t], 
                                          a[t]))) {
        _printInternalError("too many threads");
      }
      retv |= twrk[t].error;
    }

    // wait on those fork'd
    for (t=0; t<nthreads; t++) {
      if (!twrk[t].error) {                // if we fork'd successfully
        twrk[t].error = pthread_join(twrk[t].thread, (void*) &fn_retv);
        retv |= twrk[t].error;
      }
    }
  }
  return retv;
}


// Will call the real begin statement function. Only purpose of this
// helper thread is to wait on that function and coordinate the exiting
// of the main Chapel thread.
static void
_chpl_begin_helper (_chpl_createarg_t *nt) {

  (*nt->fun)(nt->arg);

  // decrement begin thread count and see if we can signal Chapel exit
  _chpl_mutex_lock(&_chpl_begin_cnt_lock);
  _chpl_begin_cnt--;
  if (_chpl_begin_cnt == 0) {
    _chpl_condvar_signal(&_chpl_can_exit);
  }
  _chpl_mutex_unlock(&_chpl_begin_cnt_lock);

  _chpl_free(nt, 0, 0);

}


// Similar to _chpl_cobegin above, be we do not wait on the forked
// thread.  Also we only expect one thread to fork with a begin block.
int
_chpl_begin (_chpl_threadfp_t fp, _chpl_threadarg_t a) {
  int                 error;
  _chpl_thread_t      thread;
  _chpl_createarg_t  *nt;

  error = 0;
  if (_chpl_get_serial()) {
    (*fp)(a);
  } else {
    _chpl_mutex_lock(&_chpl_begin_cnt_lock);
    _chpl_begin_cnt++;                     // assume begin will succeed
    _chpl_mutex_unlock(&_chpl_begin_cnt_lock);

    nt = (_chpl_createarg_t*) _chpl_malloc(1, sizeof(_chpl_createarg_t), "_chpl_begin helper arg", 0, 0);
    nt->fun = fp;
    nt->arg = a;
    error |= pthread_create(&thread, NULL, (_chpl_threadfp_t) _chpl_begin_helper, nt);
    if (error)
      _printInternalError("too many threads");
    pthread_detach(thread);
  }
  return error;
}
