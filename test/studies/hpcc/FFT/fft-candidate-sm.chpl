// TODO: clean up complex arithmetic
use BitOps;
use Random;
use Time;

use HPCCProblemSize;


param radix = 4;

param numVectors = 2;
type elemType = complex(128);


config const n = computeProblemSize(elemType, numVectors, returnLog2 = true);
const m = 2**n;

config const epsilon = 2.0 ** -51.0,
             threshold = 16.0;

config const useRandomSeed = true,
             seed = if useRandomSeed then SeedGenerator.clockMS else 314159265;

config const printParams = true,
             printArrays = false,
             printStats = true;


def main() {
  printConfiguration();

  const TwiddleDom = [0..m/4);
  var Twiddles: [TwiddleDom] elemType;

  const ProblemDom = [0..m);
  var Z, z: [ProblemDom] elemType;

  initVectors(Twiddles, z, Z);

  const startTime = getCurrentTime();

  bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  const execTime = getCurrentTime() - startTime;

  const validAnswer = verifyResults(z, Z, execTime, Twiddles);
  printResults(validAnswer, execTime);
}


def dfft(Z, W) {
  cft1st(Z, W);

  var span = radix;
  var lasti = 2;
  for i in radix..n/2 by 2 {
    cftmd1(span, Z, W);
    span *= radix;
    lasti = i;
  }
  for i in [lasti+2..n) by 2 {
    cftmd2(span, Z, W);
    span *= radix;
  }

  const vectSize = Z.numElements;
  if (radix*span == vectSize) then
    forall j in [0..span) do
      butterfly(1.0, 1.0, 1.0, Z[j..j+3*span by span]);
  else
    forall j in [0..span) {
      var a = Z(j);
      var b = Z(j+span);
      Z(j) = a + b;
      Z(j+span) = a - b;
    }
}


def printConfiguration() {
  if (printStats) then printProblemSize(elemType, numVectors, m);
}


def initVectors(Twiddles, z, Z) {
  computeTwiddles(Twiddles);
  bitReverseShuffle(Twiddles);

  fillRandom(z, seed);
  Z = conjg(z);

  if (printArrays) {
    writeln("After initialization, Twiddles is: ", Twiddles, "\n");
    writeln("z is: ", z, "\n");
    writeln("Z is: ", Z, "\n");
  }

}


def computeTwiddles(W) {
  const nt = W.numElements;
  const delta = 2.0 * atan(1.0) / nt;

  W(0) = 1.0;
  W(nt/2) = let x = cos(delta * nt/2)
            in (x, x):elemType;
  forall i in [1..nt/2) {
    const x = cos(delta*i);
    const y = sin(delta*i);
    W(i)      = (x, y):elemType;
    W(nt - i) = (y, x):elemType;
  }
}


def bitReverseShuffle(W: [?WD]) {
  const numBits = log2(W.numElements),
        Perm: [i in WD] index(WD) = bitReverse(i, revBits = numBits),
        V = W(Perm);
  W = V;
}


// reverses numBits low-order bits of val
def bitReverse(val: ?valType, revBits = 64) {
  param mask: uint(64) = 0x0102040810204080;
  const valReverse64 = bitMatMultOr(mask, bitMatMultOr(val:uint(64), mask));
  const valReverse = bitRotLeft(valReverse64, revBits);
  return valReverse: valType;
}



def verifyResults(z, Z, execTime, Twiddles) {
  Z = conjg(Z) / m;
  bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  var maxerr = max reduce sqrt((z.re - Z.re)**2 + (z.im - Z.im)**2);
  maxerr /= (epsilon * n);

  return (maxerr < threshold);
}


def printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    writeln("Performance (Gflop/s) = ", 5.0 * (m * n) / execTime / 1.0e-9);
  }
}


