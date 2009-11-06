/*   $Source: /var/local/cvs/gasnet/other/mpi-spawner/gasnet_bootstrap_mpi.c,v $
 *     $Date: 2006/05/11 09:43:44 $
 * $Revision: 1.17 $
 * Description: GASNet conduit-independent mpi-based spawner
 * Copyright 2003, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */


#include <gasnet_internal.h>
#include <gasnet_core_internal.h>
#include <signal.h>
#include <mpi.h>

static MPI_Comm gasnetc_mpi_comm;
static int gasnetc_mpi_preinitialized = 0;

void gasneti_bootstrapInit_mpi(int *argc, char ***argv, gasnet_node_t *nodes, gasnet_node_t *mynode) {
  MPI_Group world;
  int err;
  int tmp;

  /* Call MPI_Init exactly once */
  err = MPI_Initialized(&gasnetc_mpi_preinitialized);
  gasneti_assert(err == MPI_SUCCESS);
  if (!gasnetc_mpi_preinitialized) {
    err = MPI_Init(argc, argv);
    gasneti_assert(err == MPI_SUCCESS);
  }

  /* Create private communicator */
  err = MPI_Comm_group(MPI_COMM_WORLD, &world);
  gasneti_assert(err == MPI_SUCCESS);
  err = MPI_Comm_create(MPI_COMM_WORLD, world, &gasnetc_mpi_comm);
  gasneti_assert(err == MPI_SUCCESS);
  err = MPI_Group_free(&world);
  gasneti_assert(err == MPI_SUCCESS);

  /* Get size and rank */
  err = MPI_Comm_size(gasnetc_mpi_comm, &tmp);
  gasneti_assert(err == MPI_SUCCESS);
  *nodes = tmp;

  err = MPI_Comm_rank(gasnetc_mpi_comm, &tmp);
  gasneti_assert(err == MPI_SUCCESS);
  *mynode = tmp;

  gasneti_setupGlobalEnvironment(*nodes, *mynode,
				 &gasneti_bootstrapExchange_mpi,
				 &gasneti_bootstrapBroadcast_mpi);
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
  int err;

  err = MPI_Allgather(src, len, MPI_CHAR, dest, len, MPI_CHAR, gasnetc_mpi_comm);
  gasneti_assert(err == MPI_SUCCESS);
}

void gasneti_bootstrapAlltoall_mpi(void *src, size_t len, void *dest) {
  int err;

  err = MPI_Alltoall(src, len, MPI_CHAR, dest, len, MPI_CHAR, gasnetc_mpi_comm);
  gasneti_assert(err == MPI_SUCCESS);
}

void gasneti_bootstrapBroadcast_mpi(void *src, size_t len, void *dest, int rootnode) {
  int err;
  
  if (gasneti_mynode == rootnode) {
    memcpy(dest, src, len);
  }
  err = MPI_Bcast(dest, len, MPI_CHAR, rootnode, gasnetc_mpi_comm);
}


