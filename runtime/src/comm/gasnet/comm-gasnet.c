#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "chplrt.h"
#include "chplcomm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chpltasks.h"
#include "error.h"
#include "gasnet.h"
#include "comm-gasnet.h"

#ifdef GASNET_NEEDS_MAX_SEGSIZE
#define CHPL_COMM_GASNET_SETENV chpl_comm_gasnet_set_max_segsize();
#else
#define CHPL_COMM_GASNET_SETENV
#endif

static chpl_sync_aux_t chpl_comm_diagnostics_sync;
static int chpl_comm_gets = 0;
static int chpl_comm_puts = 0;
static int chpl_comm_forks = 0;
static int chpl_comm_fast_forks = 0;
static int chpl_comm_nb_forks = 0;
static int chpl_comm_no_debug_private = 0;

typedef struct {
  int           caller;
  int*          ack;
  chpl_bool     serial_state; // true if not allowed to spawn new threads
  chpl_fn_int_t fid;
  int           arg_size;
  char          arg[0];       // variable-sized data here
} fork_t;

typedef struct {
  int id;        // private broadcast table entry to update
  int size;      // size of data
  char data[0];  // data
} priv_bcast_t;

typedef struct {
  int id;        // private broadcast table entry to update
  int size;      // size of data
  int offset;    // offset of piece of data
  char data[0];  // data
} priv_bcast_large_t;

void AM_fork_fast(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t *f = buf;

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
#ifdef BLAH
  GASNET_Safe(gasnet_AMRequestMedium0(f->caller,
                                      SIGNAL,
                                      &(f->ack),
                                      sizeof(f->ack)));
#endif
}

static void fork_wrapper(fork_t *f) {
  if (f->arg_size)
    (*chpl_ftable[f->fid])(&f->arg);
  else
    (*chpl_ftable[f->fid])(0);
  GASNET_Safe(gasnet_AMRequestMedium0(f->caller,
                                      SIGNAL,
                                      &(f->ack),
                                      sizeof(f->ack)));
  chpl_free(f, 0, 0);
}

void AM_fork(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t *f = (fork_t*)chpl_malloc(nbytes, sizeof(char), CHPL_RT_MD_AM_FORK_DATA, 0, 0);
  memcpy(f, buf, nbytes);
  CHPL_BEGIN((chpl_fn_p)fork_wrapper, (void*)f, true, f->serial_state, NULL);
}

static void fork_large_wrapper(fork_t* f) {
  void* arg = chpl_malloc(1, f->arg_size, CHPL_RT_MD_AM_FORK_ARG, 0, 0);

  chpl_comm_get(arg, f->caller, *(void**)f->arg, f->arg_size, 0, "fork large");
  (*chpl_ftable[f->fid])(arg);
  GASNET_Safe(gasnet_AMRequestMedium0(f->caller,
                                      SIGNAL,
                                      &(f->ack),
                                      sizeof(f->ack)));
  chpl_free(f, 0, 0);
  chpl_free(arg, 0, 0);
}

////GASNET - can we send as much of user data as possible initially
////           hide data copy by making get non-blocking
////GASNET - can we allocate f big enough so as not to need malloc in wrapper
void AM_fork_large(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t* f = (fork_t*)chpl_malloc(1, nbytes, CHPL_RT_MD_AM_FORK_LARGE_DATA, 0, 0);
  memcpy(f, buf, nbytes);
  CHPL_BEGIN((chpl_fn_p)fork_large_wrapper, (void*)f,
             true, f->serial_state, NULL);
}

static void fork_nb_wrapper(fork_t *f) {
  if (f->arg_size)
    (*chpl_ftable[f->fid])(&f->arg);
  else
    (*chpl_ftable[f->fid])(0);
  chpl_free(f, 0, 0);
}

void AM_fork_nb(gasnet_token_t  token,
                        void           *buf,
                        size_t          nbytes) {
  fork_t *f = (fork_t*)chpl_malloc(nbytes, sizeof(char),
                                   CHPL_RT_MD_AM_NB_FORK_DATA, 0, 0);
  memcpy(f, buf, nbytes);
  CHPL_BEGIN((chpl_fn_p)fork_nb_wrapper, (void*)f,
             true, f->serial_state, NULL);
}

