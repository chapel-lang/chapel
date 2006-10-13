// BLC: For certain inputs, not having a complex/real
// function can result in numerical instability.

// BLC: clean up complex arithmetic

// BLC: insert problem size computation based on memory size?

use BitOps;
use Random;
use Time;


// problem size configs
config const logN = 5;

// pseudo-random input configs
config const deterministic = false,
             seed = 314159265.0,
             arand = 1220703125.0;

// verification configs
config const epsilon = 2.0 ** -51.0,
             threshold = 16.0;

// boolean configs
config const printTiming = true;

// BLC: control printing out of error value to avoid portability issues


def main() {
  // compute problem size
  const N = 1 << logN;

  // twiddle domain and arrays
  const TwiddleDom = [0..N/4);
  var Twiddles: [TwiddleDom] complex;

  computeTwiddles(Twiddles);
  bitReverseShuffle(Twiddles);

  // problem domain and arrays
  const ProblemDom = [0..N);
  var Z, z: [ProblemDom] complex;

  // generate pseudo-random input
  if deterministic then
    fillRandom(z, seed=seed, arand=arand);
  else
    fillRandom(z);

  // conjugate input, storing result to work array
  Z = conjg(z);


  // TIMED SECTION
  var startTime = getCurrentTime();

  bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  var execTime = getCurrentTime() - startTime;

  verifyResults(z, Z, execTime, Twiddles);
}


def computeTwiddles(W) {
  const n = W.numElements;
  const delta = 2.0 * atan(1.0) / n;

  W(0) = 1.0;
  W(n/2) = let cosDeltaN = cos(delta * n/2)
            in (cosDeltaN, cosDeltaN):complex;
  forall i in [1..n/2) {
    const x = cos(delta*i);
    const y = sin(delta*i);
    W(i)     = (x, y):complex;
    W(n - i) = (y, x):complex;
  }
}


def bitReverseShuffle(W: [?WD]) {
  const n = WD.numIndices;
  const reverse = lg(n);
  var V: [WD] W.elt_type;  // BLC: rename elt_type to remove underscore?
  // BLC: would like to use:
  //  var Perm: [i in WD] int = bitReverse(i:uint(64), numBits = reverse):int;
  var Perm: [WD] index(WD);
  [i in WD] Perm(i) = bitReverse(i, numBits = reverse);
  // BLC: would like to use:
  // V(Perm) = W;
  [i in WD] V(Perm(i)) = W(i);
  W = V;
}


// reverses numBits low-order bits of val
def bitReverse(val: ?valType, numBits = 64) {
  param mask: uint(64) = 0x0102040810204080;
  const valReverse64 = bitMatMultOr(mask, bitMatMultOr(val:uint(64), mask));
  const valReverse = bitRotLeft(valReverse64, numBits);
  return valReverse: valType;
}



def dfft(A: [?AD] complex, W) {
  cft1st(A, W);

  var l = 4;
  var lasti = 2;
  for i in 4..logN/2 by 2 {
    cftmd1(l, A, W);
    l *= 4;
    lasti = i;
  }
  for i in [lasti+2..logN) by 2 {
    cftmd2(l, A, W);
    l *= 4;
  }

  const n = AD(1).length;
  if ((l << 2) == n) {
    forall j in [0..l) {
      butterfly(1.0, 1.0, 1.0, A[j..j+3*l by l]);
    }
  } else {
    forall j in [0..l) {
      var a = A(j);
      var b = A(j+l);
      A(j) = a + b;
      A(j+l) = a - b;
    }
  }
}


def verifyResults(z, Z, execTime, Twiddles) {
  const N = Z.numElements;

  // BLC: This line wants /(complex,real) to be implemented directly:
  Z = conjg(Z) / N;

  bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  // BLC: need to check this against written spec
  var maxerr = max reduce sqrt((z.real - Z.real)**2 + (z.imag - Z.imag)**2);

  maxerr = maxerr / logN / epsilon;

  write(if (maxerr < threshold) then "SUCCESS" else "FAILURE");
  writeln(", error = ", maxerr);
  writeln();
  writeln("N      = ", N);
  if (printTiming) {
    writeln("Time   = ", execTime);
    const gflop = 5.0 * N * logN / 1000000000.0;
    writeln("GFlops = ", gflop / execTime);
  }
}


