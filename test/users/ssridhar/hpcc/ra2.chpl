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

var T: [TableSpace] elemType;

config param safeUpdates: bool = false;
config param useOn: bool = false;
config param trackStmStats = false;
config param seed1: randType = 0x2;
config param seed2: randType = 0x7fff;
config param forkFast: bool = false;

def updateValues(myR: indexType, myS: indexType, factor: int(64), mySLocale: locale) {
  const myRIdx = indexMask(myR, n);
  const mySIdx = indexMask(myS, n);
  const myRVal = myS * factor:uint(64);
  const mySVal = myR * factor:uint(64);
  var x: elemType;
  if (myRIdx != mySIdx) {
    local T(myRIdx) += myRVal;
    if useOn then
      on mySLocale {
	const mySIdx1 = mySIdx;
	const mySVal1 = mySVal;
	if forkFast then
	  local T(mySIdx1) += mySVal1;
	else
	  T(mySIdx1) += mySVal1;
      }
    else {
      T(mySIdx) += mySVal;
    }
  } else {
    local T(myRIdx) += (2 * myRVal);
  }
}

def main() {
  printConfiguration(); 
  
  [i in TableSpace] T(i) = 0;
 
  const startTime = getCurrentTime();               // capture the start time

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) { 
      const myR = r;
      const myS = s;
      const mySLocale = TableDist.idxToLocale(indexMask(myS, n));
      if safeUpdates then
  	atomic updateValues(myR, myS, 1, mySLocale);
      else 
	updateValues(myR, myS, 1, mySLocale);
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
  writeln("Verification Begins");
  if (printArrays) then writeln("After updates, T is: ", T, "\n");

  if (trackStmStats) then startStmStats();

  var startTime = getCurrentTime();

  forall ( , r, s) in (Updates, RAStream(seed1), RAStream(seed2)) do
    on TableDist.idxToLocale(indexMask(r, n)) { 
      const myR = r;
      const myS = s;
      const mySLocale = TableDist.idxToLocale(indexMask(myS, n));
      atomic updateValues(myR, myS, -1, mySLocale);
    }

  const verifyTime = getCurrentTime() - startTime; 
  
  if trackStmStats then stopStmStats();

  if (printArrays) then writeln("After verification, T is: ", T, "\n");

  const numErrors = + reduce [i in TableSpace] (T(i) != 0);
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