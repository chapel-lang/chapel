/*
 * Copyright 2004-2015 Cray Inc.
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

#include "chplrt.h"

#include "gasnet.h"
#include "gasnet_vis.h"
#include "gasnet_coll.h"
#include "gasnet_tools.h"
#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "chplcgfns.h"
#include "chpl-gen-includes.h"
#include "chpl-atomics.h"
#include "error.h"
#include "chpl-cache.h" // to call chpl_cache_init()

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"

#include <signal.h>
#include <sched.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

static chpl_sync_aux_t chpl_comm_diagnostics_sync;
static chpl_commDiagnostics chpl_comm_commDiagnostics;
static int chpl_comm_no_debug_private = 0;
static gasnet_seginfo_t* seginfo_table = NULL;


//
// Build acknowledgement address arguments for gasnetAMRequest*() calls.
//
#define AckArg0(addr) ((gasnet_handlerarg_t) \
                       ((((uint64_t) (intptr_t) (addr)) << 32UL) >> 32UL))
#define AckArg1(addr) ((gasnet_handlerarg_t) \
                       (((uint64_t) (intptr_t) (addr)) >> 32UL))

//
// The following macro is from the GASNet test.h distribution
//
#define GASNET_Safe(fncall) do {                                        \
    int _retval;                                                        \
    if ((_retval = fncall) != GASNET_OK) {                              \
      fprintf(stderr, "ERROR calling: %s\n"                             \
              " at: %s:%i\n"                                            \
              " error: %s (%s)\n",                                      \
              #fncall, __FILE__, __LINE__,                              \
              gasnet_ErrorName(_retval), gasnet_ErrorDesc(_retval));    \
      fflush(stderr);                                                   \
      gasnet_exit(_retval);                                             \
    }                                                                   \
  } while(0)

//
// Adapted from the above, this one is for the case where the caller
// needs to do the fncall separately (see chpl_comm_barrier()).  Here
// the fn is not actually called; fncall is only used to produce the
// message.
//
#define GASNET_Safe_Retval(fncall, _retval) do {                        \
    if (_retval != GASNET_OK) {                                         \
      fprintf(stderr, "ERROR calling: %s\n"                             \
              " at: %s:%i\n"                                            \
              " error: %s (%s)\n",                                      \
              #fncall, __FILE__, __LINE__,                              \
              gasnet_ErrorName(_retval), gasnet_ErrorDesc(_retval));    \
      fflush(stderr);                                                   \
      gasnet_exit(_retval);                                             \
    }                                                                   \
  } while(0)

//
// This is the type of object we use to manage GASNet acknowledgements.
//
// Initialize the count to 0, the target to the number of return signal
// events you expect, and the flag to 0.  Fire the request, then do a
// BLOCKUNTIL(flag).  When all the return signals have occurred, the AM
// handler will set the flag to 1 and your BLOCKUNTIL will complete.
// (Note that the GASNet documentation says that GASNet code assumes
// the condition for a BLOCKUNTIL can only be changed by the execution
// of an AM handler.)
//
typedef struct {
  atomic_uint_least32_t count;
  uint_least32_t        target;
  volatile int          flag;
} done_t;

//
// Initialize one of the above.
//
#define INIT_DONE_OBJ(done, _target) do {                               \
    atomic_init_uint_least32_t(&done.count, 0);                         \
    done.target = _target;                                              \
    done.flag = 0;                                                      \
  } while (0)

typedef struct {
  int           caller;
  c_sublocid_t  subloc;
  void*         ack;
  chpl_bool     serial_state; // true if not allowed to spawn new threads
  chpl_fn_int_t fid;
  int           arg_size;
  char          arg[0];       // variable-sized data here
} fork_t;

typedef struct {
  void*   ack;
  int     id;       // private broadcast table entry to update
  int     size;     // size of data
  char    data[0];  // data
} priv_bcast_t;

typedef struct {
  void* ack;
  int   id;       // private broadcast table entry to update
  int   size;     // size of data
  int   offset;   // offset of piece of data
  char  data[0];  // data
} priv_bcast_large_t;

//
// AM functions
//
#define FORK          128 // synchronous fork
#define FORK_LARGE    129 // synchronous fork with a huge argument
#define FORK_NB       130 // non-blocking fork 
#define FORK_NB_LARGE 131 // non-blocking fork with a huge argument
#define FORK_FAST     132 // run the function in the handler (use with care)
#define SIGNAL        133 // ack of synchronous fork
#define PRIV_BCAST    134 // put data at addr (used for private broadcast)
#define PRIV_BCAST_LARGE 135 // put data at addr (used for private broadcast)
#define FREE          136 // free data at addr
#define EXIT_ANY      137 // free data at addr
#define BCAST_SEGINFO 138 // broadcast for segment info table

static void AM_fork_fast(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t *f = buf;

  if (f->arg_size)
    chpl_ftable_call(f->fid, &f->arg);
  else
    chpl_ftable_call(f->fid, NULL);

  // Signal that the handler has completed
  GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL,
                                   AckArg0(f->ack), AckArg1(f->ack)));
}

static void fork_wrapper(fork_t *f) {
  if (f->arg_size)
    chpl_ftable_call(f->fid, &f->arg);
  else
    chpl_ftable_call(f->fid, NULL);
  GASNET_Safe(gasnet_AMRequestShort2(f->caller, SIGNAL,
                                     AckArg0(f->ack), AckArg1(f->ack)));

  chpl_mem_free(f, 0, 0);
}

static void AM_fork(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t *f = (fork_t*)chpl_mem_allocMany(nbytes, sizeof(char),
                                          CHPL_RT_MD_COMM_FORK_RECV_INFO, 0, 0);
  chpl_memcpy(f, buf, nbytes);
  chpl_task_startMovedTask((chpl_fn_p)fork_wrapper, (void*)f,
                           f->subloc, chpl_nullTaskID,
                           f->serial_state);
}

static void fork_large_wrapper(fork_t* f) {
  void* arg = chpl_mem_allocMany(1, f->arg_size,
                                 CHPL_RT_MD_COMM_FORK_RECV_LARGE_ARG, 0, 0);

  // A note on strict aliasing:
  // We used to say something like *(void**)f->arg,
  // but that leads to compiler errors about type-punning
  // since it breaks strict aliasing rules. The memcpy approach
  // employed here is one way around the problem, and a
  // more appealing solution would be to use a union.
  void* f_arg;
  chpl_memcpy(&f_arg, f->arg, sizeof(void*));

  chpl_comm_get(arg, f->caller, f_arg,
                f->arg_size, -1 /*typeIndex: unused*/, 1, 0, "fork large");
  chpl_ftable_call(f->fid, arg);
  GASNET_Safe(gasnet_AMRequestShort2(f->caller, SIGNAL,
                                     AckArg0(f->ack), AckArg1(f->ack)));

  chpl_mem_free(f, 0, 0);
  chpl_mem_free(arg, 0, 0);
}

