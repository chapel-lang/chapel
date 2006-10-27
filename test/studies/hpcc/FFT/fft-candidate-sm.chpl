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

  initVectors(Twiddles, z);

  const startTime = getCurrentTime();

  Z = conjg(z);
  bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  const execTime = getCurrentTime() - startTime;

  const validAnswer = verifyResults(z, Z, Twiddles);
  printResults(validAnswer, execTime);
}


def dfft(Z, W) {
  cft1st(Z, W);

  var span = radix,
      lasti = 2;
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
      const a = Z(j),
            b = Z(j+span);
      Z(j) = a + b;
      Z(j+span) = a - b;
    }
}


def printConfiguration() {
  if (printParams) then printProblemSize(elemType, numVectors, m);
}


def initVectors(Twiddles, z) {
  computeTwiddles(Twiddles);
  bitReverseShuffle(Twiddles);

  fillRandom(z, seed);

  if (printArrays) {
    writeln("After initialization, Twiddles is: ", Twiddles, "\n");
    writeln("z is: ", z, "\n");
  }
}


def computeTwiddles(W) {
  const nt = W.numElements;
  const delta = 2.0 * atan(1.0) / nt;

  W(0) = 1.0;
  W(nt/2) = let x = cos(delta * nt/2)
            in (x, x):complex;
  forall i in [1..nt/2) {
    const x = cos(delta*i);
    const y = sin(delta*i);
    W(i)      = (x, y):complex;
    W(nt - i) = (y, x):complex;
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



def verifyResults(z, Z, Twiddles) {
  if (printArrays) then writeln("After FFT, Z is: ", Z, "\n");

  Z = conjg(Z) / m;
  bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  if (printArrays) then writeln("After inverse FFT, Z is: ", Z, "\n");

  var maxerr = max reduce sqrt((z.re - Z.re)**2 + (z.im - Z.im)**2);
  maxerr /= (epsilon * n);
  if (printStats) then writeln("error = ", maxerr);

  return (maxerr < threshold);
}


def printResults(successful, execTime) {
  writeln("Validation: ", if successful then "SUCCESS" else "FAILURE");
  if (printStats) {
    writeln("Execution time = ", execTime);
    writeln("Performance (Gflop/s) = ", 5.0 * (m * n) / execTime / 1.0e-9);
  }
}


def butterfly(wk1, wk2, wk3, inout A:[1..radix]) {
  var x0 = A[1] + A[2],
      x1 = A[1] - A[2],
      x2 = A[3] + A[4],
      x3rot = (A[3] - A[4])*1.0i;

  A[1] = x0 + x2;
  x0 -= x2;
  A[3] = wk2 * x0;
  x0 = x1 + x3rot;
  A[2] = wk1 * x0;
  x0 = x1 - x3rot;
  A[4] = wk3 * x0;
}


def cft1st(A, W) {
  const n = A.numElements;
  var x0 = A(0) + A(1),
      x1 = A(0) - A(1),
      x2 = A(2) + A(3),
      x3rot = (A(2) - A(3))*1.0i;
  const wk1r = W(1).re;

  A(0) = x0 + x2;
  A(2) = x0 - x2;
  A(1) = x1 + x3rot;
  A(3) = x1 - x3rot;

  x0 = A(4) + A(5);
  x1 = A(4) - A(5);
  x2 = A(6) + A(7);
  const x3 = A(6) - A(7);  // BLC: try to eliminate this
  x3rot = (A(6) - A(7))*1.0i;
  A(4) = x0 + x2;
  A(6) = (x2.im - x0.im, x0.re - x2.re):complex;
  x0 = x1 + x3rot;
  A(5) = wk1r * (x0.re - x0.im, x0.re + x0.im):complex;
  x0 = (x3.im + x1.re, x3.re - x1.im):complex;
  A(7) = wk1r * (x0.im - x0.re, x0.im + x0.re):complex;

  forall (j,k1) in ([8..n) by 8, 1..) {
    var wk2 = W(k1),
        wk1 = W(2*k1),
        wk3 = (wk1.re - 2* wk2.im * wk1.im,
               2 * wk2.im * wk1.re - wk1.im):complex;

    butterfly(wk1, wk2, wk3, A[j..j+3]);

    wk1 = W(2*k1+1);
    wk3 = (wk1.re - 2*wk2.re * wk1.im, 
           2*wk2.re * wk1.re - wk1.im):complex;
    wk2 = wk2*1.0i;
    butterfly(wk1, wk2, wk3, A[j+4..j+7]);
  }
}


def cftmd0(span, A, W) {
  const wk1r = W(1).re,
        m = radix*span;

  forall j in [0..span) do
    butterfly(1.0, 1.0, 1.0, A[j..j+3*span by span]);

  forall j in [m..m+span) do
    butterfly((wk1r, wk1r):complex, 1.0i, (-wk1r, wk1r):complex,
              A[j..j+3*span by span]);
}


def cftmd1(span, A, W) {
  const m = radix*span;
  const m2 = 2*m;
  const n = A.numElements;

  cftmd0(span, A, W);
  forall (k,k1) in ([m2..n) by m2, 1..) {
    var wk2 = W[k1],
        wk1 = W[2*k1],
        wk3 = (wk1.re - 2 * wk2.im * wk1.im,
               2 * wk2.im * wk1.re - wk1.im):complex;
    for j in [k..k+span) do
      butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);

    wk1 = W[2*k1+1];
    wk3 = (wk1.re - 2 * wk2.re * wk1.im,
           2 * wk2.re * wk1.re - wk1.im):complex;

    for j in [k+m..k+m+span) do
      butterfly(wk1, wk2*1.0i, wk3, A[j..j+3*span by span]);
  }
}


def cftmd2(span, A, W) {
  const m = radix*span,
        m2 = 2*m,
        n = A.numElements;

  cftmd0(span, A, W);
  if (m2 >= n) return;
  if (m2 >= n / 8) {
    cftmd21(span, A, W);
    return;
  }

  forall j in [0..span) {
    forall (k,k1) in ([m2..n) by m2, 1..) {
      const wk2 = W[k1],
            wk1 = W[k1 + k1],
            wk3 = (wk1.re - 2*wk2.im * wk1.im,
                   2 * wk2.im * wk1.re - wk1.im):complex;
      butterfly(wk1, wk2, wk3, A[j+k..j+k+3*span by span]);
    }

    forall (k,k1) in ([m2..n) by m2, 1..) {
      const wk2 = W[k1],
            wk1 = W[2*k1 + 1],
            wk3 = (wk1.re - 2*wk2.re * wk1.im,
                   2*wk2.re * wk1.re - wk1.im):complex;
      wk2 = wk2*1.0i;

      butterfly(wk1, wk2, wk3, A[j+k+m..j+k+m+3*span by span]);
    }
  }
}


def cftmd21(span, A, W) {
  const n = A.numElements,
        m = radix*span,
        m2 = 2*m;

  for (k,k1) in ([m2..n) by m2, 1..) {
    var wk2 = W[k1],
        wk1 = W[2*k1],
        wk3 = (wk1.re - 2*wk2.im * wk1.im,
               2* wk2.im * wk1.re - wk1.im):complex;

    forall j in [k..k+span) do
      butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);

    wk1 = W[2*k1 + 1];
    wk3 = (wk1.re - 2*wk2.re * wk1.im,
           2*wk2.re * wk1.re - wk1.im):complex;
    wk2 = wk2*1.0i;

    forall j in [k+m..k+m+span) do
      butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);
  }
}
