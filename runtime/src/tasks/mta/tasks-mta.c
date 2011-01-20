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


// Sync variables

void chpl_sync_lock(chpl_sync_aux_t *s) {
  readfe(&(s->is_full));            // mark empty
}

void chpl_sync_unlock(chpl_sync_aux_t *s) {
  int64_t is_full = readxx(&(s->is_full));
  writeef(&(s->is_full), is_full);  // mark full
}

void chpl_sync_wait_full_and_lock(chpl_sync_aux_t *s,
                                  int32_t lineno, chpl_string filename) {
  chpl_sync_lock(s);
  while (!readxx(&(s->is_full))) {
    chpl_sync_unlock(s);
    readfe(&(s->signal_full));
    chpl_sync_lock(s);
  }
}

void chpl_sync_wait_empty_and_lock(chpl_sync_aux_t *s,
                                   int32_t lineno, chpl_string filename) {
  chpl_sync_lock(s);
  while (readxx(&(s->is_full))) {
    chpl_sync_unlock(s);
    readfe(&(s->signal_empty));
    chpl_sync_lock(s);
  }
}

void chpl_sync_mark_and_signal_full(chpl_sync_aux_t *s) {
  writexf(&(s->signal_full), true);                // signal full
  writeef(&(s->is_full), true);                    // mark full and unlock
}

void chpl_sync_mark_and_signal_empty(chpl_sync_aux_t *s) {
  writexf(&(s->signal_empty), true);               // signal empty
  writeef(&(s->is_full), false);                   // mark empty and unlock
}

chpl_bool chpl_sync_is_full(void *val_ptr, chpl_sync_aux_t *s,
                            chpl_bool simple_sync_var) {
  if (simple_sync_var)
    return (chpl_bool)(((unsigned)MTA_STATE_LOAD(val_ptr)<<3)>>63 == 0);
  else
    return (chpl_bool)readxx(&(s->is_full));
}

void chpl_sync_init_aux(chpl_sync_aux_t *s) {
  writexf(&(s->is_full), 0);          // mark empty and unlock
  purge(&(s->signal_empty));
  purge(&(s->signal_full));
}

void chpl_sync_destroy_aux(chpl_sync_aux_t *s) { }


// Tasks

void chpl_tasking_init(int32_t maxThreadsPerLocale, uint64_t callStackSize) {
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

void chpl_tasking_exit(void) {
}

void chpl_tasking_call_main(void (*chpl_main)(void)) {
  chpl_main();
}

void chpl_add_to_task_list(chpl_fn_int_t fid,
                           void* arg,
                           chpl_task_list_p *task_list,
                           int32_t task_list_locale,
                           chpl_bool call_chpl_begin,
                           int lineno,
                           chpl_string filename) {
  chpl_begin(chpl_ftable[fid], arg, false, false, NULL);
}

void chpl_process_task_list(chpl_task_list_p task_list) { }

void chpl_execute_tasks_in_list(chpl_task_list_p task_list) { }

void chpl_free_task_list(chpl_task_list_p task_list) { }

void
chpl_begin(chpl_fn_p fp, void* arg, chpl_bool ignore_serial, 
           chpl_bool serial_state, chpl_task_list_p task_list_entry) {

  if (!ignore_serial && chpl_get_serial())
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

chpl_taskID_t chpl_task_id(void) {
  return (chpl_taskID_t) mta_get_threadid(); 
}

void chpl_task_sleep(int secs) {
  sleep(secs);
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
    p = (chpl_bool*) chpl_alloc(sizeof(chpl_bool), CHPL_RT_MD_SERIAL_FLAG, 0, 0);
  if (p) {
    *p = state;
    mta_register_task_data(p);
  } else
    chpl_internal_error("out of memory while creating serial state");
}

uint64_t chpl_task_callstacksize(void) { return 0; }

// not sure what the correct value should be here!
uint32_t chpl_numQueuedTasks(void) { return 0; }

// not sure what the correct value should be here!
uint32_t chpl_numRunningTasks(void) { return 1; }

// not sure what the correct value should be here!
int32_t  chpl_numBlockedTasks(void) { return -1; }


// Threads

int32_t chpl_threads_getMaxThreads(void) {
  return 0;
}

int32_t chpl_threads_maxThreadsLimit(void) {
  return 0;
}

// not sure what the correct value should be here!
uint32_t chpl_numThreads(void) { return 1; }

// not sure what the correct value should be here!
uint32_t chpl_numIdleThreads(void) { return 0; }
