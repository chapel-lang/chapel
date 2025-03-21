/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/mpi-spawner/gasnet_bootstrap_mpi.c $
 * Description: GASNet conduit-independent mpi-based spawner
 * Copyright 2003, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */


#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <signal.h>
#include <mpi.h>

#if (MPI_VERSION > 2) || (MPI_VERSION == 2 && MPI_SUBVERSION > 1)
#  define GASNETC_MPI_ALLGATHER_IN_PLACE 1
#  define GASNETC_MPI_ALLTOALL_IN_PLACE  1
#elif MPI_VERSION == 2
#  define GASNETC_MPI_ALLGATHER_IN_PLACE 1
#  define GASNETC_MPI_ALLTOALL_IN_PLACE  0
#else
#  define GASNETC_MPI_ALLGATHER_IN_PLACE 0
#  define GASNETC_MPI_ALLTOALL_IN_PLACE  0
#endif

static MPI_Comm gasnetc_mpi_comm;
static int gasnetc_mpi_preinitialized = 0;
static int gasnetc_mpi_size = -1;
static int gasnetc_mpi_rank = -1;

static int is_verbose = 0;

GASNETI_IDENT(gasnetc_IdentString_HaveMPISpawner, "$GASNetMPISpawner: 1 $");

static gasneti_spawnerfn_t const spawnerfn;

#ifndef HAVE_MPI_INIT_THREAD
#define HAVE_MPI_INIT_THREAD (MPI_VERSION >= 2)
#endif
#ifndef HAVE_MPI_QUERY_THREAD
#define HAVE_MPI_QUERY_THREAD (MPI_VERSION >= 2)
#endif
#ifndef GASNET_MPI_THREAD_STRICT
#define GASNET_MPI_THREAD_STRICT 0  // strictly adhere to the MPI threading specification
#endif

static int threadstr2int(const char *str) {
  char tmp[80];
  char *p;
  strncpy(tmp, str, sizeof(tmp)-1);
  for (p = tmp; *p; p++) if (*p >= 'a' && *p <= 'z') *p -= 'a'-'A'; /* upper-case */
  int ret = -1;
  #if HAVE_MPI_INIT_THREAD
    if (strstr(tmp,"SINGLE"))     return MPI_THREAD_SINGLE;
    if (strstr(tmp,"FUNNELED"))   return MPI_THREAD_FUNNELED;
    if (strstr(tmp,"SERIALIZED")) return MPI_THREAD_SERIALIZED;
    if (strstr(tmp,"MULTIPLE"))   return MPI_THREAD_MULTIPLE;
    ret = MPI_THREAD_SINGLE-1;
  #endif
  return ret;
}
static const char *threadint2str(int id) {
  switch (id) {
    #if HAVE_MPI_INIT_THREAD
      case MPI_THREAD_SINGLE:     return "MPI_THREAD_SINGLE";
      case MPI_THREAD_FUNNELED:   return "MPI_THREAD_FUNNELED";
      case MPI_THREAD_SERIALIZED: return "MPI_THREAD_SERIALIZED";
      case MPI_THREAD_MULTIPLE:   return "MPI_THREAD_MULTIPLE";
    #endif
      default: return "UNKNOWN VALUE";
  }
}

// check whether MPI_Finalize has already occurred and we should bail out
GASNETI_INLINE(check_early_finalize)
void check_early_finalize(void) {
#if (MPI_VERSION > 1)
  int isfini = 0;
  int err = MPI_Finalized(&isfini);
  gasneti_assert_always(err == MPI_SUCCESS);
  if (isfini) {
    // MPI specifies a very narrow range of guaranteed functionality after finalize, 
    // which notably does NOT include MPI_Abort. 
    // We cannot complete this client request, so just warn and hard-exit the process
    static char message[] = 
      "GASNet mpi-spawner noticed that MPI has been finalized by the client. The process will now exit.";
    if (gasneti_spawn_verbose) gasneti_console_message("EXIT STATE","%s",message);
    if (gasneti_getenv_yesno_withdefault("GASNET_QUIET",0))
      GASNETI_TRACE_PRINTF(I,("*** WARNING: %s",message));
    else
      gasneti_console0_message("WARNING","%s",message);

    gasneti_flush_streams();
    int exitcode = (int)gasneti_atomic_read(&gasneti_exit_code, 0);
    gasneti_killmyprocess(exitcode);
  }
#endif
}

