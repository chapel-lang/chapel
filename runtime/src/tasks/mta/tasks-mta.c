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


static void init_task_private_data(void);

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

  init_task_private_data();
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

typedef struct {
  chpl_bool serial_state;
  c_locale_t locale_id;
  void* here;	// Local address of the current locale object.
  // Add fields here as needed....
} chpl_task_private_data_t;

//////////////////////////////////////////////////////////////////////////////////
//
// Routines for handling task-private data.
//

static void init_task_private_data(void)
{
  chpl_task_private_data_t* p =
    (chpl_task_private_data_t*) malloc(sizeof(chpl_task_private_data_t));
  if (!p)
    chpl_internal_error("out of memory trying to allocate task-private data.");
  mta_register_task_data(p);
}

chpl_bool chpl_task_getSerial(void) {
  chpl_task_private_data_t *p = NULL;
  p = (chpl_task_private_data_t*) mta_register_task_data(p);
  if (p == NULL)
    return false;
  else {
    mta_register_task_data(p); // Put back the value retrieved above.
    return p->serial_state;
  }
}

void* chpl_task_getHere(void) {
  chpl_task_private_data_t *p = NULL;
  p = (chpl_task_private_data_t*) mta_register_task_data(p);
  if (p == NULL)
    return 0;
  else {
    mta_register_task_data(p); // Put back the value retrieved above.
    return p->here;
  }
}

c_locale_t chpl_task_getLocaleID(void) {
  chpl_task_private_data_t *p = NULL;
  p = (chpl_task_private_data_t*) mta_register_task_data(p);
  if (p == NULL)
    return 0;
  else {
    mta_register_task_data(p); // Put back the value retrieved above.
    return p->locale_id;
  }
}

void chpl_task_setSerial(chpl_bool state) {
  chpl_task_private_data_t *p = NULL;
  p = (chpl_task_private_data_t*) mta_register_task_data(p);
  if (p == NULL)
    chpl_internal_error("no task-private data in chpl_task_setSerial.");
  else
  {
    p->serial_state = state;
    mta_register_task_data(p);
  }
}

void chpl_task_setHere(void* here) {
  chpl_task_private_data_t *p = NULL;
  p = (chpl_task_private_data_t*) mta_register_task_data(p);
  if (p == NULL)
    chpl_internal_error("no task-private data in chpl_task_setHere.");
  else
  {
    p->here = here;
    mta_register_task_data(p);
  }
}

void chpl_task_setLocaleID(c_locale_t locale) {
  chpl_task_private_data_t *p = NULL;
  p = (chpl_task_private_data_t*) mta_register_task_data(p);
  if (p == NULL)
    chpl_internal_error("no task-private data in chpl_task_setLocaleID.");
  else
  {
    p->locale_id = locale;
    mta_register_task_data(p);
  }
}

//
//////////////////////////////////////////////////////////////////////////////////

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