////GASNET - can we send as much of user data as possible initially
////           hide data copy by making get non-blocking
////GASNET - can we allocate f big enough so as not to need malloc in wrapper
static void AM_fork_large(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t* f = (fork_t*)chpl_mem_allocMany(1, nbytes,
                                          CHPL_RT_MD_COMM_FORK_RECV_LARGE_INFO,
                                          0, 0);
  chpl_memcpy(f, buf, nbytes);
  chpl_task_startMovedTask((chpl_fn_p)fork_large_wrapper, (void*)f,
                           f->subloc, chpl_nullTaskID,
                           f->serial_state);
}

static void fork_nb_wrapper(fork_t *f) {
  if (f->arg_size)
    chpl_ftable_call(f->fid, &f->arg);
  else
    chpl_ftable_call(f->fid, NULL);
  chpl_mem_free(f, 0, 0);
}

static void AM_fork_nb(gasnet_token_t  token,
                        void           *buf,
                        size_t          nbytes) {
  fork_t *f = (fork_t*)chpl_mem_allocMany(nbytes, sizeof(char),
                                          CHPL_RT_MD_COMM_FORK_RECV_NB_INFO,
                                          0, 0);
  chpl_memcpy(f, buf, nbytes);
  chpl_task_startMovedTask((chpl_fn_p)fork_nb_wrapper, (void*)f,
                           f->subloc, chpl_nullTaskID,
                           f->serial_state);
}

static void fork_nb_large_wrapper(fork_t* f) {
  void* arg = chpl_mem_allocMany(1, f->arg_size,
                                 CHPL_RT_MD_COMM_FORK_RECV_NB_LARGE_ARG, 0, 0);

  // See "A note on strict aliasing" in fork_large_wrapper
  void* f_arg;
  chpl_memcpy(&f_arg, f->arg, sizeof(void*));

  chpl_comm_get(arg, f->caller, f_arg,
                f->arg_size, -1 /*typeIndex: unused*/, 1, 0, "fork large");
  GASNET_Safe(gasnet_AMRequestMedium0(f->caller,
                                      FREE,
                                      &(f->ack),
                                      sizeof(f->ack)));
  chpl_ftable_call(f->fid, arg);
  chpl_mem_free(f, 0, 0);
  chpl_mem_free(arg, 0, 0);
}

static void AM_fork_nb_large(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t* f = (fork_t*)chpl_mem_allocMany(1, nbytes,
                                          CHPL_RT_MD_COMM_FORK_RECV_NB_LARGE_INFO,
                                          0, 0);
  chpl_memcpy(f, buf, nbytes);
  chpl_task_startMovedTask((chpl_fn_p)fork_nb_large_wrapper, (void*)f,
                           f->subloc, chpl_nullTaskID,
                           f->serial_state);
}

static void AM_signal(gasnet_token_t token, gasnet_handlerarg_t a0, gasnet_handlerarg_t a1) {
  done_t* done = (done_t*) (intptr_t)
                 (((uint64_t) (uint32_t) a0)
                  | (((uint64_t) (uint32_t) a1) << 32UL));
  uint_least32_t prev;
  prev = atomic_fetch_add_explicit_uint_least32_t(&done->count, 1,
                                                  memory_order_seq_cst);
  if (prev + 1 == done->target)
    done->flag = 1;
}

static void AM_priv_bcast(gasnet_token_t token, void* buf, size_t nbytes) {
  priv_bcast_t* pbp = buf;
  chpl_memcpy(chpl_private_broadcast_table[pbp->id], pbp->data, pbp->size);

  // Signal that the handler has completed
  GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL,
                                   AckArg0(pbp->ack), AckArg1(pbp->ack)));
}

static void AM_priv_bcast_large(gasnet_token_t token, void* buf, size_t nbytes) {
  priv_bcast_large_t* pblp = buf;
  chpl_memcpy((char*)chpl_private_broadcast_table[pblp->id]+pblp->offset, pblp->data, pblp->size);

  // Signal that the handler has completed
  GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL,
                                   AckArg0(pblp->ack), AckArg1(pblp->ack)));
}

static void AM_free(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t* f;
  void* f_arg;
  
  // See "A note on strict aliasing" in fork_large_wrapper
  chpl_memcpy(&f, buf, sizeof(fork_t*));
  chpl_memcpy(&f_arg, f->arg, sizeof(void*));

  chpl_mem_free(f_arg, 0, 0);
  chpl_mem_free(f, 0, 0);
}

// this is currently unused; it's intended to be used to implement
// exit_any with cleanup on all nodes. 
static void AM_exit_any(gasnet_token_t token, void* buf, size_t nbytes) {
//  int **status = (int**)buf; // Some compilers complain about unused variable 'status'.
  chpl_internal_error("clean exit_any is not implemented.");
  // here we basically need to call chpl_exit_all, but we need to
  // ensure only one thread calls chpl_exit_all on this locale.
}

