use UnorderedAtomics;
use DynamicIters;
use Time;

// Test multi-threaded performance of ordered and unordered atomics with static
// and dynamic scheduling.

config const iters = 1000;
config const printStats = false;
config const oversubscription = 1,
             tasksPerLocale = here.maxTaskPar * oversubscription;

proc test(param useUnordered, dynamicIters, iters, printStats) {
  var a: atomic int;
  on Locales[numLocales-1] {
    const d = {1..iters};
    const chunk =  2**16;

    var t: Timer; t.start();
    if dynamicIters {
      forall dynamic(d, chunkSize=chunk, numTasks=tasksPerLocale) do
        if useUnordered then a.unorderedAdd(1);
                        else a.add(1);
    } else {
      forall d.these(tasksPerLocale=tasksPerLocale) do
        if useUnordered then a.unorderedAdd(1);
                        else a.add(1);
    }
    t.stop();

    if printStats {
      const ordering = if useUnordered then "Unordered " else "Ordered ";
      const scheduling = if dynamicIters then "dynamic " else "static ";
      const time = "time(sec): " + t.elapsed():string;
      const rate = "rate(mOps/sec): " + ((iters / t.elapsed()) / 1e6):string;
      writeln(ordering, scheduling, time);
      writeln(ordering, scheduling, rate);
    }
  }
  assert(a.read() == iters);
}

// warmup
test(useUnordered=false, dynamicIters=false, iters=iters/100, printStats=false);

test(useUnordered=false, dynamicIters=false, iters=iters/10,  printStats=printStats);
test(useUnordered=false, dynamicIters=true,  iters=iters/10,  printStats=printStats);
test(useUnordered=true,  dynamicIters=false, iters=iters,     printStats=printStats);
test(useUnordered=true,  dynamicIters=true,  iters=iters,     printStats=printStats);
