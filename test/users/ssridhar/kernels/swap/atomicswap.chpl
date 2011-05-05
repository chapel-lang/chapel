use BlockDist, Time;

use HPCCProblemSize, RARandomStream, myParams; 

const numTables = 1;
type indexType = randType,
  elemType = randType;

config const n = computeProblemSize(numTables, elemType,
				    returnLog2=true, retType=indexType),
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

var T: [TableSpace] elemType;

proc swapValues(i: indexType, j: indexType) {
  var x, y: elemType;
  x = T(i);
  if useOn {
    on TableDist.idxToLocale(j) {
      y = T(j);
      T(j) = x;      
    }      
  } else {
    y = T(j);
    T(j) = x;
  }
  T(i) = y;
}

proc main() {
  printConfiguration(); 
  
  [i in TableSpace] T(i) = i;
  
  const startTime = getCurrentTime();               // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = indexMask(r, n);
      const myS = indexMask(s, n);
      if safeUpdates then
	atomic swapValues(myR, myS);
      else 
        swapValues(myR, myS);
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
    writeln("Number of updates = ", N_U, "\n");
  }
}

proc verifyResults() {
  var numErrors: int;

  if (printArrays) then writeln("After swap, T is: ", T, "\n");

  var startTime = getCurrentTime();

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = indexMask(r, n);
      const myS = indexMask(s, n);
      atomic swapValues(myS, myR);
    }

  const verifyTime = getCurrentTime() - startTime; 

  if (printArrays) then writeln("After verification, T is: ", T, "\n");
 
  numErrors = + reduce [i in TableSpace] (T(i) != i);

  if (printStats) {
    writeln("Number of errors is: ", numErrors, "\n");
    writeln("Verification time = ", verifyTime);
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