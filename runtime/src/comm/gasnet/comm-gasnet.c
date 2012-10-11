#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gasnet.h"
#include "gasnet_tools.h"
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplsys.h"
#include "chpl-tasks.h"
#include "error.h"

#ifdef GASNET_NEEDS_MAX_SEGSIZE
#define CHPL_COMM_GASNET_SETENV chpl_comm_gasnet_set_max_segsize();
#else
#define CHPL_COMM_GASNET_SETENV
#endif

static chpl_sync_aux_t chpl_comm_diagnostics_sync;
static chpl_commDiagnostics chpl_comm_commDiagnostics;
static int chpl_comm_no_debug_private = 0;
static gasnet_seginfo_t* seginfo_table = NULL;


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

typedef struct {
  int           caller;
  int*          ack;
  chpl_bool     serial_state; // true if not allowed to spawn new threads
  chpl_fn_int_t fid;
  int           arg_size;
  char          arg[0];       // variable-sized data here
} fork_t;

typedef struct {
  int* ack;
  int  id;       // private broadcast table entry to update
  int  size;     // size of data
  char data[0];  // data
} priv_bcast_t;

typedef struct {
  int* ack;
  int  id;       // private broadcast table entry to update
  int  size;     // size of data
  int  offset;   // offset of piece of data
  char data[0];  // data
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
  gasnet_handlerarg_t a0, a1;

  if (chpl_verbose_comm) {
    char mybuf[128];
    sprintf(mybuf, "%d: running (fast) remote task created by %d\n",
            chpl_localeID, f->caller);
    write(2, mybuf, strlen(mybuf));
  }

  if (f->arg_size)
    (*chpl_ftable[f->fid])(&f->arg);
  else
    (*chpl_ftable[f->fid])(0);

  // Signal that the handler has completed
  a0 = (gasnet_handlerarg_t) ((((uint64_t) (intptr_t) f->ack)<<32UL)>>32UL);
  a1 = (gasnet_handlerarg_t) (((uint64_t) (intptr_t) f->ack)>>32UL);
  GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL, a0, a1));
}

static void fork_wrapper(fork_t *f) {
  gasnet_handlerarg_t a0, a1;
  if (f->arg_size)
    (*chpl_ftable[f->fid])(&f->arg);
  else
    (*chpl_ftable[f->fid])(0);
  a0 = (gasnet_handlerarg_t) ((((uint64_t) (intptr_t) f->ack)<<32UL)>>32UL);
  a1 = (gasnet_handlerarg_t) (((uint64_t) (intptr_t) f->ack)>>32UL);
  GASNET_Safe(gasnet_AMRequestShort2(f->caller, SIGNAL, a0, a1));

  chpl_mem_free(f, 0, 0);
}

static void AM_fork(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t *f = (fork_t*)chpl_mem_allocMany(nbytes, sizeof(char), CHPL_RT_MD_COMM_FORK_RECV_INFO, 0, 0);
  memcpy(f, buf, nbytes);
  chpl_task_begin((chpl_fn_p)fork_wrapper, (void*)f, true, f->serial_state, NULL);
}

static void fork_large_wrapper(fork_t* f) {
  gasnet_handlerarg_t a0, a1;
  void* arg = chpl_mem_allocMany(1, f->arg_size, CHPL_RT_MD_COMM_FORK_RECV_LARGE_ARG, 0, 0);

  // A note on strict aliasing:
  // We used to say something like *(void**)f->arg,
  // but that leads to compiler errors about type-punning
  // since it breaks strict aliasing rules. The memcpy approach
  // employed here is one way around the problem, and a
  // more appealing solution would be to use a union.
  void* f_arg;
  memcpy(&f_arg, f->arg, sizeof(void*));

  chpl_comm_get(arg, f->caller, f_arg,
                f->arg_size, -1 /*typeIndex: unused*/, 1, 0, "fork large");
  (*chpl_ftable[f->fid])(arg);
  a0 = (gasnet_handlerarg_t) ((((uint64_t) (intptr_t) f->ack)<<32UL)>>32UL);
  a1 = (gasnet_handlerarg_t) (((uint64_t) (intptr_t) f->ack)>>32UL);
  GASNET_Safe(gasnet_AMRequestShort2(f->caller, SIGNAL, a0, a1));

  chpl_mem_free(f, 0, 0);
  chpl_mem_free(arg, 0, 0);
}

