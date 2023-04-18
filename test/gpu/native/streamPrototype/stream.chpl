//
// Use standard modules for Block distributions, Timing routines, Type
// utility functions.
//
use Time, Types /*, Random */;

use GpuDiagnostics;

//
// Use shared user module for computing HPCC problem sizes
//
use HPCCProblemSize;

proc verifyLaunches() {
  use ChplConfig;
  param expected = if CHPL_GPU_MEM_STRATEGY == "unified_memory" then 12 else 16;
  const actual = getGpuDiagnostics()[0].kernel_launch;
  assert(actual == expected,
         "observed ", actual, " launches instead of ", expected);
}

config param useForeach = true;
config const useGpuDiags = true;
config const SI = true;

//
// The number of vectors and element type of those vectors
//
const numVectors = 3;
type elemType = real(64);

//
// Configuration constants to set the problem size (m) and the scalar
// multiplier, alpha
//

// TODO : we need the dynamic scope checking, because there is a reduction in
// `computeProblemSize`
config const m = 1024, /*computeProblemSize(numVectors, elemType),*/
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
/*
config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.oddCurrentTime else 314159265;
 */

//
// Configuration constants to control what's printed -- benchmark
// parameters, input and output arrays, and/or statistics
//
config const printParams = true,
             printArrays = false,
             printStats = true;

//
// The program entry point
//
proc main() {
  printConfiguration();   // print the problem size, number of trials, etc.

  if useGpuDiags then startGpuDiagnostics();
  on here.gpus[0] {
    //
    // ProblemSpace describes the index set for the three vectors.  It
    // is a 1D domain that has indices 1 to m.
    //
    const ProblemSpace = {1..m};

    //
    // A, B, and C are the three vectors, declared to store a variable of type
    // elemType for each index in ProblemSpace. As they are local to the `on`
    // statement that executes on GPU, the memory for these arrays will be
    // allocated on GPU-accessible memory.
    //
    var A, B, C: [ProblemSpace] elemType;

    initVectors(B, C);    // Initialize the input vectors, B and C

    var execTime: [1..numTrials] real;                 // an array of timings

    for trial in 1..numTrials {                        // loop over the trials
      const startTime = timeSinceEpoch().totalSeconds();              // capture the start time

      //
      // The main loop: Iterate over the vectors A, B, and C in a
      // parallel, zippered manner storing the elements as a, b, and c.
      // Compute the multiply-add on b and c, storing the result to a.
      //
      // This forall loop will be offloaded onto the GPU.
      if useForeach then
        foreach (a, b, c) in zip(A, B, C) do
          a = b + alpha * c;
      else
        forall (a, b, c) in zip(A, B, C) do
          a = b + alpha * c;

      execTime(trial) = timeSinceEpoch().totalSeconds() - startTime;  // store the elapsed time
    }

    const validAnswer = verifyResults(A, B, C);        // verify...
    printResults(validAnswer, execTime);               // ...and print the results
  }
  if useGpuDiags {
    stopGpuDiagnostics();
    verifyLaunches();
  }
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
proc initVectors(B, C) {
  /*

  TODO: fillRandom has a `forall`. We either need dynamic check and this
  function to be called outside of the `on gpu` statement, or we fix transitive
  GPU kernel creation.

  var randlist = new NPBRandomStream(eltType=real, seed=seed);

  randlist.fillRandom(B);
  randlist.fillRandom(C);
  */
  forall b in B do b = 1;
  forall c in C do c = 1;

  if (printArrays) {
    writeln("B is: ", B, "\n");
    writeln("C is: ", C, "\n");
  }
}

//
// Verify that the computation is correct
//
proc verifyResults(A, B, C) {
  if (printArrays) then writeln("A is:     ", A, "\n");  // optionally print A

  //
  // recompute the computation, destructively storing into B to save space
  //
  // Originally this was a forall, but I want the validation to be run on CPU.
  for (b, c) in zip(B, C) do
    b += alpha *c;  

  if (printArrays) then writeln("A-hat is: ", B, "\n");  // and A-hat too

  //
  // Compute the infinity-norm by computing the maximum reduction of the
  // absolute value of A's elements minus the new result computed in B.
  // "[i in I]" represents an expression-level loop: "forall i in I"
  //

  /*
  TODO: we can't do reductions on GPU right now. This forall also calls `abs`,
  which we don't handle today.
  const infNorm = max reduce [(a,b) in zip(A,B)] abs(a - b);
  */

  var infNorm = min(real);
  for (a,b) in zip(A,B) {
    /*
    TODO: for some reason I can't call `abs` here, even though it is inside a
    `for` loop.

     const delta = abs(a-b);
       
    */

    const delta = if a<b then b-a else a-b;
    if delta > infNorm then
      infNorm = delta;
  }

  return (infNorm <= epsilon);    // return whether the error is acceptable
}

//
// Print out success/failure, the timings, and the throughput
//
proc printResults(successful, execTimes) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    /*
    TODO: these reductions somehow compile but don't generate the right result.
     totalTime is always zero, but minTime is non zero ?

    const totalTime = + reduce execTimes,
          avgTime = totalTime / numTrials,
          minTime = min reduce execTimes;
    */

    var totalTime = 0.0;
    var minTime = max(real);
    for t in execTimes {
      totalTime += t;
      minTime = min(minTime, t);
    }
    var avgTime = totalTime/numTrials;

    writeln("Execution time:");
    writeln("  tot = ", totalTime);
    writeln("  avg = ", avgTime);
    writeln("  min = ", minTime);

    if SI {
      const GBPerSec =
        numVectors * numBytes(elemType) * (m / minTime) * 1e-9;
      writeln("Performance (GB/s) = ", GBPerSec);
    } else {
      const GiBPerSec =
        numVectors * numBytes(elemType) * (m / minTime) / (1<<30):real;
      writeln("Performance (GiB/s) = ", GiBPerSec);
    }
  }
}