extern gasneti_spawnerfn_t const *gasneti_bootstrapInit_mpi(int *argc, char ***argv, gex_Rank_t *nodes, gex_Rank_t *mynode) {
  MPI_Group world;

  // environment may or may not be propagated yet, so this is "best effort"
  is_verbose = gasneti_getenv_yesno_withdefault("GASNET_SPAWN_VERBOSE",0);

  /* Call MPI_Init exactly once */
  int err = MPI_Initialized(&gasnetc_mpi_preinitialized);
  if (MPI_SUCCESS != err) return NULL;
  if (is_verbose) gasneti_console_message("MPI-SPAWNER","MPI %s already initialized",
                                          (gasnetc_mpi_preinitialized ? "IS" : "IS NOT"));
#if !HAVE_MPI_INIT_THREAD
  if (!gasnetc_mpi_preinitialized) {
  #if MPI_VERSION < 2
    if (!argc || !argv) return NULL;
  #endif
    if (is_verbose) gasneti_console_message("MPI-SPAWNER","MPI_Init()");
    err = MPI_Init(argc, argv);
    if (MPI_SUCCESS != err) return NULL;
  }
#else /* HAVE_MPI_INIT_THREAD */
  // Setup/verify the threading mode
  // By default we assume mpi-spawner is the only conduit-level client of MPI
  // If the process is fully single-threaded, we should be using THREAD_SINGLE
  // If other threads exist that don't call mpi-spawner/MPI then THREAD_FUNNELED
  // If other threads might call mpi-spawner (eg fini/exit) then THREAD_SERIALIZED
  // Clients who want to make asynchronous calls to MPI directly should override our thread mode to THREAD_MULTIPLE
  #if GASNET_MPI_THREAD_STRICT
    #if GASNETI_THREADS
      int required = MPI_THREAD_SERIALIZED;
    #elif _REENTRANT
      int required = MPI_THREAD_FUNNELED;
    #else
      int required = MPI_THREAD_SINGLE;
    #endif
  #else
    int required = MPI_THREAD_SINGLE; // temporarily loosened for backwards compatibility
  #endif
    int provided = -1;
    const char *override = gasneti_getenv_withdefault("GASNET_MPI_THREAD",threadint2str(required));
    if (override) {
      int overreq = threadstr2int(override);
      if (overreq >= MPI_THREAD_SINGLE) required = overreq;
      else gasneti_console_message("WARNING","Ignoring unrecognized GASNET_MPI_THREAD value: %s",override);
    }
    if (gasnetc_mpi_preinitialized) {  // MPI already init, query current thread support level
      #if HAVE_MPI_QUERY_THREAD
        if (is_verbose) gasneti_console_message("MPI-SPAWNER","MPI_Query_thread()");
        MPI_Query_thread(&provided);
        // deliberately ignore errors on query
      #else
        provided = required;
      #endif
    } else { // init MPI and request our needed level of thread safety
      if (is_verbose)
        gasneti_console_message("MPI-SPAWNER","MPI_Init_thread(%s)",threadint2str(required));
      err = MPI_Init_thread(argc, argv, required, &provided);
      if (err != MPI_SUCCESS) return NULL;
    }
    if (is_verbose)
      gasneti_console_message("MPI-SPAWNER","MPI threading mode: %s required, %s provided.",
                     threadint2str(required), threadint2str(provided));
    if (provided < required) {
      gasneti_console_message("MPI-SPAWNER",
                     "WARNING: GASNet requested MPI threading support model: %s\n"
                     "    but the MPI library only provided: %s\n"
                     "    You may need to link a more thread-safe MPI library to ensure correct operation.\n"
                     "    You can override the required level by setting GASNET_MPI_THREAD.",
                     threadint2str(required), threadint2str(provided)
             );
    } 
#endif

  /* Create private communicator */
  err = MPI_Comm_group(MPI_COMM_WORLD, &world);
  gasneti_assert_always(err == MPI_SUCCESS);
  err = MPI_Comm_create(MPI_COMM_WORLD, world, &gasnetc_mpi_comm);
  gasneti_assert_always(err == MPI_SUCCESS);
  err = MPI_Group_free(&world);
  gasneti_assert_always(err == MPI_SUCCESS);

  /* Get size and rank */
  err = MPI_Comm_size(gasnetc_mpi_comm, &gasnetc_mpi_size);
  gasneti_assert_always(err == MPI_SUCCESS);
  *nodes = gasnetc_mpi_size;
  if ((int)(*nodes) != gasnetc_mpi_size) *nodes = 0; /* Overflow! */

  err = MPI_Comm_rank(gasnetc_mpi_comm, &gasnetc_mpi_rank);
  gasneti_assert_always(err == MPI_SUCCESS);
  *mynode = gasnetc_mpi_rank;

  gasneti_setupGlobalEnvironment(*nodes, *mynode,
				 spawnerfn.Exchange,
				 spawnerfn.Broadcast);

  return &spawnerfn;
}

static void bootstrapFini(void) {
  int err;

#if (MPI_VERSION > 1)
  /* Check to see if MPI is already finalized, for instance on an error path. */
  int isfini = 0;
  err = MPI_Finalized(&isfini);
  gasneti_assert_always(err == MPI_SUCCESS);
  if (isfini) return;
#endif

  err = MPI_Comm_free(&gasnetc_mpi_comm);
  gasneti_assert_always(err == MPI_SUCCESS);

  /* In most cases it appears that calling MPI_Finalize() will
   * prevent us from propagating the exit code to the spawner.
   * However, as seen w/ mpich-1.2.5, the alternative is to
   * hang on exit, which is no alternative at all.
   */
  if (gasneti_getenv_yesno_withdefault("GASNET_MPI_FINALIZE", 1)) {
    if (is_verbose) gasneti_console_message("MPI-SPAWNER","MPI_Finalize()");
    (void) MPI_Finalize();
  }
}

