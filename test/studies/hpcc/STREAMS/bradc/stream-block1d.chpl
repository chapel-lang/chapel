use Time, Types, Random;
use BlockDist;

use HPCCProblemSize;


param numVectors = 3;
type elemType = real(64);

config const m = computeProblemSize(elemType, numVectors),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 1e-15;

config const useRandomSeed = true;

config const printParams = true,
             printArrays = false,
             printStats = true;


proc main() {
  printConfiguration();
  var t1, t2, t3: stopwatch;

  t1.start();
  const BlockDist = new blockDist(rank=1, idxType=int(64), boundingBox={1..m}, targetLocales=Locales);

  const ProblemSpace: domain(1, int(64)) dmapped BlockDist = {1..m};

  var A, B, C: [ProblemSpace] elemType;
  t1.stop();

  t2.start();
  initVectors(B, C);
  t2.stop();

  var execTime: [1..numTrials] real;

  for trial in 1..numTrials {
    const startTime = timeSinceEpoch().totalSeconds();
    // TODO: Want:
    // A = B + alpha * C;
    // But this doesn't yet result in parallelism

    forall (a, b, c) in zip(A, B, C) {
      a = b + alpha * c;
    }

    execTime(trial) = timeSinceEpoch().totalSeconds() - startTime;
  }

  t3.start();
  const validAnswer = verifyResults(A, B, C);
  t3.stop();
  printResults(validAnswer, execTime);
  //  writeln("declarations  = ", t1.elapsed());
  //  writeln("initVectors   = ", t2.elapsed());
  //  writeln("verifyResults = ", t3.elapsed());
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

  const infNorm = max reduce [(a,b,c) in zip(A,B,C)] abs(a - (b + alpha * c));

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
