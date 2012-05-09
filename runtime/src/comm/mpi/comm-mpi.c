#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include "chplexit.h"
#include "chpl-comm.h"
#include "chpl-mem.h"
#include "chplrt.h"
#include "chpl-tasks.h"
#include "error.h"
#include "chplsys.h"
#include "mpi.h"

#define CHPL_DIST_DEBUG 0

#if CHPL_DIST_DEBUG
#define DEBUG_MSG_LENGTH 256
#define PRINTF(_s) do {                                                      \
  printf("%d/%d:%d/%d:%s\n", chpl_localeID, chpl_numLocales,                 \
                             (int)getpid(), (int)pthread_self(), _s);        \
  fflush(stdout);                                                            \
} while(0)
#else
#define PRINTF(_s)
#endif

#define MPI_SAFE(call) do {                                                 \
  int retcode = call;                                                       \
  if (retcode != MPI_SUCCESS) {                                             \
    char msg[256];                                                          \
    char mpiError[256];                                                     \
    int length;                                                             \
    MPI_Error_string(retcode, mpiError, &length);                           \
    sprintf(msg, "\n\n%d/%d:%d MPI call failed with %s\n\n",                \
            chpl_localeID, chpl_numLocales, (int)pthread_self(), mpiError); \
    chpl_error(msg, 0, 0);                                                  \
  }                                                                         \
} while(0)

// The MPI standard only requires tags as large as 32767 to work.
#define TAGMASK 4194303

chpl_sync_aux_t termination_sync;

volatile int tag_count;
chpl_sync_aux_t tag_count_sync;
pthread_key_t tag_count_key;


int32_t chpl_comm_getMaxThreads(void) {
  return 0;
}

int chpl_comm_run_in_gdb(int argc, char* argv[], int gdbArgnum, int* status) {
  return 0;
}

typedef enum {
  ChplCommPut = 0,
  ChplCommGet = 1,
  ChplCommFork = 2,
  ChplCommForkNB = 3,
  ChplCommFinish = 4
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
  int size;            // Size of data or function arguments
  union {
    void* data;        // Data location
    chpl_fn_int_t fid; // Function id
  } u;
} _chpl_mpi_message_info;

//
// A function and argument for a remote procedure call. When done executing
// the function, send an empty message to joinLocale with replyTag.
// 
typedef struct __chplForkedTaskArg {
  chpl_fn_int_t fid;
  void* arg;
  int replyTag;
  int joinLocale;
  int blockingCall;
} _chplForkedTaskArg;


static void chplExecForkedTask(_chplForkedTaskArg* arg);
static MPI_Status chpl_mpi_recv(void* buf, int count, MPI_Datatype datatype,
                                int source, int tag, MPI_Comm comm);
static void chpl_mpi_send(void* buf, int count, MPI_Datatype datatype,
                          int dest, int tag, MPI_Comm comm);
static int makeTag(void);

//
// Create an MPI message tag that is unique for each thread in the program, not
// just within a locale.
//
//
static int makeTag(void) {
  int* tag = (int*)pthread_getspecific(tag_count_key);
  const int maxtag = (TAGMASK/chpl_numLocales)*(1+chpl_localeID) - 1;
  if (tag == NULL) {
    tag = chpl_mem_allocMany(1, sizeof(int), 0, 0, 0);
    chpl_sync_lock(&tag_count_sync);
    *tag = tag_count;
    tag_count += 1;
    chpl_sync_unlock(&tag_count_sync);
    if (*tag > maxtag)
      chpl_error("tag is too big", 0, 0);
    pthread_setspecific(tag_count_key, tag);
  }
  return *tag;
}


