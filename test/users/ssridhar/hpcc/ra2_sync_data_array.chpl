use BlockDist, Time;

use HPCCProblemSize, RARandomStream; 

type indexType = randType,
  elemType = randType;

config const n: indexType = 10,
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

config param safeUpdates: bool = false;
config param useOn: bool = false;
config param seed1: randType = 0;
config param seed2: randType = 0x7fff;

def updateValues(myR: indexType, myS: indexType, factor: int(64), mySLocale: locale) {
  const myRIdx = indexMask(myR, n);
  const mySIdx = indexMask(myS, n);
  const myRVal = myS * factor:uint(64);
  const mySVal = myR * factor:uint(64); 

  if (myRIdx < mySIdx) {
    var x: elemType;

    // Acquire myRIdx Lock
    local x = T$(myRIdx);

    // Acquire mySIdx Lock, update mySIdx entry, and release lock   
    if useOn then
      on mySLocale {
	const mySIdx1 = mySIdx;
	const mySVal1 = mySVal;
	local T$(mySIdx1) += mySVal1;
      }
    else 
      T$(mySIdx) += mySVal;	

    // Update myRIdx entry and release lock
    local T$(myRIdx) = x + myRVal;

  } else if (myRIdx > mySIdx) {

    // Acquire mySIdx Lock
    if useOn then 
      on mySLocale {
	const mySIdx1 = mySIdx;
	local T$(mySIdx1);
      }
    else
      T$(mySIdx);

    // Acquire myRIdx Lock, update myRIdx entry, and release lock
    local T$(myRIdx) += myRVal;

    // Update mySIdx entry and release lock
    if useOn then 
      on mySLocale {
	const mySIdx1 = mySIdx;
	const mySVal1 = mySVal;
	local T$(mySIdx1) = T$(mySIdx1).readXX() + mySVal1;
      }
    else
      T$(mySIdx) = T$(mySIdx).readXX() + mySVal;

  } else {
    local T$(myRIdx) += (2 * myRVal);
  }
}

def updateValuesNoSync(myR: indexType, myS: indexType, mySLocale: locale) {
  const myRIdx = indexMask(myR, n);
  const mySIdx = indexMask(myS, n);
  const myRVal = myS;
  const mySVal = myR;

  if (myRIdx != mySIdx) {
    // Update myRIdx entry
    local T$(myRIdx).writeXF(T$(myRIdx).readXX() + myRVal);

    // Update mySIdx entry   
    if useOn then
      on mySLocale {
	const mySIdx1 = mySIdx;
	const mySVal1 = mySVal;
	local T$(mySIdx1).writeXF(T$(mySIdx1).readXX() + mySVal1);
      }
    else 
      T$(mySIdx).writeXF(T$(mySIdx).readXX() + mySVal);
  } else {
    local T$(myRIdx).writeXF(T$(myRIdx).readXX() + (2 * myRVal));
  }
}

def main() {
  printConfiguration(); 
  
  [i in TableSpace] T$(i).writeXF(0);
 
  const startTime = getCurrentTime();               // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) { 
      const myR = r;
      const myS = s;
      const mySLocale = TableDist.idxToLocale(indexMask(myS, n));
      if safeUpdates then
	updateValues(myR, myS, 1, mySLocale);
      else 
	updateValuesNoSync(myR, myS, mySLocale);
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
    writeln("RNG = ", whichRNG());
    writeln("Number of updates = ", N_U, "\n");
  }
}

def verifyResults() {
  if (printArrays) then writeln("After updates, T is: ", T$, "\n");

  var startTime = getCurrentTime();

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) { 
      const myR = r;
      const myS = s;
      const mySLocale = TableDist.idxToLocale(indexMask(myS, n));
      updateValues(myR, myS, -1, mySLocale);
    }

  const verifyTime = getCurrentTime() - startTime; 

  if (printArrays) then writeln("After verification, T is: ", T$, "\n");

  const numErrors = + reduce [i in TableSpace] (T$(i) != 0);
  if (printStats) {
    writeln("Number of errors is: ", numErrors, "\n");
    writeln("Verification time = ", verifyTime);
    writeln("Verification (GUPS) = ", (N_U / verifyTime) * 1e-9);
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