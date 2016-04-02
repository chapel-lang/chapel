/* A simple point to point communication. 

I don't have a good idea for generating Chapel-esque
 Sends and Recvs, so we just use the C API here. Of course, 
 for specialized cases, like contiguous arrays of known types, 
 this can be significantly simplified.

*/

use MPI;
use C_MPI; // less to type

// Abort if size != 2
if worldSize != 2 {
  writeln("Please run with two processes...");
  MPI_Abort(MPI_COMM_WORLD, 1);
}

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

MPI_Finalize();