static void fork_nb_large_wrapper(fork_t* f) {
  void* arg = chpl_malloc(1, f->arg_size, CHPL_RT_MD_AM_NB_FORK_ARG, 0, 0);

  chpl_comm_get(arg, f->caller, *(void**)f->arg, f->arg_size, 0, "fork large");
  GASNET_Safe(gasnet_AMRequestMedium0(f->caller,
                                      FREE,
                                      &(f->ack),
                                      sizeof(f->ack)));
  (*chpl_ftable[f->fid])(arg);
  chpl_free(f, 0, 0);
  chpl_free(arg, 0, 0);
}

void AM_fork_nb_large(gasnet_token_t token, void* buf, size_t nbytes) {
  fork_t* f = (fork_t*)chpl_malloc(1, nbytes, CHPL_RT_MD_AM_NB_FORK_LARGE_DATA, 0, 0);
  memcpy(f, buf, nbytes);
  CHPL_BEGIN((chpl_fn_p)fork_nb_large_wrapper, (void*)f,
             true, f->serial_state, NULL);
}

void AM_signal(gasnet_token_t token, void* buf, size_t nbytes) {
  int **done = (int**)buf;
  **done = 1;
}

void AM_priv_bcast(gasnet_token_t token, void* buf, size_t nbytes) {
  priv_bcast_t* pbp = buf;
  memcpy(chpl_private_broadcast_table[pbp->id], pbp->data, pbp->size);
}

void AM_priv_bcast_large(gasnet_token_t token, void* buf, size_t nbytes) {
  priv_bcast_large_t* pblp = buf;
  memcpy((char*)chpl_private_broadcast_table[pblp->id]+pblp->offset, pblp->data, pblp->size);
}

void AM_free(gasnet_token_t token, void* buf, size_t nbytes) {
  chpl_free(*(void**)(*(fork_t**)buf)->arg, 0, 0);
  chpl_free(*(void**)buf, 0, 0);
}

// this is currently unused; it's intended to be used to implement
// exit_any with cleanup on all nodes. 
void AM_exit_any(gasnet_token_t token, void* buf, size_t nbytes) {
  int **status = (int**)buf;
  chpl_internal_error("clean exit_any is not implemented.");
  // here we basically need to call chpl_exit_all, but we need to
  // ensure only one thread calls chpl_exit_all on this locale.
}

static gasnet_seginfo_t* seginfo_table;

//
// Chapel interface starts here
//

int32_t chpl_comm_getMaxThreads(void) {
  return GASNETI_MAX_THREADS-1;
}

int32_t chpl_comm_maxThreadsLimit(void) {
  return GASNETI_MAX_THREADS-1;
}

static int done = 0;

