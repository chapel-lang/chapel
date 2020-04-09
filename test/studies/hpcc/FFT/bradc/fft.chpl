// BLC: For certain inputs, not having a complex/real
// function can result in numerical instability.

// BLC: clean up complex arithmetic

// BLC: insert problem size computation based on memory size?

use BitOps;
use Random;
use Time;


// problem size configs
config const logN = 5;

param radix = 4;

// pseudo-random input configs
config const deterministic = false,
             seed = 314159265;

// verification configs
config const epsilon = 2.0 ** -51.0,
             threshold = 16.0;

// boolean configs
config const printTiming = true;

// BLC: control printing out of error value to avoid portability issues


proc main() {
  // compute problem size
  const N = 1 << logN;

  // twiddle domain and arrays
  const TwiddleDom = {0..#N/4};
  var Twiddles: [TwiddleDom] complex;

  computeTwiddles(Twiddles);
  bitReverseShuffle(Twiddles);

  // problem domain and arrays
  const ProblemDom = {0..#N};
  var Z, z: [ProblemDom] complex;

  // generate pseudo-random input
  if deterministic then
    fillRandom(z, seed);
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


proc computeTwiddles(W) {
  const n = W.size;
  const delta = 2.0 * atan(1.0) / n;

  W(0) = 1.0;
  W(n/2) = let x = cos(delta * n/2)
            in (x, x):complex;
  forall i in 1..#n/2 {
    const x = cos(delta*i);
    const y = sin(delta*i);
    W(i)     = (x, y):complex;
    W(n - i) = (y, x):complex;
  }
}


proc bitReverseShuffle(W: [?WD]) {
  const n = WD.size;
  const reverse = log2(n);
  var Perm: [WD] index(WD) = [i in WD] bitReverse(i, numBits = reverse);
  const V = W(Perm);
  W = V;
}


// reverses numBits low-order bits of val
proc bitReverse(val: ?valType, numBits = 64) {
  param mask: uint(64) = 0x0102040810204080;
  const valReverse64 = bitMatMultOr(mask, bitMatMultOr(val:uint(64), mask));
  const valReverse = rotl(valReverse64, numBits);
  return valReverse: valType;
}



proc dfft(Z, W) {
  cft1st(Z, W);

  var span = radix;
  var lasti = 2;
  for i in radix..logN/2 by 2 {
    cftmd1(span, Z, W);
    span *= radix;
    lasti = i;
  }
  for i in lasti+2..logN-1 by 2 {
    cftmd2(span, Z, W);
    span *= radix;
  }

  const n = Z.size;
  if (radix*span == n) {
    forall j in 0..#span {
      butterfly(1.0, 1.0, 1.0, Z[j..j+3*span by span]);
    }
  } else {
    forall j in 0..#span {
      var a = Z(j);
      var b = Z(j+span);
      Z(j) = a + b;
      Z(j+span) = a - b;
    }
  }
}


proc verifyResults(z, Z, execTime, Twiddles) {
  const N = Z.size;

  // BLC: This line wants /(complex,real) to be implemented directly:
  Z = conjg(Z) / N;

  bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  // BLC: need to check this against written spec
  var maxerr = max reduce sqrt((z.re - Z.re)**2 + (z.im - Z.im)**2);

  maxerr = maxerr / logN / epsilon;

  write(if (maxerr < threshold) then "SUCCESS" else "FAILURE");
  writeln(if (maxerr < threshold) then "" else ", error = " + maxerr:string);
  writeln();
  writeln("N      = ", N);
  if (printTiming) {
    writeln("Time   = ", execTime);
    const gflop = 5.0 * N * logN / 1000000000.0;
    writeln("GFlops = ", gflop / execTime);
  }
}


proc cft1st(A, W) {
  const n = A.domain.dim(0).size;
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
  A(6) = (x2.im - x0.im, x0.re - x2.re):complex;
  x0 = x1 + x3rot;
  A(5) = wk1r * (x0.re - x0.im, x0.re + x0.im):complex;
  x0 = (x3.im + x1.re, x3.re - x1.im):complex;
  A(7) = wk1r * (x0.im - x0.re, x0.im + x0.re):complex;

  // BLC: would like to use an indefinite arithmetic array here
  // BLC: would also like to use () on both indices and zipping
  //      together of ranges
  forall (j,k1) in zip(8..n-1 by 8, 1..) {
    var wk2 = W(k1);
    var wk1 = W(2*k1);
    var wk3 = (wk1.re - 2* wk2.im * wk1.im, 
               2 * wk2.im * wk1.re - wk1.im):complex;

    butterfly(wk1, wk2, wk3, A[j..j+3]);

    wk1 = W(2*k1+1);
    wk3 = (wk1.re - 2*wk2.re * wk1.im, 
           2*wk2.re * wk1.re - wk1.im):complex;
    wk2 = wk2*1.0i;
    butterfly(wk1, wk2, wk3, A[j+4..j+7]);
  }
}


