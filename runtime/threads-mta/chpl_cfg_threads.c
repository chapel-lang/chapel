// MTA implementation

#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"
#include <stdlib.h>
#include <stdint.h>
#include <machine/runtime.h>


// The global vars are to synchronize with threads created with 
// begin's which are not joined.  We need to wait on them before the
// main thread can call exit for the process.
static int64_t          chpl_begin_cnt;      // number of unjoined threads 
static sync int64_t     chpl_can_exit;       // can main thread exit?


// Mutex

static void chpl_mutex_init(chpl_mutex_p mutex) {
  purge(mutex);                     // set to zero and mark empty
}

int chpl_mutex_lock(chpl_mutex_p mutex) {
  writeef(mutex, 1);                // set to one and mark full
  return 0;
}

void chpl_mutex_unlock(chpl_mutex_p mutex) {
  chpl_mutex_init(mutex);
}


// Sync variables

int chpl_sync_lock(chpl_sync_aux_t *s) {
  readfe(&(s->is_full));            // mark empty
  return 0;
}

void chpl_sync_unlock(chpl_sync_aux_t *s) {
  int64_t is_full = readxx(&(s->is_full));
  writeef(&(s->is_full), is_full);  // mark full
}

int chpl_sync_wait_full_and_lock(chpl_sync_aux_t *s, int32_t lineno, _string filename) {
  chpl_sync_lock(s);
  while (!readxx(&(s->is_full))) {
    chpl_sync_unlock(s);
    readfe(&(s->signal_full));
    chpl_sync_lock(s);
  }
  return 0;
}

int chpl_sync_wait_empty_and_lock(chpl_sync_aux_t *s, int32_t lineno, _string filename) {
  chpl_sync_lock(s);
  while (readxx(&(s->is_full))) {
    chpl_sync_unlock(s);
    readfe(&(s->signal_empty));
    chpl_sync_lock(s);
  }
  return 0;
}

void chpl_sync_mark_and_signal_full(chpl_sync_aux_t *s) {
  writexf(&(s->signal_full), true);                // signal full
  writeef(&(s->is_full), true);                    // mark full and unlock
}

void chpl_sync_mark_and_signal_empty(chpl_sync_aux_t *s) {
  writexf(&(s->signal_empty), true);               // signal empty
  writeef(&(s->is_full), false);                   // mark empty and unlock
}

chpl_bool chpl_sync_is_full(void *val_ptr, chpl_sync_aux_t *s, chpl_bool simple_sync_var) {
  if (simple_sync_var)
    return (chpl_bool)(((unsigned)MTA_STATE_LOAD(val_ptr)<<3)>>63 == 0);
  else
    return (chpl_bool)readxx(&(s->is_full));
}

void chpl_init_sync_aux(chpl_sync_aux_t *s) {
  writexf(&(s->is_full), 0);          // mark empty and unlock
  purge(&(s->signal_empty));
  purge(&(s->signal_full));
}


// Single variables

int chpl_single_lock(chpl_single_aux_t *s) {
  readfe(&(s->is_full));            // mark empty
  return 0;
}

int chpl_single_wait_full(chpl_single_aux_t *s, int32_t lineno, _string filename) {
  while (!readxx(&(s->is_full)))
    readff(&(s->signal_full));
  return 0;
}

void chpl_single_mark_and_signal_full(chpl_single_aux_t *s) {
  writexf(&(s->is_full), true);     // mark full and unlock
  writexf(&(s->signal_full), true); // signal full
}

chpl_bool chpl_single_is_full(void *val_ptr, chpl_single_aux_t *s, chpl_bool simple_single_var) {
  if (simple_single_var)
    return (chpl_bool)(((unsigned)MTA_STATE_LOAD(val_ptr)<<3)>>63 == 0);
  else
    return (chpl_bool)readxx(&(s->is_full));
}

void chpl_init_single_aux(chpl_single_aux_t *s) {
  writexf(&(s->is_full), 0);          // mark empty and unlock
  purge(&(s->signal_full));
}


// Threads

int32_t chpl_threads_getMaxThreads(void) {
  return chpl_coresPerLocale() * 100;
}

int32_t chpl_threads_maxThreadsLimit(void) {
  return chpl_coresPerLocale() * 104;
}

void initChplThreads() {
  chpl_begin_cnt = 0;                     // only main thread running
  chpl_can_exit = 1;                      // mark full - no threads created yet

  chpl_mutex_init(&_memtrack_lock);
  chpl_mutex_init(&_memstat_lock);
  chpl_mutex_init(&_memtrace_lock);

  chpl_thread_init();
}


void exitChplThreads() {
  int ready=0;
  do
    // this will block until chpl_can_exit is marked full!
    ready = readff(&chpl_can_exit);
  while (!ready);
}


void chpl_thread_init(void) {}  // No need to do anything!


uint64_t chpl_thread_id(void) {
  return mta_get_threadid();
}


chpl_bool chpl_get_serial(void) {
  chpl_bool *p = NULL;
  p = (chpl_bool*) mta_register_task_data(p);
  if (p == NULL)
    return false;
  else {
    mta_register_task_data(p); // Put back the value retrieved above.
    return *p;
  }
}


void chpl_set_serial(chpl_bool state) {
  chpl_bool *p = NULL;
  p = (chpl_bool*) mta_register_task_data(p);
  if (p == NULL)
    p = (chpl_bool*) _chpl_alloc(sizeof(chpl_bool), "serial flag", 0, 0);
  if (p) {
    *p = state;
    mta_register_task_data(p);
  } else
    chpl_internal_error("out of memory while creating serial state");
}


int
chpl_begin (chpl_threadfp_t fp, chpl_threadarg_t arg, chpl_bool ignore_serial, 
            chpl_bool serial_state) {

  if (!ignore_serial && chpl_get_serial())
    (*fp)(arg);

  else {
    int init_begin_cnt =
      int_fetch_add(&chpl_begin_cnt, 1);       // assume begin will succeed
    purge(&chpl_can_exit);                     // set to zero and mark as empty

    // Will call the real begin statement function. Only purpose of this
    // thread is to wait on that function and coordinate the exiting
    // of the main Chapel thread.
    future (fp, arg, init_begin_cnt) {
      int64_t         begin_cnt;

      (*fp)(arg);

      // decrement begin thread count and see if we can signal Chapel exit
      begin_cnt = int_fetch_add(&chpl_begin_cnt, -1);
      if (begin_cnt == 1)   // i.e., chpl_begin_cnt is now zero
        chpl_can_exit = 1; // mark this variable as being full
    }
  }

  return 0;
}
