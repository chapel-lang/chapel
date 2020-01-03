/*
 * Copyright 2004-2020 Cray Inc.
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
#include "chpl-comm-diags.h"
#include "chpl-comm-callbacks.h"
#include "chpl-comm-callbacks-internal.h"
#include "chpl-comm-internal.h"
#include "chpl-env.h"
#include "chpl-mem.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "chpl-topo.h"
#include "chplcgfns.h"
#include "chpl-gen-includes.h"
#include "chpl-atomics.h"
#include "chpl-linefile-support.h"
#include "error.h"
#include "chpl-mem-desc.h"
#include "chpl-mem-sys.h" // mem layer not initialized in init, need sys alloc

// Don't get warning macros for chpl_comm_get etc
#include "chpl-comm-no-warning-macros.h"

#include <signal.h>
#include <sched.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <time.h>

static gasnet_seginfo_t* seginfo_table = NULL;

// Gasnet AM handler arguments are only 32 bits, so here we have
// functions to get the 2 arguments for a 64-bit pointer,
// and a function to reconstitute the pointer from the 2 arguments.
static inline
gasnet_handlerarg_t get_arg_from_ptr0(uintptr_t addr)
{
  // This one returns the bottom 32 bits.
  return ((gasnet_handlerarg_t)
            ((((uint64_t) (addr)) << 32UL) >> 32UL));
}
static inline
gasnet_handlerarg_t get_arg_from_ptr1(uintptr_t addr)
{
  // this one returns the top 32 bits.
  return ((gasnet_handlerarg_t)
            (((uint64_t) (addr)) >> 32UL));
}
static inline
uintptr_t get_uintptr_from_args(gasnet_handlerarg_t a0, gasnet_handlerarg_t a1 )
{
  return (uintptr_t)
           (((uint64_t) (uint32_t) a0)
            | (((uint64_t) (uint32_t) a1) << 32UL));
}
static inline
void* get_ptr_from_args(gasnet_handlerarg_t a0, gasnet_handlerarg_t a1 )
{
  return (void*) get_uintptr_from_args(a0, a1);
}


//
// Build acknowledgement address arguments for gasnetAMRequest*() calls.
//
#define Arg0(addr) get_arg_from_ptr0((uintptr_t)addr)
#define Arg1(addr) get_arg_from_ptr1((uintptr_t)addr)

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
static inline
void init_done_obj(done_t* done, int target) {
  atomic_init_uint_least32_t(&done->count, 0);
  done->target = target;
  done->flag = 0;
}

static inline
void wait_done_obj(done_t* done, chpl_bool do_yield)
{
#ifndef CHPL_COMM_YIELD_TASK_WHILE_POLLING
  GASNET_BLOCKUNTIL(done->flag);
#else
  while (!done->flag) {
    (void) gasnet_AMPoll();
    if (do_yield)
      chpl_task_yield();
  }
#endif
}

typedef struct {
  c_nodeid_t    caller;
  c_sublocid_t  subloc;

  void*         ack;

  chpl_fn_int_t fid;
  uint16_t      payload_size;

  // TODO: is there a way to "compress" this?
  chpl_task_ChapelData_t state;
} small_fork_hdr_t;

typedef struct {
  small_fork_hdr_t hdr;
  void*            arg;
  size_t           arg_size;
} large_fork_t;

#define MAX_SMALL_FORK_SIZE 128

typedef struct {
  unsigned char space[MAX_SMALL_FORK_SIZE];
} small_fork_space_t;

typedef union {
  small_fork_hdr_t   small;
  large_fork_t       large;
  small_fork_space_t space;
} special_fork_t;

typedef struct {
  chpl_comm_on_bundle_t bundle;
  special_fork_t        space;
} small_fork_task_t;

typedef struct {
  chpl_comm_on_bundle_t bundle;
  large_fork_t          large;
} large_fork_task_t;

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

typedef struct {
  void* ack; // acknowledgement object
  void* tgt; // target memory address
  void* src; // source memory address
  size_t size; // number of bytes.
} xfer_info_t;


//
// AM functions
//
typedef enum {
  FORK = 128,           // synchronous fork
  FORK_SMALL,           // synchronous small fork
  FORK_LARGE,           // synchronous fork with a huge argument
  FORK_NB,              // non-blocking fork
  FORK_NB_SMALL,        // non-blocking small fork
  FORK_NB_LARGE,        // non-blocking fork with a huge argument
  FORK_FAST,            // run the function in the handler (use with care)
  FORK_FAST_SMALL,      // run the function in the handler (use with care)

  SIGNAL,               // ack to a done_t via gasnet_AMReplyShortM()
  SIGNAL_LONG,          // ack to a done_t via gasnet_AMReplyLongM()
  PRIV_BCAST,           // put data at addr (used for private broadcast)
  PRIV_BCAST_LARGE,     // put data at addr (used for private broadcast)
  FREE,                 // free data at addr
  SHUTDOWN,             // tell nodes to get ready for shutdown
  BCAST_SEGINFO,        // broadcast for segment info table
  DO_REPLY_PUT,         // do a PUT here from another locale
  DO_COPY_PAYLOAD       // copy AM payload to another address
} AM_handler_function_idx_t;

static void AM_fork_fast(gasnet_token_t token, void* buf, size_t nbytes) {
  chpl_comm_on_bundle_t *f = buf;

  // Run the function
  chpl_ftable_call(f->task_bundle.requested_fid, f);

  // Signal that the handler has completed if that was requested.
  if (f->comm.ack)
    GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL,
                                     Arg0(f->comm.ack), Arg1(f->comm.ack)));
}

static inline
size_t setup_small_fork_task(small_fork_task_t* dst, small_fork_hdr_t* f, size_t nbytes)
{
  chpl_comm_bundleData_t comm  = { .caller = f->caller,
                                   .ack    = f->ack };
  chpl_comm_on_bundle_t bundle = { .comm =  comm };
  chpl_comm_on_bundle_t *bptr  = &dst->bundle;
  size_t payload_size = nbytes - sizeof(small_fork_hdr_t);

  // Copy task-local data to the new task
  bundle.task_bundle.state = f->state;

  dst->bundle = bundle;

  // Copy the payload into the special task
  memcpy(bptr + 1, f + 1, payload_size);

  return sizeof(chpl_comm_on_bundle_t) + payload_size;
}


static inline
size_t setup_large_fork_task(large_fork_task_t* dst, large_fork_t* f, size_t nbytes)
{
  chpl_comm_bundleData_t comm  = { .caller = f->hdr.caller,
                                   .ack    = f->hdr.ack };
  chpl_comm_on_bundle_t bundle = { .comm =  comm };

  // Copy task-local data to the new task
  bundle.task_bundle.state = f->hdr.state;

  dst->bundle = bundle;
  // Copy the large fork info into the task bundle
  dst->large = *f;

  return sizeof(large_fork_task_t);
}


static void AM_fork_fast_small(gasnet_token_t token, void* buf, size_t nbytes) {
  small_fork_hdr_t *f = buf;
  small_fork_task_t task;
  chpl_comm_on_bundle_t *bptr = &task.bundle;

  // Copy the data into a chpl_comm_on_bundle_t
  setup_small_fork_task(&task, f, nbytes);

  // Run the function
  chpl_ftable_call(f->fid, bptr);

  // Signal that the handler has completed if that was requested.
  if (f->ack)
    GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL,
                                     Arg0(f->ack), Arg1(f->ack)));
}


static void fork_wrapper(chpl_comm_on_bundle_t *f) {
  chpl_ftable_call(f->task_bundle.requested_fid, f);

  GASNET_Safe(gasnet_AMRequestShort2(f->comm.caller, SIGNAL,
                                     Arg0(f->comm.ack), Arg1(f->comm.ack)));
}

static void AM_fork(gasnet_token_t token, void* buf, size_t nbytes) {
  chpl_comm_on_bundle_t *f = (chpl_comm_on_bundle_t*) buf;
  chpl_task_startMovedTask(f->task_bundle.requested_fid,
                           (chpl_fn_p)fork_wrapper,
                           chpl_comm_on_bundle_task_bundle(f), nbytes,
                           f->task_bundle.requestedSubloc, chpl_nullTaskID);
}

static void AM_fork_small(gasnet_token_t token, void* buf, size_t nbytes) {
  small_fork_hdr_t *f = buf;
  small_fork_task_t task;
  chpl_comm_on_bundle_t *bptr = &task.bundle;
  size_t size;

  // Copy the data into a chpl_comm_on_bundle_t
  size = setup_small_fork_task(&task, f, nbytes);
 
  chpl_task_startMovedTask(f->fid, (chpl_fn_p)fork_wrapper,
                           chpl_comm_on_bundle_task_bundle(bptr),
                           size,
                           f->subloc, chpl_nullTaskID);
}


static void fork_large_wrapper(large_fork_task_t* f) {
  large_fork_t *lg = &f->large;
  chpl_comm_on_bundle_t* arg;
  int caller;
  size_t bundle_size_on_caller;
  void* arg_on_caller;
  void* ack;
  chpl_fn_int_t fid;

  caller = lg->hdr.caller;
  bundle_size_on_caller = lg->arg_size;
  arg_on_caller = lg->arg;
  ack = lg->hdr.ack;
  fid = lg->hdr.fid;

  // Allocate the bundle
  arg = chpl_mem_allocMany(1, bundle_size_on_caller,
                           CHPL_RT_MD_COMM_FRK_RCV_ARG, 0, 0);

  // GET the bundle data
  // TODO: This could get only the payload
  chpl_comm_get(arg, caller, arg_on_caller, bundle_size_on_caller,
                CHPL_COMM_UNKNOWN_ID, 0, CHPL_FILE_IDX_FORK_LARGE);

  // Call the on body function
  chpl_ftable_call(fid, arg);

  // Signal completion
  GASNET_Safe(gasnet_AMRequestShort2(caller, SIGNAL, Arg0(ack), Arg1(ack)));

  // Free the bundle we just allocated.
  chpl_mem_free(arg, 0, 0);
}

////GASNET - can we send as much of user data as possible initially
////           hide data copy by making get non-blocking
////GASNET - can we allocate f big enough so as not to need malloc in wrapper
static void AM_fork_large(gasnet_token_t token, void* buf, size_t nbytes) {
  large_fork_t *f = buf;
  large_fork_task_t task;
  chpl_comm_on_bundle_t *bptr = &task.bundle;
  size_t size;

  // Copy the data into a chpl_comm_on_bundle_t
  size = setup_large_fork_task(&task, f, nbytes);

  chpl_task_startMovedTask(f->hdr.fid, (chpl_fn_p)fork_large_wrapper,
                           chpl_comm_on_bundle_task_bundle(bptr), size,
                           f->hdr.subloc, chpl_nullTaskID);
}

static void fork_nb_wrapper(chpl_comm_on_bundle_t *f) {
  chpl_ftable_call(f->task_bundle.requested_fid, f);
}

static void AM_fork_nb(gasnet_token_t  token,
                        void           *buf,
                        size_t          nbytes) {
  chpl_comm_on_bundle_t *f = (chpl_comm_on_bundle_t*) buf;
  
  chpl_task_startMovedTask(f->task_bundle.requested_fid,
                           (chpl_fn_p)fork_nb_wrapper,
                           chpl_comm_on_bundle_task_bundle(f), nbytes,
                           f->task_bundle.requestedSubloc, chpl_nullTaskID);
}

static void AM_fork_nb_small(gasnet_token_t  token,
                             void           *buf,
                             size_t          nbytes) {
  small_fork_hdr_t *f = buf;
  small_fork_task_t task;
  chpl_comm_on_bundle_t *bptr = &task.bundle;
  size_t size;

  // Copy the data into a chpl_comm_on_bundle_t
  size = setup_small_fork_task(&task, f, nbytes);
 
  chpl_task_startMovedTask(f->fid, (chpl_fn_p)fork_nb_wrapper,
                           chpl_comm_on_bundle_task_bundle(bptr), size,
                           f->subloc, chpl_nullTaskID);
}


static void fork_nb_large_wrapper(large_fork_task_t* f) {
  large_fork_t *lg = &f->large;
  chpl_comm_on_bundle_t* arg;
  int caller;
  size_t bundle_size_on_caller;
  void* arg_on_caller;
  chpl_fn_int_t fid;

  caller = lg->hdr.caller;
  bundle_size_on_caller = lg->arg_size;
  arg_on_caller = lg->arg;
  fid = lg->hdr.fid;

  // Allocate the bundle
  arg = chpl_mem_allocMany(1, bundle_size_on_caller,
                           CHPL_RT_MD_COMM_FRK_RCV_ARG, 0, 0);

  // GET the bundle data
  chpl_comm_get(arg, caller, arg_on_caller, bundle_size_on_caller,
                CHPL_COMM_UNKNOWN_ID, 0, CHPL_FILE_IDX_FORK_LARGE);

  // Signal that the allocated region can be freed
  GASNET_Safe(gasnet_AMRequestShort2(caller, FREE,
                                     Arg0(arg_on_caller),
                                     Arg1(arg_on_caller)));

  // Call the user function
  chpl_ftable_call(fid, arg);

  // Free the bundle we just allocated
  chpl_mem_free(arg, 0, 0);
}

static void AM_fork_nb_large(gasnet_token_t token, void* buf, size_t nbytes) {
  large_fork_t *f = buf;
  large_fork_task_t task;
  chpl_comm_on_bundle_t *bptr = &task.bundle;
  size_t size;

  // Copy the data into a chpl_comm_on_bundle_t
  size = setup_large_fork_task(&task, f, nbytes);

  chpl_task_startMovedTask(f->hdr.fid, (chpl_fn_p)fork_nb_large_wrapper,
                           chpl_comm_on_bundle_task_bundle(bptr), size,
                           f->hdr.subloc, chpl_nullTaskID);
}

static void AM_signal(gasnet_token_t token, gasnet_handlerarg_t a0, gasnet_handlerarg_t a1) {
  done_t* done = (done_t*) get_ptr_from_args(a0, a1);
  uint_least32_t prev;
  prev = atomic_fetch_add_explicit_uint_least32_t(&done->count, 1,
                                                  memory_order_seq_cst);
  if (prev + 1 == done->target)
    done->flag = 1;
}

static void AM_signal_long(gasnet_token_t token, void *buf, size_t nbytes,
                           gasnet_handlerarg_t a0, gasnet_handlerarg_t a1) {
  done_t* done = (done_t*) get_ptr_from_args(a0, a1);
  uint_least32_t prev;
  prev = atomic_fetch_add_explicit_uint_least32_t(&done->count, 1,
                                                  memory_order_seq_cst);
  if (prev + 1 == done->target)
    done->flag = 1;
}

static void AM_priv_bcast(gasnet_token_t token, void* buf, size_t nbytes) {
  priv_bcast_t* pbp = buf;
  chpl_memcpy(chpl_rt_priv_bcast_tab[pbp->id], pbp->data, pbp->size);

  // Signal that the handler has completed
  GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL,
                                   Arg0(pbp->ack), Arg1(pbp->ack)));
}

static void AM_priv_bcast_large(gasnet_token_t token, void* buf, size_t nbytes) {
  priv_bcast_large_t* pblp = buf;
  chpl_memcpy((char*)chpl_rt_priv_bcast_tab[pblp->id]+pblp->offset, pblp->data, pblp->size);

  // Signal that the handler has completed
  GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL,
                                   Arg0(pblp->ack), Arg1(pblp->ack)));
}

static void AM_free(gasnet_token_t token, gasnet_handlerarg_t a0, gasnet_handlerarg_t a1) {
  void* to_free = get_ptr_from_args(a0, a1);
  
  chpl_mem_free(to_free, 0, 0);
}

static void AM_shutdown(gasnet_token_t token) {
  chpl_signal_shutdown();
}

//
// This global and routine are used to broadcast the seginfo_table at the outset
// of the program's execution.  It is designed to only be used once.  This code
// was modeled after the _test_segbcast() routine in
// third-party/gasnet/gasnet-src/tests/test.h
//
static int bcast_seginfo_done = 0;
static void AM_bcast_seginfo(gasnet_token_t token, void *buf, size_t nbytes) {
  assert(nbytes == sizeof(gasnet_seginfo_t)*gasnet_nodes());
  chpl_memcpy(seginfo_table, buf, nbytes);
  gasnett_local_wmb();
  bcast_seginfo_done = 1;
}

// Put from arg->src (which is local to the AM handler) back to
// arg->dst (which is local to the caller of this AM).
// nbytes is < gasnet_AMMaxLongReply here (see chpl_comm_get).
static void AM_reply_put(gasnet_token_t token, void* buf, size_t nbytes) {
  xfer_info_t* x = buf;

  assert(nbytes == sizeof(xfer_info_t));

  GASNET_Safe(gasnet_AMReplyLong2(token, SIGNAL_LONG,
                                  x->src, x->size, x->tgt,
                                  Arg0(x->ack), Arg1(x->ack)));
}

// Copy from the payload in this active message to dst.
static
void AM_copy_payload(gasnet_token_t token, void* buf, size_t nbytes,
                     gasnet_handlerarg_t ack0, gasnet_handlerarg_t ack1,
                     gasnet_handlerarg_t dst0, gasnet_handlerarg_t dst1)
{
  void* dst = get_ptr_from_args(dst0, dst1);

  memcpy(dst, buf, nbytes);

  GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL, ack0, ack1));
}

static gasnet_handlerentry_t ftable[] = {
  {FORK,          AM_fork},
  {FORK_SMALL,    AM_fork_small},
  {FORK_LARGE,    AM_fork_large},
  {FORK_NB,       AM_fork_nb},
  {FORK_NB_SMALL, AM_fork_nb_small},
  {FORK_NB_LARGE, AM_fork_nb_large},
  {FORK_FAST,     AM_fork_fast},
  {FORK_FAST_SMALL, AM_fork_fast_small},
  {SIGNAL,        AM_signal},
  {SIGNAL_LONG,   AM_signal_long},
  {PRIV_BCAST,    AM_priv_bcast},
  {PRIV_BCAST_LARGE, AM_priv_bcast_large},
  {FREE,          AM_free},
  {SHUTDOWN,      AM_shutdown},
  {BCAST_SEGINFO, AM_bcast_seginfo},
  {DO_REPLY_PUT,  AM_reply_put},
  {DO_COPY_PAYLOAD, AM_copy_payload}
};

//
// Chapel interface starts here
//
chpl_comm_nb_handle_t chpl_comm_put_nb(void *addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t commID,
                                       int ln, int32_t fn)
{
  gasnet_handle_t ret;
  int remote_in_segment;

  // Communication callbacks
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put_nb)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_put_nb, chpl_nodeID, node,
       .iu.comm={addr, raddr, size, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }
    
#ifdef GASNET_SEGMENT_EVERYTHING
    remote_in_segment = 1;
#else
    remote_in_segment = chpl_comm_addr_gettable(node, raddr, size);
#endif

  if(!remote_in_segment) {
    chpl_comm_put(addr, node, raddr, size, commID, ln, fn);
    ret = NULL;
    return (chpl_comm_nb_handle_t) ret;
  }

  ret = gasnet_put_nb_bulk(node, raddr, addr, size);

  chpl_comm_diags_incr(put_nb);

  return (chpl_comm_nb_handle_t) ret;
}

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, c_nodeid_t node, void* raddr,
                                       size_t size, int32_t commID,
                                       int ln, int32_t fn)
{
  gasnet_handle_t ret;
  int remote_in_segment;

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get_nb)) {
    chpl_comm_cb_info_t cb_data = 
      {chpl_comm_cb_event_kind_get_nb, chpl_nodeID, node,
       .iu.comm={addr, raddr, size, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }

#ifdef GASNET_SEGMENT_EVERYTHING
    remote_in_segment = 1;
#else
    remote_in_segment = chpl_comm_addr_gettable(node, raddr, size);
#endif

  if(!remote_in_segment) {
    chpl_comm_get(addr, node, raddr, size, commID, ln, fn);
    ret = NULL;
    return (chpl_comm_nb_handle_t) ret;
  }

  ret = gasnet_get_nb_bulk(addr, node, raddr, size);

  chpl_comm_diags_incr(get_nb);

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

int chpl_comm_addr_gettable(c_nodeid_t node, void* start, size_t len)
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
static chpl_bool pollingRequired;

static void polling(void* x) {
  pollingRunning = 1;

  while (!pollingQuit) {
    (void) gasnet_AMPoll();
    chpl_task_yield();
  }

  pollingRunning = 0;
}

static void setup_polling(void) {
#if defined(GASNET_CONDUIT_IBV)
  pollingRequired = false;
  chpl_env_set("GASNET_RCV_THREAD", "1", 1);
#else
  pollingRequired = true;
#endif
}

static void start_polling(void) {
  if (!pollingRequired) return;

  pollingRunning = 0;
  pollingQuit = 0;

  if (chpl_task_createCommTask(polling, NULL)) {
    chpl_internal_error("unable to start polling task for gasnet");
  }

  while (!pollingRunning) {
    sched_yield();
  }
}

static void stop_polling(chpl_bool wait) {
  if (!pollingRequired) return;

  pollingQuit = 1;

  if (wait) {
    while (pollingRunning) {
      sched_yield();
    }
  }
}

static void set_max_segsize_env_var(size_t size) {
  chpl_env_set_uint("GASNET_MAX_SEGSIZE", size, 1);
}

static void set_max_segsize() {
  size_t size;

  // GASNet defaults to 85% of physical memory, which is a good default for us,
  // so only override if a user explicitly set CHPL_RT_MAX_HEAP_SIZE
  if ((size = (size_t) chpl_comm_getenvMaxHeapSize()) != 0) {
    set_max_segsize_env_var(size);
    return;
  }
}

static void set_num_comm_domains() {
#if defined(GASNET_CONDUIT_ARIES)
  const int num_cpus = chpl_topo_getNumCPUsPhysical(true) + 1;
  chpl_env_set_uint("GASNET_DOMAIN_COUNT", num_cpus, 0);
  chpl_env_set("GASNET_AM_DOMAIN_POLL_MASK", "3", 0);

  // GASNET_DOMAIN_COUNT increases the shutdown time. Work around this for now.
  // See https://github.com/chapel-lang/chapel/issues/7251 and
  // https://upc-bugs.lbl.gov/bugzilla/show_bug.cgi?id=3621
  chpl_env_set("GASNET_EXITTIMEOUT_FACTOR", "0.5", 0);
  chpl_env_set("GASNET_EXITTIMEOUT_MIN", "10.0", 0);
#endif
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
//  int status; // Some compilers complain about unused variable 'status'.

  set_max_segsize();
  set_num_comm_domains();
  setup_polling();

  assert(sizeof(gasnet_handlerarg_t)==sizeof(uint32_t));

  gasnet_init(argc_p, argv_p);
  chpl_nodeID = gasnet_mynode();
  chpl_numNodes = gasnet_nodes();
  GASNET_Safe(gasnet_attach(ftable, 
                            sizeof(ftable)/sizeof(gasnet_handlerentry_t),
                            gasnet_getMaxLocalSegmentSize(),
                            0));
  // TODO (EJR: 03/03/16): we currently "leak" seginfo_table. We should
  // probably free it on exit (but only for "clean" exits.)
  seginfo_table = (gasnet_seginfo_t*)sys_malloc(chpl_numNodes*sizeof(gasnet_seginfo_t));
  //
  // The following call has no real effect on the .addr and .size
  // fields for GASNET_SEGMENT_EVERYTHING, but is recommended to be
  // used anyway (see third-party/gasnet/gasnet-src/tests/test.h)
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
  // _test_attach() routine from third-party/gasnet/gasnet-src/tests/test.h
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
    void* global_table = sys_malloc(global_table_size);
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
    // Skip loc 0, since that would end up memcpy'ing seginfo_table to itself
    for (i=1; i < chpl_numNodes; i++) {
      GASNET_Safe(gasnet_AMRequestMedium0(i, BCAST_SEGINFO, seginfo_table, 
                                          chpl_numNodes*sizeof(gasnet_seginfo_t)));
    }
  } else {
    GASNET_BLOCKUNTIL(bcast_seginfo_done);
  }
  chpl_comm_barrier("making sure everyone's done with the broadcast");
#endif

  gasnet_set_waitmode(GASNET_WAIT_BLOCK);

}

void chpl_comm_post_mem_init(void) {
  chpl_comm_init_prv_bcast_tab();
}

//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

void chpl_comm_post_task_init(void) {
  start_polling();
}

void chpl_comm_rollcall(void) {
  // Initialize diags
  chpl_comm_diags_init();

  chpl_msg(2, "executing on node %d of %d node(s): %s\n", chpl_nodeID, 
           chpl_numNodes, chpl_nodeName());
}

void chpl_comm_impl_regMemHeapInfo(void** start_p, size_t* size_p) {
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

wide_ptr_t* chpl_comm_broadcast_global_vars_helper(void) {
  //
  // Gather the global variables' wide pointers on node 0 into the
  // buffer at the front of our communicable segment.  We don't have
  // to communicate that because everyone already knows where it is.
  // We do need to delay returning on the non-0 nodes until after
  // node 0 has filled in that buffer, however.
  //
  if (chpl_nodeID == 0) {
    for (int i = 0; i < chpl_numGlobalsOnHeap; i++) {
      ((wide_ptr_t*) seginfo_table[0].addr)[i] = *chpl_globals_registry[i];
    }
    chpl_comm_barrier("fill node 0 globals buf");
    return NULL; // so common code won't try to free it!
  } else {
    chpl_comm_barrier("fill node 0 globals buf");
    return (wide_ptr_t*) seginfo_table[0].addr;
  }
}

void chpl_comm_broadcast_private(int id, size_t size) {
  int  node, offset;
  int  payloadSize = size + sizeof(priv_bcast_t);
  done_t* done;
  int numOffsets=1;

  // This can use the system allocator because it involves internode communication.
  done = (done_t*) chpl_mem_allocManyZero(chpl_numNodes, sizeof(*done),
                                          CHPL_RT_MD_COMM_FRK_DONE_FLAG,
                                          0, 0);
  if (payloadSize <= gasnet_AMMaxMedium()) {
    priv_bcast_t* pbp = chpl_mem_allocMany(1, payloadSize, CHPL_RT_MD_COMM_PRV_BCAST_DATA, 0, 0);
    chpl_memcpy(pbp->data, chpl_rt_priv_bcast_tab[id], size);
    pbp->id = id;
    pbp->size = size;
    for (node = 0; node < chpl_numNodes; node++) {
      if (node != chpl_nodeID) {
        pbp->ack = &done[node];
        init_done_obj(&done[node], 1);
        GASNET_Safe(gasnet_AMRequestMedium0(node, PRIV_BCAST, pbp, payloadSize));
      }
    }
    chpl_mem_free(pbp, 0, 0);
  } else {
    size_t maxpayloadsize = gasnet_AMMaxMedium();
    size_t maxsize = maxpayloadsize - sizeof(priv_bcast_large_t);
    priv_bcast_large_t* pblp = chpl_mem_allocMany(1, maxpayloadsize, CHPL_RT_MD_COMM_PRV_BCAST_DATA, 0, 0);
    pblp->id = id;
    numOffsets = (size+maxsize)/maxsize;
    for (node = 0; node < chpl_numNodes; node++) {
      if (node != chpl_nodeID)
        init_done_obj(&done[node], numOffsets);
    }
    for (offset = 0; offset < size; offset += maxsize) {
      size_t thissize = size - offset;
      if (thissize > maxsize)
        thissize = maxsize;
      pblp->offset = offset;
      pblp->size = thissize;
      chpl_memcpy(pblp->data, (char*)chpl_rt_priv_bcast_tab[id]+offset, thissize);
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

    if (chpl_nodeID == 0) {
     for (int node = 1; node < chpl_numNodes; node++) {
        GASNET_Safe(gasnet_AMRequestShort0(node, SHUTDOWN));
      }
    } else {
      chpl_wait_for_shutdown();
    }

    chpl_comm_barrier("stop polling");

    //
    // Tell the polling task to stop, then wait for it to do so.
    //
    stop_polling(/*wait*/ true);
  }
}

