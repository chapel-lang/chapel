// constants related to memory
param MB = 1024**2,
      GB = 1024*MB,
      bitsPerByte = 8;

// constants and config constants used to compute the problem size
param numVectors = 3;

config const memInMBs = 3;
const memInBytes = memInMBs*MB;

type elemType = real(64);
const elemSizeInBytes = numBits(elemType)/bitsPerByte;

const maxIntBits2 = numBits(int) - 2;
const maxPossibleElems = (memInBytes/elemSizeInBytes)/numVectors;
const flg2 = log2(maxPossibleElems);
config const usePow2VectorSize = false;

config const vectorSize = if flg2 <= maxIntBits2
                          then if usePow2VectorSize then (1 << flg2)
                                                    else maxPossibleElems
                          else 1 << maxIntBits2;


// the vectors
var A, B, C: [1..vectorSize] elemType;


// config constants for steering the computation
config const numIters = 10;
config const scalar = 3.0;

config const doIO = true;


proc main() {
  param HLINE="-------------------------------------------------------------";

  if doIO {
    writeln(HLINE);
    writeln("This system uses ", elemSizeInBytes, " bytes per 64-bit real.");
    writeln(HLINE);
    writeln("Array size (B) = ", vectorSize*elemSizeInBytes);
    writeln("Total memory required (GB) = ", 
            (numVectors * elemSizeInBytes) * (vectorSize / GB:real));
  }

  A = 1.0;
  B = 2.0;
  C = 0.0;

  if doIO then writeln(HLINE);

  A = 2.0 * A;

  for k in 1..numIters {
    C = A;
    B = scalar * C;
    C = A + B;
    A = B + scalar * C;
  }

  if (checkSTREAMresults()) {
    writeln("Solution Failed!");
  }
}


proc checkSTREAMresults() {
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

  if doIO {
    writeln("Results Comparison: ");
    writeln("        Expected  : ", aj, bj, cj);
    writeln("        Observed  : ", asum, bsum, csum);
  }

  param epsilon = 1.0e-8;
  var failure = true;
  if (abs(aj-asum)/asum > epsilon) {
    if doIO {
      writeln("Failed Validation on array a[]");
      writeln("        Expected  : ", aj);
      writeln("        Observed  : ", asum);
    }
  } else if (abs(bj-bsum)/bsum > epsilon) {
    if doIO {
      writeln("Failed Validation on array b[]");
      writeln("        Expected  : ", bj);
      writeln("        Observed  : ", bsum);
    }
  } else if (abs(cj-csum)/csum > epsilon) {
    if doIO {
      writeln("Failed Validation on array c[]");
      writeln("        Expected  : ", cj);
      writeln("        Observed  : ", csum);
    }
  } else {
    failure = false;
    if doIO then writeln("Solution Validates");
  }
  return failure;
}
