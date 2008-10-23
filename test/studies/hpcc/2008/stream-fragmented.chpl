use Time, Types, Random;
use hpccMultilocale;

use HPCCProblemSize;


param numVectors = 3;
type elemType = real(64),
     indexType = int(64);

config const m = computeProblemSize(numVectors, elemType),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 0.0;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.clockMS else 314159265;

config const printParams = true,
             printArrays = false,
             printStats = true;


def main() {
  printConfiguration();

  const ProblemSpace: domain(1, indexType) = [1..m];

  var minTimes, maxTimes, avgTimes: [LocaleSpace] real,
      allValidAnswer: [LocaleSpace] bool;
  
  coforall loc in Locales {
    on loc {
      const MyProblemSpace: domain(1, indexType) 
                          = BlockPartition(ProblemSpace, here.id, numLocales);

      var A, B, C: [MyProblemSpace] elemType;

      initVectors(B, C, ProblemSpace);

      const localTimings: [1..numTrials] real;

      for trial in 1..numTrials {
        const startTime = getCurrentTime();
        A = B + alpha * C;
        localTimings(trial) = getCurrentTime() - startTime;
      }

      minTimes[here.id] = min reduce localTimings;
      maxTimes[here.id] = max reduce localTimings;
      avgTimes[here.id] = (+ reduce localTimings) / numTrials;

      allValidAnswer(here.id) = verifyResults(A, B, C);
    }
  }

  const minTime = min reduce minTimes,
        maxTime = max reduce maxTimes,
        avgTime = + reduce avgTimes / numLocales;

  const validAnswer = & reduce allValidAnswer;

  printResults(validAnswer, minTime, avgTime, maxTime);
}


def printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}


def initVectors(B, C, ProblemSpace) {
  var randlist = new RandomStream(seed);

  randlist.skipToNth(B.domain.low);
  randlist.fillRandom(B);
  randlist.skipToNth(ProblemSpace.numIndices + C.domain.low);
  randlist.fillRandom(C);

  if (printArrays) {
    writelnFragArray("B is: ", B, "\n");
    writelnFragArray("C is: ", C, "\n");
  }
}


def verifyResults(A, B, C) {
  if (printArrays) then writelnFragArray("A is: ", A, "\n");

  const infNorm = max reduce [i in A.domain] abs(A(i) - (B(i) + alpha * C(i)));

  return (infNorm <= epsilon);
}


def printResults(successful, minTime, avgTime, maxTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time:");
    writeln("  min = ", minTime);
    writeln("  avg = ", avgTime);
    writeln("  max = ", maxTime);

    const GBPerSec = numVectors * numBytes(elemType) * (m / minTime) * 1e-9;
    writeln("Performance (GB/s) = ", GBPerSec);
  }
}
