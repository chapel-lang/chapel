//
// MTA implementation of Chapel tasking interface
//

#include "chplrt.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chplcgfns.h" // for chpl_ftable
#include "config.h"
#include "error.h"
#include <machine/runtime.h>
#include <stdint.h>
#include <stdlib.h>


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

void chpl_task_init(int32_t numThreadsPerLocale, int32_t maxThreadsPerLocale, 
                    int numCommTasks, uint64_t callStackSize) {
  //
  // If a value was specified for the call stack size or max threads
  // config consts, warn the user that it's ignored on this system.
  //
  if (numThreadsPerLocale != 0)
    chpl_warning("the numThreadsPerLocale config constant has no effect "
                 "on XMT systems", 0, NULL);

  if (callStackSize != 0)
    chpl_warning("the callStackSize config constant has no effect "
                 "on XMT systems",
                 0, NULL);

  chpl_task_setSerial(true);
}

void chpl_task_exit(void) {
}

void chpl_task_callMain(void (*chpl_main)(void)) {
  chpl_task_setSerial(false);
  chpl_main();
}

void chpl_task_addToTaskList(chpl_fn_int_t fid,
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
    p = (chpl_bool*) chpl_mem_alloc(sizeof(chpl_bool), CHPL_RT_MD_SERIAL_FLAG, 0, 0);
  if (p) {
    *p = state;
    mta_register_task_data(p);
  } else
    chpl_internal_error("out of memory while creating serial state");
}

uint64_t chpl_task_getCallStackSize(void) { return 0; }

// not sure what the correct value should be here!
uint32_t chpl_task_getNumQueuedTasks(void) { return 0; }

// not sure what the correct value should be here!
uint32_t chpl_task_getNumRunningTasks(void) { return 1; }

// not sure what the correct value should be here!
int32_t  chpl_task_getNumBlockedTasks(void) { return -1; }


// Threads

// not sure what the correct value should be here!
uint32_t chpl_task_getNumThreads(void) { return 1; }

// not sure what the correct value should be here!
uint32_t chpl_task_getNumIdleThreads(void) { return 0; }
