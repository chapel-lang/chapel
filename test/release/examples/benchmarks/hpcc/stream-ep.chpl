//
// Embarassingly Parallel Implementation of STREAM Triad
//
// This version of the stream benchmark is not as elegant as
// stream.chpl.  It is a per-locale code with no communication in the
// actual computation.  It highlights the ability of a Chapel
// programmer to escape the global-view programming model and write
// codes with a fragmented, per-locale model.
//
// Comments marked with '***' point out differences with the global
// version of this benchmark in stream.chpl.
//

//
// *** Since this benchmark is written on a per-locale basis, there is no
// *** need to use the Block distribution.  The following use omits
// *** BlockDist, included in stream.chpl.
//
// Use standard modules for Timing routines, Type utility functions,
// and Random numbers
//
use Time, Types, Random;

//
// Use shared user module for computing HPCC problem sizes
//
use HPCCProblemSize;

//
// The number of vectors and element type of those vectors
//
const numVectors = 3;
type elemType = real(64);

//
// *** To ensure a local problem size, we spoof the number of vectors
// *** passed to the computeProblemSize function to be the number of
// *** vectors times the number of locales.
//
// Configuration constants to set the problem size (m) and the scalar
// multiplier, alpha
//
config const m = computeProblemSize(numVectors*numLocales, elemType),
             alpha = 3.0;

//
// Configuration constants to set the number of trials to run and the
// amount of error to permit in the verification
//
config const numTrials = 10,
             epsilon = 0.0;

//
// Configuration constants to indicate whether or not to use a
// pseudo-random seed (based on the clock) or a fixed seed; and to
// specify the fixed seed explicitly
//
config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.currentTime else 314159265;

//
// *** To ensure determinism of output, there is no more printing of the
// *** arrays in initVectors and verifyResults.
//
//
// Configuration constants to control what's printed -- benchmark
// parameters and/or statistics
//
config const printParams = true,
             printStats = true;

//
// The program entry point
//
proc main() {
  printConfiguration();   // print the problem size, number of trials, etc.

  //
  // *** Aggregates for collecting per-locale results for the minimum
  // *** execution time per trial, and whether verification passed
  //
  var minTimes: [LocaleSpace] real;
  var validAnswers: [LocaleSpace] bool;

  //
  // *** Fragment control so that we have a single task running on
  // *** every locale.
  //
  coforall loc in Locales do on loc {

    var execTime: [1..numTrials] real;

    //
    // *** A, B, and C are the three local vectors
    //
    var A, B, C: [1..m] elemType;

    initVectors(B, C);    // Initialize the input vectors, B and C

    for trial in 1..numTrials {                        // loop over the trials
      const startTime = getCurrentTime();              // capture the start time

      //
      // *** The main loop looks identical to stream.chpl.  However,
      // *** in this version we are iterating over arrays that are
      // *** not distributed.
      //
      forall (a, b, c) in zip(A, B, C) do
        a = b + alpha * c;

      execTime(trial) = getCurrentTime() - startTime;  // store the elapsed time
    }

    //
    // *** Write times and verification result into aggregates
    // *** declared above.  These are declared over LocaleSpace so we
    // *** can write to them in parallel.
    //
    minTimes[here.id] = min reduce execTime;
    validAnswers[here.id] = verifyResults(A, B, C);
  }

  //
  // *** Pass minimum, average, and maximum times to printResults
  //
  printResults(&& reduce validAnswers, minTimes);
}

//
// Print the problem size and number of trials
//
proc printConfiguration() {
  if (printParams) {
    //
    // *** Here we multiply m by the number of locales so that we can
    // *** print out the global problem size.
    //
    printProblemSize(elemType, numVectors, m * numLocales);
    writeln("Number of trials = ", numTrials, "\n");
  }
}

//
// *** Both initVectors and verifyResults are almost identical to
// *** stream.chpl even though they are called with arrays that are
// *** not distributed.  For initialization, the same random stream is
// *** used on each locale.  In the global version, a single logical
// *** stream of random numbers is used across all of the locales.
// ***
// *** In this version, we've omitted a way to print the arrays.  This
// *** ensures determinism of output.
//
// Initialize vectors B and C using a random stream of values
//
proc initVectors(B, C) {
  var randlist = new RandomStream(seed);

  randlist.fillRandom(B);
  randlist.fillRandom(C);

  delete randlist;
}

//
// Verify that the computation is correct
//
proc verifyResults(A, B, C) {

  //
  // recompute the computation, destructively storing into B to save space
  //
  forall (b, c) in zip(B, C) do
    b += alpha *c;  

  //
  // Compute the infinity-norm by computing the maximum reduction of the
  // absolute value of A's elements minus the new result computed in B.
  // "[i in I]" represents an expression-level loop: "forall i in I"
  //
  const infNorm = max reduce [(a,b) in zip(A,B)] abs(a - b);

  return (infNorm <= epsilon);    // return whether the error is acceptable
}

//
// Print out success/failure, the timings, and the GB/s value.
//
// *** Here we report maximum, average, and minimum times instead of
// *** total, average, and minimum.
//
proc printResults(successful, minTimes) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    const maxTime = max reduce minTimes,
          avgTime = + reduce minTimes / numLocales,
          minTime = min reduce minTimes;
    writeln("Execution time:");
    writeln("  max (seconds) = ", maxTime);
    writeln("  avg (seconds) = ", avgTime);
    writeln("  min (seconds) = ", minTime);

    const maxGBPerSec = numVectors * numBytes(elemType) * (m / minTime) * 1e-9,
          avgGBPerSec = numVectors * numBytes(elemType) * (m / avgTime) * 1e-9,
          minGBPerSec = numVectors * numBytes(elemType) * (m / maxTime) * 1e-9;
    writeln("Performance (GB/s):");
    writeln("  max = ", maxGBPerSec);
    writeln("  avg = ", avgGBPerSec);
    writeln("  min = ", minGBPerSec);
  }
}
