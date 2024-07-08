use Time;
use Types;
use Random;

use RangeChunk;

use HPCCProblemSize;

param numVectors = 3;
type elemType = real(64);
enum BarrierType {
  Atomic,
  Sync
}

config const m = computeProblemSize(elemType, numVectors),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 1e-15;

config const useRandomSeed = true;

config const printParams = true,
             printArrays = false,
             printStats = true;

config const numTasks = min(here.maxTaskPar, m);
config const barrierType = BarrierType.Atomic;

proc main() {
  printConfiguration();

  var A, B, C: [1..m] elemType;

  initVectors(B, C);

  var execTime: [1..numTrials] real;

  for trial in 1..numTrials {
    var count: atomic int = numTasks;
    var sgate: sync int;
    var agate: atomic int;

    const startTime = timeSinceEpoch().totalSeconds();
    coforall tid in 0..#numTasks with (ref A) {
      // do a basic hand-coded barrier using either syncs or atomics
      var c = count.fetchSub(1);
      if c == 1 {
        if barrierType == BarrierType.Sync then sgate.writeXF(1);
                                           else agate.write(1);
      }
      if barrierType == BarrierType.Sync then sgate.readFF();
                                         else agate.waitFor(1);
      for i in chunk(1..m, numTasks, tid) {
        A[i] = B[i] + alpha * C[i];
      }
    }
    execTime(trial) = timeSinceEpoch().totalSeconds() - startTime;
  }

  const validAnswer = verifyResults(A, B, C);
  printResults(validAnswer, execTime);
}


proc printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}


proc initVectors(ref B, ref C) {
  var randlist = if useRandomSeed
    then new randomStream(eltType=real)
    else new randomStream(eltType=real, seed=314159265);

  randlist.fill(B);
  randlist.fill(C);

  if (printArrays) {
    writeln("B is: ", B, "\n");
    writeln("C is: ", C, "\n");
  }
}


proc verifyResults(A, B, C) {
  if (printArrays) then writeln("A is: ", A, "\n");

  const infNorm = max reduce [i in A.domain] abs(A(i) - (B(i) + alpha * C(i)));

  return (infNorm <= epsilon);
}


proc printResults(successful, execTimes) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    const totalTime = + reduce execTimes,
          avgTime = totalTime / numTrials,
          minTime = min reduce execTimes;
    writeln("Execution time:");
    writeln("  tot = ", totalTime);
    writeln("  avg = ", avgTime);
    writeln("  min = ", minTime);

    const GBPerSec = numVectors * numBytes(elemType) * (m/minTime) * 1.0e-9;
    writeln("Performance (GB/s) = ", GBPerSec);
  }
}
