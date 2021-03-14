// This is mostly the same as release/examples/benchmarks/hpcc/stream.chpl

// The sole purpose of this test is to gauge the negative impact of automatic
// aggregation firing for cases where the data is completely local, and
// aggregation is redundant

use BlockDist, Time, Types, Random;
use HPCCProblemSize;

const numVectors = 2;
type elemType = real(64);

config const m = computeProblemSize(numVectors, elemType),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 0.0;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 314159265;

config const printParams = true,
             printArrays = false,
             printStats = true;

proc main() {
  printConfiguration();   // print the problem size, number of trials, etc.

  const ProblemSpace: domain(1) dmapped Block(boundingBox={1..m}) = {1..m};

  var A, B: [ProblemSpace] elemType;

  initVector(B);

  var execTime: [1..numTrials] real;                 // an array of timings

  for trial in 1..numTrials {                        // loop over the trials
    const startTime = getCurrentTime();              // capture the start time

    forall i in ProblemSpace do
      A[i] = B[i+0];

    execTime(trial) = getCurrentTime() - startTime;  // store the elapsed time
  }

  const validAnswer = verifyResults(A, B);           // verify...
  printResults(validAnswer, execTime);               // ...and print the results
}

//
// Print the problem size and number of trials
//
proc printConfiguration() {
  if (printParams) {
    if (printStats) then printLocalesTasks();
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}

//
// Initialize vectors B and C using a random stream of values and
// optionally print them to the console
//
proc initVector(B) {
  var randlist = new NPBRandomStream(eltType=real, seed=seed);

  randlist.fillRandom(B);

  if (printArrays) {
    writeln("B is: ", B, "\n");
  }
}

//
// Verify that the computation is correct
//
proc verifyResults(A, B) {
  if (printArrays) then writeln("A is:     ", A, "\n");  // optionally print A

  const infNorm = max reduce [(a,b) in zip(A,B)] abs(a - b);

  return (infNorm <= epsilon);    // return whether the error is acceptable
}

//
// Print out success/failure, the timings, and the GB/s value
//
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
