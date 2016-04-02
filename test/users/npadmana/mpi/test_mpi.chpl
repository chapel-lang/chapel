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
  ring();

  MPI_Finalize();
}


proc hello() {
  /* Simple test of MPI initialization */
  writef("This is rank %i of %i processes saying Hello, Chapel!\n",worldRank, worldSize);
  MPI_Barrier(MPI_COMM_WORLD);
}

/* Simple blocking point-to-point communication */
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

/* Non-blocking communication in a ring */
proc ring() {

  var left = mod(worldRank-1, worldSize);
  var right = mod(worldRank+1, worldSize);
  var toleft : c_int = 1;
  var toright : c_int = 2;
  var fromleft = toright,
      fromright = toleft;


  var buf : [1..2]int(32);
  var requests : [1..4]MPI_Request;
  var status : [1..4]MPI_Status;

  MPI_Irecv(buf[1], 1, MPI_INT, left, fromleft, MPI_COMM_WORLD, requests[1]);
  MPI_Irecv(buf[2], 1, MPI_INT, right, fromright, MPI_COMM_WORLD, requests[2]);

  MPI_Isend(worldRank, 1, MPI_INT, left, toleft, MPI_COMM_WORLD, requests[3]);
  MPI_Isend(worldRank, 1, MPI_INT, right, toright, MPI_COMM_WORLD, requests[4]);

  MPI_Waitall(4, requests, status);

  writef("Rank %i recieved %i from the left, and %i from the right\n",worldRank, buf[1], buf[2]);
  MPI_Barrier(MPI_COMM_WORLD);
}