void chpl_comm_exit(int all, int status) {
  stop_polling(/*wait*/ false);

  if (all)
    chpl_comm_barrier("exit_comm_gasnet");

  gasnet_exit(status);
}

void  chpl_comm_put(void* addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t commID, int ln, int32_t fn) {
  int remote_in_segment;

  if (chpl_nodeID == node) {
    memmove(raddr, addr, size);
  } else {
    // Communications callback support
    if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_put, chpl_nodeID, node,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
    }

    chpl_comm_diags_verbose_rdma("put", node, size, ln, fn, commID);
    chpl_comm_diags_incr(put);

    // Handle remote address not in remote segment.
#ifdef GASNET_SEGMENT_EVERYTHING
    remote_in_segment = 1;
#else
    remote_in_segment = chpl_comm_addr_gettable(node, raddr, size);
#endif

    if( remote_in_segment ) {
      // If it's in the remote segment, great, do a normal gasnet_put.
      // GASNet will handle the local portion not being in the segment.
      gasnet_put(node, raddr, addr, size); // node, dest, src, size
    } else {
      // If it's not in the remote segment, we need to send an
      // active message so that the other node will copy the data
      // that we're sending.
      size_t max_chunk = gasnet_AMMaxMedium();
      size_t start;

      // use AMRequestMedium to send the PUT data to the remote node
      // and then its AM handler will memcpy.
      // We could have the remote node do a GET, but that would require
      // it to start a task since you can't do a GET in an AM handler
      // (and the reply doesn't help).
      for(start = 0; start < size; start += max_chunk) {
        size_t this_size;
        void* addr_chunk;
        void* raddr_chunk;
        done_t done;

        this_size = size - start;
        if( this_size > max_chunk ) {
          this_size = max_chunk;
        }

        addr_chunk = ((char*) addr) + start;
        raddr_chunk = ((char*) raddr) + start;

        init_done_obj(&done, 1);

        // Send an AM over to ask for a them to copy the data
        // passed in the active message (addr_chunk) to raddr_chunk.
        GASNET_Safe(gasnet_AMRequestMedium4(node, DO_COPY_PAYLOAD,
                                            addr_chunk, this_size,
                                            Arg0(&done), Arg1(&done),
                                            Arg0(raddr_chunk),
                                            Arg1(raddr_chunk)));

        // Wait for the PUT to complete.
        wait_done_obj(&done, false);
      }
    }
  }
}

