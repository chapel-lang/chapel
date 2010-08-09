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
config param trackStmStats = false;

def main() {
  printConfiguration(); 
  
  [i in TableSpace] T$(i).writeXF(0);
 
  const startTime = getCurrentTime();               // capture the start time

  writeln("begin update");

  forall ( , r, s) in (Updates, LCGRAStream(0), LCGRAStream(1)) {
    on TableDist.idxToLocale(r >> (64 - n)) {
      const i = r >> (64 - n);
      const j = s >> (64 - n);
      if safeUpdates {
	if (i == j) {
	  T$(i) = T$(i) + (2 * i);
	} else if (i < j) {
	  const x = T$(i);
	  const y = T$(j);
	  T$(j) = y + i;
	  T$(i) = x + j;
	} else {
	  const y = T$(j);
	  const x = T$(i);
	  T$(i) = x + j;
	  T$(j) = y + i;
	} 
      } else {
	if (i == j) {
	  T$(i).writeXF(T$(i).readXX() + (2 * i));
	} else if (i < j) {
	  const x = T$(i).readXX();
	  const y = T$(j).readXX();
	  T$(j).writeXF(y + i);
	  T$(i).writeXF(x + j);
	} else {
	  const y = T$(j).readXX();
	  const x = T$(i).readXX();
	  T$(i).writeXF(x + j);
	  T$(j).writeXF(y + i);
	} 
      }
    }
  }

  writeln("end update");

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
  if (printArrays) then writeln("After updates, T is: ", T$, "\n");

  if (trackStmStats) then startStmStats();

  var startTime = getCurrentTime();

  forall ( , r, s) in (Updates, LCGRAStream(0), LCGRAStream(1)) {
    on TableDist.idxToLocale(r >> (64 - n)) {
      const i = r >> (64 - n);
      const j = s >> (64 - n);
      if (i == j) {
	T$(i) = T$(i) - (2 * i);
      } else if (i < j) {
	const x = T$(i);
	const y = T$(j);
	T$(j) = y - i;
	T$(i) = x - j;
      } else {
	const y = T$(j);
	const x = T$(i);
	T$(i) = x - j;
	T$(j) = y - i;
      } 
    }
  }

  const verifyTime = getCurrentTime() - startTime; 
  
  if trackStmStats then stopStmStats();

  if (printArrays) then writeln("After verification, T is: ", T$, "\n");

  const numErrors = + reduce [i in TableSpace] (T$(i) != 0);
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