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

proc swapValues(myR: indexType, myS: indexType) {
  var x, y: elemType;

  x = T(myR);
  T(myR) = T(myS);
  T(myS) = x;
}

proc swapValuesLocality(myR: indexType, myS: indexType, mySLocale: locale) {
  var x, y: elemType;

  x = T(myR);
  on mySLocale {
    y = T(myS);
    T(myS) = x;      
  }      
  T(myR) = y;
}

proc main() {
  printConfiguration(); 
  
  [i in TableSpace] T(i) = i;

  // UNSYNC -- no synchronization 

  var startTime = getCurrentTime();               // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = indexMask(r, n);
      const myS = indexMask(s, n);
      if useOn {
	const mySLocale: locale = TableDist.idxToLocale(myS);
	swapValuesLocality(myR, myS, mySLocale);
      } else {
	swapValues(myR, myS);
      }
    }

  var execTime = getCurrentTime() - startTime;   // capture the end time

  if (printStats) {
    writeln("UNSYNC: Execution time = ", execTime);
    writeln("UNSYNC: Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }

  // ATOMIC -- atomic transactions

  if (trackStmStats) then startStmStats();

  startTime = getCurrentTime();                  // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = indexMask(r, n);
      const myS = indexMask(s, n);
      if useOn {
	const mySLocale: locale = TableDist.idxToLocale(myS);
	atomic swapValuesLocality(myR, myS, mySLocale);
      } else {
	atomic swapValues(myR, myS);
      }
    }

  execTime = getCurrentTime() - startTime;       // capture the end time

  if (printStats) {
    writeln("ATOMIC: Execution time = ", execTime);
    writeln("ATOMIC: Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }

  if trackStmStats then stopStmStats();
}

proc printConfiguration() {
  if (printParams) {
    if (printStats) then writeln("Number of Locales = ", numLocales);
    printProblemSize(elemType, 1, m);
    writeln("Number of updates = ", N_U, "\n");
  }
}
