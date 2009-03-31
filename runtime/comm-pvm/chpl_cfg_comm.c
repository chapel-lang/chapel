#include "chplrt.h"
#include "chplcomm.h"
#include <stdio.h>

//
// Chapel interface starts here
//


int32_t chpl_comm_getMaxThreads(void) {
  fprintf(stderr, "chpl_comm_getMaxThreads called");
  return -1;
}

int32_t chpl_comm_maxThreadsLimit(void) {
  fprintf(stderr, "chpl_comm_maxThreadsLimit called");
  return -1;
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
  fprintf(stderr, "chpl_comm_init called");
  return;
}

//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

void chpl_comm_rollcall(void) {
  fprintf(stderr, "chpl_comm_rollcall called");
  return;
}

void chpl_comm_init_shared_heap(void) {
  fprintf(stderr, "chpl_comm_init_shared_heap called");
  return;
}

void chpl_comm_alloc_registry(int numGlobals) {
  fprintf(stderr, "chpl_comm_alloc_registry called");
  return;
}

void chpl_comm_broadcast_global_vars(int numGlobals) {
  fprintf(stderr, "chpl_comm_broadcast_global_vars called");
  return;
}

void chpl_comm_broadcast_private(void* addr, int size) {
  fprintf(stderr, "chpl_comm_broadcast_private called");
  return;
}

void chpl_comm_barrier(const char *msg) {
  fprintf(stderr, "chpl_comm_barrier called");
  return;
}

void chpl_comm_exit_all(int status) {
  fprintf(stderr, "chpl_comm_exit_all called");
  return;
}

void chpl_comm_exit_any(int status) {
  fprintf(stderr, "chpl_comm_exit_any called");
  return;
}

void  chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  fprintf(stderr, "chpl_comm_put called");
  return;
}

void  chpl_comm_get(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  fprintf(stderr, "chpl_comm_get called");
  return;
}

void  chpl_comm_fork(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  fprintf(stderr, "chpl_comm_fork called");
  return;
}

void  chpl_comm_fork_nb(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  fprintf(stderr, "chpl_comm_fork_nb called");
  return;
}
