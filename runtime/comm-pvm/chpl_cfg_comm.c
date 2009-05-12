#include "chplrt.h"
#include "chplcomm.h"
#include "chplmem.h"
#include "chplsys.h"
#include "chplthreads.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pvm3.h"

#define CHPL_DIST_DEBUG 0

#if CHPL_DIST_DEBUG
#define DEBUG_MSG_LENGTH 256
#define PRINTF(_s) {                                                       \
  printf("%d/%d:%d/%d:%s\n", chpl_localeID, chpl_numLocales,               \
                             (int)getpid(), (int)pthread_self(), _s);      \
  fflush(stdout);                                                          \
}
#else
#define PRINTF(_s)
#endif

int lock_num = 0;
#define PVM_SAFE(call, who, in) {                                          \
  int retcode;                                                             \
  chpl_mutex_lock(&pvm_lock);                                              \
  retcode = call;                                                          \
  chpl_mutex_unlock(&pvm_lock);                                            \
  lock_num++;                                                              \
  if (retcode < 0) {                                                       \
    char msg[256];                                                         \
    sprintf(msg, "\n\n%d/%d:%d PVM call failed.\n\n", chpl_localeID, chpl_numLocales, (int)pthread_self());                                                \
    chpl_error(msg, __LINE__, __FILE__);                                   \
  }                                                                        \
}

#define PVM_PACK_SAFE(call, who, in) {                                     \
  int retcode;                                                             \
  chpl_mutex_lock(&pvm_lock);                                              \
  retcode = call;                                                          \
  lock_num++;                                                              \
  if (retcode < 0) {                                                       \
    char msg[256];                                                         \
    sprintf(msg, "\n\n%d/%d:%d PVM call failed.\n\n", chpl_localeID, chpl_numLocales, (int)pthread_self());                                                \
    chpl_error(msg, __LINE__, __FILE__);                                   \
  }                                                                        \
}

#define PVM_UNPACK_SAFE(call, who, in) {                                   \
  int retcode;                                                             \
  retcode = call;                                                          \
  chpl_mutex_unlock(&pvm_lock);                                            \
  if (retcode < 0) {                                                       \
    char msg[256];                                                         \
    sprintf(msg, "\n\n%d/%d:%d PVM call failed.\n\n", chpl_localeID, chpl_numLocales, (int)pthread_self());                                                \
    chpl_error(msg, __LINE__, __FILE__);                                   \
  }                                                                        \
}

#define PVM_NO_LOCK_SAFE(call, who, in) {                                  \
  int retcode;                                                             \
  retcode = call;                                                          \
  if (retcode < 0) {                                                       \
    char msg[256];                                                         \
    sprintf(msg, "\n\n%d/%d:%d PVM call failed.\n\n", chpl_localeID, chpl_numLocales, (int)pthread_self());                                                \
    chpl_error(msg, __LINE__, __FILE__);                                   \
  }                                                                        \
}

#define TAGMASK 4194303
#define BCASTTAG 4194299

chpl_mutex_t pvm_lock;
chpl_mutex_t termination_lock;

static int chpl_comm_diagnostics = 0;           // set via startCommDiagnostics
static int chpl_comm_gets = 0;
static int chpl_comm_puts = 0;
static int chpl_comm_forks = 0;
static int chpl_comm_nb_forks = 0;
static int chpl_verbose_comm = 0;               // set via startVerboseComm
static int chpl_comm_no_debug_private = 0;

int tids[64]; // tid list for all nodes
int instance;

int okay_to_barrier = 1;

//
// Chapel interface starts here
//


int32_t chpl_comm_getMaxThreads(void) {
  return 0;
}

int32_t chpl_comm_maxThreadsLimit(void) {
  return 0;
}

typedef enum {
  ChplCommPut,
  ChplCommGet,
  ChplCommFork,
  ChplCommForkNB,
  ChplCommFinish
} ChplCommMsgType;

