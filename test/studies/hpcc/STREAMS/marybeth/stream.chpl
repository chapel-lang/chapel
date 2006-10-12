// Modules for timing and random number generator routines
use Timers;
use Random;

// constants related to memory
param MB = 1024**2,
      GB = 1024*MB,
      bitsPerByte = 8;

// constants and config constants used to compute the problem size
param numVectors = 3;

config const memInMBs = 3;
const memInBytes = memInMBs*MB;

type elemType = float(64);
const elemSizeInBytes = bits(elemType)/bitsPerByte;
const bytes = 3*elemSizeInBytes;

const maxIntBits2 = bits(int) - 2;
const maxPossibleElems = (memInBytes/elemSizeInBytes)/numVectors;
const flg2 = lg(maxPossibleElems);
config const usePow2VectorSize = false;

config const vectorSize = if flg2 <= maxIntBits2
                          then if usePow2VectorSize then (1 << flg2)
                                                    else maxPossibleElems
                          else 1 << maxIntBits2;
// the vectors
var A, B, C: [1..vectorSize] elemType;

// config constants for output
config const doIO = true;
config const showTiming = false;

// config constant for scalar in TRIAD computation
config const scalar = 3.0;

// config constant for setting the number of times the computation is repeated
config const numIters = 10;
const iterDomain = [1..numIters];

// timing array and variables for results
var time: [iterDomain] float;
var avgtime, sumtime, mintime, maxtime: float;
var curGBs: float;

// setting the seed for the random number generator
const seed = 333444.0;

def main() {
  var clock: Timer;

  initStreamVectors();

  if doIO then writeStreamData();

  clock.start();
  for k in iterDomain {
    time(k) = clock.value;
    A = B + scalar * C;
    time(k) = clock.value - time(k);
  }
  clock.stop();

  computeStreamResults();

  if (showTiming && doIO) then writeStreamResults();

  if (checkSTREAMresults() && doIO) then writeln("Solution Failed!");
}

def initStreamVectors() {
  var randlist = rng(seed);

  randlist.fillRandom(A);
  randlist.fillRandom(B);
  randlist.fillRandom(C);

  A = 2.0 * A;
}

def computeStreamResults() {

  sumtime = + reduce time[2..numIters];
  mintime = min reduce time[2..numIters];
  maxtime = max reduce time[2..numIters];
}


def checkSTREAMresults() {
  var randlist = rng(seed);
  var vector = [1..vectorSize];

  var Aref, Bref, Cref, error : [vector] elemType;

  randlist.fillRandom(Aref);
  randlist.fillRandom(Bref);
  randlist.fillRandom(Cref);

  for i in vector {
    Aref(i) = 2.0 * Aref(i);
  }

  for k in iterDomain {
    for i in vector {
      Aref(i) = Bref(i) + scalar*Cref(i);
    }
  }

  const arefsum = max reduce (abs(Aref));
  const asum = max reduce (abs(A));
  const esum = max reduce (abs(A - Aref));

  if doIO {
    writeln("Results Comparison: ");
    writeln("        Expected  : ", arefsum);
    writeln("        Observed  : ", asum);
  }

  param epsilon = 1.0e-8;
  var failure = true;
  if (esum/asum > epsilon) {
    if doIO {
      writeln("Failed Validation on array a[]");
      writeln("        Expected  : ", arefsum);
      writeln("        Observed  : ", asum);
    }
  } else {
    failure = false;
    if doIO then writeln("Solution Validates");
  }
  return failure;
}

def writeStreamData() {
  param HLINE="-------------------------------------------------------------";
  writeln(HLINE);
  writeln("This system uses ", elemSizeInBytes, " bytes per 64-bit float.");
  writeln(HLINE);
  writeln("Array size (B) = ", vectorSize*elemSizeInBytes);
  writeln("Total memory required (GB) = ",
          (numVectors * elemSizeInBytes) * (vectorSize / GB:float));
  writeln(HLINE);
}

def writeStreamResults() {
  writeln( "Function\tRate (GB/s)\tAvg time\tMin time\tMax time");
  curGBs = mintime;
  curGBs *= 1.0e-9 * bytes * vectorSize;
  avgtime = sumtime/(numIters-1);  // skipped the 1st iteration
  writeln( "Triad    ", curGBs:"\t%g", avgtime:"\t%g", mintime:"\t%g", maxtime:"\t%g");
}

