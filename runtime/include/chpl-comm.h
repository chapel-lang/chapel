/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "chplsys.h"
#include "chpltypes.h"
#include "chpl-comm-impl.h"
#include "chpl-tasks.h"
#include "chpl-comm-task-decls.h"
#include "chpl-comm-locales.h"
#include "chpl-mem-desc.h"

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

size_t chpl_comm_getenvMaxHeapSize(void);


//
// Shared interface (implemented in the compiler generated code)
//
extern void chpl__heapAllocateGlobals(void);

//
// chpl_globals_registry is an array of size chpl_numGlobalsOnHeap
// storing ptr_wide_ptr_t, that is, local addresses of wide pointers.
// It is filled in and used by chpl_comm_register_global_var() and
// chpl_comm_broadcast_global_vars(), respectively, declared below.
//
extern const int chpl_numGlobalsOnHeap;
extern ptr_wide_ptr_t chpl_globals_registry[];

extern void* const chpl_private_broadcast_table[];
extern int const chpl_private_broadcast_table_len;

extern void* const chpl_global_serialize_table[];

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
                                       size_t size, int32_t commID,
                                       int ln, int32_t fn);

// Do a PUT in a nonblocking fashion, returning a handle which can be used to
// wait for the PUT to complete. The source buffer must not be modified before
// the request completes (after waiting on the returned handle)
chpl_comm_nb_handle_t chpl_comm_put_nb(void *addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t commID,
                                       int ln, int32_t fn);

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
// if possible, run in lldb (because the user threw the --lldb flag)
// using argc and argv.  lldbArgnum gives the index of the argv[]
// element containing the --lldb flag.  Return the status of that
// process in "status" and return 1 if it was possible to run in lldb,
// 0 otherwise
//
int chpl_comm_run_in_lldb(int argc, char* argv[], int lldbArgnum, int* status);

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
// This is the comm layer sub-interface for heap management and dynamic
// memory allocation, when memory has to be registered with the network
// for best performance.
//
// chpl_comm_regMemHeapInfo():
//   This provides the address and size of the initial registered heap.
//
// chpl_comm_regMemHeapPageSize():
//   This returns the page size for the comm layer registered heap,
//   either the size of a system page or some hugepage size.
//
// chpl_comm_regMemAllocThreshold():
//   Allocations smaller than this should be done normally, by the
//   memory layer.  Those at least this size may be done through this
//   comm layer sub-interface.  SIZE_MAX is returned if the comm layer
//   cannot or will not do allocations at all.
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
#ifndef CHPL_COMM_IMPL_REG_MEM_HEAP_INFO
#define CHPL_COMM_IMPL_REG_MEM_HEAP_INFO(start_p, size_p)   \
        do { *(start_p) = NULL ; *(size_p) = 0; } while (0)
#endif
static inline
void chpl_comm_regMemHeapInfo(void** start_p, size_t* size_p) {
  CHPL_COMM_IMPL_REG_MEM_HEAP_INFO(start_p, size_p);
}

#ifndef CHPL_COMM_IMPL_REG_MEM_HEAP_PAGE_SIZE
  #define CHPL_COMM_IMPL_REG_MEM_HEAP_PAGE_SIZE() chpl_getSysPageSize()
#endif
static inline
size_t chpl_comm_regMemHeapPageSize(void) {
  return CHPL_COMM_IMPL_REG_MEM_HEAP_PAGE_SIZE();
}

#ifndef CHPL_COMM_IMPL_REG_MEM_ALLOC_THRESHOLD
  #define CHPL_COMM_IMPL_REG_MEM_ALLOC_THRESHOLD() SIZE_MAX
#endif
static inline
size_t chpl_comm_regMemAllocThreshold(void) {
  return CHPL_COMM_IMPL_REG_MEM_ALLOC_THRESHOLD();
}

#ifndef CHPL_COMM_IMPL_REG_MEM_ALLOC
#define CHPL_COMM_IMPL_REG_MEM_ALLOC(size, desc, ln, fn) NULL
#endif
static inline
void* chpl_comm_regMemAlloc(size_t size,
                            chpl_mem_descInt_t desc, int ln, int32_t fn) {
  return CHPL_COMM_IMPL_REG_MEM_ALLOC(size, desc, ln, fn);
}

#ifndef CHPL_COMM_IMPL_REG_MEM_POST_ALLOC
#define CHPL_COMM_IMPL_REG_MEM_POST_ALLOC(p, size) return
#endif
static inline
void chpl_comm_regMemPostAlloc(void* p, size_t size) {
  CHPL_COMM_IMPL_REG_MEM_POST_ALLOC(p, size);
}

#ifndef CHPL_COMM_IMPL_REG_MEM_REALLOC
#define CHPL_COMM_IMPL_REG_MEM_REALLOC(p, oldSize, newSize, desc, ln, fn) NULL
#endif
static inline
void* chpl_comm_regMemRealloc(void* p, size_t oldSize, size_t newSize,
                              chpl_mem_descInt_t desc, int ln, int32_t fn) {
  return CHPL_COMM_IMPL_REG_MEM_REALLOC(p, oldSize, newSize, desc, ln, fn);
}

#ifndef CHPL_COMM_IMPL_REG_MEM_POST_REALLOC
#define CHPL_COMM_IMPL_REG_MEM_POST_REALLOC(oldp, oldSize, newp, newSize) \
        return
