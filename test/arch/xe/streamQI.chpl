// streamQI.chpl (qthreads, Integrated)
//
// Embarrassingly Parallel Implementation of STREAM Triad
//
// This version of the stream benchmark is not as elegant as
// stream.chpl.  It is a per-locale code with no communication in the
// actual computation.  It highlights the ability of a Chapel
// programmer to escape the global-view programming model and write
// codes with a fragmented, per-locale model.
//
// This version uses qthreads to inspect the target architecture and
// control task dispatching.  It assumes shared memory on each locale, but 
// initializes and processes each segment within respective workers.
//
// A specialized version of the iterators in DefaultRectangular are used to 
// dispatch the inner forall loop.
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
use XEQ;

//
// The number of vectors and element type of those vectors
//
const numVectors = 3;
type elemType = real(64);

//
// *** To ensure a large enough local problem size, we spoof the number of
// *** vectors passed to the computeProblemSize function to be the number of
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
  var minTimes: [rootLocale.getLocaleSpace()] real;
  var validAnswers: [rootLocale.getLocaleSpace()] bool;

  //
  // *** Fragment control so that we have a single task running on
  // *** every locale.
  //
  coforall loc in rootLocale.getLocales() do on loc {

    //
    // *** We declare these variables outside of the local block since
    // *** we'll need to access them when we write back to the global
    // *** aggregates declared above.
    //
    var validAnswer: bool;
    var execTime: [1..numTrials] real;

    //
    // *** Indicates that all of the code in this block is local to
    // *** this locale.  There is no communication.  A violation will
    // *** result in an error, though error checking is disabled with
    // *** --fast or --no-checks.
    //
    local {
      //
      // *** A, B, and C are the three local vectors
      //
      var A, B, C: [1..m] elemType;

      initVectors(B, C);    // Initialize the input vectors, B and C

      for trial in 1..numTrials {                        // loop over the trials
        const startTime = getCurrentTime();              // capture the start time
        forall j in 1..m do
          A[j] = B[j] + alpha * C[j];
        execTime[trial] = getCurrentTime() - startTime;  // store the elapsed time
      }
        
      validAnswer = verifyResults(A, B, C);              // verify...
    }

    //
    // *** Write times and verification result into aggregates
    // *** declared above.  These are declared over LocaleSpace so we
    // *** can write to them in parallel.
    //
    minTimes[here.id] = min reduce execTime;
    validAnswers[here.id] = validAnswer;
  }

  //
  // *** Pass minimum, average, and maximum times to printResults
  //
  printResults(&& reduce validAnswers, minTimes);
}

// Compute the chunk associated with the given chunk number.
// Each chunk is executed on a different sublocale.
proc getChunk(nChunk:int, chunkCount, problemSize)
{
  var start = ((nChunk-1) * problemSize) / chunkCount + 1;
  var end = ((nChunk) * problemSize) / chunkCount;
  return start..end;
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
    writeln("Number of trials = ", numTrials);
    writeln();
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
// *** ensures determinism of output.  Printing the arrays also
// *** violates the locality constraint imposed by the local block
// *** from which these functions are called.
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
  forall (b, c) in (B, C) do
    b += alpha *c;  

  //
  // Compute the infinity-norm by computing the maximum reduction of the
  // absolute value of A's elements minus the new result computed in B.
  // "[i in I]" represents an expression-level loop: "forall i in I"
  //
  const infNorm = max reduce [(a,b) in (A,B)] abs(a - b);

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
