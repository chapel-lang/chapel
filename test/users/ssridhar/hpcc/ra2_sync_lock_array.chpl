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
config param trackStmStats = false;
config param useLCG: bool = true;
config param seed1: randType = 0;
config param seed2: randType = 0x7fffffffffffffff;

def indexMask(r: randType): randType {
  if useLCG then
    return r >> (64 - n);
  else
    return r & (m - 1);
}

def updateValues (myR:indexType, myS:indexType, factor: int(64)) {
  const myRIdx = indexMask(myR);
  const mySIdx = indexMask(myS);
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
      on LockDist.idxToLocale(mySLock) {
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
      on LockDist.idxToLocale(mySLock) {
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
      on LockDist.idxToLocale(mySLock) {
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
	local T(mySIdx1) += mySVal1;
      }
    else 
      T(mySIdx) += mySVal;

    // Release myRLock
    local TLock$(myRLock) = true; 

    // Release mySIdx Lock
    if useOn then
      on LockDist.idxToLocale(mySLock) {
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

def updateValuesNoSync(myR: indexType, myS: indexType) {
  const myRIdx = indexMask(myR);
  const mySIdx = indexMask(myS);
  const myRVal = myS;
  const mySVal = myR;

  if (myRIdx != mySIdx) {
    local T(myRIdx) += myRVal;
    if useOn then
      on TableDist.idxToLocale(mySIdx) {
	const mySVal1 = mySVal;
	const mySIdx1 = mySIdx;
	local T(mySIdx1) += mySVal1;
      }
    else 
      T(mySIdx) += mySVal;
  } else {
    local T(myRIdx) += (2 * myRVal);
  }
}

def main() {
  printConfiguration(); 
  
  [i in TableSpace] T(i) = 0;
  [i in LockSpace] TLock$(i).writeXF(true);
 
  const startTime = getCurrentTime();               // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1, useLCG), RAStream(seed2, useLCG)) do
    on TableDist.idxToLocale(indexMask(r)) { 
      const myR = r;
      const myS = s;
      if safeUpdates then
	updateValues(myR, myS, 1);
      else {
	updateValuesNoSync(myR, myS);
      }
    }

  const execTime = getCurrentTime() - startTime;   // capture the end time

  const validAnswer = verifyResults();             // verify the updates
  printResults(validAnswer, execTime);             // print the results}
}

def printConfiguration() {
  if (printParams) {
    if (printStats) then writeln("Number of Locales = ", numLocales);
    printProblemSize(elemType, 1, m);
    writeln("Atomic Update = ", safeUpdates);
    writeln("UseOn = ", useOn);
    writeln("Number of updates = ", N_U, "\n");
  }
}

def verifyResults() {
  if (printArrays) then writeln("After updates, T is: ", T, "\n");

  if (trackStmStats) then startStmStats();

  var startTime = getCurrentTime();

  forall ( , r, s) in (Updates, RAStream(seed1, useLCG), RAStream(seed2, useLCG)) do
    on TableDist.idxToLocale(indexMask(r)) { 
      const myR = r;
      const myS = s;
	updateValues(myR, myS, -1);
    }

  const verifyTime = getCurrentTime() - startTime; 
  
  if trackStmStats then stopStmStats();

  if (printArrays) then writeln("After verification, T is: ", T, "\n");

  const numErrors = + reduce [i in TableSpace] (T(i) != 0);
  if (printStats) {
    writeln("Number of errors is: ", numErrors, "\n");
    writeln("Verification time = ", verifyTime);
  }

  return numErrors <= (errorTolerance * N_U);
}

def printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    writeln("Performance (GUPS) = ", (N_U / execTime) * 1e-9);
  }
}