// standard modules for timing, type size, and random number routines
use Time;
use Types;
use Random;

// user module for computing HPCC problem sizes
use HPCCProblemSize;


// the number of vectors we'll be using and the element type
param numVectors = 3;
type elemType = float(64);

// configuration constants for specifying the problem size
config const usePow2ProbSize = false,
             m = computeProblemSize(elemType, numVectors, usePow2ProbSize);

// configuration constants for the number of trials to run
config const numTrials = 10;

// configuration constants for controlling output options
config const printParams = true,
             printArrays = false,
             printStats = true;

// configuration constant for Triad scalar multiplier
config const alpha = 3.0;

// configuration constant for error tolerance in verifying result
config const epsilon = 0.0;

// configuration constants for seeding the random number generator
config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.clockMS else 314159265;


def main() {
  printConfiguration();

  const ProblemSpace: domain(1) distributed(Block) = [1..m];
  var A, B, C: [ProblemSpace] elemType;

  initVectors(B, C);

  var execTime: [1..numTrials] float;

  for k in 1..numTrials {
    const startTime = getCurrentTime();
    A = B + alpha * C;
    execTime[k] = getCurrentTime() - startTime;
  }

  const validAnswer = verifyResults(A, B, C);
  printResults(validAnswer, execTime);
}


def printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}


def initVectors(B, C) {
  var randlist = RandomStream(seed);

  randlist.fillRandom(B);
  randlist.fillRandom(C);

  if (printArrays) {
    writeln("B is: ", B, "\n");
    writeln("C is: ", C, "\n");
  }
}


def verifyResults(A, B, C) {
  if (printArrays) then writeln("A is: ", A, "\n");

  const infNorm = max reduce [i in A.domain] abs(A(i) - (B(i) + alpha * C(i)));

  return (infNorm <= epsilon);
}


def printResults(successful, execTimes) {
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