static void chplExecForkedTask(_chplForkedTaskArg* arg) {
  PRINTF("Doing forked task");
  (*chpl_ftable[arg->fid])(arg->arg);
  PRINTF("Did task");
  if (arg->blockingCall)
    chpl_mpi_send(NULL, 0, MPI_BYTE, arg->joinLocale, arg->replyTag, MPI_COMM_WORLD);
  if (arg->arg != NULL)
    chpl_mem_free(arg->arg, 0, 0);
  chpl_mem_free(arg, 0, 0);
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


//
// Every node has a polling task that does an MPI receive accepting a
// message from any node. The message is always a struct chpl_mpi_message_info.
// It then intreprets that chpl_mpi_message_info and acts accordingly.
//
static void chpl_mpi_polling_task(void* arg) {
  int finished = 0;
  _chpl_mpi_message_info msg_info;
  MPI_Status status;
#if CHPL_DIST_DEBUG
  char debugMsg[DEBUG_MSG_LENGTH];
#endif

  PRINTF("Starting mpi polling task");

  chpl_sync_lock(&termination_sync);
  while (!finished) {
    PRINTF("Poller Receiving");
    status = chpl_mpi_recv(&msg_info, sizeof(_chpl_mpi_message_info),
                           MPI_BYTE, MPI_ANY_SOURCE, TAGMASK+1, MPI_COMM_WORLD);
    switch (msg_info.msg_type) {
      case ChplCommPut: {
#if CHPL_DIST_DEBUG
        sprintf(debugMsg, "Fulfilling ChplCommPut(data=%p, size=%d, from=%d, tag=%d)", msg_info.u.data, (int)msg_info.size, status.MPI_SOURCE, msg_info.replyTag);
        PRINTF(debugMsg);
#endif
        chpl_mpi_recv(msg_info.u.data, msg_info.size, MPI_BYTE,
                      status.MPI_SOURCE, msg_info.replyTag, MPI_COMM_WORLD);
        break;
      }
      case ChplCommGet: {
#if CHPL_DIST_DEBUG
        sprintf(debugMsg, "Fulfilling ChplCommGet(data=%p, size=%d, to=%d, tag=%d)", msg_info.u.data, (int)msg_info.size, status.MPI_SOURCE, msg_info.replyTag);
        PRINTF(debugMsg);
#endif
        chpl_mpi_send(msg_info.u.data, msg_info.size, MPI_BYTE,
                      status.MPI_SOURCE, msg_info.replyTag, MPI_COMM_WORLD);
        break;
      }
      case ChplCommFork: {
        void* args;
        _chplForkedTaskArg* rpcArg = chpl_mem_allocMany(1, sizeof(_chplForkedTaskArg), CHPL_RT_MD_COMM_FORK_SEND_INFO, 0, 0);
#if CHPL_DIST_DEBUG
        sprintf(debugMsg, "Fulfilling ChplCommFork(fromloc=%d, tag=%d)", status.MPI_SOURCE, msg_info.replyTag);
        PRINTF(debugMsg);
#endif
        if (msg_info.size != 0) {
          args = chpl_mem_allocMany(1, msg_info.size, CHPL_RT_MD_COMM_FORK_SEND_LARGE_ARG, 0, 0);
        } else {
          args = NULL;
        }

        chpl_mpi_recv(args, msg_info.size, MPI_BYTE, status.MPI_SOURCE, msg_info.replyTag, MPI_COMM_WORLD);

        rpcArg->fid = (chpl_fn_int_t)msg_info.u.fid;
        rpcArg->arg = args;
        rpcArg->replyTag = msg_info.replyTag;
        rpcArg->joinLocale = status.MPI_SOURCE;
        rpcArg->blockingCall = 1;

        chpl_task_begin((chpl_fn_p)chplExecForkedTask, rpcArg, true, false, NULL);
        break;
      }
      case ChplCommForkNB: {
        void* args;
        _chplForkedTaskArg* rpcArg = chpl_mem_allocMany(1, sizeof(_chplForkedTaskArg), CHPL_RT_MD_COMM_FORK_SEND_NB_INFO, 0, 0);
#if CHPL_DIST_DEBUG
        sprintf(debugMsg, "Fulfilling ChplCommForkNB(fromloc=%d, tag=%d)", status.MPI_SOURCE, msg_info.replyTag);
        PRINTF(debugMsg);
#endif
        if (msg_info.size != 0) {
          args = chpl_mem_allocMany(1, msg_info.size, CHPL_RT_MD_COMM_FORK_SEND_NB_LARGE_ARG, 0, 0);
        } else {
          args = NULL;
        }
        chpl_mpi_recv(args, msg_info.size, MPI_BYTE, status.MPI_SOURCE,
                      msg_info.replyTag, MPI_COMM_WORLD);
        rpcArg->fid = (chpl_fn_int_t)msg_info.u.fid;
        rpcArg->arg = args;
        rpcArg->replyTag = msg_info.replyTag;
        rpcArg->joinLocale = status.MPI_SOURCE;
        rpcArg->blockingCall = 0;
        chpl_task_begin((chpl_fn_p)chplExecForkedTask, rpcArg, true, false, NULL);
        break;
      }
      case ChplCommFinish: {
        PRINTF("ChplCommFinish");
        fflush(stdout);
        fflush(stderr);
        chpl_sync_unlock(&termination_sync);
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
  int flag, tag_bound;
  pthread_t polling_thread;

  MPI_SAFE(MPI_Init_thread(argc_p, argv_p, MPI_THREAD_MULTIPLE, &flag));
  MPI_SAFE(MPI_Comm_size(MPI_COMM_WORLD, &chpl_numLocales));
  MPI_SAFE(MPI_Comm_rank(MPI_COMM_WORLD, &chpl_localeID));

  MPI_SAFE(MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN));

  MPI_SAFE(MPI_Query_thread(&flag));
  if (MPI_THREAD_MULTIPLE != flag) {
    chpl_internal_error("MPI thread type must be MPI_THREAD_MULTIPLE");
  }

  MPI_SAFE(MPI_Attr_get(MPI_COMM_WORLD, MPI_TAG_UB, &tag_bound, &flag));
  if (TAGMASK + 1 > tag_bound) {
    chpl_internal_error("MPI tag upper bound is too low");
  }

  chpl_comm_barrier("barrier in init");

  tag_count = (TAGMASK / chpl_numLocales) * chpl_localeID;
  chpl_sync_initAux(&termination_sync);
  chpl_sync_initAux(&tag_count_sync);
  pthread_key_create(&tag_count_key, NULL);
}


void chpl_comm_post_mem_init(void) { }


void chpl_comm_post_task_init(void) {
  if (chpl_task_createCommTask(chpl_mpi_polling_task, NULL))
    chpl_internal_error("unable to start polling task for MPI");
}


void chpl_comm_rollcall(void) {
  chpl_msg(2, "executing on locale %d of %d locale(s): %s\n", chpl_localeID,
            chpl_numLocales, chpl_localeName());
}


void chpl_comm_desired_shared_heap(void** start_p, size_t* size_p) {
  // Use the regular malloc/free implementation
  *start_p = NULL;
  *size_p  = 0;
}


void chpl_comm_alloc_registry(int numGlobals) {
  chpl_globals_registry = chpl_globals_registry_static;
}


void chpl_comm_broadcast_global_vars(int numGlobals) {
  int i;
  PRINTF("start broadcast globals");
  for (i = 0; i < numGlobals; i++) {
    void* buf;
    if (chpl_localeID == 0) buf = *(void**)chpl_globals_registry[i];
    MPI_SAFE(MPI_Bcast(&buf, sizeof(void*), MPI_BYTE, 0, MPI_COMM_WORLD));
    if (chpl_localeID != 0) {
      *(void**)chpl_globals_registry[i] = buf;
    }
  }
  PRINTF("end broadcast globals");
}


void chpl_comm_broadcast_private(int id, int32_t size, int32_t tid) {
  int i;
  PRINTF("broadcast private");

  for (i = 0; i < chpl_numLocales; i++) {
    if (i != chpl_localeID) {
      chpl_comm_put(chpl_private_broadcast_table[id], i, chpl_private_broadcast_table[id], size, -1, 1, 0, 0);
    }
  }
}


void chpl_comm_barrier(const char *msg) {
  PRINTF(msg);
  MPI_SAFE(MPI_Barrier(MPI_COMM_WORLD));
}

void chpl_comm_pre_task_exit(int all) {
  _chpl_mpi_message_info msg_info;

  if (all) {
    chpl_comm_barrier("chpl_comm_pre_task_exit");

    // Stop the polling task
    PRINTF("in chpl_comm_exit_all");
    msg_info.msg_type = ChplCommFinish;
    // To prevent MPI_Finalize from being called while the polling task
    // is still making MPI calls, send it a finish message, then wait for it
    // to release the termination_sync, which it will do only once no more
    // MPI Calls will be made.
    chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                  chpl_localeID, TAGMASK+1, MPI_COMM_WORLD);
    PRINTF("Sent shutdown message");
    chpl_sync_lock(&termination_sync);
    chpl_sync_unlock(&termination_sync);
  }
}


void chpl_comm_exit(int all, int status) {
  if (all) {
    chpl_comm_barrier("About to finalize");
    MPI_SAFE(MPI_Finalize());
  }
  else {
    MPI_SAFE(MPI_Abort(MPI_COMM_WORLD, status));
  }
}


void  chpl_comm_put(void* addr, int32_t locale, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn) {
  const int32_t size = elemSize*len;
  if (chpl_localeID == locale) {
    memmove(raddr, addr, size);
  } else {
    _chpl_mpi_message_info msg_info;
    int tag = makeTag();
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_put(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d)", chpl_localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif

    msg_info.msg_type = ChplCommPut;
    msg_info.replyTag = tag;
    msg_info.size = size;
    msg_info.u.data = raddr;

    chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                  locale, TAGMASK+1, MPI_COMM_WORLD);
    chpl_mpi_send(addr, size, MPI_BYTE, locale, tag, MPI_COMM_WORLD);
#if CHPL_DIST_DEBUG
    sprintf(debugMsg, "chpl_comm_put(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d) done", chpl_localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif
  }
}


void  chpl_comm_get(void *addr, int32_t locale, void* raddr,
                    int32_t elemSize, int32_t typeIndex, int32_t len,
                    int ln, chpl_string fn) {
  const int size = elemSize*len;
  if (chpl_localeID == locale) {
    memmove(addr, raddr, size);
  } else {
    _chpl_mpi_message_info msg_info;
    int tag = makeTag();
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_get(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d)", chpl_localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif

    msg_info.msg_type = ChplCommGet;
    msg_info.replyTag = tag;
    msg_info.size = size;
    msg_info.u.data = (void*)raddr;

    chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                  locale, TAGMASK+1, MPI_COMM_WORLD);
    chpl_mpi_recv(addr, size, MPI_BYTE, locale, tag, MPI_COMM_WORLD);
#if CHPL_DIST_DEBUG
    sprintf(debugMsg, "chpl_comm_get(loc=(%d, %p), rem=(%d, %p), size=%d, tag=%d) done", chpl_localeID, addr, locale, raddr, size, tag);
    PRINTF(debugMsg);
#endif
  }
}


void  chpl_comm_fork(int locale, chpl_fn_int_t fid, void *arg,
                     int32_t arg_size, int32_t arg_tid) {
  if (chpl_localeID == locale) {
    (*chpl_ftable[fid])(arg);
  } else {
    _chpl_mpi_message_info msg_info;
    int tag = makeTag();
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_fork(locale=%d, tag=%d)", locale, tag);
    PRINTF(debugMsg);
#endif

    msg_info.msg_type = ChplCommFork;
    msg_info.replyTag = tag;
    msg_info.size = arg_size;
    msg_info.u.fid = fid;

    chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                  locale, TAGMASK+1, MPI_COMM_WORLD);
    chpl_mpi_send(arg, arg_size, MPI_BYTE, locale, tag, MPI_COMM_WORLD);
    chpl_mpi_recv(NULL, 0, MPI_BYTE, locale, tag, MPI_COMM_WORLD);
  }
}


