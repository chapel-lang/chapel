#include "chplrt.h"
#include "chplcomm.h"
#include "chplmem.h"
#include "chplthreads.h"
#include "error.h"
#include "pvm3.h"
#include <stdio.h>
#include <string.h>

static int chpl_comm_diagnostics = 0;           // set via startCommDiagnostics
static int chpl_comm_gets = 0;
static int chpl_comm_puts = 0;
static int chpl_comm_forks = 0;
static int chpl_comm_nb_forks = 0;
static int chpl_verbose_comm = 0;               // set via startVerboseComm
static int chpl_comm_no_debug_private = 0;

int instance;

int foo = 0;

//
// Chapel interface starts here
//


int32_t chpl_comm_getMaxThreads(void) {
  fprintf(stderr, "chpl_comm_getMaxThreads called\n");
  return -1;
}

int32_t chpl_comm_maxThreadsLimit(void) {
  fprintf(stderr, "chpl_comm_maxThreadsLimit called\n");
  return -1;
}

static void polling(void* x) {
  foo = 1;
  fprintf(stderr, "polling thread created\n");
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
  pthread_t polling_thread;
  int status;
  int tid;
  int i;

  for (i = 1; i < argc_p[0]; i++) {
    fprintf(stderr, "arg %d is %s\n", i, (*argv_p)[i]);
  }
  if ((instance = pvm_joingroup((char *)"job")) < 0) {
    pvm_perror((char *)"Error joining job group");
    chpl_internal_error("Exiting.");
  }
  tid = pvm_mytid();
  fprintf(stderr, "mytid is %d, instance is %d, tid is %d, size is %d\n", tid, instance, pvm_gettid((char *)"job", instance), pvm_gsize((char *)"job"));

  fprintf(stderr, "foo is prior to pthread %d\n", foo);
  status = pthread_create(&polling_thread, NULL, (void*(*)(void*))polling, 0);
  if (status)
    chpl_internal_error("unable to start polling thread for PVM");
  pthread_detach(polling_thread);
  fprintf(stderr, "foo is after pthread %d\n", foo);
  
  fprintf(stderr, "chpl_comm_init called\n");
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
  initHeap(NULL, 0);
  fprintf(stderr, "chpl_comm_init_shared_heap called\n");
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
  if (chpl_verbose_comm && !chpl_comm_no_debug_private)
    printf("%d: barrier for '%s'\n", chpl_localeID, msg);
  fprintf(stderr, "chpl_comm_barrier called\n");
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

void chpl_startVerboseComm() {
  chpl_verbose_comm = 1;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(&chpl_verbose_comm, sizeof(int));
  chpl_comm_no_debug_private = 0;
}

void chpl_stopVerboseComm() {
  chpl_verbose_comm = 0;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(&chpl_verbose_comm, sizeof(int));
  chpl_comm_no_debug_private = 0;
}

void chpl_startVerboseCommHere() {
  chpl_verbose_comm = 1;
}

void chpl_stopVerboseCommHere() {
  chpl_verbose_comm = 0;
}

void chpl_startCommDiagnostics() {
  chpl_comm_diagnostics = 1;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(&chpl_comm_diagnostics, sizeof(int));
  chpl_comm_no_debug_private = 0;
}

void chpl_stopCommDiagnostics() {
  chpl_comm_diagnostics = 0;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(&chpl_comm_diagnostics, sizeof(int));
  chpl_comm_no_debug_private = 0;
}

void chpl_startCommDiagnosticsHere() {
  chpl_comm_diagnostics = 1;
}

void chpl_stopCommDiagnosticsHere() {
  chpl_comm_diagnostics = 0;
}

int32_t chpl_numCommGets(void) {
  return chpl_comm_gets;
}

int32_t chpl_numCommPuts(void) {
  return chpl_comm_puts;
}

int32_t chpl_numCommForks(void) {
  return chpl_comm_forks;
}

int32_t chpl_numCommNBForks(void) {
  return chpl_comm_nb_forks;
}