static void polling(void* x) {
  GASNET_BLOCKUNTIL(done);
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

static pthread_t polling_thread;

void chpl_comm_init(int *argc_p, char ***argv_p) {
  int status;

  CHPL_COMM_GASNET_SETENV

  gasnet_init(argc_p, argv_p);
  chpl_localeID = gasnet_mynode();
  chpl_numLocales = gasnet_nodes();
  GASNET_Safe(gasnet_attach(ftable, 
                            sizeof(ftable)/sizeof(gasnet_handlerentry_t),
                            gasnet_getMaxLocalSegmentSize(),
                            0));
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
#undef malloc
  seginfo_table = (gasnet_seginfo_t*)malloc(chpl_numLocales*sizeof(gasnet_seginfo_t));
#define malloc dont_use_malloc_use_chpl_malloc_instead
  GASNET_Safe(gasnet_getSegmentInfo(seginfo_table, chpl_numLocales));
#endif

  gasnet_set_waitmode(GASNET_WAIT_BLOCK);

  //
  // Start polling thread on locale 0.  (On other locales, main enters
  // into a barrier wait, so the polling thread is unnecessary.)
  //
  // This should call a special function in the threading interface
  // but we have not yet initialized chapel threads!
  //
  if (chpl_localeID == 0) {
    status = pthread_create(&polling_thread, NULL, (void*(*)(void*))polling, 0);
    if (status)
      chpl_internal_error("unable to start polling thread for gasnet");
  }
}

//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

void chpl_comm_rollcall(void) {
  CHPL_SYNC_INIT_AUX(&chpl_comm_diagnostics_sync);
  chpl_msg(2, "executing on locale %d of %d locale(s): %s\n", chpl_localeID, 
           chpl_numLocales, chpl_localeName());
}

void chpl_comm_init_shared_heap(void) {
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
  void* heapStart = chpl_numGlobalsOnHeap*sizeof(void*) + (char*)seginfo_table[chpl_localeID].addr;
  size_t heapSize = seginfo_table[chpl_localeID].size - chpl_numGlobalsOnHeap*sizeof(void*);
  chpl_initHeap(heapStart, heapSize);
#else
  chpl_initHeap(NULL, 0);
#endif
}

void chpl_comm_alloc_registry(int numGlobals) {
  chpl_globals_registry = chpl_globals_registry_static;
}

void chpl_comm_broadcast_global_vars(int numGlobals) {
  int i;
  if (chpl_localeID != 0) {
    for (i = 0; i < numGlobals; i++) {
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
      chpl_comm_get(chpl_globals_registry[i], 0,
                     &((void**)seginfo_table[0].addr)[i], sizeof(void*), 0, "");
#else
      chpl_comm_get(chpl_globals_registry[i], 0,
                    chpl_globals_registry[i], sizeof(void*), 0, "");
#endif
    }
  }
}

void chpl_comm_broadcast_private(int id, int size) {
  int locale, offset;
  int payloadSize = size + sizeof(priv_bcast_t);

  if (payloadSize <= gasnet_AMMaxMedium()) {
    priv_bcast_t* pbp = chpl_malloc(1, payloadSize, CHPL_RT_MD_PRIVATE_BROADCAST_DATA, 0, 0);
    memcpy(pbp->data, chpl_private_broadcast_table[id], size);
    pbp->id = id;
    pbp->size = size;
    for (locale = 0; locale < chpl_numLocales; locale++) {
      if (locale != chpl_localeID) {
        GASNET_Safe(gasnet_AMRequestMedium0(locale, PRIV_BCAST, pbp, payloadSize));
      }
    }
    chpl_free(pbp, 0, 0);
  } else {
    int maxpayloadsize = gasnet_AMMaxMedium();
    int maxsize = maxpayloadsize - sizeof(priv_bcast_large_t);
    priv_bcast_large_t* pblp = chpl_malloc(1, maxpayloadsize, CHPL_RT_MD_PRIVATE_BROADCAST_DATA, 0, 0);
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
          GASNET_Safe(gasnet_AMRequestMedium0(locale, PRIV_BCAST_LARGE, pblp, sizeof(priv_bcast_large_t)+thissize));
        }
      }
    }
    chpl_free(pblp, 0, 0);
  }
}

void chpl_comm_barrier(const char *msg) {
  if (chpl_verbose_comm && !chpl_comm_no_debug_private)
    printf("%d: barrier for '%s'\n", chpl_localeID, msg);
  gasnet_barrier_notify(0, GASNET_BARRIERFLAG_ANONYMOUS);
  GASNET_Safe(gasnet_barrier_wait(0, GASNET_BARRIERFLAG_ANONYMOUS));
}

static void chpl_comm_exit_common(int status) {
  int* ack = &done;
  static int loopback = 0;

  if (chpl_localeID == 0) {
    GASNET_Safe(gasnet_AMRequestMedium0(chpl_localeID,
                                        SIGNAL,
                                        &ack,
                                        sizeof(ack)));
    if (loopback) {
      gasnet_exit(2);
    }
    if ( pthread_join(polling_thread, NULL ) ) { // cleanup 
      loopback = 1;
      chpl_internal_error("Polling thread join failed.");
    } 
  }

  chpl_comm_barrier("chpl_comm_exit_common_gasnet_exit"); 
  //exit(); // depending on PAT exit strategy, maybe switch to this
  gasnet_exit(status); // not a collective operation, but one locale will win and all locales will die.
}

void chpl_comm_exit_all(int status) {
  chpl_comm_exit_common(status);
}

void chpl_comm_exit_any_dirty(int status) {
  // kill the polling thread on locale 0, but other than that...
  // clean up nothing; just ask GASNet to exit
  // GASNet will then kill all other locales.
  int* ack = &done;
  static int loopback = 0;

  if (chpl_localeID == 0) {
    GASNET_Safe(gasnet_AMRequestMedium0(chpl_localeID,
                                        SIGNAL,
                                        &ack,
                                        sizeof(ack)));
    if (loopback) {
      gasnet_exit(2);
    }
    if ( pthread_join(polling_thread, NULL ) ) { // cleanup 
      loopback = 1;
      chpl_internal_error("Polling thread join failed.");
    } 
  }

  gasnet_exit(status);
}

