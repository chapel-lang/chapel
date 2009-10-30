//
// Local (Per-System) Implementation of STREAM Triad
//
// This version of the stream benchmark is not as elegant as
// stream.chpl.  It is a per-system code with no communication in the
// actual computation.  It highlights the ability of a Chapel
// programmer to escape the global-view programming model and write
// codes with a fragmented, per-locale model.
//
// Comments throughout this code point out differences with the global
// version of this benchmark in stream.chpl.
//

//
// Since this benchmark is written on a per-locale basis, there is no
// need to use the Block distribution.  The following use omits
// BlockDist, included in stream.chpl.
//
use Time, Types, Random;

use HPCCProblemSize;

const numVectors = 3;
type elemType = real(64);

//
// To ensure a local problem size, we spoof the number of vectors
// passed to the computeProblemSize function to be the number of
// vectors times the number of locales.
//
config const m = computeProblemSize(numVectors*numLocales, elemType),
             alpha = 3.0;

config const numTrials = 10,
             epsilon = 0.0;

//
// There isn't (yet) a way to set the number of tasks to use for
// implementing a forall loop over a default array.  When there is
// such a way, we will want to set it via this configuration constant
// to get functionality like we have with stream.chpl.
//
// config const tasksPerLocale = here.numCores;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.clockMS else 314159265;

//
// To ensure determinism of output, there is no more printing of the
// arrays in initVectors and verifyResults.
//
config const printParams = true,
             printStats = true;

def main() {
  printConfiguration();

  //
  // Aggregates for collecting per-locale results for the minimum
  // execution time per trial, and whether verification passed
  //
  var minTimes: [LocaleSpace] real;
  var validAnswers: [LocaleSpace] bool;

  //
  // Fragment control so that we have a single task running on every
  // locale.
  //
  coforall loc in Locales do on loc {

    //
    // We declare these variables outside of the local block since
    // we'll need to access them when we write back to the global
    // aggregates declared above.
    //
    var validAnswer: bool;
    var execTime: [1..numTrials] real;

    //
    // Indicates that all of the code in this block is local to this
    // locale.  There is no communication.  A violation will result in
    // an error, though error checking is disabled with --fast or
    // --no-checks.
    //
    local {

      //
      // A, B, and C are the three local vectors
      //
      var A, B, C: [1..m] elemType;

      initVectors(B, C); // same initialization call as in stream.chpl

      for trial in 1..numTrials {
        const startTime = getCurrentTime();

        //
        // The main loop looks identical to stream.chpl.  However, in
        // this version we are iterating over arrays that are not
        // distributed.
        //
        forall (a, b, c) in (A, B, C) do
          a = b + alpha * c;

        execTime(trial) = getCurrentTime() - startTime;
      }

      validAnswer = verifyResults(A, B, C); // same verify as in stream.chpl
    }

    //
    // Write times and verification result into aggregates declared
    // above.  These are declared over LocaleSpace so we can write to
    // them in parallel.
    //
    minTimes[here.id] = min reduce execTime;
    validAnswers[here.id] = validAnswer;
  }

  //
  // Pass minimum, average, and maximum times to printResults
  //
  printResults(&& reduce validAnswers, minTimes);
}

def printConfiguration() {
  if (printParams) {
    //
    // Here we multiple m by the number of locales so that we can
    // print out the global problem size.  We print out the number of
    // cores as tasks per locale since this is what is done by
    // default.
    //
    if (printStats) then printLocalesTasks(here.numCores);
    printProblemSize(elemType, numVectors, m * numLocales);
    writeln("Number of trials = ", numTrials, "\n");
  }
}

//
// Both initVectors and verifyResults are almost identical to
// stream.chpl even though they are called with arrays that are not
// distributed.  The only difference is that the arrays are not
// printed.  This ensures determinism of output.  Printing the arrays
// also violates the locality constraint imposed by the local block
// from which these functions are called.
//
def initVectors(B, C) {
  var randlist = new RandomStream(seed);

  randlist.fillRandom(B);
  randlist.fillRandom(C);

  delete randlist;
}

def verifyResults(A, B, C) {
  forall (b, c) in (B, C) do
    b += alpha *c;  

  const infNorm = max reduce [(a,b) in (A,B)] abs(a - b);

  return (infNorm <= epsilon);
}

//
// Print out success/failure, the timings, and the GB/s value.  Here
// we report maximum, average, and minimum times instead of total,
// average, and minimum.
//
def printResults(successful, minTimes) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    const maxTime = max reduce minTimes,
          avgTime = + reduce minTimes / numLocales,
          minTime = min reduce minTimes;
    writeln("Execution time:");
    writeln("  max = ", maxTime);
    writeln("  avg = ", avgTime);
    writeln("  min = ", minTime);

    const GBPerSec = numVectors * numBytes(elemType) * (m / minTime) * 1e-9;
    writeln("Performance (GB/s) = ", GBPerSec);
  }
}
