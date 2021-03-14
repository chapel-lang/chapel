/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testmpi.c $
 * Description: General GASNet correctness tests
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#if _FORTIFY_SOURCE > 0 && __OPTIMIZE__ <= 0 /* silence an annoying MPICH/Linux warning */
#undef _FORTIFY_SOURCE
#endif

#include <mpi.h>

#define TEST_MPI 1
#include "testthreads.c"

static char *MPI_ErrorName(int errval) {
  const char *code = NULL;
  char systemErrDesc[MPI_MAX_ERROR_STRING+10];
  int len = MPI_MAX_ERROR_STRING;
  static char msg[MPI_MAX_ERROR_STRING+100];
  switch (errval) {
    case MPI_ERR_BUFFER:    code = "MPI_ERR_BUFFER"; break;
    case MPI_ERR_COUNT:     code = "MPI_ERR_COUNT"; break;
    case MPI_ERR_TYPE:      code = "MPI_ERR_TYPE"; break;
    case MPI_ERR_TAG:       code = "MPI_ERR_TAG"; break;
    case MPI_ERR_COMM:      code = "MPI_ERR_COMM"; break;
    case MPI_ERR_RANK:      code = "MPI_ERR_RANK"; break;
    case MPI_ERR_REQUEST:   code = "MPI_ERR_REQUEST"; break;
    case MPI_ERR_ROOT:      code = "MPI_ERR_ROOT"; break;
    case MPI_ERR_GROUP:     code = "MPI_ERR_GROUP"; break;
    case MPI_ERR_OP:        code = "MPI_ERR_OP"; break;
    case MPI_ERR_TOPOLOGY:  code = "MPI_ERR_TOPOLOGY"; break;
    case MPI_ERR_DIMS:      code = "MPI_ERR_DIMS"; break;                                    
    case MPI_ERR_ARG:       code = "MPI_ERR_ARG"; break;
    case MPI_ERR_UNKNOWN:   code = "MPI_ERR_UNKNOWN"; break;
    case MPI_ERR_TRUNCATE:  code = "MPI_ERR_TRUNCATE"; break;
    case MPI_ERR_OTHER:     code = "MPI_ERR_OTHER"; break;
    case MPI_ERR_INTERN:    code = "MPI_ERR_INTERN"; break;
    case MPI_ERR_PENDING:   code = "MPI_ERR_PENDING"; break;
    case MPI_ERR_IN_STATUS: code = "MPI_ERR_IN_STATUS"; break;
    case MPI_ERR_LASTCODE:  code = "MPI_ERR_LASTCODE";  break;
    default: code = "*unknown MPI error*";
    }
  if (MPI_Error_string(errval, systemErrDesc, &len) != MPI_SUCCESS || len == 0)
    strcpy(systemErrDesc, "(no description available)");
  snprintf(msg, sizeof(msg), "%s(%i): %s", code, errval, systemErrDesc);
  return msg;
  }

#define MPI_SAFE(fncall) do {                                                                     \
   int retcode = (fncall);                                                                        \
   if_pf (retcode != MPI_SUCCESS) {                                                               \
     char msg[1024];                                                                              \
     snprintf(msg, sizeof(msg), "\ntestmpi encountered an MPI ERROR: %s(%i)\n", MPI_ErrorName(retcode), retcode); \
   }                                                                                              \
 } while (0)

void dump_args(int argc, char **argv)  { 
    int i;
    printf("my args: argc=%i argv=[", argc);
    for (i=0; i < argc; i++) {
      printf("%s'%s'",(i>0?" ":""),argv[i]);
    }
    printf("]\n"); fflush(stdout);
  }


#ifndef HAVE_MPI_INIT_THREAD
#define HAVE_MPI_INIT_THREAD (MPI_VERSION >= 2)
#endif
#ifndef HAVE_MPI_QUERY_THREAD
#define HAVE_MPI_QUERY_THREAD (MPI_VERSION >= 2)
#endif