def cft1st(A, W) {
  const n = A.domain(1).length;
  var x0 = A(0) + A(1);
  var x1 = A(0) - A(1);
  var x2 = A(2) + A(3);
  var x3 = A(2) - A(3);

  const wk1r = W(1).real;

  A(0) = x0 + x2;
  A(2) = x0 - x2;
  A(1) = (x1.real - x3.imag, x1.imag + x3.real):complex;
  A(3) = (x1.real + x3.imag, x1.imag - x3.real):complex;

  x0 = A(4) + A(5);
  x1 = A(4) - A(5);
  x2 = A(6) + A(7);
  x3 = A(6) - A(7);
  A(4) = x0 + x2;
  A(6) = (x2.imag - x0.imag, x0.real - x2.real):complex;
  x0 = (x1.real - x3.imag, x1.imag + x3.real):complex;
  A(5) = wk1r * (x0.real - x0.imag, x0.real + x0.imag):complex;
  x0 = (x3.imag + x1.real, x3.real - x1.imag):complex;
  A(7) = wk1r * (x0.imag - x0.real, x0.imag + x0.real):complex;

  // BLC: would like to use an indefinite arithmetic array here
  // BLC: would also like to use () on both indices and zipping
  //      together of ranges
  forall j,k1 in [8..n) by 8, 1..(n-8)/8 {
    var wk2 = W(k1);
    var wk1 = W(2*k1);
    var wk3 = (wk1.real - 2* wk2.imag * wk1.imag, 
               2 * wk2.imag * wk1.real - wk1.imag):complex;

    butterfly(wk1, wk2, wk3, A[j..j+3]);

    wk1 = W(2*k1+1);
    wk3 = (wk1.real - 2*wk2.real * wk1.imag, 
           2*wk2.real * wk1.real - wk1.imag):complex;
    wk2 = (-wk2.imag, wk2.real): complex;
    butterfly(wk1, wk2, wk3, A[j+4..j+7]);
  }
}


def cftmd0(l, A, W) {
  var wk1r = W(1).real;
  const m = l << 2;

  forall j in [0..l) {
    butterfly(1.0, 1.0, 1.0, A[j..j+3*l by l]);
  }

  forall j in [m..m+l) {
    butterfly((wk1r, wk1r):complex, 1.0i, (-wk1r, wk1r):complex,
              A[j..j+3*l by l]);
  }
}


def cftmd1(l, A, W) {
  const m = l << 2;
  const m2 = 2*m;
  const n = A.domain(1).length;

  cftmd0(l, A, W);
  forall k,k1 in [m2..n) by m2, 1..(n-m2)/m2 {
    var wk2 = W[k1];
    var wk1 = W[2*k1];
    var wk3 = (wk1.real - 2 * wk2.imag * wk1.imag,
               2 * wk2.imag * wk1.real - wk1.imag):complex;
    for j in [k..k+l) {
      butterfly(wk1, wk2, wk3, A[j..j+3*l by l]);
    }

    wk1 = W[2*k1+1];
    wk3 = (wk1.real - 2 * wk2.real * wk1.imag,
           2 * wk2.real * wk1.real - wk1.imag):complex;

    for j in [k+m..k+m+l) {
      butterfly(wk1, (-wk2.imag, wk2.real):complex, wk3, A[j..j+3*l by l]);
    }
  }
}


def cftmd2(l, A, W) {
  var m = l << 2;
  var m2 = 2*m;
  const n = A.domain(1).length;

  cftmd0(l, A, W);
  if (m2 >= n) return;
  if (m2 >= n / 8) {
    cftmd21(l, A, W);
    return;
  }

  forall j in [0..l) {
    forall k,k1 in [m2..n) by m2, 1..(n-m2)/m2 {
      var wk2 = W[k1];
      var wk1 = W[k1 + k1];
      var wk3 = (wk1.real - 2*wk2.imag * wk1.imag,
                 2 * wk2.imag * wk1.real - wk1.imag): complex;
      butterfly(wk1, wk2, wk3, A[j+k..j+k+3*l by l]);
    }

    forall k,k1 in [m2..n) by m2, 1..(n-m2)/m2 {
      var wk2 = W[k1];
      var wk1 = W[2*k1 + 1];
      var wk3 = (wk1.real - 2*wk2.real * wk1.imag,
                 2*wk2.real * wk1.real - wk1.imag): complex;
      wk2 = (-wk2.imag, wk2.real): complex;

      butterfly(wk1, wk2, wk3, A[j+k+m..j+k+m+3*l by l]);
    }
  }
}


def cftmd21(l, A, W) {
  const n = A.domain(1).length;
  var m = l << 2;
  var m2 = 2*m;
  var m3 = 3*m;

  for k,k1 in [m2..n) by m2, 1..(n-m2)/m2 {
    var wk2 = W[k1];
    var wk1 = W[2*k1];
    var wk3 = (wk1.real - 2*wk2.imag * wk1.imag,
               2* wk2.imag * wk1.real - wk1.imag): complex;

    forall j in [k..k+l) {
      butterfly(wk1, wk2, wk3, A[j..j+3*l by l]);
    }

    wk1 = W[2*k1 + 1];
    wk3 = (wk1.real - 2*wk2.real * wk1.imag,
           2*wk2.real * wk1.real - wk1.imag): complex;
    wk2 = (-wk2.imag, wk2.real): complex;

    forall j in [k+m..k+m+l) {
      butterfly(wk1, wk2, wk3, A[j..j+3*l by l]);
    }
  }
}


def butterfly(wk1: complex, wk2: complex, wk3: complex, 
              inout A:[1..4] complex) {
  var x0 = A[1] + A[2];
  var x1 = A[1] - A[2];
  var x2 = A[3] + A[4];
  var x3 = A[3] - A[4];

  A[1] = x0 + x2;
  x0 -= x2;
  A[3] = wk2 * x0;
  x0 = (x1.real - x3.imag, x1.imag + x3.real):complex;
  A[2] = wk1 * x0;
  x0 = (x1.real + x3.imag, x1.imag - x3.real):complex;
  A[4] = wk3 * x0;
}