//
// This global and routine are used to broadcast the seginfo_table at the outset
// of the program's execution.  It is designed to only be used once.  This code
// was modeled after the _test_segbcast() routine in
// third-party/gasnet/GASNet-*/tests/test.h
//
static int bcast_seginfo_done = 0;
static void AM_bcast_seginfo(gasnet_token_t token, void *buf, size_t nbytes) {
  assert(nbytes == sizeof(gasnet_seginfo_t)*gasnet_nodes());
  chpl_memcpy(seginfo_table, buf, nbytes);
  gasnett_local_wmb();
  bcast_seginfo_done = 1;
}

static gasnet_handlerentry_t ftable[] = {
  {FORK,          AM_fork},
  {FORK_LARGE,    AM_fork_large},
  {FORK_NB,       AM_fork_nb},
  {FORK_NB_LARGE, AM_fork_nb_large},
  {FORK_FAST,     AM_fork_fast},
  {SIGNAL,        AM_signal},
  {PRIV_BCAST,    AM_priv_bcast},
  {PRIV_BCAST_LARGE, AM_priv_bcast_large},
  {FREE,          AM_free},
  {EXIT_ANY,      AM_exit_any},
  {BCAST_SEGINFO, AM_bcast_seginfo}
};

//
// Chapel interface starts here
//
chpl_comm_nb_handle_t chpl_comm_put_nb(void *addr, c_nodeid_t node, void* raddr,
                                       int32_t elemSize, int32_t typeIndex,
                                       int32_t len,
                                       int ln, c_string fn)
{
  size_t nbytes = elemSize*len;
  gasnet_handle_t ret;

  ret = gasnet_put_nb_bulk(node, raddr, addr, nbytes);

  if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
    chpl_sync_lock(&chpl_comm_diagnostics_sync);
    chpl_comm_commDiagnostics.put_nb++;
    chpl_sync_unlock(&chpl_comm_diagnostics_sync);
  }

  if (chpl_vdebug) {
    struct timeval tv;
    struct timezone tz = {0,0};
    (void)gettimeofday(&tv, &tz);
    dprintf (chpl_vdebug_fd, "put_nb: %lld.%06d, %d, %d, 0x%lx, 0x%lx, %d, %d, %d, %d, %s\n",
	     (long long) tv.tv_sec, tv.tv_usec,  chpl_nodeID, node, (long) addr,
             (long) raddr, elemSize, typeIndex, len, ln, fn);
  }

  return (chpl_comm_nb_handle_t) ret;
}

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node, void* raddr,
                                       int32_t elemSize, int32_t typeIndex,
                                       int32_t len,
                                       int ln, c_string fn)
{
  size_t nbytes = elemSize*len;
  gasnet_handle_t ret;

  ret = gasnet_get_nb_bulk(addr, node, raddr, nbytes);

  if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
    chpl_sync_lock(&chpl_comm_diagnostics_sync);
    chpl_comm_commDiagnostics.get_nb++;
    chpl_sync_unlock(&chpl_comm_diagnostics_sync);
  }

  if (chpl_vdebug) {
    struct timeval tv;
    struct timezone tz = {0,0};
    (void)gettimeofday(&tv, &tz);
    dprintf (chpl_vdebug_fd, "get_nb: %lld.%06d, %d, %d, 0x%lx, 0x%lx, %d, %d, %d, %d, %s\n",
	     (long long) tv.tv_sec, tv.tv_usec,  chpl_nodeID, node, (long) addr,
             (long) raddr, elemSize, typeIndex, len, ln, fn);
  }

  return (chpl_comm_nb_handle_t) ret;
}

int chpl_comm_test_nb_complete(chpl_comm_nb_handle_t h)
{
  return ((void*)h) == NULL;
}

void chpl_comm_wait_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  assert(NULL == GASNET_INVALID_HANDLE);  // serious confusion if not so
  gasnet_wait_syncnb_some((gasnet_handle_t*) h, nhandles);
}

int chpl_comm_try_nb_some(chpl_comm_nb_handle_t* h, size_t nhandles)
{
  assert(NULL == GASNET_INVALID_HANDLE);  // serious confusion if not so
  return gasnet_try_syncnb_some((gasnet_handle_t*) h, nhandles) == GASNET_OK;
}

int chpl_comm_is_in_segment(c_nodeid_t node, void* start, size_t len)
{
#ifdef GASNET_SEGMENT_EVERYTHING
  return 0;
#else
  uintptr_t segstart, segend;
  uintptr_t reqstart, reqend;

  segstart = (uintptr_t) seginfo_table[node].addr;
  segend = segstart + seginfo_table[node].size;
  reqstart = (uintptr_t) start;
  reqend = reqstart + len;

  if( segstart <= reqstart &&
      reqstart <= segend &&
      segstart <= reqend &&
      reqend <= segend) {
    return 1;
  }

  return 0;
#endif
}


int32_t chpl_comm_getMaxThreads(void) {
  return GASNETI_MAX_THREADS-1;
}

//
// On all locales, we'll do the primary polling in a thread of control
// managed by the tasking layer, so that it can coordinate the use of
// hardware resources for polling and user tasks.  This symmetry will
// also allow the tasking layer to minimize its locale-based behavioral
// differences and simplify our analysis of performance effects due to
// polling.  We'll refer to this thread of control as the "polling task"
// even though the tasking layer can implement it however it likes, as a
// task or thread or whatever.
//
static volatile int pollingRunning;
static volatile int pollingQuit;

static void polling(void* x) {
  pollingRunning = 1;
  while (!pollingQuit) {
    (void) gasnet_AMPoll();
    chpl_task_yield();
  }
  pollingRunning = 0;
}

static void set_max_segsize_env_var(size_t size) {
  char segsizeval[22]; // big enough for an unsigned 64-bit quantity

  snprintf(segsizeval, sizeof(segsizeval), "%zd", size);
  if (setenv("GASNET_MAX_SEGSIZE", segsizeval, 1) != 0) {
    chpl_error("Cannot setenv(\"GASNET_MAX_SEGSIZE\")", 0, NULL);
  }
}

