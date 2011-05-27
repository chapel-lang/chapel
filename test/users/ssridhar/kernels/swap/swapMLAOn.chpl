use BlockDist, Time;

use HPCCProblemSize, RARandomStream;

use myParams, MutexLock;

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
var TLock: [LockSpace] mutex_p;

//
// Mutex Lock Array version
// Acquire lock corresponding to smaller index first. 
// Locality optimization: Optimize access to data array T
// Might fail in cases where lock is not in the same locale
//
proc swapValuesMLA(myRIdx, mySIdx, mySLocale) {
  var y: elemType;
  const myRLock = myRIdx >> lockMask;
  const mySLock = mySIdx >> lockMask;

  if (myRLock < mySLock) {
    mutex_lock(TLock(myRLock));
    mutex_lock(TLock(mySLock));
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    mutex_unlock(TLock(mySLock));
    mutex_unlock(TLock(myRLock));
  } else if (myRLock > mySLock) {
    mutex_lock(TLock(mySLock));
    mutex_lock(TLock(myRLock));
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    mutex_unlock(TLock(myRLock));
    mutex_unlock(TLock(mySLock));
  } else {
    mutex_lock(TLock(myRLock));
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    mutex_unlock(TLock(myRLock));
  }
}

//
// Mutex Lock Array version
// Acquire lock corresponding to smaller index first. 
// Locality + Affinity optimization
//
proc swapValuesMLAAffinity(myRIdx, mySIdx, mySLocale) {
  var y: elemType;
  const myRLock = myRIdx >> lockMask;
  const mySLock = mySIdx >> lockMask;
  const myRLockLocale: locale = LockDist.idxToLocale(myRLock);
  const mySLockLocale: locale = LockDist.idxToLocale(mySLock);

  if (myRLock < mySLock) {
    on myRLockLocale do mutex_lock(TLock(myRLock));
    on mySLockLocale do mutex_lock(TLock(mySLock));
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    on mySLockLocale do mutex_unlock(TLock(mySLock));
    on myRLockLocale do mutex_unlock(TLock(myRLock));
  } else if (myRLock > mySLock) {
    on mySLockLocale do mutex_lock(TLock(mySLock));
    on myRLockLocale do mutex_lock(TLock(myRLock));
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    on myRLockLocale do mutex_unlock(TLock(myRLock));
    on mySLockLocale do mutex_unlock(TLock(mySLock));
  } else {
    on myRLockLocale do mutex_lock(TLock(myRLock));
    const x = T(myRIdx);
    on mySLocale {
      y = T(mySIdx);
      T(mySIdx) = x;      
    }      
    T(myRIdx) = y;
    on myRLockLocale do mutex_unlock(TLock(myRLock));
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
  [i in LockSpace]  mutex_init(TLock(i));

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
  // MLA -- mutex lock array

  const startTime = getCurrentTime();            // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) {
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      const mySLocale: locale = TableDist.idxToLocale(mySIdx);
      if useAffinity then
	swapValuesMLAAffinity(myRIdx, mySIdx, mySLocale);
      else
	swapValuesMLA(myRIdx, mySIdx, mySLocale);
    }
  
  const execTime = getCurrentTime() - startTime;  // capture the end time
  
  if (printStats) {
    writeln("MLA: Execution time = ", execTime);
    writeln("MLA: Performance (GUPS) = ", (N_U / execTime) * 1e-9);
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