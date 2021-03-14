//
// Use standard modules for Block distributions and Timing routines
//
use BlockDist, Time;

//
// Use the user modules for computing HPCC problem sizes and for
// defining RA's random stream of values
//
use HPCCProblemSize, RARandomStream;

//
// Use atomic peek/poke
//
use PeekPoke;

//
// The number of tables as well as the element and index types of
// that table
//
const numTables = 1;
type elemType = randType,
     indexType = randType;

//
// Configuration constants defining log2(problem size) -- n -- and
// the number of updates -- N_U
//
config const n = computeProblemSize(numTables, elemType,
                                    returnLog2=true, retType=indexType),
             N_U = 2**(n+2);

//
// Constants defining the problem size (m) and a bit mask for table
// indexing
//
const m = 2**n,
      indexMask = m-1;

//
// Do verification?
//
config const verify = true;

//
// Configuration constant defining the number of errors to allow (as a
// fraction of the number of updates, N_U).  When using atomic, this
// should be 0.
//
param errorTolerance = 0.0;

//
// Configuration constants to control what's printed -- benchmark
// parameters, input and output arrays, and/or statistics
//
config const printParams = true,
             printArrays = false,
             printStats = true;

//
// Number of tasks per core to use during the main computation. Using
// multiple tasks per core may improve performance for communication
// and tasking configurations that have support for communication and
// computation overlap.
//
config const tasksPerCore = 1,
             tasksPerLocale = here.maxTaskPar * tasksPerCore;

//
// TableDist is a 1D block distribution for domains storing indices
// of type "indexType", and it is computed by blocking the bounding
// box 0..m-1 across the set of locales.  UpdateDist is a similar
// distribution that is computed by blocking the indices 0..N_U-1
// across the locales.
//
const TableDist = new dmap(new Block(boundingBox={0..m-1})),
      UpdateDist = new dmap(new Block(boundingBox={0..N_U-1},
                            dataParTasksPerLocale=tasksPerLocale));

//
// TableSpace describes the index set for the table.  It is a 1D
// domain storing indices of type indexType, it is distributed
// according to TableDist, and it contains the indices 0..m-1.
// Updates is an index set describing the set of updates to be made.
// It is distributed according to UpdateDist and contains the
// indices 0..N_U-1.
//
const TableSpace: domain(1, indexType) dmapped TableDist = {0..m-1},
      Updates: domain(1, indexType) dmapped UpdateDist = {0..N_U-1};


//
// The program entry point
//
proc main() {
  printConfiguration();   // print the problem size, number of trials, etc.

  //
  // T is the distributed table itself, storing a variable of type
  // elemType for each index in TableSpace.
  //
  var T: [TableSpace] atomic elemType;

  //
  // In parallel, initialize the table such that each position
  // contains its index.  "[i in TableSpace]" is shorthand for "forall
  // i in TableSpace"
  //
  [i in TableSpace] T(i).poke(i);

  const startTime = getCurrentTime();              // capture the start time

  //
  // The main computation: Iterate over the set of updates and the
  // stream of random values in a parallel, zippered manner, dropping
  // the update index on the ground and storing the random value
  // in r.  Compute the update using r both to compute the index and
  // as the update value.
  //
  forall (_, r) in zip(Updates, RAStream()) do
    T(r & indexMask).xor(r);

  const execTime = getCurrentTime() - startTime;   // capture the elapsed time

  const validAnswer = verifyResults(T);            // verify the updates
  printResults(validAnswer, execTime);             // print the results
}

//
// Print the problem size and number of updates
//
proc printConfiguration() {
  if (printParams) {
    if (printStats) then printLocalesTasks();
    printProblemSize(elemType, numTables, m);
    writeln("Number of updates = ", N_U, "\n");
  }
}

//
// Verify that the computation is correct
//
proc verifyResults(T) {
  if (!verify) then return true;

  //
  // Print the table, if requested
  //
  if (printArrays) then writeln("After updates, T is: ", T, "\n");

  //
  // Reverse the updates by recomputing them.
  //
   forall (_, r) in zip(Updates, RAStream()) do
     T(r & indexMask).xor(r);

  //
  // Print the table again after the updates have been reversed
  //
  if (printArrays) then writeln("After verification, T is: ", T, "\n");

  //
  // Compute the number of table positions that weren't reverted
  // correctly.  This is an indication of the number of conflicting
  // updates.
  //
  const numErrors = + reduce [i in TableSpace] (T(i).peek() != i);
  if (printStats) then writeln("Number of errors is: ", numErrors, "\n");

  //
  // Return whether or not the number of errors was within the benchmark's
  // tolerance.
  //
  return numErrors <= (errorTolerance * N_U);
}

//
// Print out success/failure, the execution time, and the GUPS value
//
proc printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    writeln("Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }
}
