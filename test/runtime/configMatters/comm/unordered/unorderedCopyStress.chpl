use BlockDist;
use CommDiagnostics;
use Time;
use UnorderedCopy;

config const printStats = true,
             verify = true;

config const oversubscription = 1,
             tasksPerLocale = here.maxTaskPar * oversubscription;

config type componentType = int;
config param numComponents = 1;
type copyType = if numComponents > 1 then numComponents*componentType else componentType;

config param useUnorderedCopy = true;
config const concurrentFencing = false;
config param commDiags = false;

config const sizePerLocale = 10000,
             size = sizePerLocale * numLocales;

const space = {0..size};
const D = space dmapped Block(space, dataParTasksPerLocale=tasksPerLocale);
var A, reversedA: [D] copyType;

forall (a, r, d) in zip(A, reversedA, D) {
  a += d:componentType;
  r += d:componentType;
}

inline proc assign(ref dst, ref src) {
  if useUnorderedCopy then unorderedCopy(dst, src);
                      else dst = src;

  if useUnorderedCopy && concurrentFencing then
    unorderedCopyTaskFence();
}

var t: Timer;

proc start() {
  reversedA -= reversedA;
  if commDiags { resetCommDiagnostics(); startCommDiagnostics(); }
  t.clear(); t.start();
}

proc stop(opType: string) {
  t.stop();
  if commDiags { stopCommDiagnostics(); }
  const ordering = if useUnorderedCopy then "Unordered " else "Ordered ";
  const timing = if printStats then " time: " + t.elapsed():string else "";
  const rate = if printStats then " rate(mOps/sec): " + ((size / t.elapsed()) / 1e6):string else "";
  const gets = +reduce getCommDiagnostics().get;
  const puts = +reduce getCommDiagnostics().put;
  const diags = if commDiags then " (GETS: " + gets + ", PUTS: " + puts + ")" else "";

  writeln(ordering, opType, timing, " : ", ordering, opType, rate, diags);

  if verify then
    forall (rA, i) in zip(reversedA, D) {
      var expected: copyType;
      expected += (size-i):componentType;
      assert(rA == expected);
    }
}


start();
forall i in D {
  assign(reversedA[i], A[size-i]);
}
stop("GET   ");

start();
forall i in D {
  assign(reversedA[size-i], A[i]);
}
stop("PUT   ");

start();
const offset = 2 * (size/numLocales);
forall i in D {
  const offIdx = (i + offset) % (size+1);
  assign(reversedA[offIdx], A[size-offIdx]);
}
stop("GETPUT");
