use Time;
use BlockDist;

use HPCCProblemSize, RARandomStream;


param numTables = 1;
type elemType = randType,
     indexType = randType;

config const n = computeProblemSize(elemType, numTables,
                                    returnLog2=true): indexType,
             N_U = 2**(n+2);   // numUpdates

const m = 2**n,                // probSize
      indexMask = m-1;

config const sequentialVerify = (numLocales < log2(m)),
             errorTolerance = 1e-2;

config const printParams = true,
             printArrays = false,
             printStats = true;


proc main() {
  printConfiguration();

  const TableDist = new dmap(new Block(rank=1, idxType=indexType, boundingBox={0..m-1}, targetLocales=Locales)),
       UpdateDist = new dmap(new Block(rank=1, idxType=indexType, boundingBox={0..N_U-1}, targetLocales=Locales));

  const TableSpace: domain(1, indexType) dmapped TableDist = {0..m-1};
  var T: [TableSpace] elemType;

  const UpdateSpace: domain(1, indexType) dmapped UpdateDist = {0..N_U-1};

  const startTime = getCurrentTime();

  [i in TableSpace] T(i) = i;

  forall (i,r) in zip(UpdateSpace, RAStream()) do
    T(r & indexMask) ^= r;

  const execTime = getCurrentTime() - startTime;

  const validAnswer = verifyResults(T, UpdateSpace);
  printResults(validAnswer, execTime);
}


proc printConfiguration() {
  if (printParams) {
    printProblemSize(elemType, numTables, m);
    writeln("Number of updates = ", N_U, "\n");
  }
}


proc verifyResults(T: [?TDom], UpdateSpace) {
  if (printArrays) then writeln("After updates, T is: ", T, "\n");

  var lock: sync bool = true;
  forall (i,r) in zip(UpdateSpace, RAStream()) {
    lock.readFE();
    atomic T(r & indexMask) ^= r;
    lock.writeEF(true);
  }

  if (printArrays) then writeln("After verification, T is: ", T, "\n");

  const numErrors = + reduce [i in TDom] (T(i) != i);
  if (printStats) then writeln("Number of errors is: ", numErrors, "\n");

  return numErrors <= (errorTolerance * N_U);
}


proc printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    writeln("Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }
}