static void bootstrapAbort(int exitcode) {
  if (is_verbose) gasneti_console_message("MPI-SPAWNER","MPI_Abort(%i)",exitcode);
  check_early_finalize();
  (void) MPI_Abort(gasnetc_mpi_comm, exitcode);

  gasneti_reghandler(SIGABRT, SIG_DFL);
  gasneti_fatalerror("gasneti_bootstrapAbort_mpi aborting...");
  /* NOT REACHED */
}

static void bootstrapBarrier(void) {
  check_early_finalize();
  int err = MPI_Barrier(gasnetc_mpi_comm);
  gasneti_assert_always(err == MPI_SUCCESS);
}

static void bootstrapExchange(void *src, size_t len, void *dest) {
  check_early_finalize();
  const int inplace = ((uint8_t *)src == (uint8_t *)dest + len * gasnetc_mpi_rank);
  int err;

  if (inplace) {
#if GASNETC_MPI_ALLGATHER_IN_PLACE
    src = MPI_IN_PLACE;
#else
    void *tmp = gasneti_malloc(len);
    GASNETI_MEMCPY(tmp, src, len);
    src = tmp;
#endif
  }

  err = MPI_Allgather(src, len, MPI_BYTE, dest, len, MPI_BYTE, gasnetc_mpi_comm);
  gasneti_assert_always(err == MPI_SUCCESS);

#if !GASNETC_MPI_ALLGATHER_IN_PLACE
  if (inplace) gasneti_free(src);
#endif
}

static void bootstrapAlltoall(void *src, size_t len, void *dest) {
  check_early_finalize();
  const int inplace = (src == dest);
  int err;

  if (inplace) {
#if GASNETC_MPI_ALLTOALL_IN_PLACE
    src = MPI_IN_PLACE;
#else
    const size_t total_len = len * gasnetc_mpi_size;
    void *tmp = gasneti_malloc(total_len);
    GASNETI_MEMCPY(tmp, src, total_len);
    src = tmp;
#endif
  }

  err = MPI_Alltoall(src, len, MPI_BYTE, dest, len, MPI_BYTE, gasnetc_mpi_comm);
  gasneti_assert_always(err == MPI_SUCCESS);

#if !GASNETC_MPI_ALLTOALL_IN_PLACE
  if (inplace) gasneti_free(src);
#endif
}

static void bootstrapBroadcast(void *src, size_t len, void *dest, int rootnode) {
  check_early_finalize();
  int err;
  
  if (gasnetc_mpi_rank == rootnode) {
    memmove(dest, src, len);
  }
  err = MPI_Bcast(dest, len, MPI_BYTE, rootnode, gasnetc_mpi_comm);
  gasneti_assert_always(err == MPI_SUCCESS);
}

// Broadcast to members of a gasneti_nodegrp_t via Isend/Irecv
static void nodegrpBcast(void *src, size_t len, void *dest, int rootnode, gasneti_nodegrp_t *scope)
{
  check_early_finalize();

  gex_Rank_t *scope_ranks = scope->nodes;
  gex_Rank_t self = scope->node_rank;
  int scope_count = scope->node_count;
  int err;

  if (gasnetc_mpi_rank == rootnode) {
    const int count = scope_count - 1;
    MPI_Request *reqs, *r;
    int i;

    memmove(dest, src, len);
    reqs = gasneti_malloc(count * sizeof(MPI_Request));
    for (i = 0, r = reqs; i < scope_count; ++i) {
      if (i == self) continue;
      err = MPI_Isend(src,len,MPI_BYTE,scope_ranks[i],0,gasnetc_mpi_comm,r++);
      gasneti_assert_always(err == MPI_SUCCESS);
    }
    err = MPI_Waitall(count,reqs,MPI_STATUSES_IGNORE);
    gasneti_assert_always(err == MPI_SUCCESS);
    gasneti_free(reqs);
  } else {
    err = MPI_Recv(dest,len,MPI_BYTE,rootnode,0,gasnetc_mpi_comm,MPI_STATUS_IGNORE);
    gasneti_assert_always(err == MPI_SUCCESS);
  }
}

static void bootstrapNbrhdBroadcast(void *src, size_t len, void *dest, int rootnode) {
  nodegrpBcast(src, len, dest, rootnode, &gasneti_mysupernode);
}

static void bootstrapHostBroadcast(void *src, size_t len, void *dest, int rootnode) {
  nodegrpBcast(src, len, dest, rootnode, &gasneti_myhost);
}

static void bootstrapCleanup(void) {
  /* Nothing to do here */
}

static gasneti_spawnerfn_t const spawnerfn = {
  bootstrapBarrier,
  bootstrapExchange,
  bootstrapBroadcast,
  bootstrapNbrhdBroadcast,
  bootstrapHostBroadcast,
  bootstrapAlltoall,
  bootstrapAbort,
  bootstrapCleanup,
  bootstrapFini,
};