////GASNET - can we send as much of user data as possible initially
////           hide data copy by making get non-blocking
////GASNET - can we allocate f big enough so as not to need malloc in wrapper
static void AM_fork_large(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t* f = (fork_t*)chpl_mem_allocMany(1, nbytes, CHPL_RT_MD_COMM_FORK_RECV_LARGE_INFO, 0, 0);
  memcpy(f, buf, nbytes);
  chpl_task_begin((chpl_fn_p)fork_large_wrapper, (void*)f,
             true, f->serial_state, NULL);
}

static void fork_nb_wrapper(fork_t *f) {
  if (f->arg_size)
    (*chpl_ftable[f->fid])(&f->arg);
  else
    (*chpl_ftable[f->fid])(0);
  chpl_mem_free(f, 0, 0);
}

static void AM_fork_nb(gasnet_token_t  token,
                        void           *buf,
                        size_t          nbytes) {
  fork_t *f = (fork_t*)chpl_mem_allocMany(nbytes, sizeof(char),
                                          CHPL_RT_MD_COMM_FORK_RECV_NB_INFO, 0, 0);
  memcpy(f, buf, nbytes);
  chpl_task_begin((chpl_fn_p)fork_nb_wrapper, (void*)f,
             true, f->serial_state, NULL);
}

static void fork_nb_large_wrapper(fork_t* f) {
  void* arg = chpl_mem_allocMany(1, f->arg_size, CHPL_RT_MD_COMM_FORK_RECV_NB_LARGE_ARG, 0, 0);

  // See "A note on strict aliasing" in fork_large_wrapper
  void* f_arg;
  memcpy(&f_arg, f->arg, sizeof(void*));

  chpl_comm_get(arg, f->caller, f_arg,
                f->arg_size, -1 /*typeIndex: unused*/, 1, 0, "fork large");
  GASNET_Safe(gasnet_AMRequestMedium0(f->caller,
                                      FREE,
                                      &(f->ack),
                                      sizeof(f->ack)));
  (*chpl_ftable[f->fid])(arg);
  chpl_mem_free(f, 0, 0);
  chpl_mem_free(arg, 0, 0);
}

static void AM_fork_nb_large(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t* f = (fork_t*)chpl_mem_allocMany(1, nbytes, CHPL_RT_MD_COMM_FORK_RECV_NB_LARGE_INFO, 0, 0);
  memcpy(f, buf, nbytes);
  chpl_task_begin((chpl_fn_p)fork_nb_large_wrapper, (void*)f,
             true, f->serial_state, NULL);
}

static void AM_signal(gasnet_token_t token, gasnet_handlerarg_t a0, gasnet_handlerarg_t a1) {
  uint64_t done = ((uint64_t) (uint32_t) a0) | (((uint64_t) (uint32_t) a1)<<32UL);
  *((int *) (intptr_t) done) = 1;
}

static void AM_priv_bcast(gasnet_token_t token, void* buf, size_t nbytes) {
  priv_bcast_t* pbp = buf;
  gasnet_handlerarg_t a0, a1;
  memcpy(chpl_private_broadcast_table[pbp->id], pbp->data, pbp->size);

  // Signal that the handler has completed
  a0 = (gasnet_handlerarg_t) ((((uint64_t) (intptr_t) pbp->ack)<<32UL)>>32UL);
  a1 = (gasnet_handlerarg_t) (((uint64_t) (intptr_t) pbp->ack)>>32UL);
  GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL, a0, a1));
}

