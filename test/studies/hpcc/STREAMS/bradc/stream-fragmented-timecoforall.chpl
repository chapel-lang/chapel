use Time, Types, Random;
use hpccMultilocale;

use HPCCProblemSize;


param numVectors = 3;
type elemType = real(64),
     indexType = int(64);

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

  const ProblemSpace: domain(1, indexType) = {1..m};

  var execTime: [1..numTrials] real;
  var allValidAnswer: [LocaleSpace] bool;
  
  for trial in 1..numTrials {
    const startTime = getCurrentTime();
    coforall loc in Locales {
      on loc {
        const MyProblemSpace: domain(1, indexType) 
                            = BlockPartition(ProblemSpace, here.id, numLocales);

        var A, B, C: [MyProblemSpace] elemType;

        initVectors(B, C, ProblemSpace, (trial == 1));

        A = B + alpha * C;

        allValidAnswer(here.id) = verifyResults(A, B, C, (trial == numTrials));
      }
    }
    execTime(trial) = getCurrentTime() - startTime;
  }

  const validAnswer = & reduce allValidAnswer;

  printResults(validAnswer, execTime);
}


proc printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}


proc initVectors(B, C, ProblemSpace, print) {
  var randlist = new owned NPBRandomStream(eltType=real, seed=seed);

  randlist.skipToNth(B.domain.low-1);
  randlist.fillRandom(B);
  randlist.skipToNth(ProblemSpace.size + C.domain.low-1);
  randlist.fillRandom(C);

  if (printArrays && print) {
    writelnFragArray("B is: ", B, "\n");
    writelnFragArray("C is: ", C, "\n");
  }
}


proc verifyResults(A, B, C, print) {
  if (printArrays && print) then writelnFragArray("A is: ", A, "\n");

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