//
// Sent over network for every chpl_comm_get, _put, _fork, or _fork_nb.
// On normal Linux64 configurations, this is 16 bytes.
//
typedef struct __chpl_message_info {
  int msg_type : 4;     // One of the ChplCommMsgType enum values
  int replyTag : 27;    // Use this tag for the reply message
  int size;             // Size of data or function arguments
  union {
    void* data;         // Data location
    chpl_fn_int_t fid;  // Function ID
  } u;
} _chpl_message_info;

//
// A function and argument for a remote procedure call. When done executing
// the function, send and empty message to joinLocale with replyTag.
//
typedef struct __chpl_RPC_arg {
  chpl_fn_int_t fid;
  void* arg;
  int replyTag;
  int joinLocale;
} _chpl_RPC_arg;

static void chpl_RPC(_chpl_RPC_arg* arg);
static int chpl_pvm_recv(int tid, int msgtag, void* buf, int sz);
static void chpl_pvm_send(int tid, int msgtag, void* buf, int sz);
static int makeTag(int threadID, int localeID);

// A simple hash to combine the threadID and localeID into one
// value that is small enough to use as a PVM message tag
static int makeTag(int threadID, int localeID) {
  const double A = .61803398874989;
  double s;
  int tag;
  int absTid = threadID < 0 ? -threadID : threadID;

  s = A * absTid;
  s -= (int)s;
  tag = (int)(4096 * s);

  tag = tag << 8;
  tag += localeID;
  tag = tag & TAGMASK;
  return tag;
}

static void chpl_RPC(_chpl_RPC_arg* arg) {
  PRINTF("Entering forked task");
  (*chpl_ftable[arg->fid])(arg->arg);
  PRINTF("Did task");
  chpl_pvm_send(arg->joinLocale, arg->replyTag, NULL, 0);
  if (arg->arg != NULL)
    chpl_free(arg->arg, false, __LINE__, __FILE__);
  chpl_free(arg, false, __LINE__, __FILE__);
}

// Return the source of the message received.
static int chpl_pvm_recv(int tid, int msgtag, void* buf, int sz) {
  int bufid;
  int bytes;
  int pvmtype;
  int source;
  int repTag;
  int fnid;
  int packagesize;

#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
#endif
  if ((tid <= chpl_numLocales) && (tid > -1)) tid = tids[tid];   // lines up Chapel locales and PVM index
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_pvm_recv(%p, from=%d, tag=%d, sz=%d)", buf, tid, msgtag, sz);
  PRINTF(debugMsg);
#endif
  bufid = 0;
  //  fprintf(stderr, "Waiting on tid=%d, msgtag=%d\n", tid, msgtag); // Dele
  while (bufid == 0) {
    PVM_PACK_SAFE(bufid = pvm_nrecv(tid, msgtag), "pvm_nrecv", "chpl_pvm_recv");
    if (bufid == 0) {
      chpl_mutex_unlock(&pvm_lock);
    }
  }
  PVM_NO_LOCK_SAFE(pvm_bufinfo(bufid, &bytes, &pvmtype, &source), "pvm_bufinfo", "chpl_pvm_recv");
  //  fprintf(stderr, "bufinfo - bufid=%d, bytes=%d, tag=%d, source=%d\n", bufid, bytes, pvmtype, source); // Dele

  // Either getting "metadata" case, which is the chpl_message_info
  // containing the information about what's to come, or getting
  // actual data.
  if (msgtag == TAGMASK+1) {
    //    fprintf(stderr, "Receiving _chpl_message_info\n"); // Dele
    PVM_NO_LOCK_SAFE(pvm_upkint(&pvmtype, 1, 1), "pvm_upkint", "chpl_pvm_recv");
    //    fprintf(stderr, "Received pvmtype=%d\n", pvmtype); // Dele
    PVM_NO_LOCK_SAFE(pvm_upkint(&repTag, 1, 1), "pvm_upkint", "chpl_pvm_recv");
    //    fprintf(stderr, "Received repTag=%d\n", repTag); // Dele
    PVM_NO_LOCK_SAFE(pvm_upkint(&bytes, 1, 1), "pvm_upkint", "chpl_pvm_recv");
    //    fprintf(stderr, "Received bytes=%d\n", bytes); // Dele
    // Metadata case either contains an address for the data or a
    // function ID (or, if ChplCommFinish, nothing).
    if ((pvmtype == ChplCommPut) || (pvmtype == ChplCommGet)) {
      PVM_NO_LOCK_SAFE(pvm_upkint(&packagesize, 1, 1), "pvm_upkint", "chpl_pvm_recv");
      PVM_UNPACK_SAFE(pvm_upkbyte((void *)&(((_chpl_message_info *)buf)->u.data), packagesize, 1), "pvm_upkbyte", "chpl_pvm_recv");
    } else if (pvmtype == ChplCommFinish) {
      // Do nothing. Nothing in the union.
      chpl_mutex_unlock(&pvm_lock);
    } else {
      PVM_UNPACK_SAFE(pvm_upkint(&fnid, 1, 1), "pvm_upkint", "chpl_pvm_recv");
      ((_chpl_message_info *)buf)->u.fid = (chpl_fn_int_t) fnid;
      //      fprintf(stderr, "Received fnid=%d\n", fnid); // Dele
    }
    ((_chpl_message_info *)buf)->msg_type = (int) pvmtype;
    ((_chpl_message_info *)buf)->replyTag = (int) repTag;
    ((_chpl_message_info *)buf)->size = (int) bytes;

  // Getting actual data
  } else {
    PVM_UNPACK_SAFE(pvm_upkbyte(((void *)buf), sz, 1), "pvm_upkbyte", "chpl_pvm_recv");
    //    fprintf(stderr, "Receiving something via upkbyte of size %d.\n", sz); // Dele
  }
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_pvm_recv(%p, from=%d, tag=%d, sz=%d) done", buf, tid, msgtag, sz);
  PRINTF(debugMsg);
#endif
  return source;
}

