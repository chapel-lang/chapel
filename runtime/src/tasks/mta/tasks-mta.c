//
// MTA implementation of Chapel tasking interface
//

#include "chplrt.h"
#include "chpl-mem.h"
#include "chpl-tasks.h"
#include "chplcgfns.h" // for chpl_ftable
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

void chpl_task_init(void) {
  //
  // If a value was specified for the call stack size or max threads,
  // warn the user that it's ignored on this system.
  //
  if (chpl_task_getenvNumThreadsPerLocale != 0)
    chpl_warning("setting CHPL_RT_NUM_THREADS_PER_LOCALE has no effect "
                 "on XMT systems", 0, NULL);

  if (chpl_task_getenvCallStackSize() != 0)
    chpl_warning("setting CHPL_RT_CALL_STACK_SIZE has no effect "
                 "on XMT systems", 0, NULL);

  //
  // Set up task-private data for locale (architectural) support.
  //
  {
    chpl_task_private_data_t* p =
      (chpl_task_private_data_t*) malloc(sizeof(chpl_task_private_data_t));
    if (!p)
      chpl_internal_error("out of memory trying to allocate task-private "
                          "data.");

    p->localeID      = 0;
    p->here          = NULL;
    p->serial_state  = true;     // Set to false in chpl_task_callMain().
    p->alloc         = chpl_malloc;
    p->calloc        = chpl_calloc;
    p->realloc       = chpl_realloc;
    p->free          = chpl_free;

    (void) mta_register_task_data(p);
  }
}

void chpl_task_exit(void) {
}

void chpl_task_callMain(void (*chpl_main)(void)) {
  chpl_task_setSerial(false);
  chpl_main();
}

void chpl_task_addToTaskList(chpl_fn_int_t fid,
                             void* arg,
                             chpl_task_subLoc_t subLoc,
                             chpl_task_list_p *task_list,
                             int32_t task_list_locale,
                             chpl_bool is_begin_stmt,
                             int lineno,
                             chpl_string filename) {
  chpl_fn_p fp = chpl_ftable[fid];

  assert(subLoc == 0
         || subLoc == chpl_task_anySubLoc
         || subLoc == chpl_task_currSubLoc);

  if (chpl_task_getSerial())
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

void chpl_task_processTaskList(chpl_task_list_p task_list) { }

void chpl_task_executeTasksInList(chpl_task_list_p task_list) { }

void chpl_task_freeTaskList(chpl_task_list_p task_list) { }

void chpl_task_startMovedTask(chpl_fn_p fp,
                              void* arg,
                              chpl_task_subLoc_t subLoc,
                              chpl_taskID_t id,
                              chpl_bool serial_state) {
  //
  // We don't expect this to be called, because on the MTA architecture
  // we don't ever do a remote fork.
  //
  chpl_internal_error("chpl_task_startMovedTask() called");
}

chpl_task_subLoc_t chpl_task_getSubLoc(void) {
  return 0;
}

void chpl_task_setSubLoc(chpl_task_subLoc_t subLoc) {
  assert(subLoc == 0
         || subLoc == chpl_task_anySubLoc
         || subLoc == chpl_task_currSubLoc);
}

chpl_taskID_t chpl_task_getId(void) {
  return (chpl_taskID_t) mta_get_threadid(); 
}

void chpl_task_sleep(int secs) {
  sleep(secs);
}

//////////////////////////////////////////////////////////////////////////////////
//
// Routines for handling task-private data.
//

chpl_task_private_data_t* chpl_task_getPrivateData(void) {
  static chpl_task_private_data_t non_task_chpl_data =
      { .serial_state = true,
        .localeID = 0,
        .here = NULL,
        .alloc = chpl_malloc,
        .calloc = chpl_calloc,
        .realloc = chpl_realloc,
        .free = chpl_free
      };

  chpl_task_private_data_t* p;

  if ((p = (chpl_task_private_data_t*) mta_register_task_data(NULL)) == NULL)
    p = &non_task_chpl_data;
  else
    (void) mta_register_task_data(p); // Put back the value retrieved above.
  return p;
}

chpl_bool chpl_task_getSerial(void) {
  return chpl_task_getPrivateData()->serial_state;
}

void chpl_task_setSerial(chpl_bool state) {
  chpl_task_private_data_t* p = chpl_task_getPrivateData();
  if (p == NULL)
    chpl_internal_error("no task-private data in chpl_task_setSerial.");
  else
    p->serial_state = state;
}

void* chpl_task_getHere(void) {
  return chpl_task_getPrivateData()->here;
}

void chpl_task_setHere(void* here) {
  chpl_task_private_data_t* p = chpl_task_getPrivateData();
  if (p == NULL)
    chpl_internal_error("no task-private data in chpl_task_setHere.");
  else
    p->here = here;
}

c_localeid_t chpl_task_getLocaleID(void) {
  return chpl_task_getPrivateData()->localeID;
}

void chpl_task_setLocaleID(c_localeid_t localeID) {
  chpl_task_private_data_t* p = chpl_task_getPrivateData();
  if (p == NULL)
    chpl_internal_error("no task-private data in chpl_task_setLocaleID.");
  else
    p->localeID = localeID;
}

//
//////////////////////////////////////////////////////////////////////////////////

chpl_task_subLoc_t chpl_task_getNumSubLocales(void) { return 1; }

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