////GASNET - pass trace info to gasnet_get
////GASNET - define GASNET_E_ PUTGET always REMOTE
////GASNET - look at GASNET tools at top of README.tools has atomic counters
void  chpl_comm_get(void* addr, c_nodeid_t node, void* raddr,
                    size_t size, int32_t commID, int ln, int32_t fn) {
  int remote_in_segment;

  if (chpl_nodeID == node) {
    memmove(addr, raddr, size);
  } else {
    // Communications callback support
    if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_get, chpl_nodeID, node,
         .iu.comm={addr, raddr, size, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
    }

    chpl_comm_diags_verbose_rdma("get", node, size, ln, fn, commID);
    chpl_comm_diags_incr(get);

    // Handle remote address not in remote segment.

    // The GASNet Spec says:
    //   The source memory address for all gets and the target memory address
    //   for all puts must fall within the memory area registered for remote
    //   access by the remote node (see gasnet_attach()), or the results are
    //   undefined

    // In other words, it is OK if the local side of a GET or PUT
    // is not in the registered memory region.

#ifdef GASNET_SEGMENT_EVERYTHING
    remote_in_segment = 1;
#else
    remote_in_segment = chpl_comm_addr_gettable(node, raddr, size);
#endif

    if( remote_in_segment ) {
      // If it's in the remote segment, great, do a normal gasnet_get.
      // GASNet will handle the local portion not being in the segment.
      gasnet_get(addr, node, raddr, size); // dest, node, src, size
    } else {
      // If it's not in the remote segment, we need to send an
      // active message so that the other node will PUT back to us.
      // In order for that to work, the local side has to be in
      // the registered memory segment.
      int local_in_segment;
      void* local_buf = NULL;
      size_t max_chunk = gasnet_AMMaxLongReply();
      size_t start;

#ifdef GASNET_SEGMENT_EVERYTHING
      local_in_segment = 1;
#else
      local_in_segment = chpl_comm_addr_gettable(chpl_nodeID, addr, size);
#endif

      // If the local address isn't in a registered segment,
      // do the GET into a temporary buffer instead, and then
      // copy the result back.
      if( ! local_in_segment ) {
        size_t buf_sz = size;
        if( buf_sz > max_chunk ) {
          buf_sz = max_chunk;
        }

        local_buf = chpl_mem_alloc(buf_sz, CHPL_RT_MD_COMM_XMIT_RCV_BUF, 0, 0);
#ifdef GASNET_SEGMENT_EVERYTHING
        // local_buf is definitely in our segment
#else
        assert(chpl_comm_addr_gettable(chpl_nodeID, local_buf, buf_sz));
#endif
      }

      // do a PUT on the remote locale back to here.
      // But do it in chunks of size gasnet_AMMaxLongReply()
      // since we use gasnet_AMReplyLong to do the PUT.
      for(start = 0; start < size; start += max_chunk) {
        size_t this_size;
        void* addr_chunk;
        xfer_info_t info;
        done_t done;

        this_size = size - start;
        if( this_size > max_chunk ) {
          this_size = max_chunk;
        }

        addr_chunk = ((char*) addr) + start;

        init_done_obj(&done, 1);

        info.ack = &done;
        info.tgt = local_buf?local_buf:addr_chunk;
        info.src = ((char*) raddr) + start;
        info.size = this_size;

        // Send an AM over to ask for a PUT back to us
        GASNET_Safe(gasnet_AMRequestMedium0(node, DO_REPLY_PUT,
                                            &info, sizeof(info)));

        // Wait for the PUT to complete.
        wait_done_obj(&done, false);

        // Now copy from local_buf back to addr if necessary.
        if( local_buf ) {
          memcpy(addr_chunk, local_buf, this_size);
        }
      }

      // If we were using a temporary local buffer free it
      if( local_buf ) {
        chpl_mem_free(local_buf, 0, 0);
      }
    }
  }
}

