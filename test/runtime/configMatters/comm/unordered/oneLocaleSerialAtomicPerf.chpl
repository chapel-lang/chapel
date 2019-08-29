use UnorderedAtomics;
use Time;

// Test single-threaded performance or ordered and unordered atomics.

config const iters = 1000;
config const printStats = false;

proc test(useUnordered) {
  var a: atomic int;
  on Locales[numLocales-1] {
    var t: Timer; t.start();
    if useUnordered {
      for 1..iters do a.unorderedAdd(1);
      unorderedAtomicTaskFence();
    } else {
      for 1..iters do a.add(1);
    }
    t.stop();
    if printStats {
      const ordering = if useUnordered then "Unordered " else "Ordered ";
      const time = "time(sec): " + t.elapsed():string;
      const rate = "rate(mOps/sec): " + ((iters / t.elapsed()) / 1e6):string;
      writeln(ordering, time);
      writeln(ordering, rate);
    }
  }
  assert(a.read() == iters);
}

test(false);
test(true);