// this is currently unused; it's intended to be used to implement
// exit_any with cleanup on all nodes
void chpl_comm_exit_any_clean(int status) {
  int* status_p = &status;
  int locale;

  // notify all other locales that this locale is entering a clean exit_any
  for (locale = 0; locale < chpl_numLocales; locale++) {
    if (locale != chpl_localeID) {
      GASNET_Safe(gasnet_AMRequestMedium0(locale, EXIT_ANY, &status_p, sizeof(status_p)));
    }
  }
    
  // (for code reuse) ask this locale to perform a clean exit_any
  GASNET_Safe(gasnet_AMRequestMedium0(chpl_localeID, EXIT_ANY, &status_p, sizeof(status_p)));
}

void chpl_comm_exit_any(int status) {
  // when chpl_comm_exit_any_clean is finished, consider switching to that.
  chpl_comm_exit_any_dirty(status); 
}

void  chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  if (chpl_localeID == locale) {
    memcpy(raddr, addr, size);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: %s:%d: remote put to %d\n", chpl_localeID, fn, ln, locale);
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      CHPL_SYNC_LOCK(&chpl_comm_diagnostics_sync);
      chpl_comm_puts++;
      CHPL_SYNC_UNLOCK(&chpl_comm_diagnostics_sync);
    }
    gasnet_put(locale, raddr, addr, size); // node, dest, src, size
  }
}

////GASNET - pass trace info to gasnet_get
////GASNET - define GASNET_E_ PUTGET always REMOTE
////GASNET - look at GASNET tools at top of README.tools has atomic counters
void  chpl_comm_get(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  if (chpl_localeID == locale) {
    memcpy(addr, raddr, size);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: %s:%d: remote get from %d\n", chpl_localeID, fn, ln, locale);
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      CHPL_SYNC_LOCK(&chpl_comm_diagnostics_sync);
      chpl_comm_gets++;
      CHPL_SYNC_UNLOCK(&chpl_comm_diagnostics_sync);
    }
    gasnet_get(addr, locale, raddr, size); // dest, node, src, size
  }
}

////GASNET - introduce locale-int size
////GASNET - is caller in fork_t redundant? active message can determine this.
void  chpl_comm_fork(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  fork_t* info;
  int     info_size;
  int     done;
  int     passArg = sizeof(fork_t) + arg_size <= gasnet_AMMaxMedium();

  if (chpl_localeID == locale) {
    (*chpl_ftable[fid])(arg);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: remote task created on %d\n", chpl_localeID, locale);
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      CHPL_SYNC_LOCK(&chpl_comm_diagnostics_sync);
      chpl_comm_forks++;
      CHPL_SYNC_UNLOCK(&chpl_comm_diagnostics_sync);
    }

    if (passArg) {
      info_size = sizeof(fork_t) + arg_size;
    } else {
      info_size = sizeof(fork_t) + sizeof(void*);
    }
    info = (fork_t*)chpl_malloc(1, info_size, CHPL_RT_MD_REMOTE_FORK_DATA, 0, 0);
    info->caller = chpl_localeID;
    info->ack = &done;
    info->serial_state = CHPL_GET_SERIAL();
    info->fid = fid;
    info->arg_size = arg_size;

    done = 0;

    if (passArg) {
      if (arg_size)
        memcpy(&(info->arg), arg, arg_size);
      GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK, info, info_size));
    } else {
      memcpy(&(info->arg), &arg, sizeof(void*));
      GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK_LARGE, info, info_size));
    }
    GASNET_BLOCKUNTIL(1==done);
    chpl_free(info, 0, 0);
  }
}