static void set_max_segsize() {
  FILE* file = NULL;
  size_t size;

  if ((size = chpl_comm_getenvMaxHeapSize()) != 0) {
    set_max_segsize_env_var(size);
    return;
  }

  // If GASNET_NEEDS_MAX_SEGSIZE is defined then we have to have
  // GASNET_MAX_SEGSIZE set.  Otherwise, we don't.
#ifdef GASNET_NEEDS_MAX_SEGSIZE
  if (getenv("GASNET_MAX_SEGSIZE")) {
    return;
  }

  // Use 90% of the available memory as the maximum segment size,
  // heuristically
  if ((size = chpl_bytesAvailOnThisLocale()) != 0) {
    set_max_segsize_env_var((size_t) (0.9 * size));
    return;
  }

  chpl_internal_error("Could not determine maximum segment size");
#endif
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
//  int status; // Some compilers complain about unused variable 'status'.

  set_max_segsize();

  assert(sizeof(gasnet_handlerarg_t)==sizeof(uint32_t));

  gasnet_init(argc_p, argv_p);
  chpl_nodeID = gasnet_mynode();
  chpl_numNodes = gasnet_nodes();
  GASNET_Safe(gasnet_attach(ftable, 
                            sizeof(ftable)/sizeof(gasnet_handlerentry_t),
                            gasnet_getMaxLocalSegmentSize(),
                            0));
#undef malloc
  seginfo_table = (gasnet_seginfo_t*)malloc(chpl_numNodes*sizeof(gasnet_seginfo_t));
  //
  // The following call has no real effect on the .addr and .size
  // fields for GASNET_SEGMENT_EVERYTHING, but is recommended to be
  // used anyway (see third-party/gasnet/GASNet-version/tests/test.h)
  // in order to ensure that the seginfo_table array is initialized
  // appropriately on all locales.
  //
  GASNET_Safe(gasnet_getSegmentInfo(seginfo_table, chpl_numNodes));
#ifdef GASNET_SEGMENT_EVERYTHING
  //
  // For SEGMENT_EVERYTHING, there is no GASNet-provided memory
  // segment, so instead we're going to create our own fake segment
  // in order to share the code that refers to it and avoid any
  // assumptions that global variables in the generated C code will
  // be stored at the same address in all instances of the executable
  // (something that is typically true, but turns out not to be on,
  // for example, OS X Lion).  This technique was modeled after the
  // _test_attach() routine from third-party/gasnet/GASNET-version/tests/test.h
  // but is significantly simplified for our purposes.
  //
  if (chpl_nodeID == 0) {
    int i;
    //
    // Only locale #0 really needs the seginfo_table to store anything since it owns all
    // of the global variable locations; everyone else will just peek at its copy.  So
    // locale 0 sets up its segment to an appropriate size:
    //
    int global_table_size = chpl_numGlobalsOnHeap * sizeof(wide_ptr_t) + GASNETT_PAGESIZE;
    void* global_table = malloc(global_table_size);
    seginfo_table[0].addr = ((void *)(((uint8_t*)global_table) + 
                                      (((((uintptr_t)global_table)%GASNETT_PAGESIZE) == 0)? 0 : 
                                       (GASNETT_PAGESIZE-(((uintptr_t)global_table)%GASNETT_PAGESIZE)))));
    seginfo_table[0].size = global_table_size;
    //
    // ...and then zeroes out everyone else's
    //
    for (i=1; i<chpl_numNodes; i++) {
      seginfo_table[i].addr = NULL;
      seginfo_table[i].size = 0;
    }
  }
  //
  // Then we're going to broadcast the seginfo_table to everyone so that each locale
  // has its own copy of it and knows where everyone else's segment lives (or, really,
  // where locale #0's lives since we're not using anyone else's at this point).
  //
  chpl_comm_barrier("getting ready to broadcast addresses");
  //
  // This is a naive O(numLocales) broadcast; we could do something
  // more scalable with more effort
  //
  if (chpl_nodeID == 0) {
    int i;
    for (i=0; i < chpl_numNodes; i++) {
      GASNET_Safe(gasnet_AMRequestMedium0(i, BCAST_SEGINFO, seginfo_table, 
                                          chpl_numNodes*sizeof(gasnet_seginfo_t)));
    }
  }
  GASNET_BLOCKUNTIL(bcast_seginfo_done);
  chpl_comm_barrier("making sure everyone's done with the broadcast");
#endif
#define malloc dont_use_malloc_use_chpl_mem_allocMany_instead

  gasnet_set_waitmode(GASNET_WAIT_BLOCK);

}

void chpl_comm_post_mem_init(void) { }

int chpl_comm_numPollingTasks(void) {
  return 1;
}

//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

void chpl_comm_post_task_init(void) {
  //
  // Start a polling task on each locale.
  //
  pollingRunning = 0;
  pollingQuit = 0;
  if (chpl_task_createCommTask(polling, NULL))
    chpl_internal_error("unable to start polling task for gasnet");
  while (!pollingRunning) {
    sched_yield();
  }

  // clear diags
  memset(&chpl_comm_commDiagnostics, 0, sizeof(chpl_commDiagnostics));

  // Initialize the caching layer, if it is active.
  chpl_cache_init();
}

void chpl_comm_rollcall(void) {
  chpl_sync_initAux(&chpl_comm_diagnostics_sync);
  chpl_msg(2, "executing on node %d of %d node(s): %s\n", chpl_nodeID, 
           chpl_numNodes, chpl_nodeName());
}

void chpl_comm_desired_shared_heap(void** start_p, size_t* size_p) {
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
  *start_p = chpl_numGlobalsOnHeap * sizeof(wide_ptr_t) 
             + (char*)seginfo_table[chpl_nodeID].addr;
  *size_p  = seginfo_table[chpl_nodeID].size
             - chpl_numGlobalsOnHeap * sizeof(wide_ptr_t);
#else /* GASNET_SEGMENT_EVERYTHING */
  *start_p = NULL;
  *size_p  = 0;
#endif
}

void chpl_comm_broadcast_global_vars(int numGlobals) {
  int i;
  if (chpl_nodeID != 0) {
    for (i = 0; i < numGlobals; i++) {
      chpl_comm_get(chpl_globals_registry[i], 0,
                    &((wide_ptr_t*)seginfo_table[0].addr)[i],
                    sizeof(wide_ptr_t), -1 /*typeIndex: unused*/, 1, 0, "");
    }
  }
}