static void chpl_pvm_send(int tid, int msgtag, void* buf, int sz) {
  int msgtype;
  int repTag;
  int datasize;
  int fnid;
  int packagesize;

#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
#endif
  if ((tid <= chpl_numLocales) && (tid > -1)) tid = tids[tid];  // lines up Chapel locales and PVM index
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_pvm_send(%p, to=%d, tag=%d, sz=%d)", buf, tid, msgtag, sz);
  PRINTF(debugMsg);
#endif
  PVM_PACK_SAFE(pvm_initsend(PvmDataDefault), "pvm_initsend", "chpl_pvm_send");

  // Either sending "metadata" case, which is the chpl_message_info
  // containing the information about what's to come, or sending
  // actual data.
  if (msgtag == TAGMASK+1) {
    //    fprintf(stderr, "Sending _chpl_message_info with type=%d, tag=%d, size=%d\n", ((_chpl_message_info *)buf)->msg_type, ((_chpl_message_info *)buf)->replyTag, ((_chpl_message_info *)buf)->size); // Dele
    msgtype = ((_chpl_message_info *)buf)->msg_type;
    repTag = ((_chpl_message_info *)buf)->replyTag;
    datasize = ((_chpl_message_info *)buf)->size;
    PVM_NO_LOCK_SAFE(pvm_pkint(&msgtype, 1, 1), "pvm_pkint", "chpl_pvm_send");
    //    fprintf(stderr, "Sent msgtype=%d\n", msgtype); // Dele
    PVM_NO_LOCK_SAFE(pvm_pkint(&repTag, 1, 1), "pvm_pkint", "chpl_pvm_send");
    //    fprintf(stderr, "Sent repTag=%d\n", repTag); // Dele
    PVM_NO_LOCK_SAFE(pvm_pkint(&datasize, 1, 1), "pvm_pkint", "chpl_pvm_send");
    //    fprintf(stderr, "Sent datasize=%d\n", datasize); // Dele
    // Metadata case either contains an address for the data or a
    // function ID (or, if ChplCommFinish, nothing).
    if ((msgtype == ChplCommPut) || (msgtype == ChplCommGet)) {
      packagesize = sizeof(void *);
      PVM_NO_LOCK_SAFE(pvm_pkint(&packagesize, 1, 1), "pvm_pkint", "chpl_pvm_send");
      PVM_NO_LOCK_SAFE(pvm_pkbyte((void *)&(((_chpl_message_info *)buf)->u.data), packagesize, 1), "pvm_pkbyte", "chpl_pvm_send");
      //      fprintf(stderr, "Sent address data of size %d.\n", packagesize); // Dele
    } else if (msgtype == ChplCommFinish) {
      // Do nothing. Nothing in the union.
      // Unlock is done in the PVM_UNPACK_SAFE on the actual pvm_send.
    } else {
      fnid = ((_chpl_message_info *)buf)->u.fid;
      PVM_NO_LOCK_SAFE(pvm_pkint(&fnid, 1, 1), "pvm_pkint", "chpl_pvm_send");
      //      fprintf(stderr, "Sent fnid=%d\n", fnid); // Dele
    }
  // Sending actual data
  } else {
    //    fprintf(stderr, "Sending something via pkbyte of size %d.\n", sz); // Dele
    PVM_NO_LOCK_SAFE(pvm_pkbyte(((void *)buf), sz, 1), "pvm_pkbyte", "chpl_pvm_send");
    //    fprintf(stderr, "Sent something.\n"); // Dele
  }
  PVM_UNPACK_SAFE(pvm_send(tid, msgtag), "pvm_pksend", "chpl_pvm_send");

#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_pvm_send(%p, to=%d, tag=%d, sz=%d) done", buf, tid, msgtag, sz);
  PRINTF(debugMsg);
#endif
  return;
}