void  chpl_comm_fork_nb(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  fork_t *info;
  int     info_size;
  int     passArg = chpl_localeID == locale || sizeof(fork_t) + arg_size <= gasnet_AMMaxMedium();

  void* argCopy = NULL;

  if (passArg) {
    info_size = sizeof(fork_t) + arg_size;
  } else {
    info_size = sizeof(fork_t) + sizeof(void*);
  }
  info = (fork_t*)chpl_malloc(info_size, sizeof(char), CHPL_RT_MD_REMOTE_NB_FORK_DATA, 0, 0);
  info->caller = chpl_localeID;
  info->ack = (int*)info; // pass address to free after get in large case
  info->serial_state = CHPL_GET_SERIAL();
  info->fid = fid;
  info->arg_size = arg_size;
  if (passArg) {
    if (arg_size)
      memcpy(&(info->arg), arg, arg_size);
  } else {
    argCopy = chpl_malloc(1, arg_size, CHPL_RT_MD_REMOTE_FORK_ARG, 0, 0);
    memcpy(argCopy, arg, arg_size);
    memcpy(&(info->arg), &argCopy, sizeof(void*));
  }

  if (chpl_localeID == locale) {
    CHPL_BEGIN((chpl_fn_p)fork_nb_wrapper, (void*)info,
               false, info->serial_state, NULL);
  } else {
    if (chpl_verbose_comm && !chpl_comm_no_debug_private)
      printf("%d: remote non-blocking task created on %d\n", chpl_localeID, locale);
    if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
      CHPL_SYNC_LOCK(&chpl_comm_diagnostics_sync);
      chpl_comm_nb_forks++;
      CHPL_SYNC_UNLOCK(&chpl_comm_diagnostics_sync);
    }
    if (passArg) {
      GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK_NB, info, info_size));
      chpl_free(info, 0, 0);
    } else {
      GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK_NB_LARGE, info, info_size));
    }
  }
}

// GASNET - should only be called for "small" functions
void  chpl_comm_fork_fast(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  char infod[gasnet_AMMaxMedium()];
  fork_t* info;
  int     info_size = sizeof(fork_t) + arg_size;
  int     done;
  int     passArg = info_size <= gasnet_AMMaxMedium();

  if (chpl_localeID == locale) {
    (*chpl_ftable[fid])(arg);
  } else {
    if (passArg) {
      if (chpl_verbose_comm && !chpl_comm_no_debug_private)
        printf("%d: remote (no-fork) task created on %d\n",
               chpl_localeID, locale);
      if (chpl_comm_diagnostics && !chpl_comm_no_debug_private) {
        CHPL_SYNC_LOCK(&chpl_comm_diagnostics_sync);
        chpl_comm_fast_forks++;
        CHPL_SYNC_UNLOCK(&chpl_comm_diagnostics_sync);
      }
      info = (fork_t *) &infod;

      info->caller = chpl_localeID;
      info->ack = &done;
      info->serial_state = CHPL_GET_SERIAL();
      info->fid = fid;
      info->arg_size = arg_size;

      done = 0;

      if (arg_size)
        memcpy(&(info->arg), arg, arg_size);
      GASNET_Safe(gasnet_AMRequestMedium0(locale, FORK_FAST, info, info_size));

      // GASNET_BLOCKUNTIL(1==done);
    } else {
      // Call the normal chpl_comm_fork()
      chpl_comm_fork(locale, fid, arg, arg_size);
    }
  }
}

void chpl_startVerboseComm() {
  chpl_verbose_comm = 1;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int));
  chpl_comm_no_debug_private = 0;
}

void chpl_stopVerboseComm() {
  chpl_verbose_comm = 0;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(0 /* &chpl_verbose_comm */, sizeof(int));
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
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int));
  chpl_comm_no_debug_private = 0;
}

void chpl_stopCommDiagnostics() {
  chpl_comm_diagnostics = 0;
  chpl_comm_no_debug_private = 1;
  chpl_comm_broadcast_private(1 /* &chpl_comm_diagnostics */, sizeof(int));
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

int32_t chpl_numCommFastForks(void) {
  return chpl_comm_fast_forks;
}

int32_t chpl_numCommForks(void) {
  return chpl_comm_forks;
}

int32_t chpl_numCommNBForks(void) {
  return chpl_comm_nb_forks;
}


void chpl_comm_gasnet_help_register_global_var(int i, void* addr) {
#if defined(GASNET_SEGMENT_FAST) || defined(GASNET_SEGMENT_LARGE)
  if (chpl_localeID == 0) {
    ((void**)seginfo_table[0].addr)[i] = addr;
  }
#endif
}
