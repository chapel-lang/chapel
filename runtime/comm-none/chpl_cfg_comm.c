#include "chplrt.h"
#include "chplcomm.h"

void _chpl_comm_init(int *argc_p, char ***argv_p) {
  _numLocales = 1;
  _localeID = 0;
}

void _chpl_comm_barrier(char *msg) { }

void _chpl_comm_done(void) { }

void  _chpl_comm_write(void* addr, _int32 locale, void* raddr, _int32 size) { }

void  _chpl_comm_read(void *addr, _int32 locale, void* raddr, _int32 size) { }

void _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) { }

void _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size) {
  (*f)(0);
}
