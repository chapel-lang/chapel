#include "chplrt.h"
#include "chplcomm.h"
#include "chplmem.h"
#include "chplsys.h"
#include "chplthreads.h"
#include "error.h"
#include "pvm3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CHPL_DIST_DEBUG 1

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

static int chpl_comm_diagnostics = 0;           // set via startCommDiagnostics
static int chpl_comm_gets = 0;
static int chpl_comm_puts = 0;
static int chpl_comm_forks = 0;
static int chpl_comm_nb_forks = 0;
static int chpl_verbose_comm = 0;               // set via startVerboseComm
static int chpl_comm_no_debug_private = 0;

int instance;

//
// Chapel interface starts here
//


int32_t chpl_comm_getMaxThreads(void) {
  fprintf(stderr, "chpl_comm_getMaxThreads called\n");
  return 0;
}

int32_t chpl_comm_maxThreadsLimit(void) {
  fprintf(stderr, "chpl_comm_maxThreadsLimit called\n");
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
static int chpl_pvm_recv(int tid, int msgtag, _chpl_message_info* buf);
static int chpl_pvm_send(int tid, int msgtag, _chpl_message_info* buf);

static void chpl_RPC(_chpl_RPC_arg* arg) {
  PRINTF("Entering forked task");
  (*chpl_ftable[arg->fid])(arg->arg);
  PRINTF("Did task");
  chpl_pvm_send(-1, -1, NULL);            // Figure this out
  if (arg->arg != NULL)
    chpl_free(arg->arg, __LINE__, __FILE__);
  chpl_free(arg, __LINE__, __FILE__);
}

static int chpl_pvm_recv(int tid, int msgtag, _chpl_message_info* buf) {
  int bufid;
  int type;
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
  sprintf(debugMsg, "chpl_pvm_recv(%p, from=%d, tag=%d)", buf, tid, msgtag);
  PRINTF(debugMsg);
#endif
  bufid = pvm_recv(tid, msgtag);
  if ((pvm_upkint(&type, 1, 1)) < 0) {
    buf->msg_type = ChplCommFinish;
  } else {
    buf->msg_type = type;
  }
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_pvm_recv(%p, from=%d, tag=%d) done", buf, tid, msgtag);
  PRINTF(debugMsg);
#endif
  return bufid;
}

static int chpl_pvm_send(int tid, int msgtag, _chpl_message_info* buf) {
  int bufid;
  int type;
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
  sprintf(debugMsg, "chpl_pvm_send(%p, to=%d, tag=%d)", buf, tid, msgtag);
  PRINTF(debugMsg);
#endif
  if ((pvm_initsend(PvmDataDefault)) < 0) {
    chpl_internal_error("Error: couldn't initialize PVM send buffer");
  }
  type = buf->msg_type;
  if ((pvm_pkint(&type, 1, 1)) < 0) {
    chpl_internal_error("Error: failue packing PVM data");
  }
  bufid = pvm_send(tid, msgtag);
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_pvm_send(%p, to=%d, tag=%d) done", buf, tid, msgtag);
  PRINTF(debugMsg);
#endif
  return bufid;
}

static void polling(void* x) {
  int finished;
  _chpl_message_info msg_info;
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
#endif

  PRINTF("Starting PVM polling thread");
  finished = 0;

  while (!finished) {
    PRINTF("Poller Receiving");
    chpl_pvm_recv(-1, -1, &msg_info);
    switch (msg_info.msg_type) {
    case ChplCommPut: {
#if CHPL_DIST_DEBUG
      sprintf(debugMsg, "Fulfilling ChplCommPut");
      PRINTF(debugMsg);
#endif
      chpl_pvm_recv(-1, -1, msg_info.u.data);     // Figure this out
      break;
    }
    case ChplCommGet: {
#if CHPL_DIST_DEBUG
      sprintf(debugMsg, "Fulfilling ChplCommGet");
      PRINTF(debugMsg);
#endif
      chpl_pvm_send(-1, -1, msg_info.u.data);     // Figure this out
      break;
    }
    case ChplCommFork: {
      void* args;
      _chpl_RPC_arg* rpcArg = chpl_malloc(1, sizeof(_chpl_RPC_arg), "RPC args", false, __LINE__, __FILE__);
#if CHPL_DIST_DEBUG
      sprintf(debugMsg, "Fulfilling ChplCommFork");
      PRINTF(debugMsg);
#endif
      if (msg_info.size != 0) {
        args = chpl_malloc(1, msg_info.size, "Args for new remote task", false, __LINE__, __FILE__);
      } else {
        args = NULL;
      }

      chpl_pvm_recv(-1, -1, args);                // Figure this out

      rpcArg->fid = (chpl_fn_int_t)msg_info.u.fid;
      rpcArg->arg = args;
      rpcArg->replyTag = msg_info.replyTag;
      //      rpcArg->joinLocale = bufid;

      chpl_begin((chpl_fn_p)chpl_RPC, rpcArg, false, false, NULL);
      break;
    }
    case ChplCommForkNB: {
      void* args;
#if CHPL_DIST_DEBUG
      sprintf(debugMsg, "Fulfilling ChplCommForkNB");
      PRINTF(debugMsg);
#endif
      if (msg_info.size != 0) {
        args = chpl_malloc(1, msg_info.size, "Args for new remote task", false, __LINE__, __FILE__);
      } else {
        args = NULL;
      }
      chpl_pvm_recv(-1, -1, args);                // Figure this out
      chpl_begin((chpl_fn_p)chpl_ftable[msg_info.u.fid], args, false, false, NULL);
      break;
    }
    case ChplCommFinish: {
      PRINTF("ChplCommFinish\n");
      fflush(stdout);
      fflush(stderr);
      finished = 1;
      break;
    }
    default: {
      fprintf(stderr, "No msgtype\n");
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

  //  char hostname[128]; // hostname

  int tid;      // master tid
  int instance; // instance number for PVM group
  //int tids[64]; // tid list for all the other nodes
  //int numt;     // number of successful spawns
  //int numprocs; // number of nodes to spawn upon
  //  int i;

  fprintf(stderr, "chpl_comm_init called\n");

  //  if (gethostname(hostname, sizeof(hostname)) != 0) {
  //    chpl_internal_error("Couldn't get hostname.");
  //  }
  //  fprintf(stderr, "I'm on %s.\n", hostname);

  //  for (i = 1; i < argc_p[0]; i++) {
  //    fprintf(stderr, "arg %d is %s\n", i, (*argv_p)[i]);
  //  }

  chpl_numLocales = (int32_t)atoi((*argv_p)[2]);
  chpl_localeID = (int32_t)atoi((*argv_p)[3]);
  fprintf(stderr, "numLocales is %d, localeID is %d\n", chpl_numLocales, chpl_localeID);

  tid = pvm_mytid();
  fprintf(stderr, "tid is %d\n", tid);

  if ((instance = pvm_joingroup((char *)"job")) < 0) {
    pvm_perror((char *)"Error joining job group");
    chpl_internal_error("Exiting.");
  }
  fprintf(stderr, "instance is %d, tid is %d, size is %d\n", instance, pvm_gettid((char *)"job", instance), pvm_gsize((char *)"job"));

  chpl_comm_barrier("Waiting for all tasks to join group.");

  //  if ( strcmp(hostname, (*argv_p)[1]) == 0 ) {
  //numt = pvm_spawn((char*)"hello_real", (char**)0, 0, (char *)"", numprocs, (int *)tids);
    //  }
  //if ( numt < numprocs ) {
  //  fprintf(stderr, "\n Trouble spawning slaves. Aborting. Error codes are:\n");
  //  for ( i = numt; i < numprocs; i++ ) {
  //    fprintf(stderr, "TID %d %d\n", i, tids[i]);
  //  }
  //  for ( i = 0; i < numt; i++ ) {
  //    pvm_kill( tids[i] );
  //  }
  //  pvm_exit();
  //  chpl_internal_error("Exiting.");
  //}

  //fprintf(stderr, "mytid is %d, instance is %d, tid is %d, size is %d\n", tid, instance, pvm_gettid((char *)"job", instance), pvm_gsize((char *)"job"));

  //system("uname -n");

  
  status = pthread_create(&polling_thread, NULL, (void*(*)(void*))polling, 0);
  if (status)
    chpl_internal_error("unable to start polling thread for PVM");
  pthread_detach(polling_thread);

  //  chpl_free(hostname, -1, "");
  
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
  chpl_msg(2, "executing on locale %d of %d locale(s): %s\n", chpl_localeID, chpl_numLocales, chpl_localeName());
  return;
}

void chpl_comm_init_shared_heap(void) {
  initHeap(NULL, 0);
  fprintf(stderr, "chpl_comm_init_shared_heap called\n");
  return;
}

void chpl_comm_alloc_registry(int numGlobals) {
  chpl_globals_registry = chpl_globals_registry_static;
  fprintf(stderr, "chpl_comm_alloc_registry called\n");
  return;
}

void chpl_comm_broadcast_global_vars(int numGlobals) {
  int i;
  for (i = 0; i < numGlobals; i++) {
    void* buf = chpl_globals_registry[i];
    if (chpl_localeID != 0) {
      *(void**)chpl_globals_registry[i] = buf;
    }
  }
  fprintf(stderr, "chpl_comm_broadcast_global_vars called\n");
  return;
}

void chpl_comm_broadcast_private(void* addr, int size) {
  fprintf(stderr, "chpl_comm_broadcast_private called");
  return;
}

void chpl_comm_barrier(const char *msg) {
  PRINTF(msg);
  pvm_barrier((char *)"job", chpl_numLocales);
  return;
}

void chpl_comm_exit_all(int status) {
  PRINTF("chpl_comm_exit_all called");
  chpl_comm_barrier("About to finalize");
  pvm_halt();
  return;
}

void chpl_comm_exit_any(int status) {
  fprintf(stderr, "chpl_comm_exit_any called\n");
  if (pvm_lvgroup((char *)"job") < 0) {
    chpl_internal_error("Unable to leave PVM group");
  }
  pvm_exit();
  return;
}

void chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  fprintf(stderr, "chpl_comm_put called");
  return;
}

void chpl_comm_get(void* addr, int32_t locale, void* raddr, int32_t size, int ln, chpl_string fn) {
  fprintf(stderr, "chpl_comm_get called");
  return;
}

void chpl_comm_fork(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
  fprintf(stderr, "chpl_comm_fork called");
  return;
}

void chpl_comm_fork_nb(int locale, chpl_fn_int_t fid, void *arg, int arg_size) {
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