void chpl_comm_broadcast_private(int id, int32_t size, int32_t tid) {
  int  node, offset;
  int  payloadSize = size + sizeof(priv_bcast_t);
  done_t* done;
  int numOffsets=1;

  // This can use the system allocator because it involves internode communication.
  done = (done_t*) chpl_mem_allocManyZero(chpl_numNodes, sizeof(*done),
                                          CHPL_RT_MD_COMM_FORK_DONE_FLAG,
                                          0, 0);
  if (payloadSize <= gasnet_AMMaxMedium()) {
    priv_bcast_t* pbp = chpl_mem_allocMany(1, payloadSize, CHPL_RT_MD_COMM_PRIVATE_BROADCAST_DATA, 0, 0);
    chpl_memcpy(pbp->data, chpl_private_broadcast_table[id], size);
    pbp->id = id;
    pbp->size = size;
    for (node = 0; node < chpl_numNodes; node++) {
      if (node != chpl_nodeID) {
        pbp->ack = &done[node];
        INIT_DONE_OBJ(done[node], 1);
        GASNET_Safe(gasnet_AMRequestMedium0(node, PRIV_BCAST, pbp, payloadSize));
      }
    }
    chpl_mem_free(pbp, 0, 0);
  } else {
    int maxpayloadsize = gasnet_AMMaxMedium();
    int maxsize = maxpayloadsize - sizeof(priv_bcast_large_t);
    priv_bcast_large_t* pblp = chpl_mem_allocMany(1, maxpayloadsize, CHPL_RT_MD_COMM_PRIVATE_BROADCAST_DATA, 0, 0);
    pblp->id = id;
    numOffsets = (size+maxsize)/maxsize;
    for (node = 0; node < chpl_numNodes; node++) {
      if (node != chpl_nodeID)
        INIT_DONE_OBJ(done[node], numOffsets);
    }
    for (offset = 0; offset < size; offset += maxsize) {
      int thissize = size - offset;
      if (thissize > maxsize)
        thissize = maxsize;
      pblp->offset = offset;
      pblp->size = thissize;
      chpl_memcpy(pblp->data, (char*)chpl_private_broadcast_table[id]+offset, thissize);
      for (node = 0; node < chpl_numNodes; node++) {
        if (node != chpl_nodeID) {
          pblp->ack = &done[node];
          GASNET_Safe(gasnet_AMRequestMedium0(node, PRIV_BCAST_LARGE, pblp, sizeof(priv_bcast_large_t)+thissize));
        }
      }
    }
    chpl_mem_free(pblp, 0, 0);
  }
  // wait for the handlers to complete
  for (node = 0; node < chpl_numNodes; node++) {
    if (node != chpl_nodeID)
      GASNET_BLOCKUNTIL(done[node].flag);
  }
  chpl_mem_free(done, 0, 0);
}

void chpl_comm_barrier(const char *msg) {
  int id = (int) msg[0];
  int retval;

#ifdef CHPL_COMM_DEBUG
  chpl_msg(2, "%d: enter barrier for '%s'\n", chpl_nodeID, msg);
#endif

  //
  // We don't want to just do a gasnet_barrier_wait() here, because
  // GASNet will put us to work polling, and we already have a polling
  // task that the tasking layer has presumably placed to best effect.
  // We don't want to compete with that.  Also, the implementation is
  // required to do chpl_task_yield() while waiting for the barrier to
  // satisfy; see chpl_comm.h.  This prevents us from monopolizing the
  // processor while waiting.
  //
  gasnet_barrier_notify(id, 0);
  while ((retval = gasnet_barrier_try(id, 0)) == GASNET_ERR_NOT_READY) {
    chpl_task_yield();
  }
  GASNET_Safe_Retval(gasnet_barrier_try(id, 0), retval);
}

void chpl_comm_pre_task_exit(int all) {
  if (all) {
    chpl_comm_barrier("stop polling");

    //
    // Tell the polling task to halt, then wait for it to do so.
    //
    pollingQuit = 1;
    while (pollingRunning) {
      sched_yield();
    }
  }
}

static void exit_common(int status) {
  static int loopback = 0;

  pollingQuit = 1;

  if (chpl_nodeID == 0) {
    if (loopback) {
      gasnet_exit(2);
    }
  }

  chpl_comm_barrier("exit_common_gasnet_exit"); 
  //exit(); // depending on PAT exit strategy, maybe switch to this
  gasnet_exit(status); // not a collective operation, but one locale will win and all locales will die.
}

static void exit_any_dirty(int status) {
  // kill the polling task, but other than that...
  // clean up nothing; just ask GASNet to exit
  // GASNet will then kill all other locales.
  static int loopback = 0;

  pollingQuit = 1;

  if (chpl_nodeID == 0) {
    if (loopback) {
      gasnet_exit(2);
    }
  }

  gasnet_exit(status);
}

#ifdef GASNET_NEEDS_EXIT_ANY_CLEAN
// this is currently unused; it's intended to be used to implement
// exit_any with cleanup on all nodes
static void exit_any_clean(int status) {
  int* status_p = &status;
  int node;

  // notify all other nodes that this node is entering a clean exit_any
  for (node = 0; node < chpl_numNodes; node++) {
    if (node != chpl_nodeID) {
      GASNET_Safe(gasnet_AMRequestMedium0(node, EXIT_ANY, &status_p, sizeof(status_p)));
    }
  }
    
  // (for code reuse) ask this node to perform a clean exit_any
  GASNET_Safe(gasnet_AMRequestMedium0(chpl_nodeID, EXIT_ANY, &status_p, sizeof(status_p)));
}
#endif

void chpl_comm_exit(int all, int status) {
  if (all) {
    exit_common(status);
  }
  else {
    // when exit_any_clean is finished, consider switching to that.
    exit_any_dirty(status); 
  }
}

