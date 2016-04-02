/* A collection of tests/demonstrations of the C-MPI bindings.

These tests are an implementation of the examples here :
  https://computing.llnl.gov/tutorials/mpi/

Each test is in a separate procedure, each of which ends with an
 MPI_Barrier (this is especially important since, in some cases, 
 we only use a subset of the ranks).

*/

/* This initializes MPI by default, you can turn
that off with the autoInitMPI parameter
 */
use MPI; 
use C_MPI; // Include the C-API, to reduce verbosity of the code.

const requiredSize = 4;

proc main() {
  if requiredSize != worldSize {
    writef("Please run with at least %i ranks..\n",requiredSize);
    MPI_Abort(MPI_COMM_WORLD, 10);
  }

  hello();
  point2point();

  MPI_Finalize();
}


proc hello() {
  /* Simple test of MPI initialization */
  writef("This is rank %i of %i processes saying Hello, Chapel!\n",worldRank, worldSize);
  MPI_Barrier(MPI_COMM_WORLD);
}

proc point2point() {

  if worldRank < 2 {
    var recvpi : real = 0.0,
        sendpi : real = 3.1415926;
    var stat : MPI_Status;

    if worldRank==0 {
      MPI_Send(sendpi, 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
      MPI_Recv(recvpi, 1, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, stat);
    } else {
      MPI_Recv(recvpi, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, stat);
      MPI_Send(sendpi, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    var count = stat.getCount(MPI_DOUBLE);

    writef("Task %i: Received %i pi=%r from task %i with tag %i\n",
        worldRank, count, recvpi, stat.MPI_SOURCE, stat.MPI_TAG);
  }
  MPI_Barrier(MPI_COMM_WORLD);
}