#endif
static inline
void chpl_comm_regMemPostRealloc(void* oldp, size_t oldSize,
                                 void* newp, size_t newSize) {
  CHPL_COMM_IMPL_REG_MEM_POST_REALLOC(oldp, oldSize, newp, newSize);
}

#ifndef CHPL_COMM_IMPL_REG_MEM_FREE
#define CHPL_COMM_IMPL_REG_MEM_FREE(p, size) false
#endif
static inline
chpl_bool chpl_comm_regMemFree(void* p, size_t size) {
  return CHPL_COMM_IMPL_REG_MEM_FREE(p, size);
}

//
// These routines are used by the Chapel runtime to broadcast the
// locations of module-level ("global") variables to all locales
// so that all locales can put/get the value of a global variable
// directly, knowing where it lives remotely.
//
// The named symbol for a global var is a wide pointer referring to
// that global's heap-allocated space on node 0.  At program start,
// all of these wide pointers must be communicated from node 0 to
// all the other nodes.  To achieve this, the compiler-emitted code
// first calls chpl_comm_register_global_var() on every node for
// each global (passing a global var index which starts at 0 and
// increments each time, and the address of the global's named
// symbol), then finally calls chpl_comm_broadcast_global_vars().
// The implementation of these two could either broadcast the wide
// pointer values one by one in the 'register' calls and then do
// nothing in the 'broadcast' call, or batch up the wide pointers
// in the 'register' calls and actually do a broadcast in the
// 'broadcast' call.  Currently we do the latter in order to
// reduce startup overhead.
//
void chpl_comm_register_global_var(int i, wide_ptr_t* ptr_to_wide_ptr);
void chpl_comm_broadcast_global_vars(int numGlobals);

//
// This routine is used by the generated Chapel code to broadcast
// the values of module-level ("global") constants to all compute
// nodes so that tasks can refer to the constants locally without
// communication.
//
// Logically, this routine implements a 1-sided broadcast of a value
// across all the compute nodes.  Only one task total will call into
// this routine per logical broadcast.
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
// This routine is used both during program initialization, for initial
// values, and during execution to do things like enabling and disabling
// memory tracking/reporting and comm diagnostics.
//
void chpl_comm_broadcast_private(int id, size_t size);

//
// Barrier for synchronization between all top-level locales; currently
// only used for startup and teardown.  msg is a string that can be used
// for debugging to determine where the barrier is being called.  This
// function may be called from a Chapel task.  As such, if the barrier
// cannot be immediately satisfied, while it waits chpl_comm_barrier()
// must call chpl_task_yield() in order not to monopolize the execution
// resources and prevent making progress. This barrier must be available
// for use in module code, so it cannot be tied up in the runtime 
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
void chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                   size_t size, int32_t commID, int ln, int32_t fn);

//
// get 'size' bytes of remote data at 'raddr' on locale 'locale' to
// local data at 'addr'
// notes:
//   address is arbitrary
//   size and locale are part of p
//
void chpl_comm_get(void *addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t commID, int ln, int32_t fn);

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
void chpl_comm_put_strd(void* dstaddr, size_t* dststrides, c_nodeid_t dstnode,
                        void* srcaddr, size_t* srcstrides, size_t* count,
                        int32_t stridelevels, size_t elemSize, int32_t commID,
                        int ln, int32_t fn);

//
// same as chpl_comm_puts(), but do get instead
//
void chpl_comm_get_strd(void* dstaddr, size_t* dststrides, c_nodeid_t srcnode,
                        void* srcaddr, size_t* srcstrides, size_t* count,
                        int32_t stridelevels, size_t elemSize, int32_t commID,
                        int ln, int32_t fn);


//
// Unordered ops
//
void chpl_comm_get_unordered(void *addr, c_nodeid_t node, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn);

void chpl_comm_put_unordered(void* addr, c_nodeid_t node, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn);

void chpl_comm_getput_unordered(c_nodeid_t dstnode, void* dstaddr,
                                c_nodeid_t srcnode, void* srcaddr,
                                size_t size, int32_t commID,
                                int ln, int32_t fn);

void chpl_comm_getput_unordered_task_fence(void);

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
                          chpl_comm_on_bundle_t *arg, size_t arg_size,
                          int ln, int32_t fn);

//
// non-blocking execute_on
// arg can be reused immediately after this call completes.
//
void chpl_comm_execute_on_nb(c_nodeid_t node, c_sublocid_t subloc,
                             chpl_fn_int_t fid,
                             chpl_comm_on_bundle_t *arg, size_t arg_size,
                             int ln, int32_t fn);

//
// fast execute_on (i.e., run in handler)
// arg can be reused immediately after this call completes.
//
void chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                               chpl_fn_int_t fid,
                               chpl_comm_on_bundle_t *arg, size_t arg_size,
                               int ln, int32_t fn);

// This is a hook that's called when a task is ending. It allows for things
// like say flushing task private buffers.
void chpl_comm_task_end(void);

void* chpl_get_global_serialize_table(int64_t idx);

// Used to park and wake up the main process
void chpl_signal_shutdown(void);
void chpl_wait_for_shutdown(void);


#else // LAUNCHER

#define chpl_comm_barrier(x)

#endif // LAUNCHER

// Warn if runtime uses e.g. chpl_comm_get
// (it should use chpl_gen_comm_get)
#include "chpl-comm-warning-macros.h"

#endif