void  chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, c_string fn) {
  const int size = elemSize*len;

  if (chpl_vdebug) {
    struct timeval tv;
    struct timezone tz = {0,0};
    (void)gettimeofday(&tv, &tz);
    dprintf (chpl_vdebug_fd, "put: %lld.%06d, %d, %d, 0x%lx, 0x%lx, %d, %d, %d, %d, %s\n",
	     (long long) tv.tv_sec, tv.tv_usec,  chpl_nodeID, node, (long) addr,
             (long) raddr, elemSize, typeIndex, len, ln, fn);
  }

  if (chpl_nodeID == node) {
    memmove(raddr, addr, size);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: %s:%d: remote put to %d\n", chpl_nodeID, fn, ln, node);
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      chpl_sync_lock(&chpl_comm_diagnostics_sync);
      chpl_comm_commDiagnostics.put++;
      chpl_sync_unlock(&chpl_comm_diagnostics_sync);
    }
    gasnet_put(node, raddr, addr, size); // node, dest, src, size
  }
}

////GASNET - pass trace info to gasnet_get
////GASNET - define GASNET_E_ PUTGET always REMOTE
////GASNET - look at GASNET tools at top of README.tools has atomic counters
void  chpl_comm_get(void* addr, c_nodeid_t node, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, c_string fn) {
  const int size = elemSize*len;

  if (chpl_vdebug) {
    struct timeval tv;
    struct timezone tz = {0,0};
    (void)gettimeofday(&tv, &tz);
    dprintf (chpl_vdebug_fd, "get: %lld.%06d, %d, %d, 0x%lx, 0x%lx, %d, %d, %d, %d, %s\n",
	     (long long) tv.tv_sec, tv.tv_usec,  chpl_nodeID, node, (long) addr,
             (long) raddr, elemSize, typeIndex, len, ln, fn);
  }

  if (chpl_nodeID == node) {
    memmove(addr, raddr, size);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: %s:%d: remote get from %d\n", chpl_nodeID, fn, ln, node);
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      chpl_sync_lock(&chpl_comm_diagnostics_sync);
      chpl_comm_commDiagnostics.get++;
      chpl_sync_unlock(&chpl_comm_diagnostics_sync);
    }
    gasnet_get(addr, node, raddr, size); // dest, node, src, size
  }
}

//
// This is an adaptor from Chapel code to GASNet's gasnet_gets_bulk. It does:
// * convert count[0] and all of 'srcstr' and 'dststr' from counts of element
//   to counts of bytes,
// * convert the element types of the above C arrays from int32_t to size_t.
// Maybe this can be done in Chapel, but would it be as efficient?
//
void  chpl_comm_get_strd(void* dstaddr, void* dststrides, c_nodeid_t srcnode_id, 
                         void* srcaddr, void* srcstrides, void* count,
                         int32_t stridelevels, int32_t elemSize, int32_t typeIndex, 
                         int ln, c_string fn) {
  int i;
  const size_t strlvls = (size_t)stridelevels;
  const gasnet_node_t srcnode = (gasnet_node_t)srcnode_id;

  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  // Only count[0] and strides are measured in number of bytes.
  cnt[0] = ((int32_t*)count)[0] * elemSize;

  if (strlvls>0) {
    srcstr[0] = ((int32_t*)srcstrides)[0] * elemSize;
    dststr[0] = ((int32_t*)dststrides)[0] * elemSize;
    for (i=1; i<strlvls; i++) { 
      srcstr[i] = ((int32_t*)srcstrides)[i] * elemSize;
      dststr[i] = ((int32_t*)dststrides)[i] * elemSize;
      cnt[i] = ((int32_t*)count)[i];
    }
    cnt[strlvls] = ((int32_t*)count)[strlvls];
  }

  if (chpl_verbose_comm && !chpl_comm_no_debug_private) {
    printf("%d: %s:%d: remote get from %d. strlvls:%d. elemSize:%d  sizeof(size_t):%d  sizeof(gasnet_node_t):%d\n", chpl_nodeID, fn, ln, srcnode,(int)strlvls,elemSize,(int)sizeof(size_t),(int)sizeof(gasnet_node_t));

    printf("dststrides in bytes:\n");                 
    for (i=0;i<strlvls;i++) printf(" %d ",(int)dststr[i]);
    printf("\n");                     
    printf("srcstrides in bytes:\n");                 
    for (i=0;i<strlvls;i++) printf(" %d ",(int)srcstr[i]);
    printf("\n");                     
    printf("count (count[0] in bytes):\n");                   
    for (i=0;i<=strlvls;i++) printf(" %d ",(int)cnt[i]);
    printf("\n");                     
  }

  if (chpl_vdebug) {
    struct timeval tv;
    struct timezone tz = {0,0};
    (void)gettimeofday(&tv, &tz);
    dprintf (chpl_vdebug_fd, "get_strd: %lld.%06d, %d, %d, 0x%lx, 0x%lx, %d, %d, %d, %s\n",
	     (long long) tv.tv_sec, tv.tv_usec,  chpl_nodeID, srcnode, (long) dstaddr,
             (long) srcaddr, elemSize, typeIndex, ln, fn);
    // print out the srcstr and dststr?
	     
  }

  // the case (chpl_nodeID == srcnode) is internally managed inside gasnet
  if (chpl_verbose_comm && !chpl_comm_no_debug_private)
    printf("%d: %s:%d: remote get from %d\n", chpl_nodeID, fn, ln, srcnode);
  if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
    chpl_sync_lock(&chpl_comm_diagnostics_sync);
    chpl_comm_commDiagnostics.get++;
    chpl_sync_unlock(&chpl_comm_diagnostics_sync);
  }
  gasnet_gets_bulk(dstaddr, dststr, srcnode, srcaddr, srcstr, cnt, strlvls); 
}