static void polling(void* x) {
  int finished;
  int source;
  _chpl_message_info msg_info;
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
#endif

  PRINTF("Starting PVM polling thread");
  chpl_mutex_lock(&termination_lock);
  finished = 0;

  while (!finished) {
    PRINTF("Poller Receiving");
    // Poller thread waits for input from anyone
    // First signal is metadata case (TAGMASK+1). Contains information
    // about the forthcoming data (or how the node should respond).
    source = chpl_pvm_recv(-1, TAGMASK+1, &msg_info, sizeof(_chpl_message_info));
    if (msg_info.msg_type == 2) {
    }
    switch (msg_info.msg_type) {
      // ChplCommPut tells node to store forthcoming data into a location.
    case ChplCommPut: {
#if CHPL_DIST_DEBUG
      sprintf(debugMsg, "Fulfilling ChplCommPut(data=%p, size=%d, from=%d, tag=%d)", msg_info.u.data, (int)msg_info.size, source, msg_info.replyTag);
      PRINTF(debugMsg);
#endif
      chpl_pvm_recv(source, msg_info.replyTag, msg_info.u.data, (int)msg_info.size);
      break;
    }
      // ChplCommGet tells node to send data to a location.
    case ChplCommGet: {
#if CHPL_DIST_DEBUG
      sprintf(debugMsg, "Fulfilling ChplCommGet(data=%p, size=%d, to=%d, tag=%d", msg_info.u.data, (int)msg_info.size, source, msg_info.replyTag);
      PRINTF(debugMsg);
#endif
      chpl_pvm_send(source, msg_info.replyTag, msg_info.u.data, (int)msg_info.size);
      break;
    }
      // ChplCommFork gets a function ID and a set of arguments. Non-blocking
      // fork works similarly, but runs it from polling thread.
    case ChplCommFork: {
      void* args;
      _chpl_RPC_arg* rpcArg = chpl_malloc(1, sizeof(_chpl_RPC_arg), "RPC args", false, __LINE__, __FILE__);
#if CHPL_DIST_DEBUG
      sprintf(debugMsg, "Fulfilling ChplCommFork(fromloc=%d, tag=%d, fnid=%d)", source, msg_info.replyTag, msg_info.u.fid);
      PRINTF(debugMsg);
#endif
      if (msg_info.size != 0) {
        args = chpl_malloc(1, msg_info.size, "Args for new remote task", false, __LINE__, __FILE__);
      } else {
        args = NULL;
      }

      chpl_pvm_recv(source, msg_info.replyTag, args, msg_info.size);

      rpcArg->fid = (chpl_fn_int_t)msg_info.u.fid;
      rpcArg->arg = args;
      rpcArg->replyTag = msg_info.replyTag;
      rpcArg->joinLocale = source;

      chpl_begin((chpl_fn_p)chpl_RPC, rpcArg, false, false, NULL);
      break;
    }
    case ChplCommForkNB: {
      void* args;
#if CHPL_DIST_DEBUG
      sprintf(debugMsg, "Fulfilling ChplCommForkNB(fromloc=%d, tag=%d)", source, msg_info.replyTag);
      PRINTF(debugMsg);
#endif
      if (msg_info.size != 0) {
        args = chpl_malloc(1, msg_info.size, "Args for new remote task", false, __LINE__, __FILE__);
      } else {
        args = NULL;
      }
      chpl_pvm_recv(source, msg_info.replyTag, args, msg_info.size);
      chpl_begin((chpl_fn_p)chpl_ftable[msg_info.u.fid], args, false, false, NULL);
      break;
    }
    case ChplCommFinish: {
      PRINTF("ChplCommFinish\n");
      fflush(stdout);
      fflush(stderr);
      chpl_mutex_unlock(&termination_lock);
      finished = 1;
      break;
    }
    default: {
      chpl_internal_error("Error: default case should never get reached");
      finished = 1;
      break;
    }
    }
  }
}

