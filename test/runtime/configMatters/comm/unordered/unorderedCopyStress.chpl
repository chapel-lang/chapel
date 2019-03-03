use BlockDist;
use Time;
use UnorderedCopy;

config const printStats = true,
             verify = true;

config const oversubscription = 1,
             tasksPerLocale = here.maxTaskPar * oversubscription;

config param useUnorderedCopy = true;
config const concurrentFencing = false;

config const sizePerLocale = 10000,
             size = sizePerLocale * numLocales;

const space = {0..size};
const D = space dmapped Block(space, dataParTasksPerLocale=tasksPerLocale);
var A, reversedA: [D] int = D;

inline proc assign(ref dst, ref src) {
  if useUnorderedCopy then unorderedCopy(dst, src);
                      else dst = src;

  if useUnorderedCopy && concurrentFencing then
    unorderedCopyFence();
}

var t: Timer;

proc start() {
  reversedA = 0;
  t.clear(); t.start();
}

proc stop(opType: string) {
  t.stop();
  const ordering = if useUnorderedCopy then "Unordered " else "Ordered ";
  const timing = if printStats then " time: " + t.elapsed() else "";
  writeln(ordering + opType + timing);

  if verify then
    forall (rA, i) in zip(reversedA, D) do
      assert(rA == size-i);
}

start();
forall i in D {
  assign(reversedA[i], A[size-i]);
}
stop("GET");

start();
forall i in D {
  assign(reversedA[size-i], A[i]);
}
stop("PUT");

start();
const offset = size/numLocales;
forall i in D {
  const offIdx = (i + offset) % (size+1);
  assign(reversedA[offIdx], A[size-offIdx]);
}
stop("GETPUT");
