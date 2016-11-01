use MPI;
use C_MPI;

proc main() {

  writeln("This is a mixed multilocale Chapel+MPI program");
  writeln("We are currently in the main thread.....");

  writef("There are %i Locales and %i MPI ranks\n",numLocales,commSize());
  writeln("Switch to MPI mode");
  coforall loc in Locales do
    on loc {
      spmd();
    }

  writeln("Back in the main thread....");

}

proc spmd() {
  var send, recv : c_int;
  writef("Locale %i : This is inside the SPMD part of the code\n",here.id);

  // The following idiom is useful
  send = commRank();
  MPI_Allreduce(send, recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  writef("Chapel locale %i, MPI rank %i : reports a sum of %i \n",here.id,send,recv);
}