// See the comment for cmpl_comm_gets().
void  chpl_comm_put_strd(void* dstaddr, void* dststrides, c_nodeid_t dstnode_id, 
                         void* srcaddr, void* srcstrides, void* count,
                         int32_t stridelevels, int32_t elemSize, int32_t typeIndex, 
                         int ln, c_string fn) {
  int i;
  const size_t strlvls = (size_t)stridelevels;
  const gasnet_node_t dstnode = (gasnet_node_t)dstnode_id;

  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  // Only count[0] and strides are measured in number of bytes.
  cnt[0] = ((int32_t*)count)[0] * elemSize;
  if (strlvls>0) {
    srcstr[0] = ((int32_t*)srcstrides)[0] * elemSize;
    dststr[0] = ((int32_t*)dststrides)[0] * elemSize;
    for (i=1; i<strlvls; i++) { 
      srcstr[i] = ((int32_t*)srcstrides)[i] * elemSize;
      dststr[i] = ((int32_t*)dststrides)[i] * elemSize;
      cnt[i] = ((int32_t*)count)[i];
    }
    cnt[strlvls] = ((int32_t*)count)[strlvls];
  }
  if (chpl_verbose_comm && !chpl_comm_no_debug_private) {
    printf("%d: %s:%d: remote get from %d. strlvls:%d. elemSize:%d  sizeof(size_t):%d  sizeof(gasnet_node_t):%d\n", chpl_nodeID, fn, ln, dstnode,(int)strlvls,elemSize,(int)sizeof(size_t),(int)sizeof(gasnet_node_t));

    printf("dststrides in bytes:\n");                 
    for (i=0;i<strlvls;i++) printf(" %d ",(int)dststr[i]);
    printf("\n");                     
    printf("srcstrides in bytes:\n");                 
    for (i=0;i<strlvls;i++) printf(" %d ",(int)srcstr[i]);
    printf("\n");                     
    printf("count (count[0] in bytes):\n");                   
    for (i=0;i<=strlvls;i++) printf(" %d ",(int)cnt[i]);
    printf("\n");                     
  }

  if (chpl_vdebug) {
    struct timeval tv;
    struct timezone tz = {0,0};
    (void)gettimeofday(&tv, &tz);
    dprintf (chpl_vdebug_fd, "put_strd: %lld.%06d, %d, %d, 0x%lx, 0x%lx, %d, %d, %d, %s\n",
	     (long long) tv.tv_sec, tv.tv_usec,  chpl_nodeID, dstnode, (long) dstaddr,
             (long) srcaddr, elemSize, typeIndex, ln, fn);
    // print out the srcstr and dststr?
	     
  }

  // the case (chpl_nodeID == dstnode) is internally managed inside gasnet
  if (chpl_verbose_comm && !chpl_comm_no_debug_private)
    printf("%d: %s:%d: remote get from %d\n", chpl_nodeID, fn, ln, dstnode);
  if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
    chpl_sync_lock(&chpl_comm_diagnostics_sync);
    chpl_comm_commDiagnostics.put++;
    chpl_sync_unlock(&chpl_comm_diagnostics_sync);
  }
  gasnet_puts_bulk(dstnode, dstaddr, dststr, srcaddr, srcstr, cnt, strlvls); 
}


////GASNET - introduce locale-int size
////GASNET - is caller in fork_t redundant? active message can determine this.
void  chpl_comm_fork(c_nodeid_t node, c_sublocid_t subloc,
                     chpl_fn_int_t fid, void *arg, int32_t arg_size) {
  fork_t* info;
  int     info_size;
  done_t  done;
  int     passArg = sizeof(fork_t) + arg_size <= gasnet_AMMaxMedium();

  if (chpl_nodeID == node) {
    chpl_ftable_call(fid, arg);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: remote task created on %d\n", chpl_nodeID, node);
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      chpl_sync_lock(&chpl_comm_diagnostics_sync);
      chpl_comm_commDiagnostics.fork++;
      chpl_sync_unlock(&chpl_comm_diagnostics_sync);
    }

    if (passArg) {
      info_size = sizeof(fork_t) + arg_size;
    } else {
      info_size = sizeof(fork_t) + sizeof(void*);
    }
    info = (fork_t*)chpl_mem_allocMany(1, info_size,
                                       CHPL_RT_MD_COMM_FORK_SEND_INFO, 0, 0);
    info->caller = chpl_nodeID;
    info->subloc = subloc;
    info->ack = &done;
    info->serial_state = chpl_task_getSerial();
    info->fid = fid;
    info->arg_size = arg_size;

    INIT_DONE_OBJ(done, 1);

    if (passArg) {
      if (arg_size)
        chpl_memcpy(&(info->arg), arg, arg_size);
      GASNET_Safe(gasnet_AMRequestMedium0(node, FORK, info, info_size));
    } else {
      chpl_memcpy(&(info->arg), &arg, sizeof(void*));
      GASNET_Safe(gasnet_AMRequestMedium0(node, FORK_LARGE, info, info_size));
    }
#ifndef CHPL_COMM_YIELD_TASK_WHILE_POLLING
    GASNET_BLOCKUNTIL(done.flag);
#else
    while (!done.flag) {
      (void) gasnet_AMPoll();
      chpl_task_yield();
    }
#endif
    chpl_mem_free(info, 0, 0);
  }
}

