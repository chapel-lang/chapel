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

var T$: [TableSpace] sync elemType;

//
// Sync Data Array version
// Acquire lock corresponding to smaller index first. 
// No locality optimization
// Might fail in cases where lock is not in the same locale
//
proc swapValuesSDA(myR: indexType, myS: indexType) {
  var x, y: elemType;
  
  if (myR < myS) {
    x = T$(myR);
    T$(myR) = T$(myS);
    T$(myS) = x;
  } else if (myR > myS) {
    x = T$(myS);
    T$(myS) = T$(myR);
    T$(myR) = x;
  } else {
    x = T$(myR);
    T$(myR) = x;
  }
}

//
// Sync Data Array version
// Acquire lock corresponding to smaller index first. 
// Locality optimization: Optimze access to data array T
//
proc swapValuesSDALocality(myR: indexType, myS: indexType, mySLocale: locale) {
  var x, y: elemType;

  if (myR < myS) {
    x = T$(myR);
    on mySLocale {
      y = T$(myS);
      T$(myS) = x;      
    }      
    T$(myR) = y;
  } else if (myR > myS) {
    on mySLocale {
      y = T$(myS);
      T$(myS) = T$(myR);      
    }      
    T$(myR) = y;
  } else {
    x = T$(myR);
    T$(myR) = x;
  }
}

proc swapValues(myR: indexType, myS: indexType) {
  var x, y: elemType;
  x = T$(myR).readXX();
  T$(myR).writeXF(T$(myS).readXX());
  T$(myS).writeXF(x);      
}

proc swapValuesLocality(myR: indexType, myS: indexType, mySLocale: locale) {
  var x, y: elemType;
  x = T$(myR).readXX();
  on mySLocale {
    y = T$(myS).readXX();
    T$(myS).writeXF(x);      
  }      
  T$(myR).writeXF(y);
}

proc main() {
  printConfiguration(); 
  
  [i in TableSpace] T$(i).writeXF(i);

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

  // SDA -- sync data array

  startTime = getCurrentTime();                  // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = indexMask(r, n);
      const myS = indexMask(s, n);
      if useOn {
	const mySLocale: locale = TableDist.idxToLocale(myS);
	swapValuesSDALocality(myR, myS, mySLocale);
      } else {
	swapValuesSDA(myR, myS);
      }
    }
      
  execTime = getCurrentTime() - startTime;       // capture the end time
  
  if (printStats) {
    writeln("SDA: Execution time = ", execTime);
    writeln("SDA: Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }
}

proc printConfiguration() {
  if (printParams) {
    if (printStats) then writeln("Number of Locales = ", numLocales);
    printProblemSize(elemType, 1, m);
    writeln("useOn = ", useOn);
    writeln("useAffinity = ", useAffinity);
    writeln("Number of updates = ", N_U, "\n");
  }
}