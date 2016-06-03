use myMPI; 
use C_MPI;
use Barrier;

var blockChapel = new Barrier(numLocales);

proc main() {


  writeln("This is a mixed multilocale Chapel+MPI program");
  writeln("We are currently in the main thread.....");

  writef("There are %i Locales and %i MPI ranks\n",numLocales,worldSize);
  for (loc, rank) in zip(LocaleSpace, worldRank) {
    writef("Chapel locale %i --> MPI Rank %i \n", loc, rank);
  }

  writeln("Switch to MPI mode");
  coforall loc in Locales do 
    on loc do spmd();

  writeln("Back in the main thread....");

  delete blockChapel;


}

proc spmd() {
  var send, recv : c_int;
  send = worldRank[here.id];
  // Finish all Chapel communication
  //blockChapel.barrier();
  //TODO : This really needs to be a Chapel barrier, but using it deadlocks
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Allreduce(send, recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  MPI_Barrier(MPI_COMM_WORLD);
  writef("Chapel locale %i reports a sum of %i \n",here.id,recv);
}
