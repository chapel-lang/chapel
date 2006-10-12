use Random;
use Timers;

// BLC: clean up complex arithmetic

config const logN = 5;
config const debug = false;
config const printTiming = false;

// BLC: This call needs to go away
def resetA(A: [?D] complex) {
  /*
  [i in D] {
    A(i).real = 2*i;
    A(i).imag = A(i).real + 1.0;
  }
  */
}

def writeA(A) {
  write("a[] =");
  for i in A.domain do write(A(i):" %g %g");
  writeln();
}




def main() {
  const N = 1 << logN;
  const gflop = 5.0 * N * logN / 1000000000.0;

  const EPS = 2.0 ** -51.0;
  const THRESHOLD = 16.0;

  const D = [0..N-1];
  const DW = [0..N/4-1];

  var A: [D] complex;
  var B: [D] complex;
  var W: [DW] complex;

  [i in D] {
    A(i).real = 2*i;
    A(i).imag = A(i).real + 1.0;
  }

  fillRandomVec(A);
  if debug then writeA(A);

  B = A;                       // save A for verification step

  twiddles(W);
  W = bitReverse(W);


  // BLC: would like this to be:
  //  A.imag = -A.imag;            // conjugate data
  A = conjg(A);
  if debug then writeA(A);

  var fftTimer: Timer;
  fftTimer.start();

  A = bitReverse(A);

  // BLC: This line needs to go away
  resetA(A);

  // BLC: Why was John timing the second version?
  if debug then writeA(A);
  dfft(A, W);
  if debug then writeA(A);
  fftTimer.stop();

  // BLC: originally wrote this as:
  //  A.real =  A.real / N;        // conjugate and scale data
  //  A.imag = -A.imag / N;
  // but it doesn't currently work.  Maybe that's OK because the
  // rewrite is pretty clean

  // BLC: This line only works if /(complex,real) is implemented
  // correctly:
  A = conjg(A) / N;
  
  if debug then writeA(A);

  A = bitReverse(A);
  dfft(A, W);
  if debug then writeA(A);

  var time = fftTimer.value;

  if debug then {
    var C: [D] float = sqrt((B.real - A.real)**2 + (B.imag - A.imag)**2);
    writeln("error[] =", C);
  }
  var maxerr = max reduce sqrt((B.real - A.real)**2 + (B.imag - A.imag)**2);

  maxerr = maxerr / logN / EPS;

  var status = if (maxerr < THRESHOLD) then "SUCCESS" else "FAILURE";
  writeln(status, ", error = ", maxerr);
  writeln("\n");

  writeln("N      = ", N);
  if (printTiming) {
    writeln("Time   = ", time);
    writeln("GFlops = ", gflop / time);
  }
}