void chpl_comm_init(int *argc_p, char ***argv_p) {
  pthread_t polling_thread;
  int status;
  int i;
  int max;

#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
#endif

  // Figure out who I am
  chpl_numLocales = (int32_t)atoi((*argv_p)[2]);
  chpl_localeID = (int32_t)atoi((*argv_p)[3]);
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "Starting with chpl_numLocales=%d. I am chpl_localeID=%d.", chpl_numLocales, chpl_localeID);
  PRINTF(debugMsg);
#endif

  // Join the group of all nodes (named "job")
  // Barrier until everyone (numLocales) has joined
  PVM_SAFE(instance = pvm_joingroup((char *)"job"), "pvm_joingroup", "chpl_comm_init");
  chpl_comm_barrier("Waiting for all tasks to join group.");

  // Figure out who everyone is
  PVM_SAFE(max = pvm_gsize((char *)"job"), "pvm_gsize", "chpl_comm_init");
  for (i=0; i < max; i++) {
    PVM_SAFE(tids[i] = pvm_gettid((char *)"job", i), "pvm_gettid", "chpl_comm_init");
  }
  
  // Create the pthread to do the work.
  status = pthread_create(&polling_thread, NULL, (void*(*)(void*))polling, 0);
  if (status)
    chpl_internal_error("unable to start polling thread for PVM");
  pthread_detach(polling_thread);

  // Ignore the last argument (the localeID) from here on out.
  // Only relevant here, and leaving it in confuses parseArgs later.
  *argc_p = *argc_p - 1;
  return;
}

//
// No support for gdb for now
//
int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

void chpl_comm_rollcall(void) {
  chpl_msg(2, "executing on locale %d of %d locale(s): %s\n", chpl_localeID, chpl_numLocales, chpl_localeName());
  okay_to_barrier = 0;
  return;
}

void chpl_comm_init_shared_heap(void) {
  initHeap(NULL, 0);
  return;
}

void chpl_comm_alloc_registry(int numGlobals) {
  chpl_globals_registry = chpl_globals_registry_static;
  return;
}

