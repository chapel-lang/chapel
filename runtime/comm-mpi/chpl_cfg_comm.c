#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "chplexit.h"
#include "chplcomm.h"
#include "chplmem.h"
#include "chplrt.h"
#include "chplthreads.h"
#include "error.h"
#include "chplsys.h"
#include "mpi.h"

#define CHPL_DIST_DEBUG 0

#if CHPL_DIST_DEBUG
#define DEBUG_MSG_LENGTH 256
#define PRINTF(_s) {                                                         \
  printf("%d/%d:%d/%d:%s\n", _localeID, _numLocales,                         \
                             (int)getpid(), (int)pthread_self(), _s);        \
  fflush(stdout);                                                            \
}
#else
#define PRINTF(_s)
#endif

#define MPI_SAFE(call) {                           \
  int retcode = call;                              \
  if (retcode != MPI_SUCCESS) {                    \
    char msg[256];                                 \
    char mpiError[256];                            \
    int length;                                    \
    MPI_Error_string(retcode, mpiError, &length);  \
    sprintf(msg, "\n\n%d/%d:%d MPI call failed with %s\n\n", _localeID, _numLocales, (int)pthread_self(), mpiError); \
    chpl_error(msg, __LINE__, __FILE__);   \
  } \
}

// The MPI standard only requires tags as large as 32767 to work.
#define TAGMASK 4194303

chpl_mutex_t termination_lock;

int32_t _chpl_comm_getMaxThreads(void) {
  return 0;
}

int32_t _chpl_comm_maxThreadsLimit(void) {
  return 0;
}

int _chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
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
// Careful to keep this struct as small as possible. It is sent over the
// network for every chpl_comm_get, chpl_comm_put, chpl_comm_fork,
// and chpl_comm_fork_nb. On normal Linux64 configurations, this is 16 bytes.
//
typedef struct __chpl_mpi_message_info {
  //ChplCommMsgType msg_type;
  int msg_type : 4;    // One of the ChplCommMsgType enum values
  int replyTag : 27;   // Use this tag for the reply message
  int size;            // Size at location, or size of function arguments
  void* location;      // Either function pointer or data location
} _chpl_mpi_message_info;

//
// A function and argument for a remote procedure call. When done executing
// the function, send an empty message to joinLocale with replyTag.
// 
typedef struct __chpl_RPC_arg {
  func_p function;
  void* arg;
  int replyTag;
  int joinLocale;
} _chpl_RPC_arg;


static void chpl_RPC(_chpl_RPC_arg* arg);
static MPI_Status chpl_mpi_recv(void* buf, int count, MPI_Datatype datatype,
                                int source, int tag, MPI_Comm comm);
static void chpl_mpi_send(void* buf, int count, MPI_Datatype datatype,
                          int dest, int tag, MPI_Comm comm);
static MPI_Status chpl_mpi_spin_recv(void* buf, int count,
                                     MPI_Datatype datatype, int source,
                                     int tag, MPI_Comm comm);
static void chpl_mpi_spin_send(void* buf, int count, MPI_Datatype datatype,
                               int dest, int tag, MPI_Comm comm);
static int makeTag(int threadID, int localeID);


//
// A simple hash to combine the threadID and localeID into one (hopefully
// unique) value that is small enough to use as an MPI message tag.
//
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
  PRINTF("Doing forked task");
  arg->function(arg->arg);
  PRINTF("Did task");
  chpl_mpi_send(NULL, 0, MPI_BYTE, arg->joinLocale, arg->replyTag, MPI_COMM_WORLD);
  if (arg->arg != NULL)
    chpl_free(arg->arg, __LINE__, __FILE__);
  chpl_free(arg, __LINE__, __FILE__);
}


static MPI_Status chpl_mpi_recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm) {
  MPI_Status status;
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
  sprintf(debugMsg, "chpl_mpi_recv(%p, size=%d, from=%d, tag=%d)", buf, count, source, tag);
  PRINTF(debugMsg);
#endif
  MPI_SAFE(MPI_Recv(buf, count, datatype, source, tag, comm, &status));
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_mpi_recv(%p, size=%d, from=%d, tag=%d) done", buf, count, source, tag);
  PRINTF(debugMsg);
#endif
  return status;
}


static void chpl_mpi_send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
  sprintf(debugMsg, "Doing chpl_mpi_send(%p, size=%d, to=%d, tag=%d)", buf, count, dest, tag);
  PRINTF(debugMsg);
