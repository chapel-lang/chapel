// MTA implementation

#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"
#include <stdint.h>

#if 1 //def MTA_DEBUG
#include <stdio.h>
#endif


typedef struct {
  _chpl_threadfp_t  fun;                   // function to fork as a new thread
  _chpl_threadarg_t arg;                   // argument for the created thread
} _chpl_createarg_t;


// The global vars are to synchronize with threads created with 
// begin's which are not joined.  We need to wait on them before the
// main thread can call exit for the process.
static _int64          _chpl_begin_cnt;      // number of unjoined threads 
static _int64          _chpl_can_exit;       // can main thread exit?
#if 0
static pthread_key_t   _chpl_serial;         // per-thread serial state
#endif


// Mutex

_chpl_mutex_p _chpl_mutex_new(void) {
#if 0
  fprintf(stderr, "_chpl_mutex_new() not implemented!\n");
  return 0;
#else
  _chpl_mutex_p m;
  m = (_chpl_mutex_p) _chpl_alloc(sizeof(_chpl_mutex_t), "mutex", 0, 0);
  _chpl_mutex_init(m);
  return m;
#endif
}

int _chpl_mutex_init(_chpl_mutex_p mutex) {
#if 0
  fprintf(stderr, "_chpl_mutex_init() not implemented!\n");
  return 0;
#else
  if (mutex) {
    purge(mutex);                     // set to zero and mark as empty
    return 0;
  } else
    return 1;                         // mutex is null
#endif
}

int _chpl_mutex_lock(_chpl_mutex_p mutex) {
#if 0
  fprintf(stderr, "_chpl_mutex_lock() not implemented!\n");
  return 0;
#else
  if (mutex) {
#ifdef MTA_DEBUG
    fprintf(stderr, "locking mutex %p\n", mutex);
#endif
    writeef(mutex, 1);                // set to one and mark as full
    return 0;
  } else
    return 1;                         // mutex is null
#endif
}

int _chpl_mutex_trylock(_chpl_mutex_p mutex) {
#if 1
  fprintf(stderr, "_chpl_mutex_trylock() not implemented!\n");
  return 0;
#else
  return pthread_mutex_trylock(mutex);
#endif
}

int _chpl_mutex_unlock(_chpl_mutex_p mutex) {
#if 0
  fprintf(stderr, "_chpl_mutex_unlock() not implemented!\n");
  return 0;
#else
#ifdef MTA_DEBUG
  fprintf(stderr, "unlocking mutex %p\n", mutex);
#endif
  return _chpl_mutex_init(mutex);
#endif
}

int _chpl_mutex_destroy(_chpl_mutex_p mutex) {
#if 0
  fprintf(stderr, "_chpl_mutex_destroy() not implemented!\n");
  return 0;
#else
  if (mutex) {
    _chpl_free(mutex, 0, 0);
    return 0;
  } else
    return 1;                         // mutex is null
#endif
}


// Condition variables

_chpl_condvar_p _chpl_condvar_new(void) {
#if 0
  fprintf(stderr, "_chpl_condvar_new() not implemented!\n");
  return 0;
#else
  _chpl_condvar_p cv;
  cv = (_chpl_condvar_p) _chpl_alloc(sizeof(_chpl_condvar_t), "condition var", 0, 0);
  _chpl_condvar_init(cv);
  return cv;
#endif
}

int _chpl_condvar_init (_chpl_condvar_p cond) {
#if 0
  fprintf(stderr, "_chpl_condvar_init() not implemented!\n");
  return 0;
#else
  if (cond) {
#ifdef MTA_DEBUG
    fprintf(stderr, "creating condvar %p\n", cond);
#endif
    purge(cond);                      // set to zero and mark as empty
    return 0;
  } else
    return 1;                         // cond is null
#endif
}

int _chpl_condvar_destroy(_chpl_condvar_p cond) {
#if 0
  fprintf(stderr, "_chpl_condvar_destroy() not implemented!\n");
  return 0;
#else
  if (cond) {
    _chpl_free(cond, 0, 0);
    return 0;
  } else
    return 1;                         // cond is null
#endif
}

int _chpl_condvar_signal(_chpl_condvar_p cond) {
#if 0
  fprintf(stderr, "_chpl_condvar_signal() not implemented!\n");
  return 0;
#else
  if (cond) {
#ifdef MTA_DEBUG
    fprintf(stderr, "signaling condvar %p\n", cond);
#endif
    writexf(cond, 1);                 // set to one and mark as full
    return 0;
  } else
    return 1;                         // cond is null
#endif
}

int _chpl_condvar_broadcast(_chpl_condvar_p cond) {
#if 1
  fprintf(stderr, "_chpl_condvar_broadcast() not implemented!\n");
  return 0;
#else
  return pthread_cond_broadcast(cond);
#endif
}

int _chpl_condvar_wait(_chpl_condvar_p cond, _chpl_mutex_p mutex) {
#if 0
  fprintf(stderr, "_chpl_condvar_wait() not implemented!\n");
  return 0;
#else
  if (cond) {
    int ready=0;
#ifdef MTA_DEBUG
  fprintf(stderr, "about to unlock mutex %p\n", mutex);
#endif
    _chpl_mutex_unlock(mutex);
    do
#ifdef MTA_DEBUG
{  fprintf(stderr, "waiting for condvar %p using mutex %p\n", cond, mutex);
#endif
      // this will block until cond is marked full!
      ready = readfe(cond);
#ifdef MTA_DEBUG
      fprintf(stderr, "ready = %d\n", ready);}
#endif
    while (!ready);
#if 0 // unlocking needs to be done sooner!
  fprintf(stderr, "about to unlock mutex %p\n", mutex);
    _chpl_mutex_unlock(mutex);
  fprintf(stderr, "About to return after waiting for condvar %p and unlocking mutex %p\n", cond, mutex);
#endif
    return 0;
  } else
    return 1;                         // cond is null
#endif
}



