use CyclicDist;
use BlockDist;
use Random;
use Time;
use UnorderedCopy;

config const printStats = true,
             printArrays = false,
             verify = true;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 314159265;

const numTasksPerLocale = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                                                       else here.maxTaskPar;
const numTasks = numLocales * numTasksPerLocale;
config const N = 1000000;
config const M = 10000;

const numUpdates = N * numTasks;
const tableSize = M * numTasks;

const Mspace = {0..tableSize-1};
const D = Mspace dmapped Cyclic(startIdx=Mspace.low);
var A: [D] int = 0..tableSize-1;

const Nspace = {0..numUpdates-1};
const D2 = Nspace dmapped Block(Nspace);
var rindex: [D2] int;

fillRandom(rindex, seed);
rindex = mod(rindex, tableSize);

var tmp: [D2] int;

enum Mode {directIndexLocal, directIndex, zipArray, promotion}

// warmup
for param explicit in false..true do
  testit(Mode.directIndexLocal, explicit, false);

for param explicit in false..true {
  for mode in Mode {
    testit(mode, explicit, printStats);
  }
}

inline proc copy(ref dst, ref src, param explicit) {
  if explicit then unorderedCopy(dst, src);
              else dst = src;
}
proc testit(mode: Mode, param explicit, printStats) {

  tmp = -1;

  var t: Timer; t.start();
  select mode {
    when Mode.directIndexLocal {
      forall i in D2 do
        copy(tmp.localAccess[i], A[rindex.localAccess[i]], explicit);
    }
    when Mode.directIndex {
      forall i in D2 do
        copy(tmp[i], A[rindex[i]], explicit);
    }
    when Mode.zipArray {
      forall (t, r) in zip(tmp, rindex) do
        copy(t, A[r], explicit);
    }
    when Mode.promotion {
      if explicit then unorderedCopy(tmp, A[rindex]);
                  else tmp = A[rindex];
    }
  }
  t.stop();

  if printStats {
    const bytesPerTask = N * numBytes(int);
    const mbPerTask = bytesPerTask:real / (1<<20):real;
    const prefix = if explicit then "explicit-" else "compiler-";
    writeln(prefix, mode, " MB/s: ", mbPerTask * numTasksPerLocale / t.elapsed());
  }

  if verify {
    [t in tmp] assert (t >= 0 && t < tableSize);
  }

  if printArrays {
    writeln(tmp);
  }
}
