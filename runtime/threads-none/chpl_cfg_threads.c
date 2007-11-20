#include <stdint.h>
#include "chplrt.h"
#include "chplthreads.h"
#include "chplmem.h"
#include "error.h"

_chpl_mutex_p _chpl_mutex_new(void) { return 0; }
int _chpl_mutex_init(_chpl_mutex_p mutex) { return 0; }
int _chpl_mutex_lock(_chpl_mutex_p mutex) { return 0; }
int _chpl_mutex_trylock(_chpl_mutex_p mutex) { return 0; }
int _chpl_mutex_unlock(_chpl_mutex_p mutex) { return 0; }
int _chpl_mutex_destroy(_chpl_mutex_p mutex) { return 0; }

_chpl_condvar_p _chpl_condvar_new(void) { return 0; }
int _chpl_condvar_init (_chpl_condvar_p cond) { return 0; }
int _chpl_condvar_destroy(_chpl_condvar_p cond) { return 0; }
int _chpl_condvar_signal(_chpl_condvar_p cond) { return 0; }
int _chpl_condvar_broadcast(_chpl_condvar_p cond) { return 0; }
#if 0
int _chpl_condvar_wait(_chpl_condvar_p cond, _chpl_mutex_p mutex) { return 0; }
#endif

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