void _chpl_serial_delete(_bool *p) {
#if 1
  fprintf(stderr, "_chpl_serial_delete() not implemented!\n");
#else
  if (NULL != p) {
    _chpl_free(p, 0, 0);
  }
#endif
}


// Threads

void initChplThreads() {
  _chpl_begin_cnt = 0;                     // only main thread running
  purge(&_chpl_can_exit);                  // set to zero and mark as empty

#if 0
  _chpl_mutex_init(&_memtrack_lock);
  _chpl_mutex_init(&_memstat_lock);
  _chpl_mutex_init(&_memtrace_lock);

  pthread_key_create(&_chpl_serial, (void(*)(void*))_chpl_serial_delete);
  _chpl_thread_init();
#endif
}


void exitChplThreads() {
  int ready=0;
#ifdef MTA_DEBUG
  fprintf(stderr, "Inside exitChplThreads()\n");
#endif
  do
    // this will block until _chpl_can_exit is marked full!
    ready = readff(&_chpl_can_exit);
  while (!ready);
#ifdef MTA_DEBUG
  fprintf(stderr, "About to return from exitChplThreads(); _chpl_can_exit=%d\n",
	  _chpl_can_exit);
#endif
}


void _chpl_thread_init(void) {
#if 1
#ifdef MTA_DEBUG
  fprintf(stderr, "_chpl_thread_init() not implemented!\n");
#endif
#else
  _bool *p;
  p = pthread_getspecific(_chpl_serial);
  if (NULL == p) {
    p = (_bool*) _chpl_alloc(sizeof(_bool), "serial flag", 0, 0);
    *p = false;
    pthread_setspecific(_chpl_serial, p);
  }
#endif
}


_uint64 _chpl_thread_id(void) {
#if 1
#ifdef MTA_DEBUG
  fprintf(stderr, "_chpl_thread_id() not implemented!\n");
#endif
  return 0;
#else
  return (intptr_t) pthread_self();
#endif
}


_bool _chpl_get_serial(void) {
#if 1
  return true;
#else
  _bool *p;
  p = (_bool*) pthread_getspecific(_chpl_serial);
  if (NULL == p) {
    _printInternalError("serial state not created");
  }
  return *p;
#endif
}


void _chpl_set_serial(_bool state) {
#if 1
  fprintf(stderr, "_chpl_set_serial() not implemented!\n");
#else
  _bool *p;
  p = (_bool*) pthread_getspecific(_chpl_serial);
  if (NULL != p) {
    *p = state;
  }
#endif
}


int _chpl_cobegin (int                      nthreads, 
                   _chpl_threadfp_t        *fps,
                   _chpl_threadarg_t       *args, 
                   _chpl_cobegin_wkspace_t *twrk) {
  int               t, retv = 0;
  _int64            finished[nthreads];

  if (_chpl_get_serial()) {
#if 0   // _chpl_get_serial is not yet implemented correctly!
    for (t=0; t<nthreads; t++)
      (*fps[t])(args[t]);
  } else {
#endif
    // create threads
    for (t=0; t<nthreads; t++, fps++, args++) {
      future void thread$;
      _int64 *fin = &finished[t];

      purge(fin);           // set to zero and mark empty
      future thread$(fps, args, fin, t) {
        (**fps)(*args, t);
        *fin = 1;           // set to one and mark full
      }
    }

    // wait on those threads
    for (t=0; t<nthreads; t++)
      retv += finished[t];  // block until the corresponding thread finishes
    retv = nthreads - retv; // return zero if all threads finished
  }

  return retv;
}


// Similar to _chpl_cobegin above, be we do not wait on the launched
// thread.  Also we only expect one thread to launch with a begin block.
int
_chpl_begin (_chpl_threadfp_t fp, _chpl_threadarg_t arg) {
  if (_chpl_get_serial()) {
#if 0   // _chpl_get_serial is not yet implemented correctly!
    (*fp)(arg);
  } else {
#endif
    int init_begin_cnt;
    //future void thread$;

#ifdef MTA_DEBUG
    fprintf(stderr, "Inside _chpl_begin\n");
#endif
    purge(&_chpl_can_exit);                     // set to zero and mark as empty
    init_begin_cnt =
      int_fetch_add(&_chpl_begin_cnt, 1);       // assume begin will succeed

    // Will call the real begin statement function. Only purpose of this
    // thread is to wait on that function and coordinate the exiting
    // of the main Chapel thread.
    future /*thread$*/(fp, arg, init_begin_cnt) {
      _int64         begin_cnt;

#ifdef MTA_DEBUG
      fprintf(stderr, "Inside future no. %d\n", init_begin_cnt);
#endif
      (*fp)(arg, init_begin_cnt);

      // decrement begin thread count and see if we can signal Chapel exit
      begin_cnt = int_fetch_add(&_chpl_begin_cnt, -1);
#ifdef MTA_DEBUG
      fprintf(stderr, "begin_cnt = %d\n", begin_cnt);
#endif
      if (begin_cnt == 1)  // i.e., _chpl_begin_cnt is now zero
        _chpl_can_exit = 1; // mark this variable as being full
#ifdef MTA_DEBUG
      fprintf(stderr, "About to exit future no. %d; begin_cnt = %d\n",
	      init_begin_cnt, begin_cnt);
#endif
    }
  }

#ifdef MTA_DEBUG
  fprintf(stderr, "About to return from _chpl_begin\n");
#endif
  return 0;
}
