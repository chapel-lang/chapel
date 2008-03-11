#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"
#include <stdint.h>

// Mutex

int chpl_mutex_lock(chpl_mutex_p mutex) { return 0; }
void chpl_mutex_unlock(chpl_mutex_p mutex) { }


// Sync variables

int chpl_sync_lock(chpl_sync_aux_t *s) { return s == NULL; }
void chpl_sync_unlock(chpl_sync_aux_t *s) { }

int chpl_sync_wait_full_and_lock(chpl_sync_aux_t *s, int32_t lineno, _string filename) {
  if (*s)
    return 0;
  else {
    chpl_error("sync var empty (running in single-threaded mode)", lineno, filename);
    return 1;
  }
}

int chpl_sync_wait_empty_and_lock(chpl_sync_aux_t *s, int32_t lineno, _string filename) {
  if (*s) {
    chpl_error("sync var full (running in single-threaded mode)", lineno, filename);
    return 1;
  }
  else return 0;
}

void chpl_sync_mark_and_signal_full(chpl_sync_aux_t *s) {
  *s = true;
}

void chpl_sync_mark_and_signal_empty(chpl_sync_aux_t *s) {
  *s = false;
}

chpl_bool chpl_sync_is_full(void *val_ptr, chpl_sync_aux_t *s, chpl_bool simple_sync_var) {
  return *s;
}

void chpl_init_sync_aux(chpl_sync_aux_t *s) {
  *s = false;
}


// Single variables

int chpl_single_lock(chpl_single_aux_t *s) { return s == NULL; }

int chpl_single_wait_full(chpl_single_aux_t *s, int32_t lineno, _string filename) {
  if (*s)
    return 0;
  else {
    chpl_error("single var empty (running in single-threaded mode)", lineno, filename);
    return 1;
  }
}

void chpl_single_mark_and_signal_full(chpl_single_aux_t *s) {
  *s = true;
}

chpl_bool chpl_single_is_full(void *val_ptr, chpl_single_aux_t *s, chpl_bool simple_single_var) {
  return *s;
}

void chpl_init_single_aux(chpl_single_aux_t *s) {
  *s = false;
}


// Threads

int32_t chpl_threads_getMaxThreads(void) { return 1; }
int32_t chpl_threads_maxThreadsLimit(void) { return 1; }

void initChplThreads() { }
void exitChplThreads() { }
void chpl_thread_init(void) { }

uint64_t chpl_thread_id(void) { return 0; }
chpl_bool chpl_get_serial(void) { return true; }
void chpl_set_serial(chpl_bool state) {
  if (!state)
    chpl_error("cannot set serial state to false when running in single-threaded mode", 0, 0);
}

int
chpl_begin(chpl_threadfp_t fp, chpl_threadarg_t a, _Bool serial_state) {
  (*fp)(a);
  return 0;
}