/* called by a single thread before gasnet_init */
void init_test_mpi(int *argc, char ***argv) {
    int initialized = 0;

    #if 0
      dump_args(*argc, *argv);
    #endif

    /* initialize MPI, if necessary */
    MPI_SAFE(MPI_Initialized(&initialized));
    #if !HAVE_MPI_INIT_THREAD
      if (!initialized) {
        printf("Initializing MPI... (legacy MPI-1 mode)\n");
        MPI_SAFE(MPI_Init(argc, argv)); // legacy init
      }
    #else
      #ifndef MPI_THREAD_REQUIRE
        #if GASNET_SEQ // technically should also assert !CONDUIT_THREADS
          #define MPI_THREAD_REQUIRE MPI_THREAD_SINGLE
        #elif GASNET_CONDUIT_MPI || CONDUIT_USES_MPI
          // bug 3521: private mutex means we need full thread safety if conduit uses MPI in steady-state
          #define MPI_THREAD_REQUIRE MPI_THREAD_MULTIPLE 
        #else
          #define MPI_THREAD_REQUIRE MPI_THREAD_SERIALIZED // this test is only MPI consumer
        #endif
      #endif
      int required = MPI_THREAD_REQUIRE;
      int provided = -1;
      if (!initialized) {
        printf("Initializing MPI...\n");
        MPI_SAFE(MPI_Init_thread(argc, argv, required, &provided));
      } else {
        #if HAVE_MPI_QUERY_THREAD
          MPI_SAFE(MPI_Query_thread(&provided));
        #else
          provided = required; // hope for the best
        #endif
      }
      #if !VERBOSE
      if (provided < required) 
      #endif
      { 
        printf("MPI Thread safety: required=%i provided=%i    (%i=SINGLE %i=FUNNELED %i=SERIALIZED %i=MULTIPLE)\n",
               required, provided, 
               MPI_THREAD_SINGLE, MPI_THREAD_FUNNELED, MPI_THREAD_SERIALIZED, MPI_THREAD_MULTIPLE);
      }
      if (provided < required) { 
        fprintf(stderr,"ERROR: MPI implementation does not report sufficient thread safety to correctly run this test.\n");
        abort(); // cannot use FATALERR this early
      }
    #endif

    #if 0
      dump_args(*argc, *argv);
    #endif

    MPI_SAFE(MPI_Barrier(MPI_COMM_WORLD));
}

int *mpirank_to_gasnetnode = NULL;
int *gasnetnode_to_mpirank = NULL;

MPI_Request *mpi_recvhandle;
MPI_Request *mpi_sendhandle;
char **mpi_buf;
int *mpi_bufsz;

/* called by a single thread after gasnet_attach and args parsing */
void attach_test_mpi(void) {
    int mpirank;
    int gasnet_node;
    int mpinodes;
    int tot_threads;
    int i;
    MPI_SAFE(MPI_Barrier(MPI_COMM_WORLD));

    /* setup gasnetnode <=> mpi mpirank mappings */
    gasnetnode_to_mpirank = test_malloc(sizeof(int)*numranks);
    mpirank_to_gasnetnode = test_malloc(sizeof(int)*numranks);

    MPI_SAFE(MPI_Comm_rank(MPI_COMM_WORLD, &mpirank));
    MPI_SAFE(MPI_Comm_size(MPI_COMM_WORLD, &mpinodes));
    printf("GASNet node %i == MPI node %i\n", (int)myrank, mpirank);
    if (myrank != mpirank) 
      printf("WARNING: Node numbering between GASNet and MPI do not coincide\n");
    assert_always(mpinodes == numranks && mpirank >= 0 && mpirank < mpinodes);
    gasnet_node = myrank;
    MPI_SAFE(MPI_Allgather(&gasnet_node,sizeof(int),MPI_BYTE,
                           mpirank_to_gasnetnode,sizeof(int),MPI_BYTE,
                           MPI_COMM_WORLD));
    assert_always(mpirank_to_gasnetnode[mpirank] == myrank);
    for (i = 0; i < mpinodes; i++) gasnetnode_to_mpirank[i] = -1;
    for (i = 0; i < mpinodes; i++) gasnetnode_to_mpirank[mpirank_to_gasnetnode[i]] = i;
    for (i = 0; i < mpinodes; i++) assert_always(gasnetnode_to_mpirank[i] != -1);

    tot_threads = threads_num * numranks;
    mpi_recvhandle = test_malloc(sizeof(MPI_Request)*tot_threads);
    mpi_sendhandle = test_malloc(sizeof(MPI_Request)*tot_threads);
    mpi_buf = test_malloc(sizeof(char *)*tot_threads);
    mpi_bufsz = test_malloc(sizeof(int)*tot_threads);
    for (i = 0; i < tot_threads; i++) {
      mpi_recvhandle[i] = MPI_REQUEST_NULL;
      mpi_sendhandle[i] = MPI_REQUEST_NULL;
      mpi_buf[i] = NULL;
      mpi_bufsz[i] = 0;
    }

    MPI_SAFE(MPI_Barrier(MPI_COMM_WORLD));
}

