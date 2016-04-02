/* Simple test of MPI initialization */

use MPI;
writef("This is rank %i of %i processes saying Hello, Chapel!\n",worldRank, worldSize);
C_MPI.MPI_Finalize();
