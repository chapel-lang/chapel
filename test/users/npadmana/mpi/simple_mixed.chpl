use MPI;
use C_MPI;
use Time;

extern proc MPI_Ibarrier(comm : MPI_Comm, ref request : MPI_Request) : c_int;

proc chapelPause(t : real = 1.0) {
  var req : MPI_Request;
  var status : MPI_Status;
  var flag : c_int = 0 : c_int;
  MPI_Ibarrier(MPI_COMM_WORLD, req);
  while true {
    MPI_Test(req, flag, status);
    if flag!=0 then break;
    sleep(t, unit = TimeUnits.microseconds);
  }
}


config const deadlock=true;

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
  if deadlock then writef("Locale %i : This is inside the SPMD part of the code\n",here.id);
  chapelPause();
  send = commRank();
  recv = 0;
  MPI_Allreduce(send, recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  writef("Chapel locale %i, MPI rank %i : reports a sum of %i \n",here.id,send,recv);
  chapelPause();
}
