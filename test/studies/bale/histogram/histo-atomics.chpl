use CyclicDist;
use BlockDist;
use Random;
use Time;
use UnorderedAtomics;
use AtomicAggregation;

config const printStats = true,
             printArrays = false,
             verify = true;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 314159265;

enum Mode {ordered, unordered, aggregated}
config const mode = Mode.ordered;

const numTasksPerLocale = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                                                       else here.maxTaskPar;
const numTasks = numLocales * numTasksPerLocale;
config const N = 2000000; // number of updates per task
config const M = 1000; // number of entries in the table per task

const numUpdates = N * numTasks;
const tableSize = M * numTasks;

// The intuitive implementation of histogram that uses global atomics
proc main() {
  const Mspace = {0..tableSize-1};
  const D = Mspace dmapped Cyclic(startIdx=Mspace.low);
  var A: [D] atomic int;
  var AggA: [D] AggregatedAtomic(int);

  const Nspace = {0..numUpdates-1};
  const D2 = Nspace dmapped Block(Nspace);
  var rindex: [D2] int;

  /* set up loop */
  fillRandom(rindex, seed);
  forall r in rindex {
    r = mod(r, tableSize);
  }

  var t: Timer;
  t.start();

  select mode {
    when Mode.ordered {
      forall r in rindex do
        A[r].add(1);
    }
    when Mode.unordered {
      forall r in rindex do
	A[r].unorderedAdd(1);
    }
    when Mode.aggregated {
      forall r in rindex with (var agg = new AtomicIncAggregator(int)) do
	agg.inc(AggA[r]);
    }
  }

  t.stop();

  if printStats {
    writeln("Time: ", t.elapsed());

    const bytesPerTask = N * numBytes(int);
    const mbPerTask = bytesPerTask:real / (1<<20):real;
    writeln("MB/s per task: ", mbPerTask / t.elapsed());
    writeln("MB/s per node: ", mbPerTask * numTasksPerLocale / t.elapsed());
  }

  if verify {
    if mode == Mode.aggregated then assert(numUpdates == +reduce AggA.read());
                               else assert(numUpdates == +reduce A.read());
  }

  if printArrays {
    if mode == Mode.aggregated then writeln(AggA);
                               else writeln(A);
  }
}
