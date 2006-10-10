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


// config constants for steering the computation
config const numIters = 10;
config const scalar = 3.0;

config const doIO = true;


def main() {
  use Random;
  var generator = rng(333444.0);

  if doIO then writeStreamData();

  generator.fillRandom(A);
  generator.fillRandom(B);
  generator.fillRandom(C);

  A = 2.0 * A;

  for k in 1..numIters {
    A = B + scalar * C;
  }

  if (checkSTREAMresults()) {
    writeln("Solution Failed!");
  }
}


def checkSTREAMresults() {
  use Random;
  var generator = rng(333444.0);
  var vector = [1..vectorSize];

  var Aref, Bref, Cref, error : [vector] elemType;

  generator.fillRandom(Aref);
  generator.fillRandom(Bref);
  generator.fillRandom(Cref);

  for i in vector {
    Aref(i) = 2.0 * Aref(i);
  }

  for k in 1..numIters {
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
