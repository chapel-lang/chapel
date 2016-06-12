proc main() {
  writeln("This is the main program");
  cobegin {
    send();
    recv();
  }
  writeln("The main program ends here...");
}

proc send() {
  var sendBarrier : LocaleBarrier;
  coforall loc in Locales do on loc {
    var rank = here.id : c_int,
    size = numLocales : c_int;
    var dest : c_int;
    dest = (rank + 1)%size;

    //writef("Rank %i sending to %i \n",rank, dest);
    sendBarrier.barrier();
  }
}

proc recv() {
  var recvBarrier : LocaleBarrier;
  coforall loc in Locales do on loc {
    var rank = here.id : c_int,
    size = numLocales : c_int;
    var src, val : c_int;
    src = mod(rank-1,size);

    //writef("Rank %i receiving from %i \n",rank, src);
    recvBarrier.barrier();
  }
}

  /* A Chapel barrier */
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
