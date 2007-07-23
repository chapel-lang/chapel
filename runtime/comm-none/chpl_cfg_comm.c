#include "chplrt.h"
#include "chplcomm.h"

void _chpl_comm_init(int *argc_p, char ***argv_p) { }

void _chpl_comm_barrier(char *msg) { }

_int32 _chpl_comm_locale_id(void) { return 0; }

_int32 _chpl_comm_num_locales(void) { return 1; }

void _chpl_comm_done(void) { }

void _chpl_comm_write(_chpl_comm_ptr_t *p, void *addr) { }

void _chpl_comm_read(void *addr, _chpl_comm_ptr_t *p) { }

void _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) { }

void _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size) {
  (*f)(0);
}