proc cftmd0(span, A, W) {
  var wk1r = W(1).re;
  const m = radix*span;

  forall j in 0..#span {
    butterfly(1.0, 1.0, 1.0, A[j..j+3*span by span]);
  }

  forall j in m..#span {
    butterfly((wk1r, wk1r):complex, 1.0i, (-wk1r, wk1r):complex,
              A[j..j+3*span by span]);
  }
}


proc cftmd1(span, A, W) {
  const m = radix*span;
  const m2 = 2*m;
  const n = A.domain.dim(0).size;

  cftmd0(span, A, W);
  forall (k,k1) in zip(m2..n-1 by m2, 1..) {
    var wk2 = W[k1];
    var wk1 = W[2*k1];
    var wk3 = (wk1.re - 2 * wk2.im * wk1.im,
               2 * wk2.im * wk1.re - wk1.im):complex;
    for j in k..#span {
      butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);
    }

    wk1 = W[2*k1+1];
    wk3 = (wk1.re - 2 * wk2.re * wk1.im,
           2 * wk2.re * wk1.re - wk1.im):complex;

    for j in k+m..#span {
      butterfly(wk1, wk2*1.0i, wk3, A[j..j+3*span by span]);
    }
  }
}


proc cftmd2(span, A, W) {
  var m = radix*span;
  var m2 = 2*m;
  const n = A.domain.dim(0).size;

  cftmd0(span, A, W);
  if (m2 >= n) then return;
  if (m2 >= n / 8) {
    cftmd21(span, A, W);
    return;
  }

  forall j in 0..#span {
    forall (k,k1) in zip(m2..n-1 by m2, 1..) {
      var wk2 = W[k1];
      var wk1 = W[k1 + k1];
      var wk3 = (wk1.re - 2*wk2.im * wk1.im,
                 2 * wk2.im * wk1.re - wk1.im):complex;
      butterfly(wk1, wk2, wk3, A[j+k..j+k+3*span by span]);
    }

    forall (k,k1) in zip(m2..n-1 by m2, 1..) {
      var wk2 = W[k1];
      var wk1 = W[2*k1 + 1];
      var wk3 = (wk1.re - 2*wk2.re * wk1.im,
                 2*wk2.re * wk1.re - wk1.im):complex;
      wk2 = wk2*1.0i;

      butterfly(wk1, wk2, wk3, A[j+k+m..j+k+m+3*span by span]);
    }
  }
}


proc cftmd21(span, A, W) {
  const n = A.domain.dim(0).size;
  var m = radix*span;
  var m2 = 2*m;

  for (k,k1) in zip(m2..n-1 by m2, 1..) {
    var wk2 = W[k1];
    var wk1 = W[2*k1];
    var wk3 = (wk1.re - 2*wk2.im * wk1.im,
               2* wk2.im * wk1.re - wk1.im):complex;

    forall j in k..#span {
      butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);
    }

    wk1 = W[2*k1 + 1];
    wk3 = (wk1.re - 2*wk2.re * wk1.im,
           2*wk2.re * wk1.re - wk1.im):complex;
    wk2 = wk2*1.0i;

    forall j in k+m..#span {
      butterfly(wk1, wk2, wk3, A[j..j+3*span by span]);
    }
  }
}


proc butterfly(wk1, wk2, wk3, inout A:[?D]) {
  const i1 = D.low,
        i2 = i1 + D.stride,
        i3 = i2 + D.stride,
        i4 = i3 + D.stride;
  var x0 = A[i1] + A[i2];
  var x1 = A[i1] - A[i2];
  var x2 = A[i3] + A[i4];
  var x3rot = (A[i3] - A[i4])*1.0i;

  A[i1] = x0 + x2;
  x0 -= x2;
  A[i3] = wk2 * x0;
  x0 = x1 + x3rot;
  A[i2] = wk1 * x0;
  x0 = x1 - x3rot;
  A[i4] = wk3 * x0;
}


