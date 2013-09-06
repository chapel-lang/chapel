//
// Qthreads implementation of Chapel communication interface
//
// Copyright 2011 Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000, there is a non-exclusive license for use of this work by
// or on behalf of the U.S. Government. Export of this program may require a
// license from the United States Government
//

#ifndef _chpl_comm_h_
#define _chpl_comm_h_

#ifndef LAUNCHER

#include <stdint.h>

#include "qthread/qthread.h"
#include "qthread/multinode.h"
#include "qthread/spr.h"

//
// Shared interface (implemented in chpl-comm.c)
//
extern int32_t chpl_localeID; // unique ID for each locale: 0, 1, 2, ...
extern int32_t chpl_numLocales; // number of locales

extern int32_t chpl_numPrivateObjects;
extern void** chpl_privateObjects; // privatized array and domain objects

#define chpl_numPrivatizedClasses() chpl_numPrivateObjects
extern void chpl_newPrivatizedClass(void*);
extern void* chpl_getPrivatizedClass(int32_t);

//
// Shared interface (implemented in the compiler generated code)
//
extern void chpl__heapAllocateGlobals(void);

extern const int chpl_numGlobalsOnHeap;
extern void** chpl_globals_registry;
extern void* chpl_globals_registry_static[];

extern void* const chpl_private_broadcast_table[];

extern const int chpl_heterogeneous;


//
// Comm layer-specific interface
//

//
// returns the maximum number of threads that can be handled
// by this communication layer (used to ensure numThreadsPerLocale is
// legal); should return the sentinel value of 0 if the communication
// layer imposes no particular limit on the number of threads.
//
int32_t chpl_comm_getMaxThreads(void);


//
// initializes the communications package
//   set chpl_localeID and chpl_numLocales
// notes:
//   * Called with the argc/argv pair passed to main()
//
void chpl_comm_init(int *argc_p, char ***argv_p);

//
// Allow the communication layer to do any secondary initialization it needs
// to, after the memory layer is initialized.
//
void chpl_comm_post_mem_init(void);

//
// if possible, run in gdb (because the user threw the --gdb flag)
// using argc and argv.  gdbArgnum gives the index of the argv[]
// element containing the --gdb flag.  Return the status of that
// process in "status" and return 1 if it was possible to run in gdb,
// 0 otherwise
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status);

//
// Allow the communication layer to do any further initialization it
// needs to, after the tasking layer is initialized.
//
void chpl_comm_post_task_init(void);

//
// a final comm layer stub before barrier synching and calling into
// the user code.  It is recommended that a debugging message be
// printed here indicating that each locale has started using
// chpl_msg() and a verbosity level of 2 (which will cause it to be
// displayed using the -v flag).
//
void chpl_comm_rollcall(void);

//
// Inform callers as to the communication layer's desired starting address
// and length for the shared heap, if any.
//
void chpl_comm_desired_shared_heap(void** start_p, size_t* size_p);

//
// allocate chpl_globals_registry or make it point to
// chpl_globals_registry_static depending on the communication layer
//
void chpl_comm_alloc_registry(int numGlobals);

//
// chpl_globals_registry is an array of pointers to addresses; on
// locale 0, these addresses point locally to a class; they are
// uninitialized elsewhere.  This function makes it so that the
// addresses on all other locales are the same as on locale 0.
//
// This function is called collectively.
//
void chpl_comm_broadcast_global_vars(int numGlobals);

//
// Broadcast the value of 'id'th entry in chpl_private_broadcast_table
// on the calling locale onto every other locale.  This is done to set
// up global constants of simple scalar types (primarily).
//
void chpl_comm_broadcast_private(int id, int32_t size, int32_t tid);

//
// barrier for synchronization between all processes; currently only
// used for startup and teardown.  msg is a string that can be used
// for debugging to determine where the barrier is being called.
//
void chpl_comm_barrier(const char *msg);

//
// Do exit processing that has to occur before the tasking layer is
// shut down.  "The "all" parameter is true for normal, collective
// program termination, and false for that done by a single locale
// due to error or a user program halt or exit.
//
// Note: Chapel's program termination is not yet fully thought out.
//
void chpl_comm_pre_task_exit(int all);

//
// Terminate the communication layer.  "The "all" parameter is true
// for normal, collective program termination, and false for that
// done by a single locale due to error or a user program halt or
// exit.  If the communication layer likes to call exit, the exit
// code is provided using the "status" argument; if it doesn't, it
// can simply return and the Chapel program will call exit().
//
// Notes:
//   this function is called last
//   a barrier is invoked before calling into this function
//   Chapel's program termination is not yet fully thought out.
//
void chpl_comm_exit(int all, int status);

//
// put 'size' bytes of local data at 'addr' to remote data at
// 'raddr' on locale 'locale'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  chpl_comm_put(void* addr, int32_t locale, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn);

//
// get 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  chpl_comm_get(void *addr, int32_t locale, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn);

//
// remote fork should launch a thread on locale that runs function f
// passing it arg where the size of arg is stored in arg_size
// notes:
//   multiple forks to the same locale should be handled concurrently
//
void chpl_comm_fork(int locale, chpl_fn_int_t fid,
                    void *arg, int32_t arg_size, int32_t arg_tid);

//
// non-blocking fork
//
void chpl_comm_fork_nb(int locale, chpl_fn_int_t fid,
                       void *arg, int32_t arg_size, int32_t arg_tid);

//
// fast (non-forking) fork (i.e., run in handler)
//
void chpl_comm_fork_fast(int locale, chpl_fn_int_t fid, void *arg,
                         int32_t arg_size, int32_t arg_tid);


//
// This call specifies the number of polling tasks that the
// communication layer will need (see just below for a definition).
// The value it returns is passed to chpl_task_init(), in order to
// forewarn the tasking layer whether the comm layer will need a
// polling task.  In the current implementation, it should only
// return 0 or 1.
//
int chpl_comm_numPollingTasks(void);



//
// Comm diagnostics stuff
//
typedef struct _chpl_commDiagnostics {
  uint64_t get;
  uint64_t get_nb;
  uint64_t get_nb_test;
  uint64_t get_nb_wait;
  uint64_t put;
  uint64_t fork;
  uint64_t fork_fast;
  uint64_t fork_nb;
} chpl_commDiagnostics;

void chpl_startVerboseComm(void);
void chpl_stopVerboseComm(void);
void chpl_startVerboseCommHere(void);
void chpl_stopVerboseCommHere(void);

void chpl_startCommDiagnostics(void);
void chpl_stopCommDiagnostics(void);
void chpl_startCommDiagnosticsHere(void);
void chpl_stopCommDiagnosticsHere(void);
void chpl_resetCommDiagnosticsHere(void);
void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd);

//
// These are still supported because our extern record support is
//  still a bit lacking.
//
uint64_t chpl_numCommGets(void);
uint64_t chpl_numCommNBGets(void);
uint64_t chpl_numCommTestNBGets(void);
uint64_t chpl_numCommWaitNBGets(void);
uint64_t chpl_numCommPuts(void);
uint64_t chpl_numCommForks(void);
uint64_t chpl_numCommFastForks(void);
uint64_t chpl_numCommNBForks(void);

#else // LAUNCHER

#define chpl_comm_barrier(x)
#define chpl_comm_exit_all(x) exit(x)
#define chpl_comm_exit_any(x) exit(x)

#endif // LAUNCHER

#endif