void chpl_comm_broadcast_global_vars(int numGlobals) {
  int i;
  int size;
  char debugMsg[128];
  PRINTF("start broadcast globals");
  for (i = 0; i < numGlobals; i++) {
    // Either the root node broadcasting with pvm_bcast, or we're one of
    // the slave nodes getting the data.
    if (chpl_localeID == 0) {
      PVM_PACK_SAFE(pvm_initsend(PvmDataRaw), "pvm_initsend", "chpl_comm_broadcast_global_vars");
      size = sizeof(void *);
      PVM_NO_LOCK_SAFE(pvm_pkint(&size, 1, 1), "pvm_pkint", "chpl_comm_broadcast_global_vars");
      PVM_NO_LOCK_SAFE(pvm_pkbyte((char *)&chpl_globals_registry[i], size, 1), "pvm_pkbyte", "chpl_comm_broadcast_global_vars");
      PVM_UNPACK_SAFE(pvm_bcast((char *)"job", BCASTTAG), "pvm_bcast", "chpl_comm_broadcast_global_vars");
    } else {
      PVM_PACK_SAFE(pvm_recv(-1, BCASTTAG), "pvm_recv", "chpl_comm_broadcast_global_vars");
      PVM_NO_LOCK_SAFE(pvm_upkint(&size, 1, 1), "pvm_upkint", "chpl_comm_broadcast_global_vars");
      PVM_UNPACK_SAFE(pvm_upkbyte(chpl_globals_registry[i], size, 1), "pvm_upkbyte", "chpl_comm_broadcast_global_vars");
    }
    sprintf(debugMsg, "chpl_comm_broadcast_global_vars barrier %d.", i);;
    chpl_comm_barrier(debugMsg);
  }
  PRINTF("end broadcast globals");
  return;
}

void chpl_comm_broadcast_private(void* addr, int size) {
  int i;
  PRINTF("broadcast private");

  for (i = 0; i < chpl_numLocales; i++) {
    if (i != chpl_localeID) {
      chpl_comm_put(addr, i, addr, size, 0, 0);
    }
  }
  return;
}

void chpl_comm_barrier(const char *msg) {
  int bufid = 0;
  PRINTF(msg);

  // Exit is a funny case, and most of the barrier case is built up around it.
  // Node 0 ignores the barrier before the exit and tears down its threads
  // (which have finished), and calls chpl_comm_exit_all. The rest of the
  // nodes wait for a signal from node 0 that it's ready for them.

  // The signal is okay_to_barrier.

  // okay_to_barrier starts out true, but once all the setup is complete,
  // okay_to_barrier is set to false (done in chpl_comm_rollcall). This
  // safegaurd is important because chpl_comm_barrier is blocking, not
  // thread-safe, and it holds the pvm_lock.

  // Once node 0 tells everyone it's okay to barrier, everyone does the
  // barrier (including node 0 -- done in chpl_comm_exit_all), and proceeds
  // to the termination.
  if (!(strcmp(msg, "chpl_comm_exit_all")) && chpl_localeID == 0) {
    return;
  }
  while (!okay_to_barrier) {
    while (bufid == 0) {
      PVM_PACK_SAFE(bufid = pvm_nrecv(-1, BCASTTAG), "pvm_recv", "chpl_comm_barrier");
      if (bufid == 0) {
        chpl_mutex_unlock(&pvm_lock);
      }
    }
    PVM_UNPACK_SAFE(pvm_upkint(&okay_to_barrier, 1, 1), "pvm_upkint", "chpl_comm_barrier");
  }

  PVM_SAFE(pvm_barrier((char *)"job", chpl_numLocales), "pvm_barrier", "chpl_comm_barrier");
  return;
}

void chpl_comm_exit_all(int status) {
  _chpl_message_info msg_info;
  PRINTF("chpl_comm_exit_all called\n");
  // Matches code in chpl_comm_barrier. Node 0, on exit, needs to signal
  // to everyone that it's okay to barrier (and thus exit).
  if (chpl_localeID == 0) {
    okay_to_barrier = 1;
    PVM_PACK_SAFE(pvm_initsend(PvmDataRaw), "pvm_initsend", "chpl_comm_exit_all");
    PVM_NO_LOCK_SAFE(pvm_pkint(&okay_to_barrier, 1, 1), "pvm_pkint", "chpl_comm_exit_all");
    PVM_UNPACK_SAFE(pvm_bcast((char *)"job", BCASTTAG), "pvm_bcast", "chpl_comm_exit_all");
    // Do a matching barrier to everyone still in chpl_comm_barrier.
    PVM_SAFE(pvm_barrier((char *)"job", chpl_numLocales), "pvm_barrier", "chpl_comm_exit_all");
  }
  msg_info.msg_type = ChplCommFinish;
  chpl_pvm_send(chpl_localeID, TAGMASK+1, &msg_info, sizeof(_chpl_message_info));
  PRINTF("Sent shutdown message.");
  chpl_mutex_lock(&termination_lock);
  chpl_mutex_unlock(&termination_lock);
  chpl_comm_barrier("About to finalize");
  PVM_SAFE(pvm_halt(), "pvm_halt", "chpl_comm_exit_all");
  return;
}

