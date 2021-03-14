// Modules for timing and random number generator routines
use Time;
use Random;

// constants related to memory
param MB = 1024**2,
      GB = 1024*MB;

// constants and config constants used to compute the problem size
param numVectors = 3;

config const memInMBs = 3;
const memInBytes = memInMBs*MB;

type elemType = real(64);
const elemSizeInBytes = numBits(elemType)/bitsPerByte;
const nBytes = 3*elemSizeInBytes;

const maxIntBits2 = numBits(int) - 2;
const maxPossibleElems = (memInBytes/elemSizeInBytes)/numVectors;
const flg2 = log2(maxPossibleElems);
config const usePow2VectorSize = false;

config const vectorSize = if flg2 <= maxIntBits2
                          then if usePow2VectorSize then (1 << flg2)
                                                    else maxPossibleElems
                          else 1 << maxIntBits2;
// the vectors
const VecDomain: domain(1) = {1..vectorSize};
var A, B, C: [VecDomain] elemType;

// config constants for output
config const doIO = true;
config const showTiming = false;

// config constant for scalar in TRIAD computation
config const scalar = 3.0;

// config constant for setting the number of times the computation is repeated
config const numIters = 10;
const iterDomain = {1..numIters};

// timing array and variables for results
var time: [iterDomain] real;
var avgtime, sumtime, mintime, maxtime: real;
var curGBs: real;

// setting the seed for the random number generator
param seed = 314159265;

proc main() {
  var clock: Timer;

  initStreamVectors();

  if doIO then writeStreamData();

  clock.start();
  for k in iterDomain {
    time(k) = clock.elapsed();
    A = B + scalar * C;
    time(k) = clock.elapsed() - time(k);
  }
  clock.stop();

  computeStreamResults();

  if (showTiming && doIO) then writeStreamResults();

  if (checkSTREAMresults() && doIO) then writeln("Solution Failed!");
}

proc initStreamVectors() {
  var randlist = new borrowed NPBRandomStream(eltType=real, seed=seed);

  randlist.fillRandom(A);
  randlist.fillRandom(B);
  randlist.fillRandom(C);

  A = 2.0 * A;
}

proc computeStreamResults() {

  sumtime = + reduce time[2..numIters];
  mintime = min reduce time[2..numIters];
  maxtime = max reduce time[2..numIters];
}


proc checkSTREAMresults() {
  var randlist = new unmanaged NPBRandomStream(real, seed);

  var Aref, Bref, Cref, error : [VecDomain] elemType;

  randlist.fillRandom(Aref);
  randlist.fillRandom(Bref);
  randlist.fillRandom(Cref);

  delete randlist;

  for i in VecDomain {
    Aref(i) = 2.0 * Aref(i);
  }

  for k in iterDomain {
    for i in VecDomain {
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

proc writeStreamData() {
  param HLINE="-------------------------------------------------------------";
  writeln(HLINE);
  writeln("This system uses ", elemSizeInBytes, " bytes per 64-bit real.");
  writeln(HLINE);
  writeln("Array size (B) = ", vectorSize*elemSizeInBytes);
  writeln("Total memory required (GB) = ",
          (numVectors * elemSizeInBytes) * (vectorSize / GB:real));
  writeln(HLINE);
}

proc writeStreamResults() {
  writeln( "Function\tRate (GB/s)\tAvg time\tMin time\tMax time");
  curGBs = mintime;
  curGBs *= 1.0e-9 * nBytes * vectorSize;
  avgtime = sumtime/(numIters-1);  // skipped the 1st iteration
  writeln( "Triad    \t", curGBs, "\t", avgtime, "\t", mintime, "\t", maxtime);
}

