/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*-
 *
 * This file contains the interface for the star single cpu RandomAccess test.
 * The test runs on all cpus in the MPI universe, but there is no communication
 * between cpus during the process (each cpu runs its own version of the
 * single_cpu test).  The final result is the average of the entire system.
 *
 * This test uses the computational core found in core_single_cpu.c
 */

#include <hpcc.h>
#include "RandomAccess.h"

int
HPCC_StarRandomAccess(HPCC_Params *params)
{
  int myRank, commSize;
  int rv, errCount, failure = 0, failureAll = 0;
  double minGUPs, avgGUPs, maxGUPs, localGUPs;
  FILE *outputFile = NULL;
  MPI_Comm comm = MPI_COMM_WORLD;

  minGUPs = avgGUPs = maxGUPs = localGUPs = 0.0;

  MPI_Comm_size( comm, &commSize );
  MPI_Comm_rank( comm, &myRank );

  rv = HPCC_RandomAccess( params, 0 == myRank, &localGUPs, &failure );
  MPI_Reduce( &rv, &errCount, 1, MPI_INT, MPI_SUM, 0, comm );
  MPI_Allreduce( &failure, &failureAll, 1, MPI_INT, MPI_MAX, comm );
  if (failureAll) params->Failure = 1;

  MPI_Reduce( &localGUPs, &minGUPs, 1, MPI_DOUBLE, MPI_MIN, 0, comm );
  MPI_Reduce( &localGUPs, &avgGUPs, 1, MPI_DOUBLE, MPI_SUM, 0, comm );
  MPI_Reduce( &localGUPs, &maxGUPs, 1, MPI_DOUBLE, MPI_MAX, 0, comm );

  avgGUPs /= commSize;

  MPI_Bcast( &avgGUPs, 1, MPI_DOUBLE, 0, comm );
  params->StarGUPs = avgGUPs;

  BEGIN_IO( myRank, params->outFname, outputFile);
  fprintf( outputFile, "Node(s) with error %d\n", errCount );
  fprintf( outputFile, "Minimum GUP/s %.6f\n", minGUPs );
  fprintf( outputFile, "Average GUP/s %.6f\n", avgGUPs );
  fprintf( outputFile, "Maximum GUP/s %.6f\n", maxGUPs );
  END_IO( myRank, outputFile );

  return 0;
}
