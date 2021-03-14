use MPI;
use C_MPI;

writeln("Test...");

writeln("numLocales = ",numLocales);
writeln("MPI size = ", commSize(MPI_COMM_WORLD));

coforall loc in Locales do on loc {
  MPI_Barrier(CHPL_COMM_WORLD);
}

writeln("All done");
