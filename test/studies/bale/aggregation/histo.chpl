use CyclicDist;
use BlockDist;
use Random;
use Time;
use AtomicAggregation;

const numTasksPerLocale = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                                                       else here.maxTaskPar;
const numTasks = numLocales * numTasksPerLocale;
config const N = 1000000; // number of updates per task
config const M = 10000; // number of entries in the table per task

const numUpdates = N * numTasks;
const tableSize = M * numTasks;

config param useBlockArr = false;

var t: Timer;
proc startTimer() {
  t.start();
}
proc stopTimer(name) {
    t.stop(); var src = t.elapsed(); t.clear();
    const bytesPerTask = N * numBytes(int);
    const gbPerNode = bytesPerTask:real / (10**9):real * numTasksPerLocale;
    writef("%10s:\t%.3dr seconds\t%.3dr GB/s/node\n", name, src, gbPerNode/src);
}

proc main() {
  const D = if useBlockArr then newBlockDom(0..#tableSize)
                           else newCyclicDom(0..#tableSize);
  var A: [D] atomic int;
  var AggA: [D] AggregatedAtomic(int);

  const D2 = newBlockDom(0..#numUpdates);
  var Rindex: [D2] int;

  fillRandom(Rindex, 208);
  Rindex = mod(Rindex, tableSize);

  startTimer();
  forall r in Rindex with (var agg = new AtomicIncAggregator(int)) {
    agg.inc(AggA[r]);
  }
  stopTimer("AGP");

  assert(numUpdates == +reduce A.read());
  assert(numUpdates == +reduce AggA.read());
}
