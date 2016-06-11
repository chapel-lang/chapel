use MPI;
use C_MPI;
config const loop=2;

proc main() {
  writeln("This is the main program");
  coforall ii in 0.. #loop do send();
  writeln("The main program ends here...");
}

proc send() {
  var sendBarrier = new LocaleBarrier();
  coforall loc in Locales do on loc {
    sendBarrier.barrier();
    MPI_Barrier(CHPL_COMM_WORLD(1));
  }
  delete sendBarrier;
}