static void AM_priv_bcast_large(gasnet_token_t token, void* buf, size_t nbytes) {
  priv_bcast_large_t* pblp = buf;
  gasnet_handlerarg_t a0, a1;
  memcpy((char*)chpl_private_broadcast_table[pblp->id]+pblp->offset, pblp->data, pblp->size);

  // Signal that the handler has completed
  a0 = (gasnet_handlerarg_t) ((((uint64_t) (intptr_t) pblp->ack)<<32UL)>>32UL);
  a1 = (gasnet_handlerarg_t) (((uint64_t) (intptr_t) pblp->ack)>>32UL);
  GASNET_Safe(gasnet_AMReplyShort2(token, SIGNAL, a0, a1));
}

static void AM_free(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t* f;
  void* f_arg;
  
  // See "A note on strict aliasing" in fork_large_wrapper
  memcpy(&f, buf, sizeof(fork_t*));
  memcpy(&f_arg, f->arg, sizeof(void*));

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
  memcpy(seginfo_table, buf, nbytes);
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

int32_t chpl_comm_getMaxThreads(void) {
  return GASNETI_MAX_THREADS-1;
}

static volatile int alldone = 0;
static volatile int pollingRunning = 0;

static void polling(void* x) {
  pollingRunning = 1;
  GASNET_BLOCKUNTIL(alldone);
  pollingRunning = 0;
}

#ifdef GASNET_NEEDS_MAX_SEGSIZE
static char segsizeval[80];

static void chpl_comm_gasnet_set_max_segsize() {
  FILE* file = NULL;
  int memtotal;

  if (getenv("GASNET_MAX_SEGSIZE")) {
    return;
  }

  file = fopen( "/proc/meminfo", "r" );
  if (file == NULL) {
    return;
  }
  /* The first line of /proc/meminfo looks something like:
   * MemTotal:      1027296 kB
   */
  if (fscanf(file, "MemTotal: %d kB", &memtotal) != 1) {
    return;
  }

  /* Use 90% of the /proc/meminfo as the maximum segment size,
     heuristically */
  memtotal *= 0.9;

  snprintf(segsizeval, 80, "%dKB", memtotal);
  setenv( "GASNET_MAX_SEGSIZE", segsizeval, 0 );
}
#endif

void chpl_comm_init(int *argc_p, char ***argv_p) {
//  int status; // Some compilers complain about unused variable 'status'.

  CHPL_COMM_GASNET_SETENV

  assert(sizeof(gasnet_handlerarg_t)==sizeof(uint32_t));

  gasnet_init(argc_p, argv_p);
  chpl_localeID = gasnet_mynode();
  chpl_numLocales = gasnet_nodes();
  GASNET_Safe(gasnet_attach(ftable, 
                            sizeof(ftable)/sizeof(gasnet_handlerentry_t),
                            gasnet_getMaxLocalSegmentSize(),
                            0));
#undef malloc
  seginfo_table = (gasnet_seginfo_t*)malloc(chpl_numLocales*sizeof(gasnet_seginfo_t));
  //
  // The following call has no real effect on the .addr and .size
  // fields for GASNET_SEGMENT_EVERYTHING, but is recommended to be
  // used anyway (see third-party/gasnet/GASNet-version/tests/test.h)
  // in order to ensure that the seginfo_table array is initialized
  // appropriately on all locales.
  //
  GASNET_Safe(gasnet_getSegmentInfo(seginfo_table, chpl_numLocales));
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
  if (chpl_localeID == 0) {
    int i;
    //
    // Only locale #0 really needs the seginfo_table to store anything since it owns all
    // of the global variable locations; everyone else will just peek at its copy.  So
    // locale 0 sets up its segment to an appropriate size:
    //
    int global_table_size = chpl_numGlobalsOnHeap * sizeof(void*) + GASNETT_PAGESIZE;
    void* global_table = malloc(global_table_size);
    seginfo_table[0].addr = ((void *)(((uint8_t*)global_table) + 
                                      (((((uintptr_t)global_table)%GASNETT_PAGESIZE) == 0)? 0 : 
                                       (GASNETT_PAGESIZE-(((uintptr_t)global_table)%GASNETT_PAGESIZE)))));
    seginfo_table[0].size = global_table_size;
    //
    // ...and then zeroes out everyone else's
    //
    for (i=1; i<chpl_numLocales; i++) {
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
  if (chpl_localeID == 0) {
    int i;
    for (i=0; i < chpl_numLocales; i++) {
      GASNET_Safe(gasnet_AMRequestMedium0(i, BCAST_SEGINFO, seginfo_table, 
                                          chpl_numLocales*sizeof(gasnet_seginfo_t)));
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
  return (chpl_localeID == 0);
}

//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

void chpl_comm_post_task_init(void) {
  //
  // Start polling task on locale 0.  (On other locales, main enters
  // into a barrier wait, so the polling task is unnecessary.)
  //
  if (chpl_localeID == 0) {
    int status = chpl_task_createCommTask(polling, NULL);
    if (status) {
      alldone = 1;
      chpl_internal_error("unable to start polling task for gasnet");
    }
  }

  // clear diags
  memset(&chpl_comm_commDiagnostics, 0, sizeof(chpl_commDiagnostics));
}

void chpl_comm_rollcall(void) {
  chpl_sync_initAux(&chpl_comm_diagnostics_sync);
  chpl_msg(2, "executing on locale %d of %d locale(s): %s\n", chpl_localeID, 
           chpl_numLocales, chpl_localeName());
}

void chpl_comm_desired_shared_heap(void** start_p, size_t* size_p) {
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
  *start_p = chpl_numGlobalsOnHeap * sizeof(void*) 
             + (char*)seginfo_table[chpl_localeID].addr;
  *size_p  = seginfo_table[chpl_localeID].size
             - chpl_numGlobalsOnHeap * sizeof(void*);
#else /* GASNET_SEGMENT_EVERYTHING */
  *start_p = NULL;
  *size_p  = 0;
#endif
}

void chpl_comm_alloc_registry(int numGlobals) {
  chpl_globals_registry = chpl_globals_registry_static;
}

void chpl_comm_broadcast_global_vars(int numGlobals) {
  int i;
  if (chpl_localeID != 0) {
    for (i = 0; i < numGlobals; i++) {
      chpl_comm_get(chpl_globals_registry[i], 0,
                    &((void**)seginfo_table[0].addr)[i],
                    sizeof(void*), -1 /*typeIndex: unused*/, 1, 0, "");
    }
  }
}

void chpl_comm_broadcast_private(int id, int32_t size, int32_t tid) {
  int  locale, offset;
  int  payloadSize = size + sizeof(priv_bcast_t);
  int* done; /* These are not declared volatile because we capture &done[i]
                (and pass it to a GASNet function), so the compiler should
                not optimize out any accesses to it. */

  done = (int*) chpl_mem_allocMany(chpl_numLocales, sizeof(*done),
                                   CHPL_RT_MD_COMM_FORK_DONE_FLAG, 0, 0);
  if (payloadSize <= gasnet_AMMaxMedium()) {
    priv_bcast_t* pbp = chpl_mem_allocMany(1, payloadSize, CHPL_RT_MD_COMM_PRIVATE_BROADCAST_DATA, 0, 0);
    memcpy(pbp->data, chpl_private_broadcast_table[id], size);
    pbp->id = id;
    pbp->size = size;
    for (locale = 0; locale < chpl_numLocales; locale++) {
      if (locale != chpl_localeID) {
        pbp->ack = &done[locale];
        GASNET_Safe(gasnet_AMRequestMedium0(locale, PRIV_BCAST, pbp, payloadSize));
      }
    }
    chpl_mem_free(pbp, 0, 0);
  } else {
    int maxpayloadsize = gasnet_AMMaxMedium();
    int maxsize = maxpayloadsize - sizeof(priv_bcast_large_t);
    priv_bcast_large_t* pblp = chpl_mem_allocMany(1, maxpayloadsize, CHPL_RT_MD_COMM_PRIVATE_BROADCAST_DATA, 0, 0);
    pblp->id = id;
    for (offset = 0; offset < size; offset += maxsize) {
      int thissize = size - offset;
      if (thissize > maxsize)
        thissize = maxsize;
      pblp->offset = offset;
      pblp->size = thissize;
      memcpy(pblp->data, (char*)chpl_private_broadcast_table[id]+offset, thissize);
      for (locale = 0; locale < chpl_numLocales; locale++) {
        if (locale != chpl_localeID) {
          pblp->ack = &done[locale];
          GASNET_Safe(gasnet_AMRequestMedium0(locale, PRIV_BCAST_LARGE, pblp, sizeof(priv_bcast_large_t)+thissize));
        }
      }
    }
    chpl_mem_free(pblp, 0, 0);
  }
  // wait for the handlers to complete
  for (locale = 0; locale < chpl_numLocales; locale++) {
    if (locale != chpl_localeID)
      GASNET_BLOCKUNTIL(done[locale]==1);
  }
  chpl_mem_free(done, 0, 0);
}

void chpl_comm_barrier(const char *msg) {
  if (chpl_verbose_comm && !chpl_comm_no_debug_private)
    printf("%d: barrier for '%s'\n", chpl_localeID, msg);
  gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
  GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS));
}

void chpl_comm_pre_task_exit(int all) {
  if (all) {
    chpl_comm_barrier("chpl_comm_pre_task_exit");

    if (chpl_localeID == 0) {
      //
      // Only locale 0 actually runs a polling task.  Tell that task to
      // halt, and then wait for it to do so.
      //
      alldone = 1;
      while (pollingRunning) {}
    }
  }
}

static void exit_common(int status) {
  int* ack = (int*)&alldone;
  static int loopback = 0;

  if (chpl_localeID == 0) {
    gasnet_handlerarg_t a0;
    gasnet_handlerarg_t a1;
    a0 = (gasnet_handlerarg_t) ((((uint64_t) (intptr_t) ack)<<32UL)>>32UL);
    a1 = (gasnet_handlerarg_t) (((uint64_t) (intptr_t) ack)>>32UL);
    GASNET_Safe(gasnet_AMRequestShort2(chpl_localeID, SIGNAL, a0, a1));

    if (loopback) {
      gasnet_exit(2);
    }
  }

  chpl_comm_barrier("exit_common_gasnet_exit"); 
  //exit(); // depending on PAT exit strategy, maybe switch to this
  gasnet_exit(status); // not a collective operation, but one locale will win and all locales will die.
}

static void exit_any_dirty(int status) {
  // kill the polling task on locale 0, but other than that...
  // clean up nothing; just ask GASNet to exit
  // GASNet will then kill all other locales.
  int* ack = (int*)&alldone;
  static int loopback = 0;

  if (chpl_localeID == 0) {
    gasnet_handlerarg_t a0;
    gasnet_handlerarg_t a1;
    a0 = (gasnet_handlerarg_t) ((((uint64_t) (intptr_t) ack)<<32UL)>>32UL);
    a1 = (gasnet_handlerarg_t) (((uint64_t) (intptr_t) ack)>>32UL);
    GASNET_Safe(gasnet_AMRequestShort2(chpl_localeID, SIGNAL, a0, a1));
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
  for (node = 0; node < chpl_numLocales; node++) {
    if (node != chpl_localeID) {
      GASNET_Safe(gasnet_AMRequestMedium0(node, EXIT_ANY, &status_p, sizeof(status_p)));
    }
  }
    
  // (for code reuse) ask this node to perform a clean exit_any
  GASNET_Safe(gasnet_AMRequestMedium0(chpl_localeID, EXIT_ANY, &status_p, sizeof(status_p)));
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

void  chpl_comm_put(void* addr, int32_t node, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn) {
  const int size = elemSize*len;
  if (chpl_localeID == node) {
    memcpy(raddr, addr, size);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: %s:%d: remote put to %d\n", chpl_localeID, fn, ln, node);
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
void  chpl_comm_get(void* addr, int32_t node, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn) {
  const int size = elemSize*len;
  if (chpl_localeID == node) {
    memcpy(addr, raddr, size);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: %s:%d: remote get from %d\n", chpl_localeID, fn, ln, node);
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      chpl_sync_lock(&chpl_comm_diagnostics_sync);
      chpl_comm_commDiagnostics.get++;
      chpl_sync_unlock(&chpl_comm_diagnostics_sync);
    }
    gasnet_get(addr, node, raddr, size); // dest, node, src, size
  }
}

//
// Optional non-blocking get interface
//
void chpl_comm_get_nb(void* addr, int32_t node, void* raddr,
                      int32_t elemSize, int32_t typeIndex, int32_t len,
                      chpl_comm_get_nb_token_t *token, int ln, chpl_string fn) {
  const int size = elemSize*len;
  if (chpl_localeID == node) {
    memcpy(addr, raddr, size);
  } else {
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      chpl_sync_lock(&chpl_comm_diagnostics_sync);
      chpl_comm_commDiagnostics.get_nb++;
      chpl_sync_unlock(&chpl_comm_diagnostics_sync);
    }
    *token = gasnet_get_nb(addr, node, raddr, size); // dest, node, src, size
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: %s:%d: remote non-blocking get from %d (%p)\n",
             chpl_localeID, fn, ln, node, *token);
  }
}

int chpl_comm_test_get_nb(chpl_comm_get_nb_token_t token,
                          int ln, chpl_string fn) {
  if (chpl_verbose_comm && !chpl_comm_no_debug_private)
    printf("%d: %s:%d: test non-blocking get (%p)\n",
           chpl_localeID, fn, ln, token);
  if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
    chpl_sync_lock(&chpl_comm_diagnostics_sync);
    chpl_comm_commDiagnostics.get_nb_test++;
    chpl_sync_unlock(&chpl_comm_diagnostics_sync);
  }
  return gasnet_try_syncnb(token)==GASNET_OK;
}

void chpl_comm_wait_get_nb(chpl_comm_get_nb_token_t token,
                           int ln, chpl_string fn) {
  if (chpl_verbose_comm && !chpl_comm_no_debug_private)
    printf("%d: %s:%d: wait non-blocking get (%p)\n",
           chpl_localeID, fn, ln, token);
  if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
    chpl_sync_lock(&chpl_comm_diagnostics_sync);
    chpl_comm_commDiagnostics.get_nb_wait++;
    chpl_sync_unlock(&chpl_comm_diagnostics_sync);
  }
  gasnet_wait_syncnb(token);
}


////GASNET - introduce locale-int size
////GASNET - is caller in fork_t redundant? active message can determine this.
void  chpl_comm_fork(c_nodeid_t node, chpl_fn_int_t fid, void *arg,
                     int32_t arg_size, int32_t arg_tid) {
  fork_t* info;
  int     info_size;
  int     done; /* This is not declared volatile because we capture &done
                   (and pass it to a GASNet function), so the compiler
                   should not optimize out any accesses to it. */
  int     passArg = sizeof(fork_t) + arg_size <= gasnet_AMMaxMedium();

  if (chpl_localeID == node) {
    (*chpl_ftable[fid])(arg);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: remote task created on %d\n", chpl_localeID, node);
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
    info = (fork_t*)chpl_mem_allocMany(1, info_size, CHPL_RT_MD_COMM_FORK_SEND_INFO, 0, 0);
    info->caller = chpl_localeID;
    info->ack = &done;
    info->serial_state = chpl_task_getSerial();
    info->fid = fid;
    info->arg_size = arg_size;

    done = 0;

    if (passArg) {
      if (arg_size)
        memcpy(&(info->arg), arg, arg_size);
      GASNET_Safe(gasnet_AMRequestMedium0(node, FORK, info, info_size));
    } else {
      memcpy(&(info->arg), &arg, sizeof(void*));
      GASNET_Safe(gasnet_AMRequestMedium0(node, FORK_LARGE, info, info_size));
    }
#ifndef CHPL_COMM_YIELD_TASK_WHILE_POLLING
    GASNET_BLOCKUNTIL(done==1);
#else
    while (done != 1) {
      (void) gasnet_AMPoll();
      chpl_task_yield();
    }
#endif
    chpl_mem_free(info, 0, 0);
  }
}

void  chpl_comm_fork_nb(c_nodeid_t node, chpl_fn_int_t fid, void *arg,
                        int32_t arg_size, int32_t arg_tid) {
  fork_t *info;
  int     info_size;
  int     passArg = chpl_localeID == node || sizeof(fork_t) + arg_size <= gasnet_AMMaxMedium();

  void* argCopy = NULL;

  if (passArg) {
    info_size = sizeof(fork_t) + arg_size;
  } else {
    info_size = sizeof(fork_t) + sizeof(void*);
  }
  info = (fork_t*)chpl_mem_allocMany(info_size, sizeof(char), CHPL_RT_MD_COMM_FORK_SEND_NB_INFO, 0, 0);
  info->caller = chpl_localeID;
  info->ack = (int*)info; // pass address to free after get in large case
  info->serial_state = chpl_task_getSerial();
  info->fid = fid;
  info->arg_size = arg_size;
  if (passArg) {
    if (arg_size)
      memcpy(&(info->arg), arg, arg_size);
  } else {
      argCopy = chpl_mem_allocMany(1, arg_size, CHPL_RT_MD_COMM_FORK_SEND_NB_LARGE_ARG, 0, 0);
    memcpy(argCopy, arg, arg_size);
    memcpy(&(info->arg), &argCopy, sizeof(void*));
  }

  if (chpl_localeID == node) {
    chpl_task_begin((chpl_fn_p)fork_nb_wrapper, (void*)info,
               false, info->serial_state, NULL);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: remote non-blocking task created on %d\n", chpl_localeID, node);
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
void  chpl_comm_fork_fast(c_nodeid_t node, chpl_fn_int_t fid, void *arg,
                          int32_t arg_size, int32_t arg_tid) {
  char infod[gasnet_AMMaxMedium()];
  fork_t* info;
  int     info_size = sizeof(fork_t) + arg_size;
  int     done; // See chpl_comm_fork() for why this is not volatile
  int     passArg = info_size <= gasnet_AMMaxMedium();

  if (chpl_localeID == node) {
    (*chpl_ftable[fid])(arg);
  } else {
    if (passArg) {
      if (chpl_verbose_comm && !chpl_comm_no_debug_private)
        printf("%d: remote (no-fork) task created on %d\n",
               chpl_localeID, node);
      if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
        chpl_sync_lock(&chpl_comm_diagnostics_sync);
        chpl_comm_commDiagnostics.fork_fast++;
        chpl_sync_unlock(&chpl_comm_diagnostics_sync);
      }
      info = (fork_t *) &infod;

      info->caller = chpl_localeID;
      info->ack = &done;
      info->serial_state = chpl_task_getSerial();
      info->fid = fid;
      info->arg_size = arg_size;

      done = 0;

      if (arg_size)
        memcpy(&(info->arg), arg, arg_size);
      GASNET_Safe(gasnet_AMRequestMedium0(node, FORK_FAST, info, info_size));
      // NOTE: We still have to wait for the handler to complete
#ifndef CHPL_COMM_YIELD_TASK_WHILE_POLLING
      GASNET_BLOCKUNTIL(done==1);
#else
      while (done != 1) {
        (void) gasnet_AMPoll();
        chpl_task_yield();
      }
#endif
    } else {
      // Call the normal chpl_comm_fork()
      chpl_comm_fork(node, fid, arg, arg_size, arg_tid);
    }
  }
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
  memcpy(cd, &chpl_comm_commDiagnostics, sizeof(chpl_commDiagnostics));
  chpl_sync_unlock(&chpl_comm_diagnostics_sync);
}

uint64_t chpl_numCommGets(void) {
  return chpl_comm_commDiagnostics.get;
}

uint64_t chpl_numCommNBGets(void) {
  return chpl_comm_commDiagnostics.get_nb;
}

uint64_t chpl_numCommTestNBGets(void) {
  return chpl_comm_commDiagnostics.get_nb_test;
}

uint64_t chpl_numCommWaitNBGets(void) {
  return chpl_comm_commDiagnostics.get_nb_wait;
}

uint64_t chpl_numCommPuts(void) {
  return chpl_comm_commDiagnostics.put;
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


void chpl_comm_gasnet_help_register_global_var(int i, void* addr) {
  if (chpl_localeID == 0) {
    ((void**)seginfo_table[0].addr)[i] = addr;
  }
}
