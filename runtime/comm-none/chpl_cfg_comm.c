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
#include "chplthreads.h"

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

int32_t chpl_comm_getMaxThreads(void) {
  return 0;
}

int32_t chpl_comm_maxThreadsLimit(void) {
  return 0;
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
  chpl_numLocales = 1;
  chpl_localeID = 0;
}

int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  int i;
  char* command = chpl_glom_strings(2, "gdb -q -ex 'break gdbShouldBreakHere' --args ",
                                    argv[0]);
  for (i=1; i<argc; i++) {
    if (i != gdbArgnum) {
      command = chpl_glom_strings(3, command, " ", argv[i]);
    }
  }
  *status = mysystem(command, "running gdb", 0);

  return 1;
}

void chpl_comm_init_shared_heap(void) {
  chpl_initHeap(NULL, 0);
}

void chpl_comm_rollcall(void) {
  chpl_msg(2, "executing on a single locale\n");
}

void chpl_comm_alloc_registry(int numGlobals) { 
  chpl_globals_registry = chpl_globals_registry_static;
}

void chpl_comm_broadcast_global_vars(int numGlobals) { }

void chpl_comm_broadcast_private(void* addr, int size) { }

void chpl_comm_barrier(const char *msg) { }

void chpl_comm_exit_any(int status) { }

void chpl_comm_exit_all(int status) { }

void  chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  memcpy(raddr, addr, size);
}

void  chpl_comm_get(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  memcpy(addr, raddr, size);
}

typedef struct {
  chpl_fn_int_t fid;
  int           arg_size;
  char          arg[0];       // variable-sized data here
} fork_t;

static void fork_nb_wrapper(fork_t* f) {
  if (f->arg_size)
    (*chpl_ftable[f->fid])(&f->arg);
  else
    (*chpl_ftable[f->fid])(0);
  chpl_free(f, false, 0, 0);
}

void chpl_comm_fork_nb(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  fork_t *info;
  int     info_size;

  info_size = sizeof(fork_t) + arg_size;
  info = (fork_t*)chpl_malloc(info_size, sizeof(char), "chpl_comm_fork_nb info", false, 0, 0);
  info->fid = fid;
  info->arg_size = arg_size;
  if (arg_size)
    memcpy(&(info->arg), arg, arg_size);
  chpl_begin((chpl_fn_p)fork_nb_wrapper, (void*)info, false, false, NULL);
}

void chpl_comm_fork(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  (*chpl_ftable[fid])(arg);
}

void chpl_startVerboseComm() { }
void chpl_stopVerboseComm() { }
void chpl_startVerboseCommHere() { }
void chpl_stopVerboseCommHere() { }

void chpl_startCommDiagnostics() { }
void chpl_stopCommDiagnostics() { }
void chpl_startCommDiagnosticsHere() { }
void chpl_stopCommDiagnosticsHere() { }


int32_t chpl_numCommGets(void) { return 0; }
int32_t chpl_numCommPuts(void) { return 0; }
int32_t chpl_numCommForks(void) { return 0; }
int32_t chpl_numCommNBForks(void) { return 0; }
