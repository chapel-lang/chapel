//
// Generic implementation of Chapel interface for sync and single variables
//

#ifdef __OPTIMIZE__
// Turn assert() into a no op if the C compiler defines the macro above.
#define NDEBUG
#endif

#include "chapel_code.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "chpl_mem.h"
#include "chplrt.h"
#include "chplthreads.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>


//
// This file is effectively empty unless the thread-specific configuration
// file says it is needed.
//
#ifdef USE_GENERIC_SYNC_SINGLE_SUPPORT


// Sync variables

static void sync_wait_and_lock(chpl_sync_aux_t *s,
                               chpl_bool want_full,
                               int32_t lineno,
                               chpl_string filename) {
  chpl_mutex_lock(s->lock);
  while (s->is_full != want_full) {
    if (chpl_thread_set_block_loc(lineno, filename)) {
      // all other tasks appear to be blocked
      struct timeval deadline, now;
      chpl_bool timed_out;
      gettimeofday(&deadline, NULL);
      deadline.tv_sec += 1;
      do {
        timed_out = threadlayer_sync_suspend(s, &deadline);
        if (s->is_full != want_full && !timed_out)
            gettimeofday(&now, NULL);
      } while (s->is_full != want_full
               && !timed_out
               && (now.tv_sec < deadline.tv_sec
                   || (now.tv_sec == deadline.tv_sec
                       && now.tv_usec < deadline.tv_usec)));
      if (s->is_full != want_full)
        chpl_thread_check_for_deadlock();
    }
    else {
      do {
        (void) threadlayer_sync_suspend(s, NULL);
      } while (s->is_full != want_full);
    }
    chpl_thread_unset_block_loc();
  }
}

void chpl_sync_lock_generic(chpl_sync_aux_t *s) {
  chpl_mutex_lock(s->lock);
}

void chpl_sync_unlock_generic(chpl_sync_aux_t *s) {
  chpl_mutex_unlock(s->lock);
}

void chpl_sync_wait_full_and_lock_generic(chpl_sync_aux_t *s,
                                          int32_t lineno,
                                          chpl_string filename) {
  sync_wait_and_lock(s, true, lineno, filename);
}

void chpl_sync_wait_empty_and_lock_generic(chpl_sync_aux_t *s,
                                           int32_t lineno,
                                           chpl_string filename) {
  sync_wait_and_lock(s, false, lineno, filename);
}

void chpl_sync_mark_and_signal_full_generic(chpl_sync_aux_t *s) {
  s->is_full = true;
  threadlayer_sync_awaken(s);
  chpl_sync_unlock_generic(s);
}

void chpl_sync_mark_and_signal_empty_generic(chpl_sync_aux_t *s) {
  s->is_full = false;
  threadlayer_sync_awaken(s);
  chpl_sync_unlock_generic(s);
}

chpl_bool chpl_sync_is_full_generic(void *val_ptr,
                                    chpl_sync_aux_t *s,
                                    chpl_bool simple_sync_var) {
  return s->is_full;
}

void chpl_init_sync_aux_generic(chpl_sync_aux_t *s) {
  s->is_full = false;
  s->lock = chpl_mutex_new();
  threadlayer_init_sync(s);
}

void chpl_destroy_sync_aux_generic(chpl_sync_aux_t *s) {
  chpl_free(s->lock, 0, 0);
  threadlayer_destroy_sync(s);
}


// Single variables

void chpl_single_lock_generic(chpl_single_aux_t *s) {
  chpl_mutex_lock((chpl_mutex_t*)s->lock);
}

void chpl_single_unlock_generic(chpl_single_aux_t *s) {
  chpl_mutex_unlock((chpl_mutex_t*)s->lock);
}

void chpl_single_wait_full_generic(chpl_single_aux_t *s,
                                   int32_t lineno, chpl_string filename) {
  chpl_mutex_lock(s->lock);
  while (!s->is_full) {
    if (chpl_thread_set_block_loc(lineno, filename)) {
      // all other tasks appear to be blocked
      struct timeval deadline, now;
      chpl_bool timed_out;
      gettimeofday(&deadline, NULL);
      deadline.tv_sec += 1;
      do {
        timed_out = threadlayer_single_suspend(s, &deadline);
        if (!s->is_full && !timed_out)
            gettimeofday(&now, NULL);
      } while (!s->is_full
               && !timed_out
               && (now.tv_sec < deadline.tv_sec
                   || (now.tv_sec == deadline.tv_sec
                       && now.tv_usec < deadline.tv_usec)));
      if (!s->is_full)
        chpl_thread_check_for_deadlock();
    }
    else {
      do {
        (void) threadlayer_single_suspend(s, NULL);
      } while (!s->is_full);
    }
    chpl_thread_unset_block_loc();
  }
}

void chpl_single_mark_and_signal_full_generic(chpl_single_aux_t *s) {
  s->is_full = true;
  threadlayer_single_awaken(s);
  chpl_single_unlock_generic(s);
}

chpl_bool chpl_single_is_full_generic(void *val_ptr,
                                      chpl_single_aux_t *s,
                                      chpl_bool simple_single_var) {
  return s->is_full;
}

void chpl_init_single_aux_generic(chpl_single_aux_t *s) {
  s->is_full = false;
  s->lock = chpl_mutex_new();
  threadlayer_init_single(s);
}

void chpl_destroy_single_aux_generic(chpl_single_aux_t *s) {
  chpl_free(s->lock, 0, 0);
  threadlayer_destroy_single(s);
}


#endif  // USE_GENERIC_SYNC_SINGLE_SUPPORT
