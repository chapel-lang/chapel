use BlockDist, Time;

use HPCCProblemSize, RARandomStream; 

use myParams;

type indexType = randType,
  elemType = randType;

config const n: indexType = 10,
  N_U = 2**(n+2);

const m = 2**n;

config const errorTolerance = 1e-2;

config const printParams = true,
             printArrays = false,
             printStats = true;

const TableDist = new dmap(new Block(boundingBox=[0..m-1])),
  UpdateDist = new dmap(new Block(boundingBox=[0..N_U-1]));

const TableSpace: domain(1, indexType) dmapped TableDist = [0..m-1],
      Updates: domain(1, indexType) dmapped UpdateDist = [0..N_U-1];

var T$: [TableSpace] sync elemType;

proc updateValuesSDA(myR, myS, myRIdx, mySIdx) {
  if (myRIdx < mySIdx) {
    // Acquire myRIdx Lock
    // Acquire mySIdx Lock, update mySIdx entry, and release mySIdx lock   
    // Update myRIdx entry and release myRIdx lock
    const x = T$(myRIdx);
    T$(mySIdx) -= myS;	
    T$(myRIdx) = x - myR;
  } else if (myRIdx > mySIdx) {
    // Acquire mySIdx Lock
    // Acquire myRIdx Lock, update myRIdx entry, and release myRIdx lock
    // Update mySIdx entry and release mySIdx lock
    const x = T$(mySIdx);
    T$(myRIdx) -= myR;
    T$(mySIdx) = x - myS;
  } else {
    // Special case: cannot acquire same lock twice
    T$(myRIdx) -= (2 * myR);
  }
}

proc main() {
  printConfiguration(); 
  
  [i in TableSpace] T$(i).writeXF(0);
 
  const startTime = getCurrentTime();               // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) { 
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      T$(myRIdx).writeXF(T$(myRIdx).readXX() + myR);
      T$(mySIdx).writeXF(T$(mySIdx).readXX() + myS);
    }

  const execTime = getCurrentTime() - startTime;   // capture the end time

  const validAnswer = verifyResults();             // verify the updates
  printResults(validAnswer, execTime);             // print the results}
}

proc printConfiguration() {
  if (printParams) {
    if (printStats) then writeln("Number of Locales = ", numLocales);
    printProblemSize(elemType, 1, m);
    writeln("Atomic Update = ", safeUpdates);
    writeln("UseOn = ", useOn);
    writeln("RNG = ", whichRNG());
    writeln("Number of updates = ", N_U, "\n");
  }
}

proc verifyResults() {
  if (printArrays) then writeln("After updates, T is: ", T$, "\n");

  var startTime = getCurrentTime();

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) { 
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      updateValuesSDA(myR, myS, myRIdx, mySIdx);
    }

  const verifyTime = getCurrentTime() - startTime; 

  if (printArrays) then writeln("After verification, T is: ", T$, "\n");

  const numErrors = + reduce [i in TableSpace] (T$(i) != 0);
  if (printStats) {
    writeln("Number of errors is: ", numErrors, "\n");
    writeln("Verification time = ", verifyTime);
    writeln("Verification (GUPS) = ", (N_U / verifyTime) * 1e-9);
  }

  return numErrors <= (errorTolerance * N_U);
}

proc printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    writeln("Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }
}
