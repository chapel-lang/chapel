// MTA implementation

#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"
#include <stdint.h>
#include <stdlib.h>

#if MTA_DEBUG
#include <stdio.h>
#endif


// The global vars are to synchronize with threads created with 
// begin's which are not joined.  We need to wait on them before the
// main thread can call exit for the process.
static _int64          _chpl_begin_cnt;      // number of unjoined threads 
static sync _int64     _chpl_can_exit;       // can main thread exit?
#if 0
static pthread_key_t   _chpl_serial;         // per-thread serial state
#endif


// Mutex

int _chpl_mutex_init(_chpl_mutex_p mutex) {
  if (mutex) {
    purge(mutex);                     // set to zero and mark empty
    return 0;
  } else
    return 1;                         // mutex is null
}

int _chpl_mutex_lock(_chpl_mutex_p mutex) {
  if (mutex) {
    writeef(mutex, 1);                // set to one and mark full
    return 0;
  } else
    return 1;                         // mutex is null
}

int _chpl_mutex_unlock(_chpl_mutex_p mutex) {
  return _chpl_mutex_init(mutex);
}

int _chpl_mutex_destroy(_chpl_mutex_p mutex) {
  return _chpl_mutex_lock(mutex);     // lock it so no thread can use it
}

int _chpl_sync_lock(_chpl_sync_aux_t *s) {
#ifdef MTA_DEBUG
  fprintf(stderr, "In %s, s = %p\n", __func__, s);
#endif
  if (s) {
    readfe(&(s->is_full));            // mark empty
    return 0;
  } else return 1;                    // s is null
}

int _chpl_sync_unlock(_chpl_sync_aux_t *s) {
#ifdef MTA_DEBUG
  fprintf(stderr, "In %s, s = %p\n", __func__, s);
#endif
  if (s) {
    _int64 is_full = readxx(&(s->is_full));
    writeef(&(s->is_full), is_full);  // mark full
    return 0;
  } else return 1;                    // s is null
}


#if 0
// Condition variables

_chpl_condvar_p _chpl_condvar_new(void) {
#if 1
  fprintf(stderr, "%s() not implemented!\n", __func__);
  return 0;
#else
  _chpl_condvar_p cv;
  cv = (_chpl_condvar_p) _chpl_alloc(sizeof(_chpl_condvar_t), "condition var", 0, 0);
  _chpl_condvar_init(cv);
  return cv;
#endif
}

int _chpl_condvar_init (_chpl_condvar_p cond) {
#if 1
  fprintf(stderr, "%s() not implemented!\n", __func__);
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
#if 1
  fprintf(stderr, "%s() not implemented!\n", __func__);
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
#if 1
  fprintf(stderr, "%s() not implemented!\n", __func__);
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
  fprintf(stderr, "%s() not implemented!\n", __func__);
  return 0;
#else
  return pthread_cond_broadcast(cond);
#endif
}
#endif

int _chpl_sync_wait_full_and_lock(_chpl_sync_aux_t *s, _int32 lineno, _string filename) {
  if (_chpl_sync_lock(s)) {
    _printError("invalid mutex", lineno, filename);  // s is null
    return 1;
  } else {
    while (!readxx(&(s->is_full))) {
      _chpl_sync_unlock(s);
      readfe(&(s->signal_full));
      _chpl_sync_lock(s);
    }
    return 0;
  }
}

int _chpl_sync_wait_empty_and_lock(_chpl_sync_aux_t *s, _int32 lineno, _string filename) {
  if (_chpl_sync_lock(s)) {
    _printError("invalid mutex", lineno, filename);   // s is null
    return 1;
  } else {
    while (readxx(&(s->is_full))) {
      _chpl_sync_unlock(s);
      readfe(&(s->signal_empty));
      _chpl_sync_lock(s);
    }
    return 0;
  }
}

int _chpl_sync_mark_and_signal_full(_chpl_sync_aux_t *s) {
  if (s) {
    writexf(&(s->signal_full), true);                // signal full
    writeef(&(s->is_full), true);                    // mark full and unlock
    return 0;
  } else {
    _printError("invalid mutex", 0/*lineno*/, 0/*filename*/);  // s is null
    return 1;
  }
}

int _chpl_sync_mark_and_signal_empty(_chpl_sync_aux_t *s) {
  if (s) {
    writexf(&(s->signal_empty), true);               // signal empty
    writeef(&(s->is_full), false);                   // mark empty and unlock
    return 0;
  } else {
    _printError("invalid mutex", 0/*lineno*/, 0/*filename*/);  // s is null
    return 1;
  }
}

int _chpl_sync_is_full(_chpl_sync_aux_t *s) {
#if 1
  _printError("_chpl_sync_is_full() not implemented!", 0, 0);
#else
  // If the sync var is a simple type, the implementation below won't work!
  if (s)
    return readxx(&(s->is_full));
  else
    _printError("invalid mutex", lineno, filename);  // s is null
#endif
  return -1;
}


void _chpl_init_sync_aux(_chpl_sync_aux_t *s) {
  purge(&(s->is_full));
  purge(&(s->signal_empty));
  purge(&(s->signal_full));
}


void _chpl_serial_delete(_bool *p) {
#if 1
  _printError("_chpl_serial_delete() not implemented!", 0, 0);
#else
  if (NULL != p) {
    _chpl_free(p, 0, 0);
  }
#endif
}


// Threads

void initChplThreads() {
  _chpl_begin_cnt = 0;                     // only main thread running
#ifdef MTA_DEBUG
  fprintf(stderr, "Inside %s(), ", __func__);
#endif
  _chpl_can_exit = 1;                      // mark full - no threads created yet

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
  fprintf(stderr, "Inside %s()\n", __func__);
#endif
  do
    // this will block until _chpl_can_exit is marked full!
    ready = readff(&_chpl_can_exit);
  while (!ready);
#ifdef MTA_DEBUG
  fprintf(stderr, "About to return from %s(); _chpl_can_exit=%d\n",
          __func__, _chpl_can_exit);
#endif
}


void _chpl_thread_init(void) {
#if 1
#ifdef MTA_DEBUG
  fprintf(stderr, "%s() not implemented!\n", __func__);
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
  fprintf(stderr, "%s() not implemented!\n", __func__);
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
  _printError("_chpl_set_serial() not implemented!", 0, 0);
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
  _int64            *finished;

  // _chpl_get_serial is not yet implemented correctly!
  if (0 && _chpl_get_serial()) {
    for (t=0; t<nthreads; t++)
      (*fps[t])(args[t], t);
  } else if (finished = (_int64 *)_chpl_malloc(nthreads, sizeof(_int64),
                                               "finished", 0, 0)) {
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
    _chpl_free(finished, 0, 0);
  } else {
    retv = 1;
    halt("Out of memory in %s!\n", __func__);
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
    fprintf(stderr, "Inside %s\n", __func__);
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
  fprintf(stderr, "About to return from %s\n", __func__);
#endif
  return 0;
}
