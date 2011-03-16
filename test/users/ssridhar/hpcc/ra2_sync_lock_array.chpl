use BlockDist, Time;

use HPCCProblemSize, RARandomStream; 

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
var TLock$: [LockSpace] sync bool;

config param safeUpdates: bool = false;
config param useOn: bool = false;
config param seed1: randType = 0;
config param seed2: randType = 0x7fff;
config param forkFast: bool = false;

proc updateValues(myR: indexType, myS: indexType, factor: int(64), mySLocale: locale) {
  const myRIdx = indexMask(myR, n);
  const mySIdx = indexMask(myS, n);
  const myRVal = myS * factor:uint(64);
  const mySVal = myR * factor:uint(64); 
  const myRLock = myRIdx >> lockMask;
  const mySLock = mySIdx >> lockMask;
  var x: elemType;

  if (myRLock < mySLock) {
    // Acquire myRLock
    local TLock$(myRLock);

    // Acquire mySIndex Lock, update mySIdx entry
    if useOn {
      on mySLocale { // LockDist.idxToLocale(mySLock) {
	const mySIdx1 = mySIdx;
	const mySVal1 = mySVal;
	const mySLock1 = mySLock;
	local {
	  TLock$(mySLock1);
	  T(mySIdx1) += mySVal1;
	}
      }
    } else {
      TLock$(mySLock);
      T(mySIdx) += mySVal;
    } 

    // Update myRIdx
    local T(myRIdx) += myRVal;

    // Release mySIdx Lock
    if useOn then
      on mySLocale { // LockDist.idxToLocale(mySLock) {
	const mySLock1 = mySLock;
	local TLock$(mySLock1) = true;
      }
    else 
      TLock$(mySLock) = true;

    // Release myRLock
    local TLock$(myRLock) = true; 
  } else if (myRLock > mySLock) {
    // Acquire mySIdx Lock
    if useOn then
      on mySLocale { // LockDist.idxToLocale(mySLock) {
	const mySLock1 = mySLock;
	local TLock$(mySLock1);
      }
    else 
      TLock$(mySLock);

    // Acquire myRLock and update myRIdx
    local {
      TLock$(myRLock);
      T(myRIdx) += myRVal;
    }

    // Update mySIdx
    if useOn then
      on TableDist.idxToLocale(mySIdx) {
	const mySIdx1 = mySIdx;
	const mySVal1 = mySVal;
	if forkFast then local T(mySIdx1) += mySVal1;
	else T(mySIdx1) += mySVal1;
      }
    else 
      T(mySIdx) += mySVal;

    // Release myRLock
    local TLock$(myRLock) = true; 

    // Release mySIdx Lock
    if useOn then
      on mySLocale { // LockDist.idxToLocale(mySLock) {
	const mySLock1 = mySLock;
	local TLock$(mySLock1) = true;
      }
    else 
      TLock$(mySLock) = true;

  } else {
    local {
      TLock$(myRLock);
      T(myRIdx) += myRVal;
      T(mySIdx) += mySVal;
      TLock$(myRLock) = true; 
    } 
  }
}

proc updateValuesNoSync(myR: indexType, myS: indexType, mySLocale: locale) {
  const myRIdx = indexMask(myR, n);
  const mySIdx = indexMask(myS, n);
  const myRVal = myS;
  const mySVal = myR;

  if (myRIdx != mySIdx) {
    local T(myRIdx) += myRVal;
    if useOn then
      on mySLocale {
	const mySVal1 = mySVal;
	const mySIdx1 = mySIdx;
	if forkFast then local T(mySIdx1) += mySVal1;
	else T(mySIdx1) += mySVal1;
      }
    else 
      T(mySIdx) += mySVal;
  } else {
    local T(myRIdx) += (2 * myRVal);
  }
}

proc main() {
  printConfiguration(); 
  
  [i in TableSpace] T(i) = 0;
  [i in LockSpace] TLock$(i).writeXF(true);
 
  const startTime = getCurrentTime();               // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) { 
      const myR = r;
      const myS = s;
      if safeUpdates {
	const mySLocale: locale = LockDist.idxToLocale(indexMask(myS, n) >> lockMask);
	updateValues(myR, myS, 1, mySLocale);
      } else {
	const mySLocale: locale = TableDist.idxToLocale(indexMask(myS, n));
	updateValuesNoSync(myR, myS, mySLocale);
      }
    }

  const execTime = getCurrentTime() - startTime;   // capture the end time

  const validAnswer = verifyResults();             // verify the updates
  printResults(validAnswer, execTime);             // print the results
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
      const mySLocale: locale = LockDist.idxToLocale(indexMask(myS, n) >> lockMask);
      updateValues(myR, myS, -1, mySLocale);
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