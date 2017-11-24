use MPI;
use C_MPI;

proc main() {
  writeln("This is the main program");
  cobegin {
    send();
    recv();
  }
  writeln("The main program ends here...");
}

proc send() {
  // Does the right thing, even when running single-locale
  coforall loc in Locales do on loc {
    var rank = commRank(CHPL_COMM_WORLD) : c_int,
        size = commSize(CHPL_COMM_WORLD) : c_int;
    var dest : c_int;
    dest = (rank + 1)%size;

    writef("Rank %i sending to %i \n",rank, dest);
    Send(rank, 1, MPI_INT, dest, 0, CHPL_COMM_WORLD);
    writef("Rank %i done sending...\n",rank);
  }
}

proc recv() {
  // Does the right thing, even when running single-locale
  coforall loc in Locales do on loc {
    var rank = commRank(CHPL_COMM_WORLD) : c_int,
        size = commSize(CHPL_COMM_WORLD) : c_int;
    var src, val : c_int;
    src = mod(rank-1,size);

    writef("Rank %i receiving from %i \n",rank, src);
    var status : MPI_Status;
    Recv(val, 1, MPI_INT, src, 0, CHPL_COMM_WORLD, status);
    writef("Rank %i received %i from the left.\n", rank, val);
  }
}

