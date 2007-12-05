// MTA implementation

#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"
#include <stdint.h>
#include <stdlib.h>
#include <machine/runtime.h>

#ifdef MTA_DEBUG
#include <stdio.h>
#endif


// The global vars are to synchronize with threads created with 
// begin's which are not joined.  We need to wait on them before the
// main thread can call exit for the process.
static _int64          _chpl_begin_cnt;      // number of unjoined threads 
static sync _int64     _chpl_can_exit;       // can main thread exit?


// Mutex

int _chpl_mutex_init(_chpl_mutex_p mutex) {
  if (mutex) {
    purge(mutex);                     // set to zero and mark empty
    return 0;
  } else {
    _printInternalError("null pointer in _chpl_mutex_init");
    return 1;
  }
}

int _chpl_mutex_lock(_chpl_mutex_p mutex) {
  if (mutex) {
    writeef(mutex, 1);                // set to one and mark full
    return 0;
  } else
    _printInternalError("null pointer in _chpl_mutex_lock");
    return 1;
}

int _chpl_mutex_unlock(_chpl_mutex_p mutex) {
  return _chpl_mutex_init(mutex);
}

int _chpl_mutex_destroy(_chpl_mutex_p mutex) {
  return _chpl_mutex_lock(mutex);     // lock it so no thread can use it
}


int _chpl_sync_lock(_chpl_sync_aux_t *s) {
  if (s) {
    readfe(&(s->is_full));            // mark empty
    return 0;
  } else {
    _printInternalError("null pointer in _chpl_sync_lock");
    return 1;
  }
}

int _chpl_sync_unlock(_chpl_sync_aux_t *s) {
  if (s) {
    _int64 is_full = readxx(&(s->is_full));
    writeef(&(s->is_full), is_full);  // mark full
    return 0;
  } else {
    _printInternalError("null pointer in _chpl_sync_unlock");
    return 1;
  }
}

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
    _printInternalError("null pointer in _chpl_sync_mark_and_signal_full");
    return 1;
  }
}

int _chpl_sync_mark_and_signal_empty(_chpl_sync_aux_t *s) {
  if (s) {
    writexf(&(s->signal_empty), true);               // signal empty
    writeef(&(s->is_full), false);                   // mark empty and unlock
    return 0;
  } else {
    _printInternalError("null pointer in _chpl_sync_mark_and_signal_empty");
    return 1;
  }
}

int _chpl_sync_is_full(void *val_ptr, _chpl_sync_aux_t *s, _bool simple_sync_var) {
  if (simple_sync_var)
    return ((unsigned)MTA_STATE_LOAD(val_ptr)<<3)>>63 == 0;
  else if (s)
    return readxx(&(s->is_full));
  else {
    _printInternalError("null pointer in _chpl_sync_is_full");
    return -1;
  }
}

void _chpl_init_sync_aux(_chpl_sync_aux_t *s) {
  writexf(&(s->is_full), 0);          // mark empty and unlock
  purge(&(s->signal_empty));
  purge(&(s->signal_full));
}


int _chpl_single_lock(_chpl_single_aux_t *s) {
  if (s) {
    readfe(&(s->is_full));            // mark empty
    return 0;
  } else {
    _printInternalError("null pointer in _chpl_single_lock");
    return 1;
  }
}

int _chpl_single_wait_full(_chpl_single_aux_t *s) {
  if (s) {
    while (!readxx(&(s->is_full))) {
      readff(&(s->signal_full));
    }
    return 0;
  } else {
    _printInternalError("null pointer in _chpl_single_wait_full");
    return 1;
  }
}

int _chpl_single_mark_and_signal_full(_chpl_single_aux_t *s) {
  if (s) {
    writexf(&(s->is_full), true);     // mark full and unlock
    writexf(&(s->signal_full), true); // signal full
    return 0;
  } else {
    _printInternalError("null pointer in _chpl_single_mark_and_signal_full");
    return 1;
  }
}

int _chpl_single_is_full(void *val_ptr, _chpl_single_aux_t *s, _bool simple_single_var) {
  if (simple_single_var)
    return ((unsigned)MTA_STATE_LOAD(val_ptr)<<3)>>63 == 0;
  else if (s)
    return readxx(&(s->is_full));
  else {
    _printInternalError("null pointer in _chpl_single_is_full");
    return -1;
  }
}

void _chpl_init_single_aux(_chpl_single_aux_t *s) {
  writexf(&(s->is_full), 0);          // mark empty and unlock
  purge(&(s->signal_full));
}


// Threads

