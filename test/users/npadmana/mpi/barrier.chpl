use Time;
config const loop=2;

proc main() {
  writeln("This is the main program");
  coforall ii in 0.. #loop do send();
  writeln("The main program ends here...");
}

proc send() {
  var sendBarrier : LocaleBarrier;
  coforall loc in Locales do on loc {
    writeln(sendBarrier.locale.id);
    //sendBarrier.barrier();
  }
}

proc yieldWait(x, val, str="") {
  while true {
    var foo = x.peek();
    if foo==val then break;
    sleep(0.001);
    writeln(str," ",here.id,' ',foo);
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
    writeln(this.locale.id," ",here.id);
    if this.locale != here {
//      on this do {
        //done.waitFor(false);
        yieldWait(done, false,"A0");
        count.sub(1);
        //done.waitFor(true);
        yieldWait(done, true,"A1");
        count.add(1);
//      }
   } else {
      //local {
        count.sub(1);
        //count.waitFor(0);
        yieldWait(count, 0,"B0");
        done.write(true);
        count.add(1);
        //count.waitFor(numLocales);
        yieldWait(count, numLocales,"B1");
        done.clear();
      //}
    }
  }
}