void mpi_barrier(threaddata_t *tdata) {
#if GASNET_PAR
  static gasnett_mutex_t  barrier_mutex = GASNETT_MUTEX_INITIALIZER;
  static gasnett_cond_t   barrier_cond = GASNETT_COND_INITIALIZER;
  static volatile int     barrier_count = 0;
  static int volatile phase = 0;
  gasnett_mutex_lock(&barrier_mutex);
  barrier_count++;
  if (barrier_count < threads_num) {
    int myphase = phase;
    while (myphase == phase) {
      gasnett_cond_wait(&barrier_cond, &barrier_mutex);
    }
  } else {
    /* All threads here - now do the MPI barrier */
    MPI_SAFE(MPI_Barrier(MPI_COMM_WORLD));
    barrier_count = 0;
    phase = !phase;
    gasnett_cond_broadcast(&barrier_cond);
  }
  gasnett_mutex_unlock(&barrier_mutex);
#else
  MPI_SAFE(MPI_Barrier(MPI_COMM_WORLD));
#endif
}

#if MPI_THREADSAFE
  #define MPI_LOCK()
  #define MPI_UNLOCK()
#else
  static gex_HSL_t  mpi_hsl = GEX_HSL_INITIALIZER;
  #define MPI_LOCK()   gex_HSL_Lock(&mpi_hsl)
  #define MPI_UNLOCK() gex_HSL_Unlock(&mpi_hsl)
#endif


void mpi_handler(gex_Token_t token, harg_t tid, harg_t sz) {
  int mpipeer;
  int tag;
  char *buf;
  gex_Rank_t node = test_msgsource(token);

  PRINT_AM(("node=%2d> AMShort MPI Request for tid=%i, nbytes=%i\n",
            (int)myrank, (int)tid, (int)sz));
  assert(tt_thread_map[tid] == node);
  assert(sz > 0);
  mpipeer = gasnetnode_to_mpirank[node];
  tag = tid;
  buf = (char*)test_malloc(sz);

  MPI_LOCK();

    assert(mpi_buf[tid] == NULL);
    assert(mpi_recvhandle[tid] == MPI_REQUEST_NULL);
    assert(mpi_sendhandle[tid] == MPI_REQUEST_NULL);
    mpi_buf[tid] = buf;
    mpi_bufsz[tid] = sz;

    ACTION_PRINTF("node=%2d> setting MPI_Irecv, %i bytes\n", (int)myrank, (int)sz);
    MPI_SAFE(MPI_Irecv(mpi_buf[tid], sz, MPI_BYTE, mpipeer, tag, MPI_COMM_WORLD, &(mpi_recvhandle[tid])));
    assert(mpi_recvhandle[tid] != MPI_REQUEST_NULL);
          
  MPI_UNLOCK();

}

void mpi_probehandler(gex_Token_t token, harg_t tid) {
  int mpipeer;
  int tag;
  int reply = 0;
  gex_Rank_t node = test_msgsource(token);
  assert(tt_thread_map[tid] == node);
  mpipeer = gasnetnode_to_mpirank[node];
  tag = tid;

  MPI_LOCK();
    if (mpi_recvhandle[tid] != MPI_REQUEST_NULL) {
      MPI_Status status;
      int flag = 0;
      MPI_SAFE(MPI_Test(&mpi_recvhandle[tid],&flag,&status));
      if (flag) {
        int sz = mpi_bufsz[tid];
        assert(mpi_recvhandle[tid] == MPI_REQUEST_NULL);
        assert(mpi_sendhandle[tid] == MPI_REQUEST_NULL);
        assert(mpi_buf[tid] != NULL && sz >= 0);
        ACTION_PRINTF("node=%2d> sending MPI reply message, %i bytes\n", (int)myrank, sz);
        MPI_SAFE(MPI_Isend(mpi_buf[tid], sz, MPI_BYTE, mpipeer, 10000+tag, MPI_COMM_WORLD, &(mpi_sendhandle[tid])));
        assert(mpi_sendhandle[tid] != MPI_REQUEST_NULL);
      } 
    } else if (mpi_sendhandle[tid] != MPI_REQUEST_NULL) {
      MPI_Status status;
      int flag = 0;
      MPI_SAFE(MPI_Test(&mpi_sendhandle[tid],&flag,&status));
      if (flag) {
        assert(mpi_recvhandle[tid] == MPI_REQUEST_NULL);
        assert(mpi_sendhandle[tid] == MPI_REQUEST_NULL);
        reply = 1;
      }
    } else {
      /* nothing to do */ 
    }
  MPI_UNLOCK();

  if (reply) {
    assert(mpi_buf[tid] != NULL);
    test_free(mpi_buf[tid]);
    mpi_buf[tid] = NULL;
    PRINT_AM(("node=%2d> Sending AMShort MPI Reply for tid=%i\n",
            (int)myrank, (int)tid));
    gex_AM_ReplyShort1(token, hidx_mpi_replyhandler, 0, tid);
  }
}

