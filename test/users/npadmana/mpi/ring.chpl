use MPI;
use C_MPI;
use Time;
use Barriers;

enum BarrierType {None, Chapel, MPI};

config const sendSleep=2;
config const recvSleep=0;
config const useBarrier = BarrierType.None;


proc main() {
  writeln("This is the main program");
  cobegin {
    send();
    recv();
  }
  writeln("The main program ends here...");
}

proc send() {
  var sendBarrier = new Barrier(numLocales);
  coforall loc in Locales do on loc {
    var rank = here.id : c_int,
    size = numLocales : c_int;
    var dest : c_int;
    dest = (rank + 1)%size;

    writef("Rank %i sending to %i \n",rank, dest);
    sleep(sendSleep);
    select useBarrier {
      when BarrierType.Chapel do sendBarrier.barrier();
      when BarrierType.MPI do MPI_Barrier(CHPL_COMM_WORLD);
      otherwise;
    }
    sendBarrier.barrier();
    MPI_Send(rank, 1, MPI_INT, dest, 0, CHPL_COMM_WORLD);
    writef("Rank %i done sending...\n",rank);
  }
  delete sendBarrier;
}

proc recv() {
  var recvBarrier = new Barrier(numLocales);
  coforall loc in Locales do on loc {
    var rank = here.id : c_int,
    size = numLocales : c_int;
    var src, val : c_int;
    src = mod(rank-1,size);

    writef("Rank %i receiving from %i \n",rank, src);
    sleep(recvSleep);
    recvBarrier.barrier();
    select useBarrier {
      when BarrierType.Chapel do recvBarrier.barrier();
      when BarrierType.MPI do MPI_Barrier(CHPL_COMM_WORLD);
      otherwise;
    }
    var status : MPI_Status;
    MPI_Recv(val, 1, MPI_INT, src, 0, CHPL_COMM_WORLD, status);
    writef("Rank %i received %i from the left.\n", rank, val);
  }
  delete recvBarrier;
}
