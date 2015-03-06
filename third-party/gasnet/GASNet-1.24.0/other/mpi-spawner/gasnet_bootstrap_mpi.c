/*   $Source: bitbucket.org:berkeleylab/gasnet.git/other/mpi-spawner/gasnet_bootstrap_mpi.c $
 * Description: GASNet conduit-independent mpi-based spawner
 * Copyright 2003, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */


#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <mpi-spawner/gasnet_bootstrap_internal.h>
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

int gasneti_bootstrapInit_mpi(int *argc, char ***argv, gasnet_node_t *nodes, gasnet_node_t *mynode) {
  MPI_Group world;
  int err;

  /* Call MPI_Init exactly once */
  err = MPI_Initialized(&gasnetc_mpi_preinitialized);
  if (MPI_SUCCESS != err) return GASNET_ERR_NOT_INIT;
  if (!gasnetc_mpi_preinitialized) {
    err = MPI_Init(argc, argv);
    if (MPI_SUCCESS != err) return GASNET_ERR_NOT_INIT;
  }

  /* Create private communicator */
  err = MPI_Comm_group(MPI_COMM_WORLD, &world);
  gasneti_assert(err == MPI_SUCCESS);
  err = MPI_Comm_create(MPI_COMM_WORLD, world, &gasnetc_mpi_comm);
  gasneti_assert(err == MPI_SUCCESS);
  err = MPI_Group_free(&world);
  gasneti_assert(err == MPI_SUCCESS);

  /* Get size and rank */
  err = MPI_Comm_size(gasnetc_mpi_comm, &gasnetc_mpi_size);
  gasneti_assert(err == MPI_SUCCESS);
  *nodes = gasnetc_mpi_size;
  if ((int)(*nodes) != gasnetc_mpi_size) *nodes = 0; /* Overflow! */

  err = MPI_Comm_rank(gasnetc_mpi_comm, &gasnetc_mpi_rank);
  gasneti_assert(err == MPI_SUCCESS);
  *mynode = gasnetc_mpi_rank;

  gasneti_setupGlobalEnvironment(*nodes, *mynode,
				 &gasneti_bootstrapExchange_mpi,
				 &gasneti_bootstrapBroadcast_mpi);

  return GASNET_OK;
}

void gasneti_bootstrapFini_mpi(void) {
  int err;

  err = MPI_Comm_free(&gasnetc_mpi_comm);
  gasneti_assert(err == MPI_SUCCESS);

  /* In most cases it appears that calling MPI_Finalize() will
   * prevent us from propagating the exit code to the spawner.
   * However, as seen w/ mpich-1.2.5, the alternative is to
   * hang on exit, which is no alternative at all.
   */
  if (!gasnetc_mpi_preinitialized) {
    (void) MPI_Finalize();
  }
}

void gasneti_bootstrapAbort_mpi(int exitcode) {
  (void) MPI_Abort(gasnetc_mpi_comm, exitcode);

  gasneti_reghandler(SIGABRT, SIG_DFL);
  gasneti_fatalerror("gasneti_bootstrapAbort_mpi aborting...");
  /* NOT REACHED */
}

void gasneti_bootstrapBarrier_mpi(void) {
  int err;

  err = MPI_Barrier(gasnetc_mpi_comm);
  gasneti_assert(err == MPI_SUCCESS);
}

void gasneti_bootstrapExchange_mpi(void *src, size_t len, void *dest) {
  const int inplace = ((uint8_t *)src == (uint8_t *)dest + len * gasnetc_mpi_rank);
  int err;

  if (inplace) {
#if GASNETC_MPI_ALLGATHER_IN_PLACE
    src = MPI_IN_PLACE;
#else
    src = memcpy(gasneti_malloc(len), src, len);
#endif
  }

  err = MPI_Allgather(src, len, MPI_BYTE, dest, len, MPI_BYTE, gasnetc_mpi_comm);
  gasneti_assert(err == MPI_SUCCESS);

#if !GASNETC_MPI_ALLGATHER_IN_PLACE
  if (inplace) gasneti_free(src);
#endif
}

void gasneti_bootstrapAlltoall_mpi(void *src, size_t len, void *dest) {
  const int inplace = (src == dest);
  int err;

  if (inplace) {
#if GASNETC_MPI_ALLTOALL_IN_PLACE
    src = MPI_IN_PLACE;
#else
    const size_t total_len = len * gasnetc_mpi_size;
    src = memcpy(gasneti_malloc(total_len), src, total_len);
#endif
  }

  err = MPI_Alltoall(src, len, MPI_BYTE, dest, len, MPI_BYTE, gasnetc_mpi_comm);
  gasneti_assert(err == MPI_SUCCESS);

#if !GASNETC_MPI_ALLTOALL_IN_PLACE
  if (inplace) gasneti_free(src);
#endif
}

void gasneti_bootstrapBroadcast_mpi(void *src, size_t len, void *dest, int rootnode) {
  int err;
  
  if (gasnetc_mpi_rank == rootnode) {
    memmove(dest, src, len);
  }
  err = MPI_Bcast(dest, len, MPI_BYTE, rootnode, gasnetc_mpi_comm);
}

void gasneti_bootstrapCleanup_mpi(void) {
  /* Nothing to do here */
}
