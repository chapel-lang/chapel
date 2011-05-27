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

var T$: [TableSpace] sync elemType;

//
// Sync Data Array version
// Acquire lock corresponding to smaller index first. 
// No locality optimization
//
proc swapValuesSDA(myRIdx, mySIdx) {
  if (myRIdx < mySIdx) {
    const x = T$(myRIdx);
    T$(myRIdx) = T$(mySIdx);
    T$(mySIdx) = x;
    /* const x = T$(myRIdx); */
    /* const y = T$(mySIdx); */
    /* T$(mySIdx) = x; */
    /* T$(myRIdx) = y; */
  } else if (myRIdx > mySIdx) {
    const x = T$(mySIdx);
    T$(mySIdx) = T$(myRIdx);
    T$(myRIdx) = x;
    /* const x = T$(mySIdx); */
    /* const y = T$(myRIdx); */
    /* T$(myRIdx) = x; */
    /* T$(mySIdx) = y; */
  }
}

proc main() {
  printConfiguration(); 
  
  [i in TableSpace] T$(i).writeXF(i);

  // UNSYNC -- no synchronization 

  const startTime = getCurrentTime();            // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      const x = T$(myRIdx).readXX();
      T$(myRIdx).writeXF(T$(mySIdx).readXX());
      T$(mySIdx).writeXF(x);
    }

  const execTime = getCurrentTime() - startTime;  // capture the end time

  if (printStats) {
    writeln("UNSYNC: Execution time = ", execTime);
    writeln("UNSYNC: Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }

  swap();
}

proc swap() {
  // SDA -- sync data array

  const startTime = getCurrentTime();             // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      swapValuesSDA(myRIdx, mySIdx);
    }

  const execTime = getCurrentTime() - startTime;  // capture the end time
  
  if (printStats) {
    writeln("SDA: Execution time = ", execTime);
    writeln("SDA: Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }
}

proc printConfiguration() {
  if (printParams) {
    if (printStats) then writeln("Number of Locales = ", numLocales);
    printProblemSize(elemType, 1, m);
    writeln("Number of updates = ", N_U, "\n");
  }
}