void  chpl_comm_fork_nb(c_nodeid_t node, c_sublocid_t subloc,
                        chpl_fn_int_t fid, void *arg, int32_t arg_size) {
  fork_t *info;
  int     info_size;
  int     passArg = (chpl_nodeID == node
                     || sizeof(fork_t) + arg_size <= gasnet_AMMaxMedium());

  void* argCopy = NULL;

  if (passArg) {
    info_size = sizeof(fork_t) + arg_size;
  } else {
    info_size = sizeof(fork_t) + sizeof(void*);
  }
  info = (fork_t*)chpl_mem_allocMany(info_size, sizeof(char), CHPL_RT_MD_COMM_FORK_SEND_NB_INFO, 0, 0);
  info->caller = chpl_nodeID;
  info->subloc = subloc;
  info->ack = info; // pass address to free after get in large case
  info->serial_state = chpl_task_getSerial();
  info->fid = fid;
  info->arg_size = arg_size;
  if (passArg) {
    if (arg_size)
      chpl_memcpy(&(info->arg), arg, arg_size);
  } else {
    // If the arg bundle is too large to fit in fork_t (i.e. passArg == false), 
    // Copy the args into auxilliary memory and pass a pointer to this instead.
    argCopy = chpl_mem_allocMany(1, arg_size,
                                 CHPL_RT_MD_COMM_FORK_SEND_NB_LARGE_ARG, 0, 0);
    chpl_memcpy(argCopy, arg, arg_size);
    *(void**)(&(info->arg)) = argCopy;
  }

  if (chpl_nodeID == node) {
    if (info->serial_state)
      fork_nb_wrapper(info);
    else
      chpl_task_startMovedTask((chpl_fn_p)fork_nb_wrapper, (void*)info,
                               subloc, chpl_nullTaskID,
                               info->serial_state);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: remote non-blocking task created on %d\n", chpl_nodeID, node);
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      chpl_sync_lock(&chpl_comm_diagnostics_sync);
      chpl_comm_commDiagnostics.fork_nb++;
      chpl_sync_unlock(&chpl_comm_diagnostics_sync);
    }
    if (passArg) {
      GASNET_Safe(gasnet_AMRequestMedium0(node, FORK_NB, info, info_size));
      chpl_mem_free(info, 0, 0);
    } else {
      GASNET_Safe(gasnet_AMRequestMedium0(node, FORK_NB_LARGE, info, info_size));
    }
  }
}

// GASNET - should only be called for "small" functions
void  chpl_comm_fork_fast(c_nodeid_t node, c_sublocid_t subloc,
                          chpl_fn_int_t fid, void *arg, int32_t arg_size) {
  char infod[gasnet_AMMaxMedium()];
  fork_t* info;
  int     info_size = sizeof(fork_t) + arg_size;
  done_t  done;
  int     passArg = info_size <= gasnet_AMMaxMedium();

  if (chpl_nodeID == node) {
    chpl_ftable_call(fid, arg);
  } else {
    if (passArg) {
      if (chpl_verbose_comm && !chpl_comm_no_debug_private)
        printf("%d: remote (no-fork) task created on %d\n",
               chpl_nodeID, node);
      if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
        chpl_sync_lock(&chpl_comm_diagnostics_sync);
        chpl_comm_commDiagnostics.fork_fast++;
        chpl_sync_unlock(&chpl_comm_diagnostics_sync);
      }
      info = (fork_t *) &infod;

      info->caller = chpl_nodeID;
      info->subloc = subloc;
      info->ack = &done;
      info->serial_state = chpl_task_getSerial();
      info->fid = fid;
      info->arg_size = arg_size;

      INIT_DONE_OBJ(done, 1);

      if (arg_size)
        chpl_memcpy(&(info->arg), arg, arg_size);
      GASNET_Safe(gasnet_AMRequestMedium0(node, FORK_FAST, info, info_size));
      // NOTE: We still have to wait for the handler to complete
#ifndef CHPL_COMM_YIELD_TASK_WHILE_POLLING
      GASNET_BLOCKUNTIL(done.flag);
#else
      while (!done.flag) {
        (void) gasnet_AMPoll();
        chpl_task_yield();
      }
#endif
    } else {
      // Call the normal chpl_comm_fork()
      chpl_comm_fork(node, subloc, fid, arg, arg_size);
    }
  }
}

void chpl_comm_make_progress(void)
{
  gasnet_AMPoll();
}


void chpl_startVerboseComm() {
  chpl_verbose_comm = 1;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_no_debug_private = 0;
}

void chpl_stopVerboseComm() {
  chpl_verbose_comm = 0;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int),
                              -1 /*typeIndex: unused*/);
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
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_no_debug_private = 0;
}

void chpl_stopCommDiagnostics() {
  chpl_comm_diagnostics = 0;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int),
                              -1 /*typeIndex: unused*/);
  chpl_comm_no_debug_private = 0;
}

void chpl_startCommDiagnosticsHere() {
  chpl_comm_diagnostics = 1;
}

void chpl_stopCommDiagnosticsHere() {
  chpl_comm_diagnostics = 0;
}

void chpl_resetCommDiagnosticsHere() {
  chpl_sync_lock(&chpl_comm_diagnostics_sync);
  memset(&chpl_comm_commDiagnostics, 0, sizeof(chpl_commDiagnostics));
  chpl_sync_unlock(&chpl_comm_diagnostics_sync);
}

void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd) {
  chpl_sync_lock(&chpl_comm_diagnostics_sync);
  chpl_memcpy(cd, &chpl_comm_commDiagnostics, sizeof(chpl_commDiagnostics));
  chpl_sync_unlock(&chpl_comm_diagnostics_sync);
}

uint64_t chpl_numCommGets(void) {
  return chpl_comm_commDiagnostics.get;
}

uint64_t chpl_numCommNBGets(void) {
  return chpl_comm_commDiagnostics.get_nb;
}

uint64_t chpl_numCommPuts(void) {
  return chpl_comm_commDiagnostics.put;
}

uint64_t chpl_numCommNBPuts(void) {
  return chpl_comm_commDiagnostics.put_nb;
}

uint64_t chpl_numCommTestNB(void) {
  return chpl_comm_commDiagnostics.test_nb;
}

uint64_t chpl_numCommWaitNB(void) {
  return chpl_comm_commDiagnostics.wait_nb;
}

uint64_t chpl_numCommTryNB(void) {
  return chpl_comm_commDiagnostics.try_nb;
}

uint64_t chpl_numCommFastForks(void) {
  return chpl_comm_commDiagnostics.fork_fast;
}

uint64_t chpl_numCommForks(void) {
  return chpl_comm_commDiagnostics.fork;
}

uint64_t chpl_numCommNBForks(void) {
  return chpl_comm_commDiagnostics.fork_nb;
}


void chpl_comm_gasnet_help_register_global_var(int i, wide_ptr_t wide_addr) {
  if (chpl_nodeID == 0) {
    ((wide_ptr_t*)seginfo_table[0].addr)[i] = wide_addr;
  }
}
