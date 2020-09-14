use BlockDist;

// This is a simplified version of the unorderedCopyStress.chpl test
// that is focused on testing a cache-remote issue with yielding
// while in a cache operation (which can happen under ugni comms).

config const oversubscription = 10,
             tasksPerLocale = here.maxTaskPar * oversubscription;

config const sizePerLocale = 10000,
             size = sizePerLocale * numLocales;

const space = {0..size};
const D = space dmapped Block(space, dataParTasksPerLocale=tasksPerLocale);
var A, reversedA: [D] int;

forall (a, r, d) in zip(A, reversedA, D) {
  a += d:int;
  r += d:int;
}

proc start() {
  reversedA -= reversedA;
}

proc stop(opType: string) {
  forall (rA, i) in zip(reversedA, D) {
    var expected: int;
    expected += (size-i):int;
    assert(rA == expected);
  }
  writeln(opType, " done");
}


start();
forall i in D {
  reversedA[i] = A[size-i];
}
stop("GET   ");

start();
forall i in D {
  reversedA[size-i] = A[i];
}
stop("PUT   ");

start();
const offset = 2 * (size/numLocales);
forall i in D {
  const offIdx = (i + offset) % (size+1);
  reversedA[offIdx] = A[size-offIdx];
}
stop("GETPUT");
