/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*-
 *
 * This file contains the interface for the single cpu RandomAccess test.  The
 * test is only run on a single (random) node in the MPI universe, with all
 * other CPUs stuck (in theory, idle) in an MPI_Bcast waiting for the selected
 * CPU to finish the RandomAccess test.
 *
 * This test uses the computational core found in core_single_cpu.c
 */

#include <hpcc.h>
#include "RandomAccess.h"

int
HPCC_SingleRandomAccess(HPCC_Params *params)
{
  int myRank, commSize;
  int rv, errCount, rank, failure = 0;
  double localGUPs;
  double scl = 1.0 / RAND_MAX;
  FILE *outputFile = NULL;
  MPI_Comm comm = MPI_COMM_WORLD;

  localGUPs = 0.0;

  MPI_Comm_size( comm, &commSize );
  MPI_Comm_rank( comm, &myRank );

  srand(time(NULL));
  scl *= commSize;

  /* select a node at random, but not node 0 (unless there is just one node) */
  if (1 == commSize)
    rank = 0;
  else
    for (rank = 0; ; rank = (int)(scl * rand())) {
      if (rank > 0 && rank < commSize)
        break;
    }

  MPI_Bcast( &rank, 1, MPI_INT, 0, comm ); /* broadcast the rank selected on node 0 */

  if (myRank == rank) /* if this node has been selected */
    rv = HPCC_RandomAccess( params, 0 == myRank, &localGUPs, &failure );

  MPI_Bcast( &rv, 1, MPI_INT, rank, comm ); /* broadcast error code */
  MPI_Bcast( &localGUPs, 1, MPI_DOUBLE, rank, comm ); /* broadcast GUPs */
  MPI_Bcast( &failure, 1, MPI_INT, rank, comm ); /* broadcast failure indication */
  errCount = rv;
  params->SingleGUPs = localGUPs;
  if (failure) params->Failure = 1;

  BEGIN_IO( myRank, params->outFname, outputFile);
  fprintf( outputFile, "Node(s) with error %d\n", errCount );
  fprintf( outputFile, "Node selected %d\n", rank );
  fprintf( outputFile, "Single GUP/s %.6f\n", localGUPs );
  END_IO( myRank, outputFile );

  return 0;
}