def twiddles(W: [?WD] complex) {
  const n = WD(1).length;
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
def bitReverse(W: [?WD] complex) {  // BLC: would be nice to drop complex?
  const n = WD(1).length;
  const reverse = lg(n);
  var V: [WD] complex;
  /* BLC: could we do this as a permutation instead?
  var P: [i in WD] index(WD) = i;
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


def dfft(A: [?AD] complex, W) {
  /*
  if (debug) {
    write("w[] =");
    for i in W.domain do write(W(i):" %g %g");
    writeln();
  }
  */
  
  if debug then writeln("cft1st();");
  cft1st(A, W);

  var l = 4;
  var lasti = 2;
  for i in 4..logN/2 by 2 {
    if debug then writeln("cftmd1(l=", l, ", A, W);");
    cftmd1(l, A, W);
    l *= 4;
    lasti = i;
  }
  //  writeln("lasti+2 = ", lasti+2, " logN-1 = ", logN-1);
  for i in lasti+2..logN-1 by 2 {
    if debug then writeln("i=", i, "  cftmd2(l=", l, ", A, W);");
    cftmd2(l, A, W);
    if debug then writeln("i=", i, "  returned from cftmd2();");
    l *= 4;
  }

  const n = AD(1).length;
  if ((l << 2) == n) {
    if debug then writeln("l << 2 == n");
    forall j in 0..l-1 {
      resetA(A);
      butterfly(1.0, 1.0, 1.0, A[j..j+3*l by l]);
    }
  } else {
    if debug then writeln("l << 2 != n");
    forall j in 0..l-1 {
      var a = A(j);
      var b = A(j+l);
      A(j) = a + b;
      A(j+l) = a - b;
    }
  }
}


def cft1st(A, W) {
  const n = A.domain(1).length;
  var x0 = A(0) + A(1);
  var x1 = A(0) - A(1);
  var x2 = A(2) + A(3);
  var x3 = A(2) - A(3);

  const wk1r = W(1).real;

  if debug then writeA(A);

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

  if debug then writeln("  // computes first 8 complexes manually");

  if debug then writeA(A);

  var k1 = 1;
  forall j in 8..n-1 by 8 {
    var wk2 = W(k1);
    var wk1 = W(2*k1);
    var wk3 = (wk1.real - 2* wk2.imag * wk1.imag, 
               2 * wk2.imag * wk1.real - wk1.imag):complex;

    resetA(A);
    butterfly(wk1, wk2, wk3, A[j..j+3]);

    //    writeln("accessing: ", 2*k1+1);
    wk1 = W(2*k1+1);
    wk3 = (wk1.real - 2*wk2.real * wk1.imag, 
           2*wk2.real * wk1.real - wk1.imag):complex;
    wk2 = (-wk2.imag, wk2.real): complex;
    resetA(A);
    butterfly(wk1, wk2, wk3, A[j+4..j+7]);

    k1 += 1;
  }
}


def cftmd0(l, A, W) {
  var wk1r = W(1).real;
  const m = l << 2;

  //  if debug then writeA(A);
  
  forall j in 0..l-1 {
    resetA(A);
    butterfly(1.0, 1.0, 1.0, A[j..j+3*l by l]);
  }

  forall j in m..l+m-1 {
    resetA(A);
    butterfly((wk1r, wk1r):complex, 1.0i, (-wk1r, wk1r):complex,
              A[j..j+3*l by l]);
  }
}


def cftmd1(l, A, W) {
  const m = l << 2;
  const m2 = 2*m;
  const n = A.domain(1).length;

  if debug then writeln("  cftmd0(l=", l, ", A, W);");
  cftmd0(l, A, W);
  if debug then writeln("  returned from cftmd0();");
  var k1 = 1;
  forall k in m2..n-1 by m2 {
    var wk2 = W[k1];
    var wk1 = W[2*k1];
    var wk3 = (wk1.real - 2 * wk2.imag * wk1.imag,
               2 * wk2.imag * wk1.real - wk1.imag):complex;
    for j in k..l+k-1 {
      resetA(A);
      butterfly(wk1, wk2, wk3, A[j..j+3*l by l]);
    }

    wk1 = W[2*k1+1];
    wk3 = (wk1.real - 2 * wk2.real * wk1.imag,
           2 * wk2.real * wk1.real - wk1.imag):complex;

    for j in k+m..l+k+m-1 {
      resetA(A);
      butterfly(wk1, (-wk2.imag, wk2.real):complex, wk3, A[j..j+3*l by l]);
    }

    k1 += 1;
  }
}


def cftmd2(l, A, W) {
  var m = l << 2;
  var m2 = 2*m;
  const n = A.domain(1).length;

  if debug then writeln("  cftmd0(l=", l, ", A, W);");
  cftmd0(l, A, W);
  if debug then writeln("  returned from cftmd0();");
  if (m2 >= n) return;
  if (m2 >= n / 8) {
    if debug then writeln("  cftmd21(l=", l, ", A, W);");
    cftmd21(l, A, W);
    if debug then writeln("  returned from cftmd21();");
    return;
  }

  forall j in 0..l-1 {
    var k1 = 1;  // BLC: zip this in
    forall k in m2..n-1 by m2 {
      var wk2 = W[k1];
      var wk1 = W[k1 + k1];
      var wk3 = (wk1.real - 2*wk2.imag * wk1.imag,
                 2 * wk2.imag * wk1.real - wk1.imag): complex;
      resetA(A);
      butterfly(wk1, wk2, wk3, A[j+k..j+k+3*l by l]);

      k1 += 1;
    }

    k1 = 1;  // BLC: zip this in
    forall k in m2..n-1 by m2 {
      var wk2 = W[k1];
      var wk1 = W[2*k1 + 1];
      var wk3 = (wk1.real - 2*wk2.real * wk1.imag,
                 2*wk2.real * wk1.real - wk1.imag): complex;
      wk2 = (-wk2.imag, wk2.real): complex;

      resetA(A);
      butterfly(wk1, wk2, wk3, A[j+k+m..j+k+m+3*l by l]);

      k1 += 1;
    }
  }
}


def cftmd21(l, A, W) {
  const n = A.domain(1).length;
  var m = l << 2;
  var m2 = 2*m;
  var m3 = 3*m;

  var k1 = 1;
  for k in m2..n-1 by m2 {
    var wk2 = W[k1];
    var wk1 = W[2*k1];
    var wk3 = (wk1.real - 2*wk2.imag * wk1.imag,
               2* wk2.imag * wk1.real - wk1.imag): complex;

    forall j in k..k+l-1 {
      resetA(A);
      butterfly(wk1, wk2, wk3, A[j..j+3*l by l]);
    }

    wk1 = W[2*k1 + 1];
    wk3 = (wk1.real - 2*wk2.real * wk1.imag,
           2*wk2.real * wk1.real - wk1.imag): complex;
    wk2 = (-wk2.imag, wk2.real): complex;

    forall j in k+m..k+m+l-1 {
      resetA(A);
      butterfly(wk1, wk2, wk3, A[j..j+3*l by l]);
    }

    k1 += 1;
  }
}


def butterfly(wk1: complex, wk2: complex, wk3: complex, 
              inout A:[1..4] complex) {
  var x0 = A[1] + A[2];
  var x1 = A[1] - A[2];
  var x2 = A[3] + A[4];
  var x3 = A[3] - A[4];

  if (debug) {
    writeln("    a=", A[1]:"{%g,%g}", ", b=", A[2]:"{%g,%g}", ", c=", A[3]:"{%g,%g}", ", d=", A[4]:"{%g,%g}");
    writeln("      wk1=", wk1:"%g + %gi");
    writeln("      wk2=", wk2:"%g + %gi");
    writeln("      wk3=", wk3:"%g + %gi");

    writeln();

    writeln("      x0=", x0:"%g %g");
    writeln("      x1=", x1:"%g %g");
    writeln("      x2=", x2:"%g %g");
    writeln("      x3=", x3:"%g %g");
  }

  A[1] = x0 + x2;
  x0 -= x2;
  if debug then writeln("      x0=", x0:"%g %g");
  A[3] = wk2 * x0;
  x0 = (x1.real - x3.imag, x1.imag + x3.real):complex;
  if debug then writeln("      x0=", x0:"%g %g");
  A[2] = wk1 * x0;
  x0 = (x1.real + x3.imag, x1.imag - x3.real):complex;
  if debug then writeln("      x0=", x0:"%g %g");
  A[4] = wk3 * x0;

  if (debug) {
    writeln("    a=", A[1]:"{%g,%g}", ", b=", A[2]:"{%g,%g}", ", c=", A[3]:"{%g,%g}", ", d=", A[4]:"{%g,%g}\n");
  }
}