//
// This is an adapter from Chapel code to GASNet's gasnet_gets_bulk. It does:
// * convert count[0] and all of 'srcstr' and 'dststr' from counts of element
//   to counts of bytes,
//
void  chpl_comm_get_strd(void* dstaddr, size_t* dststrides, c_nodeid_t srcnode_id, 
                         void* srcaddr, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t commID,
                         int ln, int32_t fn) {
  int i;
  const size_t strlvls = (size_t)stridelevels;
  const gasnet_node_t srcnode = (gasnet_node_t)srcnode_id;

  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  // Only count[0] and strides are measured in number of bytes.
  cnt[0] = count[0] * elemSize;

  if (strlvls>0) {
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1; i<strlvls; i++) { 
      srcstr[i] = srcstrides[i] * elemSize;
      dststr[i] = dststrides[i] * elemSize;
      cnt[i] = count[i];
    }
    cnt[strlvls] = count[strlvls];
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_get_strd)) {
    chpl_comm_cb_info_t cb_data =
      {chpl_comm_cb_event_kind_get_strd, chpl_nodeID, srcnode_id,
       .iu.comm_strd={srcaddr, srcstrides, dstaddr, dststrides, count,
                      stridelevels, elemSize, commID, ln, fn}};
    chpl_comm_do_callbacks (&cb_data);
  }
  
  // the case (chpl_nodeID == srcnode) is internally managed inside gasnet
  chpl_comm_diags_verbose_rdmaStrd("get", srcnode, ln, fn, commID);
  if (chpl_nodeID != srcnode) {
    chpl_comm_diags_incr(get);
  }

  // TODO -- handle strided get for non-registered memory
  gasnet_gets_bulk(dstaddr, dststr, srcnode, srcaddr, srcstr, cnt, strlvls); 
}