void  chpl_comm_fork_nb(int locale, chpl_fn_int_t fid, void *arg,
                        int32_t arg_size, int32_t arg_tid) {
  if (chpl_localeID == locale) {
    void* argCopy = chpl_mem_allocMany(1, arg_size, CHPL_RT_MD_COMM_FORK_SEND_NB_LARGE_ARG, 0, 0);
    _chplForkedTaskArg* rpcArg = chpl_mem_allocMany(1, sizeof(_chplForkedTaskArg), CHPL_RT_MD_COMM_FORK_SEND_NB_INFO, 0, 0);
    memmove(argCopy, arg, arg_size);
    rpcArg->fid = fid;
    rpcArg->arg = argCopy;
    rpcArg->replyTag = 0;
    rpcArg->joinLocale = 0;
    rpcArg->blockingCall = 0;
    chpl_task_begin((chpl_fn_p)chplExecForkedTask, rpcArg, false, false, NULL);
  } else {
    _chpl_mpi_message_info msg_info;
    int tag = makeTag();
#if CHPL_DIST_DEBUG
    char debugMsg[DEBUG_MSG_LENGTH];
    sprintf(debugMsg, "chpl_comm_fork_nb(locale=%d, tag=%d)", locale, tag);
    PRINTF(debugMsg);
#endif

    msg_info.msg_type = ChplCommForkNB;
    msg_info.u.fid = fid;
    msg_info.size = arg_size;
    msg_info.replyTag = tag;

    chpl_mpi_send(&msg_info, sizeof(_chpl_mpi_message_info), MPI_BYTE,
                  locale, TAGMASK+1, MPI_COMM_WORLD);
    chpl_mpi_send(arg, arg_size, MPI_BYTE, locale, tag, MPI_COMM_WORLD);

  }
}

