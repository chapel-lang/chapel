use Time;
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
  }
  delete sendBarrier;
}

/* A Chapel barrier */
class LocaleBarrier {
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