// See the comment for chpl_comm_gets().
void  chpl_comm_put_strd(void* dstaddr, size_t* dststrides, c_nodeid_t dstnode_id, 
                         void* srcaddr, size_t* srcstrides, size_t* count,
                         int32_t stridelevels, size_t elemSize, int32_t commID, 
                         int ln, int32_t fn) {
  int i;
  const size_t strlvls = (size_t)stridelevels;
  const gasnet_node_t dstnode = (gasnet_node_t)dstnode_id;

  size_t dststr[strlvls];
  size_t srcstr[strlvls];
  size_t cnt[strlvls+1];

  // Only count[0] and strides are measured in number of bytes.
  cnt[0] = count[0] * elemSize;
  if (strlvls>0) {
    srcstr[0] = srcstrides[0] * elemSize;
    dststr[0] = dststrides[0] * elemSize;
    for (i=1; i<strlvls; i++) { 
      srcstr[i] = srcstrides[i] * elemSize;
      dststr[i] = dststrides[i] * elemSize;
      cnt[i] = count[i];
    }
    cnt[strlvls] = count[strlvls];
  }

  // Communications callback support
  if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_put_strd)) {
      chpl_comm_cb_info_t cb_data =
        {chpl_comm_cb_event_kind_put_strd, chpl_nodeID, dstnode_id,
         .iu.comm_strd={srcaddr, srcstrides, dstaddr, dststrides, count,
                        stridelevels, elemSize, commID, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
  }

  // the case (chpl_nodeID == dstnode) is internally managed inside gasnet
  chpl_comm_diags_verbose_rdmaStrd("put", dstnode, ln, fn, commID);
  if (chpl_nodeID != dstnode) {
    chpl_comm_diags_incr(put);
  }

  // TODO -- handle strided put for non-registered memory
  gasnet_puts_bulk(dstnode, dstaddr, dststr, srcaddr, srcstr, cnt, strlvls); 
}

#define MAX_UNORDERED_TRANS_SZ 1024
void chpl_comm_getput_unordered(c_nodeid_t dstnode, void* dstaddr,
                                c_nodeid_t srcnode, void* srcaddr,
                                size_t size, int32_t commID,
                                int ln, int32_t fn) {
  assert(dstaddr != NULL);
  assert(srcaddr != NULL);

  if (size == 0)
    return;

  if (dstnode == chpl_nodeID && srcnode == chpl_nodeID) {
    memmove(dstaddr, srcaddr, size);
    return;
  }

  if (dstnode == chpl_nodeID) {
    chpl_comm_get(dstaddr, srcnode, srcaddr, size, commID, ln, fn);
  } else if (srcnode == chpl_nodeID) {
    chpl_comm_put(srcaddr, dstnode, dstaddr, size, commID, ln, fn);
  } else {
    if (size <= MAX_UNORDERED_TRANS_SZ) {
      char buf[MAX_UNORDERED_TRANS_SZ];
      chpl_comm_get(buf, srcnode, srcaddr, size, commID, ln, fn);
      chpl_comm_put(buf, dstnode, dstaddr, size, commID, ln, fn);
    } else {
      // Note, we do not expect this case to trigger, but if it does we may
      // want to do on-stmt to src node and then transfer
      char* buf = chpl_mem_alloc(size, CHPL_RT_MD_COMM_PER_LOC_INFO, 0, 0);
      chpl_comm_get(buf, srcnode, srcaddr, size, commID, ln, fn);
      chpl_comm_put(buf, dstnode, dstaddr, size, commID, ln, fn);
      chpl_mem_free(buf, 0, 0);
    }
  }
}

void chpl_comm_get_unordered(void* addr, c_nodeid_t node, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn) {
  chpl_comm_get(addr, node, raddr, size, commID, ln, fn);
}

void chpl_comm_put_unordered(void* addr, c_nodeid_t node, void* raddr,
                             size_t size, int32_t commID, int ln, int32_t fn) {
  chpl_comm_put(addr, node, raddr, size, commID, ln, fn);
}

void chpl_comm_getput_unordered_task_fence(void) { }

static inline
void  execute_on_common(c_nodeid_t node, c_sublocid_t subloc,
                        chpl_fn_int_t fid,
                        chpl_comm_on_bundle_t *arg, size_t arg_size,
                        chpl_bool fast, chpl_bool blocking) {
  done_t done;
  size_t payload_size = arg_size - sizeof(chpl_comm_on_bundle_t);
  size_t small_msg_size = payload_size + sizeof(small_fork_hdr_t);
  int large = (arg_size > gasnet_AMMaxMedium());
  int small = (small_msg_size < sizeof(special_fork_t) && !large);

  int op;

  chpl_task_ChapelData_t state = *chpl_task_getChapelData();

  if (blocking)
    init_done_obj(&done, 1);

  // Don't consider it fast if it's large, because the
  // handler has to GET the bundle.
  fast = fast && ! large;

  op = 0;
  if (fast) {
    // At this point, a fast implies !large.
    // A fast non-blocking fork is the same as a fast blocking
    // one, except the non-blocking version does not notify completion.
    if (small)      op = FORK_FAST_SMALL;
    else            op = FORK_FAST;
  } else if(blocking) {
    if (small)      op = FORK_SMALL;
    else if (large) op = FORK_LARGE;
    else            op = FORK;
  } else {
    if (small)      op = FORK_NB_SMALL;
    else if (large) op = FORK_NB_LARGE;
    else            op = FORK_NB;
  }

  if (large) {
    payload_size = sizeof(large_fork_t) - sizeof(small_fork_hdr_t);
  }

  if (small || large) {
    special_fork_t tmp;

    small_fork_hdr_t hdr = { .caller = chpl_nodeID,
                             .subloc = subloc,
                             .ack = blocking ? &done : NULL,
                             .state = state,
                             .fid = fid,
                             .payload_size = payload_size };

    if (small) {
      // Copy the argument bundle payload to a smaller message.
      // We'll reconstruct the argument bundle payload on the other end.
      // This copying should have minor performance impact because
      // arg_size is small.
      small_fork_hdr_t *f = &tmp.small;

      // Copy in the header
      *f = hdr;

      // Copy in the payload
      memcpy(f + 1, arg + 1, payload_size);
    
      // Send the AM
      GASNET_Safe(gasnet_AMRequestMedium0(node, op, f, small_msg_size));
    } else {
      // Setup a small message pointing to arg
      // so the other side can GET from it
      large_fork_t *f = &tmp.large;
      chpl_comm_on_bundle_t* use_arg;

      if (blocking)
        use_arg = arg;
      else {
        // Since the argument will necessarily continue to exist
        // unchanged after this call, for an execute_on_nb we need
        // to copy the argument if it is large.
        // An AM back to us will free it.

        use_arg = chpl_mem_allocMany(1, arg_size,
                                     CHPL_RT_MD_COMM_FRK_SND_ARG, 0, 0);
        chpl_memcpy(use_arg, arg, arg_size);
      }

      // Copy in the header
      f->hdr = hdr;

      // Set the pointer to GET with
      f->arg      = use_arg;
      f->arg_size = arg_size;

      // Send the AM
      GASNET_Safe(gasnet_AMRequestMedium0(node, op, f, sizeof(large_fork_t)));
    }
  } else {
    // Neither small nor large

    arg->task_bundle.state = state;
    arg->task_bundle.requestedSubloc = subloc;
    arg->task_bundle.requested_fid = fid;
    arg->comm.caller = chpl_nodeID;
    arg->comm.ack = blocking ? &done : NULL;

    GASNET_Safe(gasnet_AMRequestMedium0(node, op, arg, arg_size));
  }

  if (blocking)
    wait_done_obj(&done, !fast);
}

////GASNET - introduce locale-int size
////GASNET - is caller in chpl_comm_on_bundle_t redundant? active message can determine this.
void  chpl_comm_execute_on(c_nodeid_t node, c_sublocid_t subloc,
                           chpl_fn_int_t fid,
                           chpl_comm_on_bundle_t *arg, size_t arg_size,
                           int ln, int32_t fn) {
  if (chpl_nodeID == node) {
    assert(0);
    chpl_ftable_call(fid, arg);
  } else {
    // Communications callback support
    if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_executeOn, chpl_nodeID, node,
         .iu.executeOn={subloc, fid, arg, arg_size, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
    }

    chpl_comm_diags_verbose_executeOn("", node, ln, fn);
    chpl_comm_diags_incr(execute_on);

    execute_on_common(node, subloc, fid, arg, arg_size,
                     /*fast*/ false, /*blocking*/ true);
  }
}

void  chpl_comm_execute_on_nb(c_nodeid_t node, c_sublocid_t subloc,
                              chpl_fn_int_t fid,
                              chpl_comm_on_bundle_t *arg, size_t arg_size,
                              int ln, int32_t fn) {

  if (chpl_nodeID == node) {
    assert(0); // locale model code should prevent this...
  } else {
    // Communications callback support
    if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_nb)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_executeOn_nb, chpl_nodeID, node,
         .iu.executeOn={subloc, fid, arg, arg_size, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
    }

    chpl_comm_diags_verbose_executeOn("non-blocking", node, ln, fn);
    chpl_comm_diags_incr(execute_on_nb);
  
    execute_on_common(node, subloc, fid, arg, arg_size,
                      /*fast*/ false, /*blocking*/ false);
  }
}

// GASNET - should only be called for "small" functions
void  chpl_comm_execute_on_fast(c_nodeid_t node, c_sublocid_t subloc,
                                chpl_fn_int_t fid,
                                chpl_comm_on_bundle_t *arg, size_t arg_size,
                                int ln, int32_t fn) {
  if (chpl_nodeID == node) {
    assert(0);
    chpl_ftable_call(fid, arg);
  } else {
    // Communications callback support
    if (chpl_comm_have_callbacks(chpl_comm_cb_event_kind_executeOn_fast)) {
      chpl_comm_cb_info_t cb_data = 
        {chpl_comm_cb_event_kind_executeOn_fast, chpl_nodeID, node,
         .iu.executeOn={subloc, fid, arg, arg_size, ln, fn}};
      chpl_comm_do_callbacks (&cb_data);
    }

    chpl_comm_diags_verbose_executeOn("fast", node, ln, fn);
    chpl_comm_diags_incr(execute_on_fast);

    execute_on_common(node, subloc, fid, arg, arg_size,
                      /*fast*/ true, /*blocking*/ true);
  }
}

void chpl_comm_task_end(void) { }