// Just call chpl_comm_fork()
void  chpl_comm_fork_fast(int locale, chpl_fn_int_t fid, void *arg,
                          int32_t arg_size, int32_t arg_tid) {
  chpl_comm_fork(locale, fid, arg, arg_size, arg_tid);
}


int chpl_comm_numPollingTasks(void) { return 1; }

void chpl_startVerboseComm() { }
void chpl_stopVerboseComm() { }
void chpl_startVerboseCommHere() { }
void chpl_stopVerboseCommHere() { }

void chpl_startCommDiagnostics() { }
void chpl_stopCommDiagnostics() { }
void chpl_startCommDiagnosticsHere() { }
void chpl_stopCommDiagnosticsHere() { }

void chpl_resetCommDiagnosticsHere() { }
void chpl_getCommDiagnosticsHere(chpl_commDiagnostics *cd) {
  cd->put = -1;
  cd->get = -1;
  cd->get_nb = -1;
  cd->get_nb_test = -1;
  cd->get_nb_wait = -1;
  cd->fork = -1;
  cd->fork_fast = -1;
  cd->fork_nb = -1;
}

uint64_t chpl_numCommGets(void) { return -1; }
uint64_t chpl_numCommNBGets(void) { return -1; }
uint64_t chpl_numCommTestNBGets(void) { return -1; }
uint64_t chpl_numCommWaitNBGets(void) { return -1; }
uint64_t chpl_numCommPuts(void) { return -1; }
uint64_t chpl_numCommForks(void) { return -1; }
uint64_t chpl_numCommFastForks(void) { return -1; }
uint64_t chpl_numCommNBForks(void) { return -1; }


