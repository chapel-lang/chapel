//
// MTA implementation of Chapel tasking interface
//

#include "chpl_mem.h"
#include "chplrt.h"
#include "chpltasks.h"
#include "config.h"
#include "error.h"
#include <machine/runtime.h>
#include <stdint.h>
#include <stdlib.h>


// The global vars are to synchronize with threads created with 
// begin's which are not joined.  We need to wait on them before the
// main thread can call exit for the process.
static int64_t          chpl_begin_cnt;      // number of unjoined threads 
static sync int64_t     chpl_can_exit;       // can main thread exit?


// Sync variables

void chpl_sync_lock(chpl_sync_aux_t *s) {
  readfe(&(s->is_full));            // mark empty
}

void chpl_sync_unlock(chpl_sync_aux_t *s) {
  int64_t is_full = readxx(&(s->is_full));
  writeef(&(s->is_full), is_full);  // mark full
}

void chpl_sync_waitFullAndLock(chpl_sync_aux_t *s,
                                  int32_t lineno, chpl_string filename) {
  chpl_sync_lock(s);
  while (!readxx(&(s->is_full))) {
    chpl_sync_unlock(s);
    readfe(&(s->signal_full));
    chpl_sync_lock(s);
  }
}

void chpl_sync_waitEmptyAndLock(chpl_sync_aux_t *s,
                                   int32_t lineno, chpl_string filename) {
  chpl_sync_lock(s);
  while (readxx(&(s->is_full))) {
    chpl_sync_unlock(s);
    readfe(&(s->signal_empty));
    chpl_sync_lock(s);
  }
}

void chpl_sync_markAndSignalFull(chpl_sync_aux_t *s) {
  writexf(&(s->signal_full), true);                // signal full
  writeef(&(s->is_full), true);                    // mark full and unlock
}

void chpl_sync_markAndSignalEmpty(chpl_sync_aux_t *s) {
  writexf(&(s->signal_empty), true);               // signal empty
  writeef(&(s->is_full), false);                   // mark empty and unlock
}

chpl_bool chpl_sync_isFull(void *val_ptr, chpl_sync_aux_t *s,
                            chpl_bool simple_sync_var) {
  if (simple_sync_var)
    return (chpl_bool)(((unsigned)MTA_STATE_LOAD(val_ptr)<<3)>>63 == 0);
  else
    return (chpl_bool)readxx(&(s->is_full));
}

void chpl_sync_initAux(chpl_sync_aux_t *s) {
  writexf(&(s->is_full), 0);          // mark empty and unlock
  purge(&(s->signal_empty));
  purge(&(s->signal_full));
}

void chpl_sync_destroyAux(chpl_sync_aux_t *s) { }


// Tasks

void chpl_task_init(int32_t maxThreadsPerLocale, uint64_t callStackSize) {
  //
  // If a value was specified for the call stack size or max threads
  // config consts, warn the user that it's ignored on this system.
  //
  if (maxThreadsPerLocale != 0)
    chpl_warning("the maxThreadsPerLocale config constant has no effect "
                 "on XMT systems", 0, NULL);

  if (callStackSize != 0)
    chpl_warning("the callStackSize config constant has no effect "
                 "on XMT systems",
                 0, NULL);
}

void chpl_task_exit(void) {
}

void chpl_task_callMain(void (*chpl_main)(void)) {
  chpl_main();
}

chpl_bool CHPL_SINGLE_IS_FULL(void *val_ptr, chpl_single_aux_t *s,
                              chpl_bool simple_single_var) {
  if (simple_single_var)
    return (chpl_bool)(((unsigned)MTA_STATE_LOAD(val_ptr)<<3)>>63 == 0);
  else
    return (chpl_bool)readxx(&(s->is_full));
}

void CHPL_SINGLE_INIT_AUX(chpl_single_aux_t *s) {
  writexf(&(s->is_full), 0);          // mark empty and unlock
  purge(&(s->signal_full));
}

void CHPL_SINGLE_DESTROY_AUX(chpl_single_aux_t *s) { }

// Tasks

void CHPL_TASKING_INIT(void) {
  //
  // If a value was specified for the call stack size config const, warn
  // the user that it's ignored on this system.
  //
  if (chpl_config_get_value("callStackSize", "Built-in") != NULL)
    chpl_warning("the callStackSize config constant has no effect "
                 "on XMT systems",
                 0, NULL);

  chpl_begin_cnt = 0;                     // only main thread running
  chpl_can_exit = 1;                      // mark full - no threads created yet
}

void CHPL_TASKING_EXIT(void) {
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
  chpl_task_begin(chpl_ftable[fid], arg, false, false, NULL);
}

void chpl_task_processTaskList(chpl_task_list_p task_list) { }

void chpl_task_executeTasksInList(chpl_task_list_p task_list) { }

void chpl_task_freeTaskList(chpl_task_list_p task_list) { }

void
chpl_task_begin(chpl_fn_p fp, void* arg, chpl_bool ignore_serial, 
           chpl_bool serial_state, chpl_task_list_p task_list_entry) {

  if (!ignore_serial && chpl_task_getSerial())
    (*fp)(arg);

  else {
    // Will call the real begin statement function. Only purpose of this
    // thread is to wait on that function and coordinate the exiting
    // of the main Chapel thread.
    future (fp, arg) {
      (*fp)(arg);
    }
  }
}

chpl_taskID_t chpl_task_getId(void) {
  return (chpl_taskID_t) mta_get_threadid(); 
}

void chpl_task_sleep(int secs) {
  sleep(secs);
}

chpl_bool chpl_task_getSerial(void) {
  chpl_bool *p = NULL;
  p = (chpl_bool*) mta_register_task_data(p);
  if (p == NULL)
    return false;
  else {
    mta_register_task_data(p); // Put back the value retrieved above.
    return *p;
  }
}

void chpl_task_setSerial(chpl_bool state) {
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

uint64_t CHPL_TASK_CALLSTACKSIZE(void) { return 0; }

uint64_t CHPL_TASK_CALLSTACKSIZELIMIT(void) { return 0; }

// not sure what the correct value should be here!
uint32_t chpl_task_getNumQueuedTasks(void) { return 0; }

// not sure what the correct value should be here!
uint32_t chpl_task_getNumRunningTasks(void) { return 1; }

// not sure what the correct value should be here!
int32_t  chpl_task_getNumBlockedTasks(void) { return -1; }


// Threads

int32_t chpl_task_getMaxThreads(void) {
  return 0;
}

int32_t chpl_task_getMaxThreadsLimit(void) {
  return 0;
}

// not sure what the correct value should be here!
uint32_t chpl_task_getNumThreads(void) { return 1; }

// not sure what the correct value should be here!
uint32_t chpl_task_getNumIdleThreads(void) { return 0; }
