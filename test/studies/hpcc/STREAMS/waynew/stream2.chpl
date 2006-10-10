use Timers;

// constants related to memory
param MB = 1024**2,
      GB = 1024*MB,
      bitsPerByte = 8;

// constants and config constants used to compute the problem size
enum tests {COPY, SCALE, ADD, TRIAD};
param numVectors = 3;

config const memInMBs = 3;
const memInBytes = memInMBs*MB;

type elemType = float(64);
const elemSizeInBytes = bits(elemType)/bitsPerByte;

const maxIntBits2 = bits(int) - 2;
const maxPossibleElems = (memInBytes/elemSizeInBytes)/numVectors;
const flg2 = lg(maxPossibleElems);
config const usePow2VectorSize = false;

config const vectorSize = if flg2 <= maxIntBits2
                          then if usePow2VectorSize then (1 << flg2)
                                                    else maxPossibleElems
                          else 1 << maxIntBits2;


// config constants for steering the computation
config const numIters = 10;
config const scalar = 3.0;

// the vectors
var A, B, C: [1..vectorSize] elemType;

def main() {
  param HLINE = "-------------------------------------------------------------";

  var bytes: [COPY..TRIAD] elemType;
  bytes[COPY]  = 2 * elemSizeInBytes;
  bytes[ADD]   = 2 * elemSizeInBytes;
  bytes[SCALE] = 3 * elemSizeInBytes;
  bytes[TRIAD] = 3 * elemSizeInBytes;

  writeln( HLINE);
  writeln( "This system uses ", elemSizeInBytes, " bytes per 64-bit float.");
  writeln( HLINE);
  writeln( "Array size (B) = ", vectorSize*elemSizeInBytes);
  writeln( "Total memory required (GB) = ", 
           (numVectors * elemSizeInBytes) * (vectorSize / GB:float));

  A = 1.0;
  B = 2.0;
  C = 0.0;

  writeln( HLINE);

  A = 2.0 * A;

  // var clock: Timer;
  var time: [COPY..TRIAD, 1..numIters] float;
  // clock.start();
  for i in 1..numIters {
    // time[COPY, i] = clock.value;
    C = A;
    // time[COPY, i] = clock.value - time[COPY, i];

    // time[SCALE, i] = clock.value;
    B = scalar * C;
    // time[SCALE, i] = clock.value - time[SCALE, i];

    // time[ADD, i] = clock.value;
    C = A + B;
    // time[ADD, i] = clock.value - time[ADD, i];

    // time[TRIAD, i] = clock.value;
    A = B + scalar * C;
    // time[TRIAD, i] = clock.value - time[TRIAD, i];
  }

  if (checkSTREAMresults()) {
    writeln("Solution Failed!");
  }

  var avgtime, sumtime, mintime, maxtime: [COPY..TRIAD] float;
  for test in COPY..TRIAD {
    sumtime[test] = + reduce time[test, 2..numIters];
    mintime[test] = min reduce time[test, 2..numIters];
    maxtime[test] = max reduce time[test, 2..numIters];
  }

  writeln( "Function\tRate (GB/s)\tAvg time\tMin time\tMax time");
  for test in COPY..TRIAD {
    var curGBs = mintime[test];
    curGBs *= 1.0e-9 * bytes[test] * vectorSize;
    avgtime[test] = sumtime[test]/(numIters-1);  // skipped the 1st iteration
    writeln( test, curGBs:"\t\t%g", avgtime[test]:"\t\t%g", mintime[test]:"\t\t%g", maxtime[test]:"\t\t%g");
  }
}


def checkSTREAMresults(): bool {
  var aj = 1.0;
  var bj = 2.0;
  var cj = 0.0;

  aj = 2.0 * aj;

  for k in 1..numIters {
    cj = aj;
    bj = scalar * cj;
    cj = aj + bj;
    aj = bj + scalar * cj;
  }
  aj = aj * vectorSize;
  bj = bj * vectorSize;
  cj = cj * vectorSize;

  const asum = + reduce A;;
  const bsum = + reduce B;;
  const csum = + reduce C;;

  writeln( "Results Comparison: ");
  writeln( "        Expected  : ", aj, bj, cj);
  writeln( "        Observed  : ", asum, bsum, csum);

  param epsilon = 1.0e-8;
  if (abs(aj-asum)/asum > epsilon) {
    writeln( "Failed Validation on array a[]");
    writeln( "        Expected  : ", aj);
    writeln( "        Observed  : ", asum);
  } else if (abs(bj-bsum)/bsum > epsilon) {
    writeln( "Failed Validation on array b[]");
    writeln( "        Expected  : ", bj);
    writeln( "        Observed  : ", bsum);
  } else if (abs(cj-csum)/csum > epsilon) {
    writeln( "Failed Validation on array c[]");
    writeln( "        Expected  : ", cj);
    writeln( "        Observed  : ", csum);
  } else {
    writeln( "Solution Validates");
    return false;
  }
  return true;
}
