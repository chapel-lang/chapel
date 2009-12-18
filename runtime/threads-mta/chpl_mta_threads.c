// MTA implementation

#include "chplrt.h"
#include "chplthreads.h"
#include "chpl_mem.h"
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

void CHPL_MUTEX_INIT(chpl_mutex_p mutex) {
  purge(mutex);                     // set to zero and mark empty
}

void CHPL_MUTEX_LOCK(chpl_mutex_p mutex) {
  writeef(mutex, 1);                // set to one and mark full
}

void CHPL_MUTEX_UNLOCK(chpl_mutex_p mutex) {
  CHPL_MUTEX_INIT(mutex);
}


// Sync variables

void CHPL_SYNC_LOCK(chpl_sync_aux_t *s) {
  readfe(&(s->is_full));            // mark empty
}

void CHPL_SYNC_UNLOCK(chpl_sync_aux_t *s) {
  int64_t is_full = readxx(&(s->is_full));
  writeef(&(s->is_full), is_full);  // mark full
}

void CHPL_SYNC_WAIT_FULL_AND_LOCK(chpl_sync_aux_t *s,
                                  int32_t lineno, chpl_string filename) {
  CHPL_SYNC_LOCK(s);
  while (!readxx(&(s->is_full))) {
    CHPL_SYNC_UNLOCK(s);
    readfe(&(s->signal_full));
    CHPL_SYNC_LOCK(s);
  }
}

void CHPL_SYNC_WAIT_EMPTY_AND_LOCK(chpl_sync_aux_t *s,
                                   int32_t lineno, chpl_string filename) {
  CHPL_SYNC_LOCK(s);
  while (readxx(&(s->is_full))) {
    CHPL_SYNC_UNLOCK(s);
    readfe(&(s->signal_empty));
    CHPL_SYNC_LOCK(s);
  }
}

void CHPL_SYNC_MARK_AND_SIGNAL_FULL(chpl_sync_aux_t *s) {
  writexf(&(s->signal_full), true);                // signal full
  writeef(&(s->is_full), true);                    // mark full and unlock
}

void CHPL_SYNC_MARK_AND_SIGNAL_EMPTY(chpl_sync_aux_t *s) {
  writexf(&(s->signal_empty), true);               // signal empty
  writeef(&(s->is_full), false);                   // mark empty and unlock
}

chpl_bool CHPL_SYNC_IS_FULL(void *val_ptr, chpl_sync_aux_t *s,
                            chpl_bool simple_sync_var) {
  if (simple_sync_var)
    return (chpl_bool)(((unsigned)MTA_STATE_LOAD(val_ptr)<<3)>>63 == 0);
  else
    return (chpl_bool)readxx(&(s->is_full));
}

void CHPL_INIT_SYNC_AUX(chpl_sync_aux_t *s) {
  writexf(&(s->is_full), 0);          // mark empty and unlock
  purge(&(s->signal_empty));
  purge(&(s->signal_full));
}

void CHPL_DESTROY_SYNC_AUX(chpl_sync_aux_t *s) { }


// Single variables

void CHPL_SINGLE_LOCK(chpl_single_aux_t *s) {
  readfe(&(s->is_full));            // mark empty
}

void CHPL_SINGLE_UNLOCK(chpl_single_aux_t *s) {
  int64_t is_full = readxx(&(s->is_full));
  writeef(&(s->is_full), is_full);  // mark full
}

void CHPL_SINGLE_WAIT_FULL(chpl_single_aux_t *s,
                           int32_t lineno, chpl_string filename) {
  while (!readxx(&(s->is_full)))
    readff(&(s->signal_full));
}

void CHPL_SINGLE_MARK_AND_SIGNAL_FULL(chpl_single_aux_t *s) {
  writexf(&(s->is_full), true);     // mark full and unlock
  writexf(&(s->signal_full), true); // signal full
}