/*
 *  work to try to make a non-thread-safe version of MPI work
 */
/* static MPI_Status chpl_mpi_spin_recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm) { */
/*   int flag; */
/*   MPI_Status status; */
/*   MPI_Request request; */
/* #if CHPL_DIST_DEBUG */
/*   char debugMsg[DEBUG_MSG_LENGTH]; */
/*   sprintf(debugMsg, "chpl_mpi_spin_recv(%p, size=%d, from=%d, tag=%d)", buf, count, source, tag); */
/*   PRINTF(debugMsg); */
/* #endif */
/*   MPI_SAFE(MPI_Irecv(buf, count, datatype, source, tag, comm, &request)); */
/*   do { */
/*     MPI_SAFE(MPI_Test(&request, &flag, &status)); */
/*   } while (flag != 1); */
/* #if CHPL_DIST_DEBUG */
/*   sprintf(debugMsg, "chpl_mpi_spin_recv(%p, size=%d, from=%d, tag=%d) done", buf, count, source, tag); */
/*   PRINTF(debugMsg); */
/* #endif */
/*   return status; */
/* } */


/* static void chpl_mpi_spin_send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) { */
/*   int flag; */
/*   MPI_Status status; */
/*   MPI_Request request; */
/* #if CHPL_DIST_DEBUG */
/*   char debugMsg[DEBUG_MSG_LENGTH]; */
/*   sprintf(debugMsg, "chpl_mpi_spin_send(%p, size=%d, to=%d, tag=%d)", buf, count, dest, tag); */
/*   PRINTF(debugMsg); */
/* #endif */
/*   MPI_SAFE(MPI_Issend(buf, count, datatype, dest, tag, comm, &request)); */
/*   do { */
/*     MPI_SAFE(MPI_Test(&request, &flag, &status)); */
/*   } while (!flag); */
/* #if CHPL_DIST_DEBUG */
/*   sprintf(debugMsg, "chpl_mpi_spin_send(%p, size=%d, to=%d, tag=%d) done", buf, count, dest, tag); */
/*   PRINTF(debugMsg); */
/* #endif */
/* } */
