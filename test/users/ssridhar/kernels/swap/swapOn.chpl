use BlockDist, Time;

use HPCCProblemSize, RARandomStream;

use myParams; 

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

proc swapValues(myRIdx, mySIdx, mySLocale) {
  var y: elemType;
  const x = T(myRIdx);
  on mySLocale {
    y = T(mySIdx);
    T(mySIdx) = x;      
  }
  T(myRIdx) = y;
}

proc main() {
  printConfiguration(); 
  
  [i in TableSpace] T(i) = i;

  // UNSYNC -- no synchronization 

  const startTime = getCurrentTime();            // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      const mySLocale: locale = TableDist.idxToLocale(mySIdx);
      swapValues(myRIdx, mySIdx, mySLocale);
    }

  const execTime = getCurrentTime() - startTime;  // capture the end time

  if (printStats) {
    writeln("UNSYNC: Execution time = ", execTime);
    writeln("UNSYNC: Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }
  
  // There is no method to restart the random number generator
  // expect to call it from a different function. 
  swap();
}

proc swap() {
  // ATOMIC -- atomic transactions

  if (trackStmStats) then startStmStats();

  const startTime = getCurrentTime();             // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      const mySLocale: locale = TableDist.idxToLocale(mySIdx);
      atomic swapValues(myRIdx, mySIdx, mySLocale);
    }

  const execTime = getCurrentTime() - startTime;  // capture the end time

  if trackStmStats then stopStmStats();

  if (printStats) {
    writeln("ATOMIC: Execution time = ", execTime);
    writeln("ATOMIC: Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }
}

proc printConfiguration() {
  if (printParams) {
    if (printStats) then writeln("Number of Locales = ", numLocales);
    printProblemSize(elemType, 1, m);
    writeln("Number of updates = ", N_U, "\n");
  }
}
