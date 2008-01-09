#include "chplrt.h"
#include "chplcomm.h"
#include "error.h"
#include <string.h>

int _chpl_comm_user_invocation(int argc, char* argv[]) {
  return 1;
}

int _chpl_comm_default_num_locales(void) {
  return 1;
}

void _chpl_comm_verify_num_locales(_int32 proposedNumLocales) {
  if (proposedNumLocales != 1) {
    _printError("Only 1 locale may be used for CHPL_COMM layer 'none'", 0, 0);
  }
}

char** _chpl_comm_create_argcv(_int32 numLocales, int argc, char* argv[],
                               int* commArgc) {
  *commArgc = argc;
  return argv;
}

void _chpl_comm_init(int *argc_p, char ***argv_p) {
  _numLocales = 1;
  _localeID = 0;
}

void _chpl_comm_rollcall(void) {
  _chpl_msg(2, "executing on a single locale\n");
}

void _chpl_comm_barrier(const char *msg) { }

void _chpl_comm_exit_any(int status) { }

void _chpl_comm_exit_all(int status) { }

void  _chpl_comm_put(void* addr, _int32 locale, void* raddr, _int32 size) {
  memcpy(raddr, addr, size);
}

void  _chpl_comm_get(void* addr, _int32 locale, void* raddr, _int32 size) {
  memcpy(addr, raddr, size);
}

void _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) {
  (*f)(arg);
}

void _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size) {
  (*f)(arg);
}
