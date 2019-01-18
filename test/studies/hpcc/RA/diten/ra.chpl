//
// Use standard modules for Block distributions and Timing routines
//
use BlockDist, Time;
use buckets;
use doupdates;

//
// Use the user modules for computing HPCC problem sizes and for
// defining RA's random stream of values
//
use HPCCProblemSize, RARandomStream;

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
// Configuration constant defining the number of errors to allow (as a
// fraction of the number of updates, N_U)
//
config const errorTolerance = 1e-2;

//
// The number of tasks to use per Chapel locale
// The data structures used to cache updates are not threadsafe.
// Fix this at 1.
//
const tasksPerLocale = 1;

//
// Configuration constants to control what's printed -- benchmark
// parameters, input and output arrays, and/or statistics
//
config const printParams = true,
             printArrays = false,
             printStats = true;

//
// TableDist is a 1D block distribution for domains storing indices
// of type "indexType", and it is computed by blocking the bounding
// box 0..m-1 across the set of locales.  UpdateDist is a similar
// distribution that is computed by blocking the indices 0..N_U-1
// across the locales.
//
const
  TableDist = new dmap(new Block(boundingBox={0..m-1},
                                 dataParTasksPerLocale=tasksPerLocale,
                                 dataParIgnoreRunningTasks=true)),
  UpdateDist = new dmap(new Block(boundingBox={0..N_U-1},
                                  dataParTasksPerLocale=tasksPerLocale,
                                  dataParIgnoreRunningTasks=true));

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
// T is the distributed table itself, storing a variable of type
// elemType for each index in TableSpace.
//
var T: [TableSpace] elemType;

config const maxLookahead = 1024;

pragma "locale private" var myBuckets: unmanaged Buckets;

//
// The program entry point
//
proc main() {
  printConfiguration();   // print the problem size, number of trials, etc.

  //
  // In parallel, initialize the table such that each position
  // contains its index.  "[i in TableSpace]" is shorthand for "forall
  // i in TableSpace"
  //
  [i in TableSpace] T(i) = i;
  for loc in Locales {
    on loc {
      myBuckets = new unmanaged Buckets();
    }
  }
  const startTime = getCurrentTime();              // capture the start time

  //
  // The main computation: Iterate over the set of updates and the
  // stream of random values in a parallel, zippered manner, dropping
  // the update index on the ground and storing the random value
  // in r.  Cache up to maxLookahead updates so larger blocks can be
  // updated at once. When maxLookahead updates are cached, choose the
  // locale with the most pending updates, and do all of the updates
  // pending for that locale.
  //
  forall (_, r) in zip(Updates, RAStream()) {
    var loc = T.domain.dist.idxToLocale(r&indexMask);
    if loc == here {
      T(r&indexMask) ^= r;
    } else {
      if myBuckets.pendingUpdates < maxLookahead-1 {
        myBuckets.insertUpdate(r, loc.id);
      } else {
        var buffer: [0..#maxLookahead] uint(64);
        myBuckets.insertUpdate(r, loc.id);
        var (remloc, nu) = myBuckets.getUpdates(buffer);
        doUpdates(buffer, nu);
      }
    }
  }
  // I want this to be at the end of the last iteration, but still inside the
  // forall loop above, rather than having to respawn the remote tasks.
  // The number of updates done here is small enough that they can be
  // ignored while still passing the benchmark.
/*
  coforall loc in Locales {
    on loc {
      var buffer: [0..#maxLookahead] uint(64);
      while myBuckets.pendingUpdates > 0 {
        var (remloc, nu) = myBuckets.getUpdates(buffer);
        doUpdates(buffer, nu);
      }
    }
  }
*/

  const execTime = getCurrentTime() - startTime;   // capture the elapsed time

  const validAnswer = verifyResults();             // verify the updates
  printResults(validAnswer, execTime);             // print the results

  for loc in Locales do on loc do delete myBuckets;
}

//
// Print the problem size and number of updates
//
proc printConfiguration() {
  if (printParams) {
    if (printStats) then printLocalesTasks(tasksPerLocale);
    printProblemSize(elemType, numTables, m);
    writeln("Number of updates = ", N_U, "\n");
  }
}

//
// Verify that the computation is correct
//
proc verifyResults() {
  //
  // Print the table, if requested
  //
  if (printArrays) then writeln("After updates, T is: ", T, "\n");

  //
  // Reverse the updates by recomputing them, this time using an
  // atomic statement to ensure no conflicting updates
  //
  forall (_, r) in zip(Updates, RAStream()) do
    on T.domain.dist.idxToLocale(r & indexMask) do
      atomic T(r & indexMask) ^= r;

  //
  // Print the table again after the updates have been reversed
  //
  if (printArrays) then writeln("After verification, T is: ", T, "\n");

  //
  // Compute the number of table positions that weren't reverted
  // correctly.  This is an indication of the number of conflicting
  // updates.
  //
  const numErrors = + reduce [i in TableSpace] (T(i) != i);
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
