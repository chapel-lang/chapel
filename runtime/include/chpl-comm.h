/*
 * Copyright 2004-2017 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _chpl_comm_h_
#define _chpl_comm_h_

#ifndef LAUNCHER

#include <stdint.h>
#include "chpltypes.h"
#include "chpl-comm-impl.h"
#include "chpl-comm-heap-macros.h"
#include "chpl-tasks.h"
#include "chpl-comm-task-decls.h"
#include "chpl-comm-locales.h"

//
// Shared interface (implemented in chpl-comm.c)
//
extern c_nodeid_t chpl_nodeID; // unique ID for each node: 0, 1, 2, ...
// Note that this is the comm node ID: it carries only the .node
// portion of the chpl_localeID_t structure that represents the locale on which
// the current task is running.
// Note also that this value is set only in chpl_comm_init to a value which is
// (hopefully) unique to the running image, and never changed again.
extern int32_t chpl_numNodes; // number of nodes

extern int chpl_verbose_comm;     // set via startVerboseComm
extern int chpl_comm_diagnostics; // set via startCommDiagnostics
extern int chpl_verbose_mem;      // set via startVerboseMem

size_t chpl_comm_getenvMaxHeapSize(void);


//
// Shared interface (implemented in the compiler generated code)
//
extern void chpl__heapAllocateGlobals(void);

extern const int chpl_numGlobalsOnHeap;

//
// chpl_globals_registry is an array of size chpl_numGlobalsOnHeap
// storing ptr_wide_ptr_t, that is, pointers to wide pointers. All
// registered globals are wide pointers.  Locales other than 0 need to
// set their registered globals to the wide pointers received from
// Locale 0, which is why these have type ptr_wide_ptr_t.  This is
// done in chpl_comm_broadcast_global_vars() below.
//
extern ptr_wide_ptr_t chpl_globals_registry[];

extern void* const chpl_private_broadcast_table[];

extern void* const chpl_global_serialize_table[];

extern const int chpl_heterogeneous;


//
// Comm layer-specific interface
//

// chpl_comm_nb_handle_t must be defined in the comm layer header
// chpl-comm-task-decls.h

// uses comm-layer specific chpl_comm_bundleData_t
// uses task-layer specific chpl_task_bundleData_t

typedef struct {
  // Including space for the task_bundle here helps with
  // running tasks locally, but it doesn't normally need
  // to be communicated over the network.
  chpl_task_bundle_t task_bundle;
  // Including space for some comm information here helps
  // the comm layer communicate some values to a wrapper
  // function that is run in a task.
  chpl_comm_bundleData_t comm;
} chpl_comm_on_bundle_t;

typedef chpl_comm_on_bundle_t *chpl_comm_on_bundle_p;

static inline
chpl_task_bundle_t* chpl_comm_on_bundle_task_bundle(chpl_comm_on_bundle_t* a)
{
  return &a->task_bundle;
}

//
// Call a chpl_ftable[] function in a task.
//
// This is a convenience function for use by the module code, in which
// we have function table indices rather than function pointers.
//
static inline
void chpl_comm_taskCallFTable(chpl_fn_int_t fid,      // ftable[] entry to call
                              chpl_comm_on_bundle_t* arg,// function arg
                              size_t arg_size,        // length of arg in bytes
                              c_sublocid_t subloc,    // desired sublocale
                              int lineno,             // source line
                              int32_t filename) {     // source filename
    chpl_task_taskCallFTable(fid,
                             chpl_comm_on_bundle_task_bundle(arg), arg_size,
                             subloc,
                             lineno, filename);
}



// Do a GET in a nonblocking fashion, returning a handle which can be used to
// wait for the GET to complete. The destination buffer must not be modified
// before the request completes (after waiting on the returned handle)
chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t typeIndex,
                                       int32_t commID, int ln, int32_t fn);

// Do a PUT in a nonblocking fashion, returning a handle which can be used to
// wait for the PUT to complete. The source buffer must not be modified before
// the request completes (after waiting on the returned handle)
chpl_comm_nb_handle_t chpl_comm_put_nb(void *addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t typeIndex,
                                       int32_t commID, int ln, int32_t fn);

// Returns nonzero iff the handle has already been waited for and has
// been cleared out in a call to chpl_comm_{wait,try}_some.
int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h);

// Wait on handles created by chpl_comm_start_....  ignores completed handles.
// Clears out completed handles so that calling chpl_comm_nb_handle_is_complete
// on them returns nonzero.
void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles);

// Try handles created by chpl_comm_start_....  ignores completed handles.
// Clears out completed handles so that calling chpl_comm_nb_handle_is_complete
// on them returns nonzero.  Returns nonzero iff at least one completion is
// detected.
int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles);

// Returns whether or not the passed wide address is known to be in
// a communicable memory region and known to be readable. That is,
// GET to that address should succeed without an access violation
// or other memory protection error.
// Returns 1 if the entire passed region can be read with a GET.
// Returns 0 if it might not be safely readable.
//
// This function returns 1 if the requested region is known to be in a
// pre-mapped registered memory region that can be read with GET without
// any segmentation errors. For GASNET_SEGMENT=everything, for example,
// this function always returns 0 since although any address is in the
// segment, not all addresses are readable without causing a segmentation
// violation on the remote locale.
int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t len);

//
// returns the maximum number of threads that can be handled
// by this communication layer (used to ensure numThreadsPerLocale is
// legal); should return the sentinel value of 0 if the communication
// layer imposes no particular limit on the number of threads.
//
int32_t chpl_comm_getMaxThreads(void);


//
// initializes the communications package
//   set chpl_nodeID and chpl_numNodes
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
// a final comm layer stub before barrier syncing and calling into
// the user code.  It is recommended that a debugging message be
// printed here indicating that each locale has started using
// chpl_msg() and a verbosity level of 2 (which will cause it to be
// displayed using the -v flag).
//
void chpl_comm_rollcall(void);

//
// Communication layers that need or want to register memory with the
// network interface may have to provide the space for the Chapel heap
// themselves.  If so, this provides that heap's address and length.
//
// This function may be called multiple times during program
// initialization.
//
void chpl_comm_get_registered_heap(void** start_p, size_t* size_p);

//
// This is the comm layer sub-interface for dynamic allocation and
// registration of memory. The functions are as follows:
//
// chpl_comm_regMemAllocThreshold():
//   Allocations smaller than this should be done normally, by the
//   memory layer.  Others should be done through this comm layer
//   sub-interface.
//
// chpl_comm_regMemAlloc()
//   Allocate memory, returning either a non-NULL pointer or NULL when
//   no more memory is available.  After allocation the memory can be
//   localized, filled, and so on as desired by the memory layer.
//
// chpl_comm_regMemPostAlloc()
//   Perform post-allocation operations, typically registration.  This
//   call is non-destructive to the memory contents, and should be made
//   after the memory is localized.  Do not pass memory allocated from
//   anything but chpl_comm_regMemAlloc() to this function.
//
// chpl_comm_regMemFree()
//   Free memory previously allocated by chpl_comm_regMemAlloc().  If
//   the memory did indeed come from chpl_mem_regMemAlloc(), this frees
//   it and returns true.  Otherwise it does nothing and returns false.
//   Given some memory address to be freed it is therefore safe, though
//   perhaps not performance-optimal, to first try to free it here, and 
//   only free it elsewhere if this function returns false.
//
#ifndef CHPL_COMM_IMPL_REG_MEM_ALLOC_THRESHOLD
  #define CHPL_COMM_IMPL_REG_MEM_ALLOC_THRESHOLD() SIZE_MAX
#endif
static inline
size_t chpl_comm_regMemAllocThreshold(void) {
  return CHPL_COMM_IMPL_REG_MEM_ALLOC_THRESHOLD();
}

#ifndef CHPL_COMM_IMPL_REG_MEM_ALLOC
  #define CHPL_COMM_IMPL_REG_MEM_ALLOC(size) NULL
#endif
static inline
void* chpl_comm_regMemAlloc(size_t size) {
  return CHPL_COMM_IMPL_REG_MEM_ALLOC(size);
}

#ifndef CHPL_COMM_IMPL_REG_MEM_POST_ALLOC
#define CHPL_COMM_IMPL_REG_MEM_POST_ALLOC(p, size) return
#endif
static inline
void chpl_comm_regMemPostAlloc(void* p, size_t size) {
    CHPL_COMM_IMPL_REG_MEM_POST_ALLOC(p, size);
}

#ifndef CHPL_COMM_IMPL_REG_MEM_FREE
#define CHPL_COMM_IMPL_REG_MEM_FREE(p, size) false
#endif
static inline
chpl_bool chpl_comm_regMemFree(void* p, size_t size) {
    return CHPL_COMM_IMPL_REG_MEM_FREE(p, size);
}

//
// This routine is used by the Chapel runtime to broadcast the
// locations of module-level ("global") variables to all locales
// so that all locales can put/get the value of a global variable
// directly, knowing where it lives remotely.
//
// Logically, this routine implements a collective broadcast of
// the chpl_globals_registry[] array which is an array of 'numGlobals'
// wide_ptr_t values.  Note that in a one-sided implementation, the
// implementation should not assume that chpl_globals_registry[] lives
// at the same address on every compute node.
// 
void chpl_comm_broadcast_global_vars(int numGlobals);

//
// This routine is used by the generated Chapel code to broadcast
// the values of module-level ("global") constants to all compute
// nodes so that tasks can refer to the constants locally without
// communication.
//
// Logically, this routine implements a 1-sided broadcast of a value
// across all the compute nodes.  Only one task total will call into
// this routine per logical broadcast.  For that reason, this routine
// will tend to need to be implemented by utilizing an active message
// (or equivalent) on the remote side.
//
// The job of this task is to broadcast 'size' bytes stored at the
// address indicated by chpl_private_broadcast_table[id] to all of the
// other compute nodes.  On those compute nodes, the result should be
// stored in the address stored by that node's copy of
// chpl_private_broadcast_table[id].  This table is used (instead of a
// raw address) in order to support platforms in which the instances
// of the generated C code may store globals at different addresses
// (like Mac OS X).
//
// Note that this routine is currently used only during program
// initialization, so it is arguably not as performance critical as
// other more core communication routines (like puts, gets, executeOns).
//
// The third argument, 'tid' (type ID) is intended for use when
// targeting heterogeneous architectures where byte swapping may be
// required rather than just copying the 'size' bytes.  It is not
// currently in use on any platforms, but is being retained in the
// event that we wish to re-enable this capability in the future.
// 
void chpl_comm_broadcast_private(int id, size_t size, int32_t tid);

//
// Barrier for synchronization between all top-level locales; currently
// only used for startup and teardown.  msg is a string that can be used
// for debugging to determine where the barrier is being called.  This
// function may be called from a Chapel task.  As such, if the barrier
// cannot be immediately satisfied, while it waits chpl_comm_barrier()
// must call chpl_task_yield() in order not to monopolize the execution
// resources and prevent making progress.
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
void  chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t typeIndex,
                    int32_t commID, int ln, int32_t fn);

//
// get 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void  chpl_comm_get(void *addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t typeIndex,
                    int32_t commID, int ln, int32_t fn);

//
// put the number of elements pointed out by count array, with strides pointed
// out by dststrides and srcstrides arrays. These three arrays have to be int32
// to correspond to DefaultRectangularArr.doiBulkTransferStride(). via
// DefaultRectangularArr.doiBulkTransferStrideComm().
// 'elemSize' is the size of each element, used to scale the values in count[0]
//            and strides.
// When comm=gasnet, this function ends up calling gasnet_puts_bulk().
//   More info in: http://www.escholarship.org/uc/item/5hg5r5fs?display=all
//   Proposal for Extending the UPC Memory Copy Library Functions and Supporting 
//   Extensions to GASNet, Version 2.0. Author: Dan Bonachea 
//
void  chpl_comm_put_strd(void* dstaddr, size_t* dststrides, c_nodeid_t dstnode,
                     void* srcaddr, size_t* srcstrides, size_t* count,
                     int32_t stridelevels, size_t elemSize, int32_t typeIndex, 
                     int32_t commID, int ln, int32_t fn);

//
// same as chpl_comm_puts(), but do get instead
//
void  chpl_comm_get_strd(void* dstaddr, size_t* dststrides, c_nodeid_t srcnode,
                     void* srcaddr, size_t* srcstrides, size_t* count,
                     int32_t stridelevels, size_t elemSize, int32_t typeIndex, 
                     int32_t commID, int ln, int32_t fn);

//
// Get a local copy of a wide string.
//
// The local copy is also a wide string pointer, but its addr field points to 
// a locally-allocated char[] and the locale field is set to "here".
// The local char[] buffer is leaked. :(
//
void chpl_gen_comm_wide_string_get(void *addr, c_nodeid_t node, void *raddr,
                                   size_t size, int32_t typeIndex,
                                   int ln, int32_t fn);

//
// Runs a function f on a remote locale, passing it
// arg where size of arg is stored in arg_size.
// arg can be reused immediately after this call completes.
//
// This call will block the current task until the remote function has
// completed. Use chpl_comm_execute_on_nb if you do not want to wait.
// notes:
//   multiple executeOns to the same locale should be handled concurrently
//
void chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                          chpl_fn_int_t fid,
                          chpl_comm_on_bundle_t *arg, size_t arg_size);

//
// non-blocking execute_on
// arg can be reused immediately after this call completes.
//
void chpl_comm_execute_on_nb(c_nodeid_t node, c_sublocid_t subloc,
                             chpl_fn_int_t fid,
                             chpl_comm_on_bundle_t *arg, size_t arg_size);

//
// fast execute_on (i.e., run in handler)
// arg can be reused immediately after this call completes.
//
void chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                         chpl_fn_int_t fid,
                         chpl_comm_on_bundle_t *arg, size_t arg_size);


//
// This call specifies the number of polling tasks that the
// communication layer will need (see just below for a definition).
// The value it returns is passed to chpl_task_init(), in order to
// forewarn the tasking layer whether the comm layer will need a
// polling task.  In the current implementation, it should only
// return 0 or 1.
//
int chpl_comm_numPollingTasks(void);

// Some communication layers need to be periodically invoked
// in order to make progress. This call gives the comm layer
// an opportunity to move puts,gets, etc along while the
// current thread is idle (e.g. when we are waiting on
// an atomic variable for other tasks to finish).
void chpl_comm_make_progress(void);

//
// Comm diagnostics stuff
//
typedef struct _chpl_commDiagnostics {
  uint64_t get;
  uint64_t get_nb;
  uint64_t put;
  uint64_t put_nb;
  uint64_t test_nb;
  uint64_t wait_nb;
  uint64_t try_nb;
  uint64_t execute_on;
  uint64_t execute_on_fast;
  uint64_t execute_on_nb;
  uint64_t get_calls;
  uint64_t put_calls;
  uint64_t prefetch_calls;
} chpl_commDiagnostics;

void chpl_startVerboseComm(void);
void chpl_stopVerboseComm(void);
void chpl_startVerboseCommHere(void);
void chpl_stopVerboseCommHere(void);

void chpl_startCommDiagnostics(void); // this one implemented by comm layers
void chpl_gen_startCommDiagnostics(void); // this one implemented in chpl-comm.c
void chpl_stopCommDiagnostics(void);
void chpl_gen_stopCommDiagnostics(void);
void chpl_startCommDiagnosticsHere(void);
void chpl_gen_startCommDiagnosticsHere(void);
void chpl_stopCommDiagnosticsHere(void);
void chpl_gen_stopCommDiagnosticsHere(void);
void chpl_resetCommDiagnosticsHere(void);
void chpl_gen_resetCommDiagnosticsHere(void);
void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd);
void chpl_gen_getCommDiagnosticsHere(chpl_commDiagnostics *cd);

void* chpl_get_global_serialize_table(int64_t idx);

#else // LAUNCHER

#define chpl_comm_barrier(x)

#endif // LAUNCHER

// Warn if runtime uses e.g. chpl_comm_get
// (it should use chpl_gen_comm_get)
#include "chpl-comm-warning-macros.h"

#endif

