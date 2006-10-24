// standard modules for timing
use Time;

// user modules for computing problem size and generating random numbers
use HPCCProblemSize;
use RARandomStream;


// the number of tables we'll be using, the element type, and
// the type used to index into the table
param numTables = 1;
type elemType = randType,
     indexType = randType;

// configuration constants for specifying the problem size and # of updates
config const n = lg(computeProblemSize(elemType, numTables,
                                       returnPow2=true)): indexType,
             N_U = 2**(n+2);   // numUpdates

// configuration constants for controlling output options
config const printParams = true,
             printArrays = false,
             printStats = true;

// configuration constants for performing the verification
config const sequentialVerify = false,
             errorTolerance = 1.0e-2;


// constants related to the problem size
const m = 2**n,                // probSize
      indexMask = m-1;         // mask for indexing into table


def main() {
  printConfiguration();

  const TableSpace: domain(1, indexType) distributed(Block) = [0..m);
  var T: [TableSpace] elemType;

  const updateSpace: domain(1, indexType) distributed(Block) = [0..N_U);

  const startTime = getCurrentTime();

  [i in TableSpace] T(i) = i;

  // want this to be updateSpace.subBlocks but can't put iterator in class
  forall block in subBlocks(updateSpace) do
    for r in RAStream(block.numIndices, block.low) do
      T(r & indexMask) ^= r;

  const execTime = getCurrentTime() - startTime;

  const validAnswer = verifyResults(T, updateSpace);

  printResults(validAnswer, execTime);
}


def printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numTables, m);
    writeln("Number of updates = ", N_U);
    writeln();
  }
}


def verifyResults(T: [?TDom], updateSpace) {
  if (printArrays) then writeln("After updates, T is: ", T, "\n");

  if (sequentialVerify) then
    for r in RAStream(N_U) do
      T(r & indexMask) ^= r;
  else
    forall i in updateSpace {
      const r = getNthRandom(i+1);   // BLC: would be nice to get rid of this
      atomic T(r & indexMask) ^= r;
    }

  if (printArrays) then writeln("After verification, T is: ", T, "\n");

  const numErrors = + reduce [i in TDom] (T(i) != i);
  if (printStats) then writeln("Number of errors is: ", numErrors, "\n");

  return numErrors <= (errorTolerance * N_U);
}


def printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    writeln("Performance (GUPS) = ", N_U / execTime * 1.0e-9);
  }
}
