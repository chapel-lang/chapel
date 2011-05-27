use BlockDist, Time;

use HPCCProblemSize, RARandomStream;

use myParams;

const numTables = 1;
type indexType = randType,
  elemType = randType;

config const n = computeProblemSize(numTables, elemType,
				    returnLog2=true, retType=indexType),
  N_U = 2**(n+2),
  lockMask: uint(64) = 1;

const m = 2**n,
  lk = 2**(n-lockMask);

config const errorTolerance = 1e-2;

config const printParams = true,
             printArrays = false,
             printStats = true;

const TableDist = new dmap(new Block(boundingBox=[0..m-1])),
  UpdateDist = new dmap(new Block(boundingBox=[0..N_U-1])),
  LockDist = new dmap(new Block(boundingBox=[0..lk-1]));

const TableSpace: domain(1, indexType) dmapped TableDist = [0..m-1],
      Updates: domain(1, indexType) dmapped UpdateDist = [0..N_U-1],
      LockSpace: domain (1, indexType) dmapped LockDist = [0..lk-1];

var T: [TableSpace] elemType;
var TLock$: [LockSpace] sync bool;

//
// Sync Lock Array version
// Acquire lock corresponding to smaller index first. 
// Locality optimization: Optimize access to data array T
// Might fail in cases where lock is not in the same locale
//
proc swapValuesSLA(myRIdx, mySIdx, mySLocale) {
  var y: elemType;
  const myRLock = myRIdx >> lockMask;
  const mySLock = mySIdx >> lockMask;

  if (myRLock < mySLock) {
    TLock$(myRLock);
    TLock$(mySLock);
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    TLock$(mySLock) = true;
    TLock$(myRLock) = true;
  } else if (myRLock > mySLock) {
    TLock$(mySLock);
    TLock$(myRLock);
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    TLock$(myRLock) = true;
    TLock$(mySLock) = true;
  } else {
    TLock$(myRLock);
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    TLock$(myRLock) = true;
  }
}

//
// Sync Lock Array version
// Acquire lock corresponding to smaller index first. 
// Locality + Affinity optimization
//
proc swapValuesSLAAffinity(myRIdx, mySIdx, mySLocale) {
  var y: elemType;
  const myRLock = myRIdx >> lockMask;
  const mySLock = mySIdx >> lockMask;
  const myRLockLocale: locale = LockDist.idxToLocale(myRLock);
  const mySLockLocale: locale = LockDist.idxToLocale(mySLock);

  if (myRLock < mySLock) {
    on myRLockLocale do TLock$(myRLock);
    on mySLockLocale do TLock$(mySLock);
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    on mySLockLocale do TLock$(mySLock) = true;
    on myRLockLocale do TLock$(myRLock) = true;
  } else if (myRLock > mySLock) {
    on mySLockLocale do TLock$(mySLock);
    on myRLockLocale do TLock$(myRLock);
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    on myRLockLocale do TLock$(myRLock) = true;
    on mySLockLocale do TLock$(mySLock) = true;
  } else {
    on myRLockLocale do TLock$(myRLock);
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    on myRLockLocale do TLock$(myRLock) = true;
  }
}

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
  [i in LockSpace] TLock$(i).writeXF(true);

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

  swap();
}

proc swap() {
  // SLA -- mutex lock array

  const startTime = getCurrentTime();            // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      const mySLocale: locale = TableDist.idxToLocale(mySIdx);
      if useAffinity then
	swapValuesSLAAffinity(myRIdx, mySIdx, mySLocale);
      else
	swapValuesSLA(myRIdx, mySIdx, mySLocale);
    }
  
  const execTime = getCurrentTime() - startTime;  // capture the end time
  
  if (printStats) {
    writeln("SLA: Execution time = ", execTime);
    writeln("SLA: Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }
}

proc printConfiguration() {
  if (printParams) {
    if (printStats) then writeln("Number of Locales = ", numLocales);
    printProblemSize(elemType, 1, m);
    writeln("useAffinity = ", useAffinity);
    writeln("Number of updates = ", N_U, "\n");
  }
}