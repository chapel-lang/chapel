use Time, Types, Random;
use BlockDist;

use HPCCProblemSize;


param numVectors = 3;
type elemType = real(64);

config const m = computeProblemSize(elemType, numVectors),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 0.0;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 314159265;

config const printParams = true,
             printArrays = false,
             printStats = true;


proc main() {
  printConfiguration();

  const BlockDist = new dmap(new Block(rank=1, idxType=int(64), boundingBox={1..m}, targetLocales=Locales));

  const ProblemSpace: domain(1, int(64)) dmapped BlockDist = {1..m};

  var A, B, C: [ProblemSpace] elemType;

  initVectors(B, C);

  var execTime: [1..numTrials] real;

  for trial in 1..numTrials {
    const startTime = getCurrentTime();
    // TODO: Want:
    // A = B + alpha * C;
    // But this doesn't yet result in parallelism

    forall (a, b, c) in zip(A, B, C) {
      a = b + alpha * c;
    }

    execTime(trial) = getCurrentTime() - startTime;
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


proc initVectors(B, C) {
  var randlist = new NPBRandomStream(eltType=real, seed=seed);

  randlist.fillRandom(B);
  randlist.fillRandom(C);

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

    const GBPerSec = numVectors * numBytes(elemType) * (m / minTime) * 1e-9;
    writeln("Performance (GB/s) = ", GBPerSec);
  }
}
