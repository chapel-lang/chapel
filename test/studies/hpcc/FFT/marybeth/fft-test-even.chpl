use BitOps;
use Random;
use Time;


// problem size configs
config const logN = 6;

// pseudo-random input configs
config const deterministic = false,
             seed = 314159265;

// verification configs
config const epsilon = 2.0 ** -51.0,
             threshold = 16.0;

// boolean configs
config const printTiming = true;
config const printError = true;

proc main() {
  // compute problem size
  const N = 1 << logN;

  // twiddle domain and arrays
  const TwiddleDom = {0..#N/4};
  var Twiddles: [TwiddleDom] complex;

  computeTwiddles(Twiddles);
  Twiddles = bitReverseShuffle(Twiddles);

  // problem domain and arrays
  const ProblemDom: domain(1) = {0..#N};
  var Z, z: [ProblemDom] complex;
  var realtemp, imagtemp: [ProblemDom] real;

  // generate pseudo-random input
  if deterministic then
    fillRandom(z, seed);
  else
    fillRandom(z);

  // conjugate input, storing result to work array
  Z = conjg(z);


  // TIMED SECTION
  var startTime = getCurrentTime();

  Z = bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  var execTime = getCurrentTime() - startTime;

  verifyResults(z, Z, execTime, Twiddles);
}


proc computeTwiddles(W) {
  const n = W.size;
  const delta = 2.0 * atan(1.0) / n;

  W(0) = 1.0;
  W(n/2) = let cosDeltaN = cos(delta * n/2)
            in (cosDeltaN, cosDeltaN):complex;
  forall i in 1..n/2-1 {
    const x = cos(delta*i);
    const y = sin(delta*i);
    W(i)     = (x, y):complex;
    W(n - i) = (y, x):complex;
  }
}


// Check what the NSA supports for bit reversal
// rename this?
proc bitReverseShuffle(W: [?WD]) {
  const n = WD.size;
  const reverse = log2(n);
  var V: [WD] W.eltType;  // BLC: rename this field?
  /* BLC: could we do this as a permutation instead?
  var P: [WD] index(WD) = [i in WD] i;
  bitReverse(P);
  V(P) = W;
  */
  forall i in WD {  // BLC: could this be a uint domain?
    // BLC: what does this bitReverse function do with high-order bits?
    // BLC: could this be rewritten as one line?
    var ndx = bitReverse(i:uint(64), numBits=reverse);
    V(ndx:int) = W(i); // BLC: unfortunate cast
  }
  // BLC: W = V would be preferable
  return V;
}


// reverses numBits low-order bits of val
proc bitReverse(val: ?valType, numBits = 64) {
  param mask: uint(64) = 0x0102040810204080;
  const valReverse64 = bitMatMultOr(mask, bitMatMultOr(val:uint(64), mask));
  const valReverse = rotl(valReverse64, numBits);
  return valReverse: valType;
}


proc dfft(A: [?AD] complex, W) {
  
  var l = 1;
  var lasti:int;
  var m, m2: int;

  const n = AD.dim(0).size;

  for i in 2..logN by 2 {
    m = l << 2;
    m2 = 2*m;
    if (m2 > n) then break;
    forall k in 0..#n by m2 {
      var k1 = k/m2;
      var wk2 = W[k1];
      var wk1 = W[2*k1];
      var wk3 = (wk1.re - 2 * wk2.im * wk1.im,
                 2 * wk2.im * wk1.re - wk1.im):complex;
      for j in k..#l {
        butterfly(wk1, wk2, wk3, A[j..j+3*l by l]);
      }

      wk1 = W[2*k1+1];
      wk3 = (wk1.re - 2 * wk2.re * wk1.im,
             2 * wk2.re * wk1.re - wk1.im):complex;

      for j in k+m..#l {
        butterfly(wk1, (-wk2.im, wk2.re):complex, wk3, A[j..j+3*l by l]);
      }
    }
    l *= 4;
    lasti = i;
  }

  if ((l << 2) == n) {
    forall j in 0..#l {
      butterfly(1.0, 1.0, 1.0, A[j..j+3*l by l]);
    }
  } else {
    forall j in 0..#l {
      var a = A(j);
      var b = A(j+l);
      A(j) = a + b;
      A(j+l) = a - b;
    }
  }
}


proc verifyResults(z, Z, execTime, Twiddles) {
  const N = Z.size;

  // BLC: This line wants /(complex,real) to be implemented directly:
  Z = conjg(Z) / N;

  Z = bitReverseShuffle(Z);
  dfft(Z, Twiddles);

  // BLC: need to check this against written spec
  var maxerr = max reduce sqrt((z.re - Z.re)**2 + (z.im - Z.im)**2);

  maxerr = maxerr / logN / epsilon;

  write(if (maxerr < threshold) then "SUCCESS" else "FAILURE");
  if (printError) then writeln(", error = ", maxerr);
  writeln();
  writeln("N      = ", N);
  if (printTiming) {
    writeln(", error = ", maxerr);
    writeln("Time   = ", execTime);
    const gflop = 5.0 * N * logN / 1000000000.0;
    writeln("GFlops = ", gflop / execTime);
  }
}

proc butterfly(wk1: complex, wk2: complex, wk3: complex, 
              inout A:[?D] complex) {
  const i1 = D.low,
        i2 = i1 + D.stride,
        i3 = i2 + D.stride,
        i4 = i3 + D.stride;
  var x0 = A[i1] + A[i2];
  var x1 = A[i1] - A[i2];
  var x2 = A[i3] + A[i4];
  var x3 = A[i3] - A[i4];

  A[i1] = x0 + x2;
  x0 -= x2;
  A[i3] = wk2 * x0;
  x0 = (x1.re - x3.im, x1.im + x3.re):complex;
  A[i2] = wk1 * x0;
  x0 = (x1.re + x3.im, x1.im - x3.re):complex;
  A[i4] = wk3 * x0;
}
