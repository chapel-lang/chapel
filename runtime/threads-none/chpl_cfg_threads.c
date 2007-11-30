#include <stdint.h>
#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"

int _chpl_mutex_init(_chpl_mutex_p mutex) { return mutex == NULL; }
int _chpl_mutex_lock(_chpl_mutex_p mutex) { return mutex == NULL; }
int _chpl_mutex_unlock(_chpl_mutex_p mutex) { return mutex == NULL; }
int _chpl_mutex_destroy(_chpl_mutex_p mutex) { return mutex == NULL; }

int _chpl_sync_lock(_chpl_sync_aux_t *s) { return s == NULL; }
int _chpl_sync_unlock(_chpl_sync_aux_t *s) { return s == NULL; }

int _chpl_sync_wait_full_and_lock(_chpl_sync_aux_t *s, _int32 lineno, _string filename) {
  if (*s)
    return 0;
  else {
    _printError("sync var empty (running in single-threaded threaded mode)", lineno, filename);
    return 1;
  }
}

int _chpl_sync_wait_empty_and_lock(_chpl_sync_aux_t *s, _int32 lineno, _string filename) {
  if (*s) {
    _printError("sync var full (running in single-threaded threaded mode)", lineno, filename);
    return 1;
  }
  else return 0;
}

int _chpl_sync_mark_and_signal_full(_chpl_sync_aux_t *s) {
  *s = true;
  return 0;
}

int _chpl_sync_mark_and_signal_empty(_chpl_sync_aux_t *s) {
  *s = false;
  return 0;
}

_bool _chpl_sync_is_full(void *val_ptr, _chpl_sync_aux_t *s, _bool simple_sync_var) {
  return *s;
}

void _chpl_init_sync_aux(_chpl_sync_aux_t *s) {
  *s = false;
}


void _chpl_serial_delete(_bool *p) { }
void initChplThreads() { }
void exitChplThreads() { }
void _chpl_thread_init(void) { }

_uint64 _chpl_thread_id(void) { return 0; }
_bool _chpl_get_serial(void) { return true; }
void _chpl_set_serial(_bool state) { }

int
_chpl_cobegin(int                      nthreads, 
              _chpl_threadfp_t        *fp, 
              _chpl_threadarg_t       *a, 
              _chpl_cobegin_wkspace_t *twrk) {
  int t;

  for (t=0; t<nthreads; t++) {
    (*fp[t])(a[t]);
  }
  return 0;
}

int
_chpl_begin(_chpl_threadfp_t fp, _chpl_threadarg_t a) {
  (*fp)(a);
  return 0;
}
