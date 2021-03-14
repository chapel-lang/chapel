use MPI;
use C_MPI;

record LocaleBarrier {
  var count : atomic int;
  var done : atomic bool;

  proc LocaleBarrier() {
    count.write(numLocales);
    done.clear();
  }
  
  /* The barrier... 
  */
  proc barrier() {
    if this.locale != here {
      on this do {
        done.waitFor(false);
        count.sub(1);
        done.waitFor(true);
        count.add(1);
      }
    } else {
      local {
        count.sub(1);
        count.waitFor(0);
        done.write(true);
        count.add(1);
        count.waitFor(numLocales);
        done.clear();
      }
    }
  }
}

var b : LocaleBarrier;


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
  //MPI_Barrier(MPI_COMM_WORLD);
  b.barrier();
  //sleep(1);
  local {
    send = commRank();
    MPI_Allreduce(send, recv, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  }
  writef("Chapel locale %i, MPI rank %i : reports a sum of %i \n",here.id,send,recv);
}