#endif
  MPI_SAFE(MPI_Ssend(buf, count, datatype, dest, tag, comm));
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_mpi_send(%p, size=%d, to=%d, tag=%d) done", buf, count, dest, tag);
  PRINTF(debugMsg);
#endif
}


static MPI_Status chpl_mpi_spin_recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm) {
  int flag;
  MPI_Status status;
  MPI_Request request;
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
  sprintf(debugMsg, "chpl_mpi_spin_recv(%p, size=%d, from=%d, tag=%d)", buf, count, source, tag);
  PRINTF(debugMsg);
#endif
  MPI_SAFE(MPI_Irecv(buf, count, datatype, source, tag, comm, &request));
  do {
    int result;
    MPI_SAFE(MPI_Test(&request, &flag, &status));
  } while (flag != 1);
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_mpi_spin_recv(%p, size=%d, from=%d, tag=%d) done", buf, count, source, tag);
  PRINTF(debugMsg);
#endif
  return status;
}


static void chpl_mpi_spin_send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
  int flag;
  MPI_Status status;
  MPI_Request request;
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
  sprintf(debugMsg, "chpl_mpi_spin_send(%p, size=%d, to=%d, tag=%d)", buf, count, dest, tag);
  PRINTF(debugMsg);
#endif
  MPI_SAFE(MPI_Issend(buf, count, datatype, dest, tag, comm, &request));
  do {
    int result;
    MPI_SAFE(MPI_Test(&request, &flag, &status));
  } while (!flag);
#if CHPL_DIST_DEBUG
  sprintf(debugMsg, "chpl_mpi_spin_send(%p, size=%d, to=%d, tag=%d) done", buf, count, dest, tag);
  PRINTF(debugMsg);
#endif
}


