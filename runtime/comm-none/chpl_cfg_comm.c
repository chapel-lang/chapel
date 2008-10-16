#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "chplrt.h"
#include "chplcomm.h"
#include "chplexit.h"
#include "error.h"
#include "chplmem.h"

// Helper functions

static int mysystem(const char* command, const char* description, 
                    int ignorestatus) {
  int status = system(command);

  if (status == -1) {
    chpl_error("system() fork failed", 0, "(command-line)");
  } else if (status != 0 && !ignorestatus) {
    chpl_error(description, 0, "(command-line)");
  }

  return status;
}

// Chapel interface

int32_t _chpl_comm_getMaxThreads(void) {
  return 0;
}

int32_t _chpl_comm_maxThreadsLimit(void) {
  return 0;
}

int _chpl_comm_user_invocation(int argc, char* argv[]) {
  return 1;
}

char** _chpl_comm_create_argcv(int32_t numLocales, int argc, char* argv[],
                               int* commArgc) {
  *commArgc = argc;
  return argv;
}

void _chpl_comm_init(int *argc_p, char ***argv_p, int runInGDB) {
  if (runInGDB == 0) {
    _numLocales = 1;
    _localeID = 0;
  } else {
    char* command;
    int status;
    int i;

    command = _glom_strings(2, "gdb -q -ex 'break gdbShouldBreakHere' --args ", 
                            (*argv_p)[0]);
    for (i=1; i<*argc_p; i++) {
      if (i != runInGDB) {
        command = _glom_strings(3, command, " ", (*argv_p)[i]);
      }
    }
    status = mysystem(command, "running gdb", 0);

    _chpl_exit_all(status);
  }
}

void _chpl_comm_init_shared_heap(void) {
  initHeap(NULL, 0);
}

void _chpl_comm_rollcall(void) {
  chpl_msg(2, "executing on a single locale\n");
}

void _chpl_comm_alloc_registry(int numGlobals) { 
  _global_vars_registry = _global_vars_registry_static;
}

void _chpl_comm_broadcast_global_vars(int numGlobals) { }

void _chpl_comm_broadcast_private(void* addr, int size) { }

void _chpl_comm_barrier(const char *msg) { }

void _chpl_comm_exit_any(int status) { }

void _chpl_comm_exit_all(int status) { }

void  _chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size) {
  memcpy(raddr, addr, size);
}

void  _chpl_comm_get(void* addr, int32_t locale, const void* raddr, int32_t size) {
  memcpy(addr, raddr, size);
}

void _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) {
  (*f)(arg);
}

void _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size) {
  (*f)(arg);
}