void initChplThreads() {
  _chpl_begin_cnt = 0;                     // only main thread running
  _chpl_can_exit = 1;                      // mark full - no threads created yet

  _chpl_mutex_init(&_memtrack_lock);
  _chpl_mutex_init(&_memstat_lock);
  _chpl_mutex_init(&_memtrace_lock);

  _chpl_thread_init();
}


void exitChplThreads() {
  int ready=0;
  do
    // this will block until _chpl_can_exit is marked full!
    ready = readff(&_chpl_can_exit);
  while (!ready);
}


void _chpl_thread_init(void) {}  // No need to do anything!


_uint64 _chpl_thread_id(void) {
  return mta_get_threadid();
}


_bool _chpl_get_serial(void) {
  _bool *p = NULL;
  p = (_bool*) mta_register_task_data(p);
  if (p == NULL)
    return false;
  else {
    mta_register_task_data(p); // Put back the value retrieved above.
    return *p;
  }
}


void _chpl_set_serial(_bool state) {
  _bool *p = NULL;
  p = (_bool*) mta_register_task_data(p);
  if (p == NULL)
    p = (_bool*) _chpl_alloc(sizeof(_bool), "serial flag", 0, 0);
  if (p) {
    *p = state;
    mta_register_task_data(p);
  } else
    _printInternalError("out of memory while creating serial state");
}


int _chpl_cobegin (int                      nthreads, 
                   _chpl_threadfp_t        *fps,
                   _chpl_threadarg_t       *args, 
                   _chpl_cobegin_wkspace_t *twrk) {
  int               t;

  if (_chpl_get_serial())
    for (t=0; t<nthreads; t++)
      (*fps[t])(args[t]);

  else {
    _int64 can_exit, *can_exit_p = &can_exit;
    _int64 begin_cnt = 0, *begin_cnt_p = &begin_cnt;

    // create threads
    for (t=0; t<nthreads; t++, fps++, args++) {

      int_fetch_add(begin_cnt_p, 1);       // assume begin will succeed
      purge(can_exit_p);
#ifdef MTA_DEBUG
      fprintf(stderr, "About to create future no. %d\n", t);
#endif
      future (fps, args, begin_cnt_p, can_exit_p, t) {
        int prev_count;
#ifdef MTA_DEBUG
        fprintf(stderr, "Inside future no. %d\n", t);
#endif
        (**fps)(*args);
#ifdef MTA_DEBUG
        fprintf(stderr, "About to exit future no. %d\n", t);
#endif
        // decrement begin thread count and see if we can signal Chapel exit
        prev_count = int_fetch_add(begin_cnt_p, -1);
#ifdef MTA_DEBUG
        fprintf(stderr, "prev_count = %d\n", prev_count);
#endif
        if (prev_count == 1)               // i.e., begin_cnt is now zero
          writeef(can_exit_p, 1);          // mark this variable full
      }
#ifdef MTA_DEBUG
      fprintf(stderr, "Finished creating future no. %d\n", t);
#endif
    }
#ifdef MTA_DEBUG
    fprintf(stderr, "Finished creating all futures!\n");
#endif

    // wait on those threads
    readfe(can_exit_p);      // block until all threads have finished executing
  }

  return 0;
}


// Similar to _chpl_cobegin above, be we do not wait on the launched
// thread.  Also we only expect one thread to launch with a begin block.
int
_chpl_begin (_chpl_threadfp_t fp, _chpl_threadarg_t arg) {

  if (_chpl_get_serial())
    (*fp)(arg);

  else {
    int init_begin_cnt =
      int_fetch_add(&_chpl_begin_cnt, 1);       // assume begin will succeed
    purge(&_chpl_can_exit);                     // set to zero and mark as empty

    // Will call the real begin statement function. Only purpose of this
    // thread is to wait on that function and coordinate the exiting
    // of the main Chapel thread.
    future (fp, arg, init_begin_cnt) {
      _int64         begin_cnt;

#ifdef MTA_DEBUG
      fprintf(stderr, "Inside future no. %d\n", init_begin_cnt);
#endif
      (*fp)(arg);

      // decrement begin thread count and see if we can signal Chapel exit
      begin_cnt = int_fetch_add(&_chpl_begin_cnt, -1);
      if (begin_cnt == 1)   // i.e., _chpl_begin_cnt is now zero
        _chpl_can_exit = 1; // mark this variable as being full
#ifdef MTA_DEBUG
      fprintf(stderr, "About to exit future no. %d; begin_cnt = %d\n",
              init_begin_cnt, begin_cnt);
#endif
    }
  }

  return 0;
}