//
// Every node has a polling thread that does an MPI receive accepting a
// message from any node. The message is always a struct chpl_mpi_message_info.
// It then intreprets that chpl_mpi_message_info and acts accordingly.
//
static void chpl_mpi_polling_thread(void* arg) {
  int finished = 0;
  _chpl_mpi_message_info msg_info;
  MPI_Status status;
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
#endif

  PRINTF("Starting mpi polling thread");

  chpl_mutex_lock(&termination_lock);
  while (!finished) {
    PRINTF("Poller Receiving");
    status = chpl_mpi_recv(&msg_info, sizeof(_chpl_mpi_message_info),
                           MPI_BYTE, MPI_ANY_SOURCE, TAGMASK+1, MPI_COMM_WORLD);
    switch (msg_info.msg_type) {
      case ChplCommPut: {
#if CHPL_DIST_DEBUG
        sprintf(debugMsg, "Fulfilling ChplCommPut(location=%p, size=%d, from=%d, tag=%d)", msg_info.location, (int)msg_info.size, status.MPI_SOURCE, msg_info.replyTag);
        PRINTF(debugMsg);
#endif
        chpl_mpi_recv(msg_info.location, msg_info.size, MPI_BYTE,
                      status.MPI_SOURCE, msg_info.replyTag, MPI_COMM_WORLD);
        break;
      }
      case ChplCommGet: {
#if CHPL_DIST_DEBUG
        sprintf(debugMsg, "Fulfilling ChplCommGet(location=%p, size=%d, to=%d, tag=%d)", msg_info.location, (int)msg_info.size, status.MPI_SOURCE, msg_info.replyTag);
        PRINTF(debugMsg);
#endif
        chpl_mpi_send(msg_info.location, msg_info.size, MPI_BYTE,
                      status.MPI_SOURCE, msg_info.replyTag, MPI_COMM_WORLD);
        break;
      }
      case ChplCommFork: {
        chpl_threadarg_t args;
        _chpl_RPC_arg* rpcArg = chpl_malloc(1, sizeof(_chpl_RPC_arg), "RPC args", __LINE__, __FILE__);
#if CHPL_DIST_DEBUG
        sprintf(debugMsg, "Fulfilling ChplCommFork(fromloc=%d, tag=%d)", status.MPI_SOURCE, msg_info.replyTag);
        PRINTF(debugMsg);
#endif
        if (msg_info.size != 0) {
          args = chpl_malloc(1, msg_info.size, "Args for new remote task", __LINE__, __FILE__);
        } else {
          args = NULL;
        }

        chpl_mpi_recv(args, msg_info.size, MPI_BYTE, status.MPI_SOURCE, msg_info.replyTag, MPI_COMM_WORLD);

        rpcArg->function = (func_p)msg_info.location;
        rpcArg->arg = args;
        rpcArg->replyTag = msg_info.replyTag;
        rpcArg->joinLocale = status.MPI_SOURCE;

        chpl_begin((chpl_threadfp_t)chpl_RPC, rpcArg, false, false, NULL);
        break;
      }
      case ChplCommForkNB: {
        chpl_threadarg_t args;
        _chpl_RPC_arg* rpcArg = chpl_malloc(1, sizeof(_chpl_RPC_arg), "RPC args", __LINE__, __FILE__);
#if CHPL_DIST_DEBUG
        sprintf(debugMsg, "Fulfilling ChplCommForkNB(fromloc=%d, tag=%d)", status.MPI_SOURCE, msg_info.replyTag);
        PRINTF(debugMsg);
#endif
        if (msg_info.size != 0) {
          args = chpl_malloc(1, msg_info.size, "Args for new remote task", __LINE__, __FILE__);
        } else {
          args = NULL;
        }
        chpl_mpi_recv(args, msg_info.size, MPI_BYTE, status.MPI_SOURCE,
                      msg_info.replyTag, MPI_COMM_WORLD);
        chpl_begin((chpl_threadfp_t)msg_info.location, args, false, false, NULL);
        break;
      }
      case ChplCommFinish: {
        PRINTF("ChplCommFinish");
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


void _chpl_comm_init(int *argc_p, char ***argv_p) {
  int flag, tag_bound;
  pthread_t polling_thread;

  MPI_SAFE(MPI_Init_thread(argc_p, argv_p, MPI_THREAD_MULTIPLE, &flag));
  MPI_SAFE(MPI_Comm_size(MPI_COMM_WORLD, &_numLocales));
  MPI_SAFE(MPI_Comm_rank(MPI_COMM_WORLD, &_localeID));

  MPI_SAFE(MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN));

  MPI_SAFE(MPI_Query_thread(&flag));
  if (MPI_THREAD_MULTIPLE != flag) {
    chpl_internal_error("MPI thread type must be MPI_THREAD_MULTIPLE");
  }

  MPI_SAFE(MPI_Attr_get(MPI_COMM_WORLD, MPI_TAG_UB, &tag_bound, &flag));
  if (TAGMASK + 1 > tag_bound) {
    chpl_internal_error("MPI tag upper bound is too low");
  }

  _chpl_comm_barrier("barrier in init");

  if (pthread_mutex_init(&termination_lock, NULL))
    chpl_internal_error("pthread_mutex_init() failed");

  if (pthread_create(&polling_thread, NULL,
                     (chpl_threadfp_t)chpl_mpi_polling_thread, 0))
    chpl_internal_error("unable to start polling thread for mpi");
  pthread_detach(polling_thread);
}


void _chpl_comm_rollcall(void) {
  chpl_msg(2, "executing on locale %d of %d locale(s): %s\n", _localeID,
            _numLocales, chpl_localeName());
}


void _chpl_comm_alloc_registry(int numGlobals) {
  _global_vars_registry = _global_vars_registry_static;
}


void _chpl_comm_broadcast_global_vars(int numGlobals) {
  int i;
  PRINTF("start broadcast globals");
  for (i = 0; i < numGlobals; i++) {
    void* buf = _global_vars_registry[i];
    MPI_SAFE(MPI_Bcast(&buf, sizeof(void*), MPI_BYTE, 0, MPI_COMM_WORLD));
    if (_localeID != 0) {
      *(void**)_global_vars_registry[i] = buf;
    }
  }
  PRINTF("end broadcast globals");
}


void _chpl_comm_broadcast_private(void* addr, int size) {
  int i;
  PRINTF("broadcast private");

  for (i = 0; i < _numLocales; i++) {
    if (i != _localeID) {
      _chpl_comm_put(addr, i, addr, size);
    }
  }
}


void _chpl_comm_init_shared_heap(void) {
  // Use the regular malloc/free implementation
}


void _chpl_comm_barrier(const char *msg) {
  PRINTF(msg);
  MPI_SAFE(MPI_Barrier(MPI_COMM_WORLD));
}


void _chpl_comm_exit_all(int status) {
  _chpl_mpi_message_info msg_info;
  PRINTF("in chpl_comm_exit_all");
  msg_info.msg_type = ChplCommFinish;
  // To prevent MPI_Finalize from being called while the polling thread
  // is still making MPI calls, send it a finish message, then wait for it
  // to release the termination_lock, which it will do only once no more
  // MPI Calls will be made.
  chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                _localeID, TAGMASK+1, MPI_COMM_WORLD);
  PRINTF("Sent shutdown message");
  chpl_mutex_lock(&termination_lock);
  chpl_mutex_unlock(&termination_lock);
  _chpl_comm_barrier("About to finalize");
  MPI_SAFE(MPI_Finalize());
}


void _chpl_comm_exit_any(int status) {
  MPI_SAFE(MPI_Abort(MPI_COMM_WORLD, status));
}


void  _chpl_comm_put(void* addr, int32_t locale, void* raddr, int32_t size) {
  if (_localeID == locale) {
    memmove(raddr, addr, size);
  } else {
    _chpl_mpi_message_info msg_info;
    int tag = makeTag((int)pthread_self(), _localeID);
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "_chpl_comm_put(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d)", _localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif

    msg_info.msg_type = ChplCommPut;
    msg_info.replyTag = tag;
    msg_info.size = size;
    msg_info.location = raddr;

    chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                  locale, TAGMASK+1, MPI_COMM_WORLD);
    chpl_mpi_send(addr, size, MPI_BYTE, locale, tag, MPI_COMM_WORLD);
#if CHPL_DIST_DEBUG
    sprintf(debugMsg, "_chpl_comm_put(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d) done", _localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif
  }
}


void  _chpl_comm_get(void *addr, int32_t locale, void* raddr, int32_t size) {
  if (_localeID == locale) {
    memmove(addr, raddr, size);
  } else {
    _chpl_mpi_message_info msg_info;
    int tag = makeTag((int)pthread_self(), _localeID);
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "_chpl_comm_get(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d)", _localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif

    msg_info.msg_type = ChplCommGet;
    msg_info.replyTag = tag;
    msg_info.size = size;
    msg_info.location = (void*)raddr;

    chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                  locale, TAGMASK+1, MPI_COMM_WORLD);
    chpl_mpi_recv(addr, size, MPI_BYTE, locale, tag, MPI_COMM_WORLD);
#if CHPL_DIST_DEBUG
    sprintf(debugMsg, "_chpl_comm_get(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d) done", _localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif
  }
}


void  _chpl_comm_fork(int locale, func_p f, void *arg, int arg_size) {
  if (_localeID == locale) {
    (*f)(arg);
  } else {
    _chpl_mpi_message_info msg_info;
    int tag = makeTag((int)pthread_self(), _localeID);
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_fork(locale=%d, tag=%d)", locale, tag);
    PRINTF(debugMsg);
#endif
    msg_info.msg_type = ChplCommFork;
    msg_info.replyTag = tag;
    msg_info.size = arg_size;
    msg_info.location = f;

    chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                  locale, TAGMASK+1, MPI_COMM_WORLD);
    chpl_mpi_send(arg, arg_size, MPI_BYTE, locale, tag, MPI_COMM_WORLD);
    chpl_mpi_recv(NULL, 0, MPI_BYTE, locale, tag, MPI_COMM_WORLD);

  }
}


void  _chpl_comm_fork_nb(int locale, func_p f, void *arg, int arg_size) {
  _chpl_mpi_message_info msg_info;
  if (_localeID == locale) {
    void* argCopy = chpl_malloc(1, arg_size, "fork_nb argument copy", __LINE__, __FILE__);
    memmove(argCopy, arg, arg_size);
    chpl_begin((chpl_threadfp_t)f, argCopy, false, false, NULL);
  } else {
    int tag = makeTag((int)pthread_self(), _localeID);
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_fork_nb(locale=%d, tag=%d)", locale, tag);
    PRINTF(debugMsg);
#endif
    msg_info.msg_type = ChplCommForkNB;
    msg_info.location = f;
    msg_info.size = arg_size;
    msg_info.replyTag = tag;

    chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                  locale, TAGMASK+1, MPI_COMM_WORLD);
    chpl_mpi_send(arg, arg_size, MPI_BYTE, locale, tag, MPI_COMM_WORLD);

  }
}

void chpl_startVerboseComm() { }
void chpl_stopVerboseComm() { }
void chpl_startVerboseCommHere() { }
void chpl_stopVerboseCommHere() { }

void chpl_startCommDiagnostics() { }
void chpl_stopCommDiagnostics() { }
void chpl_startCommDiagnosticsHere() { }
void chpl_stopCommDiagnosticsHere() { }

int32_t chpl_numCommGets(void) { return -1; }
int32_t chpl_numCommPuts(void) { return -1; }
int32_t chpl_numCommForks(void) { return -1; }
int32_t chpl_numCommNBForks(void) { return -1; }