void chpl_comm_exit_any(int status) {
  PVM_SAFE(pvm_lvgroup((char *)"job"), "pvm_lvgroup", "chpl_comm_exit_any");
  PVM_SAFE(pvm_exit(), "pvm_exit", "chpl_comm_exit_any");
  return;
}

void chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  if (chpl_localeID == locale) {
    memmove(raddr, addr, size);
  } else {
    _chpl_message_info msg_info;
    int tag = makeTag((int)pthread_self, chpl_localeID);
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_put(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d)", chpl_localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif
    
    msg_info.msg_type = ChplCommPut;
    msg_info.replyTag = tag;
    msg_info.size = size;
    msg_info.u.data = raddr;
    
    chpl_pvm_send(locale, TAGMASK+1, &msg_info, sizeof(_chpl_message_info));
    chpl_pvm_send(locale, tag, addr, size);

#if CHPL_DIST_DEBUG
    sprintf(debugMsg, "chpl_comm_put(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d) done", chpl_localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif
  }
}

void chpl_comm_get(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  if (chpl_localeID == locale) {
    memmove(raddr, addr, size);
  } else {
    _chpl_message_info msg_info;
    int tag = makeTag((int)pthread_self, chpl_localeID);
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_get(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d)", chpl_localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif
    
    msg_info.msg_type = ChplCommGet;
    msg_info.replyTag = tag;
    msg_info.size = size;
    msg_info.u.data = (void *)raddr;

    chpl_pvm_send(locale, TAGMASK+1, &msg_info, sizeof(_chpl_message_info));
    chpl_pvm_recv(locale, tag, addr, size);

#if CHPL_DIST_DEBUG
    sprintf(debugMsg, "chpl_comm_get(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d) done", chpl_localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif
  }
}

void chpl_comm_fork(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  if (chpl_localeID == locale) {
    (*chpl_ftable[fid])(arg);
  } else {
    _chpl_message_info msg_info;
    int tag = makeTag((int)pthread_self, chpl_localeID);
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_fork(locale=%d, tag=%d)", locale, tag);
    PRINTF(debugMsg);
#endif
    
    msg_info.msg_type = ChplCommFork;
    msg_info.replyTag = tag;
    msg_info.size = arg_size;
    msg_info.u.fid = fid;
    
    chpl_pvm_send(locale, TAGMASK+1, &msg_info, sizeof(_chpl_message_info));
    chpl_pvm_send(locale, tag, arg, arg_size);
    chpl_pvm_recv(locale, tag, NULL, 0);
  }
}

void chpl_comm_fork_nb(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  _chpl_message_info msg_info;
  if (chpl_localeID == locale) {
    void* argCopy = chpl_malloc(1, arg_size, "fork_nb argument copy", false, __LINE__, __FILE__);
    memmove(argCopy, arg, arg_size);
    chpl_begin((chpl_fn_p)chpl_ftable[fid], argCopy, false, false, NULL);
  } else {
    int tag = makeTag((int)pthread_self, chpl_localeID);
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_fork_nb(locale=%d, tag=%d)", locale, tag);
    PRINTF(debugMsg);
#endif
    
    msg_info.msg_type = ChplCommForkNB;
    msg_info.replyTag = tag;
    msg_info.size = arg_size;
    msg_info.u.fid = fid;
    
    chpl_pvm_send(locale, TAGMASK+1, &msg_info, sizeof(_chpl_message_info));
    chpl_pvm_send(locale, tag, arg, arg_size);
  }
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