def cft1st(A, W) {
  const n = A.numElements;
  var x0 = A(0) + A(1);
  var x1 = A(0) - A(1);
  var x2 = A(2) + A(3);
  var x3rot = (A(2) - A(3))*1.0i;

  const wk1r = W(1).re;

  A(0) = x0 + x2;
  A(2) = x0 - x2;
  A(1) = x1 + x3rot;
  A(3) = x1 - x3rot;

  x0 = A(4) + A(5);
  x1 = A(4) - A(5);
  x2 = A(6) + A(7);
  var x3 = A(6) - A(7);  // BLC: try to eliminate this
  x3rot = (A(6) - A(7))*1.0i;
  A(4) = x0 + x2;
  A(6) = (x2.im - x0.im, x0.re - x2.re):elemType;
  x0 = x1 + x3rot;
  A(5) = wk1r * (x0.re - x0.im, x0.re + x0.im):elemType;
  x0 = (x3.im + x1.re, x3.re - x1.im):elemType;
  A(7) = wk1r * (x0.im - x0.re, x0.im + x0.re):elemType;

  // BLC: would like to use an indefinite arithmetic array here
  // BLC: would also like to use () on both indices and zipping
  //      together of ranges
  forall (j,k1) in ([8..n) by 8, 1..) {
    var wk2 = W(k1);
    var wk1 = W(2*k1);
    var wk3 = (wk1.re - 2* wk2.im * wk1.im, 
               2 * wk2.im * wk1.re - wk1.im):elemType;

    butterfly(wk1, wk2, wk3, A[j..j+3]);

    wk1 = W(2*k1+1);
    wk3 = (wk1.re - 2*wk2.re * wk1.im, 
           2*wk2.re * wk1.re - wk1.im):elemType;
    wk2 = wk2*1.0i;
    butterfly(wk1, wk2, wk3, A[j+4..j+7]);
  }
}


def cftmd0(span, A, W) {
  var wk1r = W(1).re;
  const m = radix*span;

  forall j in [0..span) do
    butterfly(1.0, 1.0, 1.0, A[j..j+3*span by span]);

  forall j in [m..m+span) do
    butterfly((wk1r, wk1r):elemType, 1.0i, (-wk1r, wk1r):elemType,
              A[j..j+3*span by span]);
}


def cftmd1(span, A, W) {
  const m = radix*span;
  const m2 = 2*m;
  const n = A.numElements;

  cftmd0(span, A, W);
  forall (k,k1) in ([m2..n) by m2, 1..) {
    var wk2 = W[k1];
    var wk1 = W[2*k1];
    var wk3 = (wk1.re - 2 * wk2.im * wk1.im,
               2 * wk2.im * wk1.re - wk1.im):elemType;
    for j in [k..k+span) do
      butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);

    wk1 = W[2*k1+1];
    wk3 = (wk1.re - 2 * wk2.re * wk1.im,
           2 * wk2.re * wk1.re - wk1.im):elemType;

    for j in [k+m..k+m+span) do
      butterfly(wk1, wk2*1.0i, wk3, A[j..j+3*span by span]);
  }
}


def cftmd2(span, A, W) {
  var m = radix*span;
  var m2 = 2*m;
  const n = A.numElements;

  cftmd0(span, A, W);
  if (m2 >= n) return;
  if (m2 >= n / 8) {
    cftmd21(span, A, W);
    return;
  }

  forall j in [0..span) {
    forall (k,k1) in ([m2..n) by m2, 1..) {
      var wk2 = W[k1];
      var wk1 = W[k1 + k1];
      var wk3 = (wk1.re - 2*wk2.im * wk1.im,
                 2 * wk2.im * wk1.re - wk1.im):elemType;
      butterfly(wk1, wk2, wk3, A[j+k..j+k+3*span by span]);
    }

    forall (k,k1) in ([m2..n) by m2, 1..) {
      var wk2 = W[k1];
      var wk1 = W[2*k1 + 1];
      var wk3 = (wk1.re - 2*wk2.re * wk1.im,
                 2*wk2.re * wk1.re - wk1.im):elemType;
      wk2 = wk2*1.0i;

      butterfly(wk1, wk2, wk3, A[j+k+m..j+k+m+3*span by span]);
    }
  }
}


def cftmd21(span, A, W) {
  const n = A.numElements;
  var m = radix*span;
  var m2 = 2*m;

  for (k,k1) in ([m2..n) by m2, 1..) {
    var wk2 = W[k1];
    var wk1 = W[2*k1];
    var wk3 = (wk1.re - 2*wk2.im * wk1.im,
               2* wk2.im * wk1.re - wk1.im):elemType;

    forall j in [k..k+span) do
      butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);

    wk1 = W[2*k1 + 1];
    wk3 = (wk1.re - 2*wk2.re * wk1.im,
           2*wk2.re * wk1.re - wk1.im):elemType;
    wk2 = wk2*1.0i;

    forall j in [k+m..k+m+span) do
      butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);
  }
}


def butterfly(wk1, wk2, wk3, inout A:[1..radix]) {
  var x0 = A[1] + A[2];
  var x1 = A[1] - A[2];
  var x2 = A[3] + A[4];
  var x3rot = (A[3] - A[4])*1.0i;

  A[1] = x0 + x2;
  x0 -= x2;
  A[3] = wk2 * x0;
  x0 = x1 + x3rot;
  A[2] = wk1 * x0;
  x0 = x1 - x3rot;
  A[4] = wk3 * x0;
}


