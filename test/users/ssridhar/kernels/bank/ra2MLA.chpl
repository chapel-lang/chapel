use BlockDist, Time;

use HPCCProblemSize, RARandomStream; 

use MutexLock, myParams;

type indexType = randType,
  elemType = randType;

config const n: indexType = 10,
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
// No locality optimization
// Might fail in cases where lock is not in the same locale
//
proc updateValuesMLA(myR, myS, myRIdx, mySIdx) {
  const myRLock = myRIdx >> lockMask;
  const mySLock = mySIdx >> lockMask;
  var x: elemType;

  if (myRLock < mySLock) {
    mutex_lock(TLock(myRLock));
    mutex_lock(TLock(mySLock));
    T(myRIdx) -= myR;
    T(mySIdx) -= myS;
    mutex_unlock(TLock(mySLock));
    mutex_unlock(TLock(myRLock)); 
  } else if (myRLock > mySLock) {
    mutex_lock(TLock(mySLock));
    mutex_lock(TLock(myRLock));
    T(myRIdx) -= myR;
    T(mySIdx) -= myS;
    mutex_unlock(TLock(myRLock)); 
    mutex_unlock(TLock(mySLock));
  } else {
    mutex_lock(TLock(myRLock));
    T(myRIdx) -= myR;
    T(mySIdx) -= myS;
    mutex_unlock(TLock(myRLock));
  }
}

//
// Mutex Lock Array version
// Acquire lock corresponding to smaller index first.
// Affinity optimization: Optimze access to lock array
//
proc updateValuesMLAAffinity(myR, myS, myRIdx, mySIdx) {
  const myRLock = myRIdx >> lockMask;
  const mySLock = mySIdx >> lockMask;
  const myRLockLocale: locale = LockDist.idxToLocale(myRLock);
  const mySLockLocale: locale = LockDist.idxToLocale(mySLock);
  var x: elemType;

  if (myRLock < mySLock) {
    on myRLockLocale do mutex_lock(TLock(myRLock));
    on mySLockLocale do mutex_lock(TLock(mySLock));
    T(myRIdx) -= myR;
    T(mySIdx) -= myS;
    on mySLockLocale do mutex_unlock(TLock(mySLock));
    on myRLockLocale do mutex_unlock(TLock(myRLock));
  } else if (myRLock > mySLock) {
    on mySLockLocale do mutex_lock(TLock(mySLock));
    on myRLockLocale do mutex_lock(TLock(myRLock));
    T(myRIdx) -= myR;
    T(mySIdx) -= myS;
    on myRLockLocale do mutex_unlock(TLock(myRLock));
    on mySLockLocale do mutex_unlock(TLock(mySLock));
  } else {
    on myRLockLocale do mutex_lock(TLock(myRLock));
    T(myRIdx) -= myR;
    T(mySIdx) -= myS;
    on myRLockLocale do mutex_unlock(TLock(myRLock));
  }
}

proc main() {
  printConfiguration(); 
  
  [i in TableSpace] T(i) = 0;
  [i in LockSpace] mutex_init(TLock(i));
 
  const startTime = getCurrentTime();               // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) { 
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      T(myRIdx) += myR;
      T(mySIdx) += myS;
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
  if (printArrays) then writeln("After updates, T is: ", T, "\n");

  var startTime = getCurrentTime();

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) { 
      const myR = r;
      const myS = s;
      const myRIdx = indexMask(myR, n);
      const mySIdx = indexMask(myS, n);
      if useAffinity then
	updateValuesMLAAffinity(myR, myS, myRIdx, mySIdx);
      else
	updateValuesMLA(myR, myS, myRIdx, mySIdx);
    }

  const verifyTime = getCurrentTime() - startTime; 
  
  if (printArrays) then writeln("After verification, T is: ", T, "\n");

  const numErrors = + reduce [i in TableSpace] (T(i) != 0);
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
