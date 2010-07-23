use BlockDist, Time;

use HPCCProblemSize, RARandomStream; 

type indexType = randType,
  elemType = int(64);

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

def main() {
  printConfiguration(); 
  
  [i in TableSpace] T(i) = 0;
 
  const startTime = getCurrentTime();               // capture the start time

  forall ( , r) in (Updates, LCGRAStream()) {
    on TableDist.idxToLocale(r >> (64 - n)) {
      const s = LCGgetNextRandom(r);  
      if safeUpdates {
	atomic {
	  T(r >> (64 - n)) += 1;
	  T(s >> (64 - n)) -= 1;
	}
      } else {
	T(r >> (64 - n)) += 1;
	T(s >> (64 - n)) -= 1;
      }
    }
  }

  const execTime = getCurrentTime() - startTime;   // capture the end time

  writeln("Begin Update"); 

  const validAnswer = verifyResults();             // verify the updates
  printResults(validAnswer, execTime);             // print the results}
}

def printConfiguration() {
  if (printParams) {
    if (printStats) then writeln("Number of Locales = ", numLocales);
    printProblemSize(elemType, 1, m);
    writeln("Atomic Update = ", safeUpdates);
    writeln("Number of updates = ", N_U, "\n");
  }
}

def verifyResults() {
  if (printArrays) then writeln("After updates, T is: ", T, "\n");

  var startTime = getCurrentTime();

  forall ( , r) in (Updates, LCGRAStream()) {
    on TableDist.idxToLocale(r >> (64 - n)) {
      const s = LCGgetNextRandom(r);  
      atomic {
	T(r >> (64 - n)) -= 1;
	T(s >> (64 - n)) += 1;
      }
    }
  }

  const verifyTime = getCurrentTime() - startTime; 

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