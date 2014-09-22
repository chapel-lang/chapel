//
// Use standard modules for Block distributions, Timing routines, Type
// utility functions, and Random numbers
//
use GPUDist, Time, Types, Random;

//
// Use shared user module for computing HPCC problem sizes
//
use HPCCProblemSize;

//
// The number of vectors and element type of those vectors
//
const numVectors = 3;
type elemType = real(32);

//
// Configuration constants to set the problem size (m) and the scalar
// multiplier, alpha
//
config const m = computeProblemSize(numVectors, elemType);

//
// Configuration constants to set the number of trials to run and the
// amount of error to permit in the verification
//
config const numTrials = 10,
             epsilon = 0.00001;

//
// The number of tasks to use per Chapel locale
//
//config const tasksPerLocale = min reduce Locales.numCores; // maxTaskPar?

//
// Configuration constants to indicate whether or not to use a
// pseudo-random seed (based on the clock) or a fixed seed; and to
// specify the fixed seed explicitly
//
config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.currentTime else 314159265;

//
// Configuration constants to control what's printed -- benchmark
// parameters, input and output arrays, and/or statistics
//
config const printParams = true,
             printArrays = false,
             printStats = true;

  config const threadsPerBlock = 128;
//
// The program entry point
//
proc main() {
  printConfiguration();   // print the problem size, number of trials, etc.

  //
  // BlockDist is a 1D block distribution that is computed by blocking
  // the bounding box 1..m across the set of locales
  //
  const GPUBlockDist = new GPUDist(rank=1, threadsPerBlock=threadsPerBlock);

  //
  // ProblemSpace describes the index set for the three vectors.  It
  // is a 1D domain storing 64-bit ints and is distributed according
  // to BlockDist.  It contains the indices 1..m.
  //
  const ProblemSpace: domain(1, int(64)) dmapped GPUBlockDist = [1..m];

  //
  // A, B, and C are the three distributed vectors, declared to store
  // a variable of type elemType for each index in ProblemSpace.
  //
  var A, B, C: [ProblemSpace] elemType;
  var _A, _B, _C : [1..m] real(32);

  const alpha = 3.0 : real(32);
  _A = 1.0 : real(32);
  A = _A;

  initVectors(_B, _C);    // Initialize the input vectors, B and C
  B = _B;
  C = _C;

  var execTime: [1..numTrials] real;                 // an array of timings

  for trial in 1..numTrials {                        // loop over the trials
    const startTime = getCurrentTime();              // capture the start time

    //
    // The main loop: Iterate over the vectors A, B, and C in a
    // parallel, zippered manner storing the elements as a, b, and c.
    // Compute the multiply-add on b and c, storing the result to a.
    //
    //forall (a, b, c) in (A, B, C) do
    //  a = b + alpha * c;
    forall i in ProblemSpace do
    	    A(i) = B(i) + alpha * C(i);

    execTime(trial) = getCurrentTime() - startTime;  // store the elapsed time
  }

  _A = A;

  const validAnswer = verifyResults(_A, _B, _C);        // verify...
  printResults(validAnswer, execTime);               // ...and print the results
}

//
// Print the problem size and number of trials
//
proc printConfiguration() {
  if (printParams) {
//    if (printStats) then printLocalesTasks(tasksPerLocale);
    printProblemSize(elemType, numVectors, m);
    writeln("Number of trials = ", numTrials, "\n");
  }
}

//
// Initialize vectors B and C using a random stream of values and
// optionally print them to the console
//
proc initVectors(B, C) {
  var randlist = new RandomStream(seed);

  randlist.fillRandom(B);
  randlist.fillRandom(C);

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
	var alpha = 3.0 : real(32);
  //
  // recompute the computation, destructively storing into B to save space
  //
  forall (b, c) in (B, C) do
    b += alpha *c;  

  if (printArrays) then writeln("A-hat is: ", B, "\n");  // and A-hat too

  //
  // Compute the infinity-norm by computing the maximum reduction of the
  // absolute value of A's elements minus the new result computed in B.
  // "[i in I]" represents an expression-level loop: "forall i in I"
  //
  const infNorm = max reduce [(a,b) in (A,B)] abs(a - b);
  writeln("error : ", infNorm);

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
	writeln("numBytes(elemType) = ", numBytes(elemType)," numVectors = ", numVectors);
  }
}