chpl_bool CHPL_SINGLE_IS_FULL(void *val_ptr, chpl_single_aux_t *s,
                              chpl_bool simple_single_var) {
  if (simple_single_var)
    return (chpl_bool)(((unsigned)MTA_STATE_LOAD(val_ptr)<<3)>>63 == 0);
  else
    return (chpl_bool)readxx(&(s->is_full));
}

void CHPL_INIT_SINGLE_AUX(chpl_single_aux_t *s) {
  writexf(&(s->is_full), 0);          // mark empty and unlock
  purge(&(s->signal_full));
}

void CHPL_DESTROY_SINGLE_AUX(chpl_single_aux_t *s) { }

// Tasks

void CHPL_TASKING_INIT() {
  chpl_begin_cnt = 0;                     // only main thread running
  chpl_can_exit = 1;                      // mark full - no threads created yet
}

void CHPL_TASKING_EXIT() {
  int ready=0;
  do
    // this will block until chpl_can_exit is marked full!
    ready = readff(&chpl_can_exit);
  while (!ready);
}

void CHPL_ADD_TO_TASK_LIST(chpl_fn_int_t fid,
                           void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_locale,
                           chpl_bool call_chpl_begin,
                           int lineno,
                           chpl_string filename) {
  CHPL_BEGIN(chpl_ftable[fid], arg, false, false, NULL);
}

void CHPL_PROCESS_TASK_LIST(chpl_task_list_p task_list) { }

void CHPL_EXECUTE_TASKS_IN_LIST(chpl_task_list_p task_list) { }

void CHPL_FREE_TASK_LIST(chpl_task_list_p task_list) { }

void
CHPL_BEGIN(chpl_fn_p fp, void* arg, chpl_bool ignore_serial, 
           chpl_bool serial_state, chpl_task_list_p task_list_entry) {

  if (!ignore_serial && CHPL_GET_SERIAL())
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
}

void CHPL_TASK_SLEEP(int secs) {
  sleep(secs);
}

chpl_bool CHPL_GET_SERIAL(void) {
  chpl_bool *p = NULL;
  p = (chpl_bool*) mta_register_task_data(p);
  if (p == NULL)
    return false;
  else {
    mta_register_task_data(p); // Put back the value retrieved above.
    return *p;
  }
}

void CHPL_SET_SERIAL(chpl_bool state) {
  chpl_bool *p = NULL;
  p = (chpl_bool*) mta_register_task_data(p);
  if (p == NULL)
    p = (chpl_bool*) chpl_alloc(sizeof(chpl_bool), CHPL_RT_MD_SERIAL_FLAG, 0, 0);
  if (p) {
    *p = state;
    mta_register_task_data(p);
  } else
    chpl_internal_error("out of memory while creating serial state");
}

// not sure what the correct value should be here!
uint32_t CHPL_NUMQUEUEDTASKS(void) { return 0; }

// not sure what the correct value should be here!
uint32_t CHPL_NUMRUNNINGTASKS(void) { return 1; }

// not sure what the correct value should be here!
int32_t  CHPL_NUMBLOCKEDTASKS(void) { return -1; }


// Threads

chpl_threadID_t CHPL_THREAD_ID(void) {
  return (chpl_threadID_t) mta_get_threadid(); 
}

void CHPL_THREAD_CANCEL(chpl_threadID_t threadID) {
  chpl_internal_error("CHPL_THREAD_CANCEL() shouldn't be called in threads-mta");
}

void CHPL_THREAD_JOIN(chpl_threadID_t threadID) {
  chpl_internal_error("CHPL_THREAD_JOIN() shouldn't be called in threads-mta");
}

int32_t CHPL_THREADS_GETMAXTHREADS(void) {
  return chpl_coresPerLocale() * 100;
}

int32_t CHPL_THREADS_MAXTHREADSLIMIT(void) {
  return chpl_coresPerLocale() * 104;
}

// not sure what the correct value should be here!
uint32_t CHPL_NUMTHREADS(void) { return 1; }

// not sure what the correct value should be here!
uint32_t CHPL_NUMIDLETHREADS(void) { return 0; }