void mpi_replyhandler(gex_Token_t token, harg_t tid) {
  int ltid = tid - myrank*threads_num;
  PRINT_AM(("node=%2d> Got AMShort MPI Reply for tid=%d\n",
                        (int)myrank, (int)tid));
  assert(tt_thread_map[tid] == myrank);
  tt_thread_data[ltid].flag = 0;
}

int mpi_test(MPI_Request *handle) {
  MPI_Status status;
  int flag = 0;

  if (*handle == MPI_REQUEST_NULL) return 1;

  MPI_LOCK();

    MPI_SAFE(MPI_Test(handle,&flag,&status));

  MPI_UNLOCK();

  if (flag) return 1;
  else return 0;
}

/* spin-poll until a request is complete */
void mpi_complete(MPI_Request *handle) {
  while (1) { 
    if (mpi_test(handle)) return;

    GASNET_Safe(gasnet_AMPoll());
  }
}

void test_mpi(threaddata_t *tdata) {
    MPI_Request sendhandle = MPI_REQUEST_NULL;
    MPI_Request recvhandle = MPI_REQUEST_NULL;
    int peer = tdata->tid_peer;
    int node = tt_thread_map[peer];
    int mpipeer = gasnetnode_to_mpirank[node];
    int sz;
    char *sendbuf;
    char *recvbuf;
    int tag = tdata->tid;
    int i;

    do { sz = RANDOM_SIZE(); } while (sz == 0); /* some MPI's may barf on 0 byte send/recv */
    sendbuf = (char*)test_malloc(sz);
    recvbuf = (char*)test_malloc(sz);

    for (i=0; i < MIN(sz,4096); i++) { /* randomize at least the first 4 KB */
      sendbuf[i] = (char)rand();
    }

    ACTION_PRINTF("tid=%3d> starting MPI ping-pong with tid=%3d.\n", tdata->tid, peer);

    MPI_LOCK();

      ACTION_PRINTF("tid=%3d> setting MPI_Irecv, %i bytes\n", tdata->tid, sz);
      MPI_SAFE(MPI_Irecv(recvbuf, sz, MPI_BYTE, mpipeer, 10000+tag, MPI_COMM_WORLD, &recvhandle));
      assert(recvhandle != MPI_REQUEST_NULL);

      ACTION_PRINTF("tid=%3d> sending MPI message, %i bytes\n", tdata->tid, sz);
      MPI_SAFE(MPI_Isend(sendbuf, sz, MPI_BYTE, mpipeer, tag, MPI_COMM_WORLD, &sendhandle));
      assert(sendhandle != MPI_REQUEST_NULL);

    MPI_UNLOCK();


    tdata->flag = -1;
    gasnett_local_wmb();
    ACTION_PRINTF("tid=%3d> MPI AMShortRequest to tid=%3d\n", tdata->tid, peer);
    gex_AM_RequestShort2(myteam, node, hidx_mpi_handler, 0, tdata->tid, sz);

    while (tdata->flag != 0) {
      ACTION_PRINTF("tid=%3d> MPI probe AMShortRequest to tid=%3d\n", tdata->tid, peer);
      gex_AM_RequestShort1(myteam, node, hidx_mpi_probehandler, 0, tdata->tid);

      gasnett_sched_yield();
      test_sleep(tdata);
      GASNET_Safe(gasnet_AMPoll());
      mpi_test(&sendhandle); /* occasional testing may be required for progress */
      mpi_test(&recvhandle);
    }
    tdata->flag = -1;

    mpi_complete(&sendhandle);
    mpi_complete(&recvhandle);

    /* verify */
    for (i=0; i < sz; i++) {
      if (sendbuf[i] != recvbuf[i])
        FATALERR("mismatch at element %i in MPI test.", i);
    }

    test_free(sendbuf);
    test_free(recvbuf);

    ACTION_PRINTF("tid=%3d> MPI ping-pong with tid=%3d complete.\n", tdata->tid, peer);

  }

