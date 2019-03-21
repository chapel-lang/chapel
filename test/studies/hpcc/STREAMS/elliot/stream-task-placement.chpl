use Time;
use Types;
use Random;

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

enum TaskDisplacement {None, Half, Rand};
config const taskDisplacement = TaskDisplacement.None;
const tasks = here.maxTaskPar,
      randTasks = (new RandomStream(int)).getNext(1, tasks);


proc main() {
  printConfiguration();

  var A, B, C: [1..m] elemType;

  initVectors(B, C);

  var execTime: [1..numTrials] real;

  select taskDisplacement {
    when TaskDisplacement.Half do
      coforall 1..tasks do coforall 1..tasks/2 {}
    when TaskDisplacement.Rand do
      coforall 1..tasks do coforall 1..randTasks {}
  }

  for trial in 1..numTrials {
    const startTime = getCurrentTime();
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
  var randlist = new borrowed NPBRandomStream(eltType=real, seed=seed);

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

    const GBPerSec = numVectors * numBytes(elemType) * (m/minTime) * 1.0e-9;
    writeln("Performance (GB/s) = ", GBPerSec);
  }
}
