config var logN = 5;
config var debug = false;

// PLACEHOLDER for external stuff
class timer {
  def start();
  def stop();
  def gettime() { return 1.0; }
}
// END PLACEHOLDER


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

  prand_array(A);

  B = A;                       // save A for verification step

  twiddles(W);
  W = bitReverse(W);

  A.imag = -A.imag;            // conjugate data

  A = bitReverse(A);

  dfft(A, W);

  A.real =  A.real / N;        // conjugate and scale data
  A.imag = -A.imag / N;

  var fftTimer = timer();
  fftTimer.start();

  A = bitReverse(A);
  dfft(A, W);

  fftTimer.stop();
  var time = fftTimer.gettime();

  var maxerr = max reduce sqrt((B.real - A.real)**2 + (B.imag - A.imag)**2);

  maxerr = maxerr / logN / EPS;

  var status = if (maxerr < THRESHOLD) then "SUCCESS" else "FAILURE";
  writeln(status, ", error = ", maxerr);
  writeln("\n");

  writeln("N      = ", N);
  writeln("Time   = ", time);
  writeln("GFlops = ", gflop / time);
}


def twiddles(W: [?WD] complex) {
  const n = WD(1).length;
  const delta = 2.0 * atan(1.0) / n;

  W(0) = 1.0;
  // TODO: need to figure out the best way to write this _complex
  W(n/2) = let cosDeltaN = cos(delta * n/2)
            in _complex(cosDeltaN, cosDeltaN);
  for i in 1..n/2-1 {
    const x = cos(delta*i);
    const y = sin(delta*i);
    W(i)     = _complex(x, y);
    W(n - i) = _complex(y, x);
  }
}


def bitReverse(W: [?WD] complex) {  // BLC: would be nice to drop complex?
  const n = WD(1).length;
  const reverse = lg(n);
  var V: [WD] complex;
  for i in WD {
    var ndx = bitReverse(i:uint(64), numBits=reverse);
    V(ndx:int) = W(i); // BLC: unfortunate cast
  }
  return V;
}


def dfft(A: [?AD] complex, W) {
  cft1st(A, W);

  var l = 4;
  for i in 4..logN/2 by 2 {
    cftmd1(l, A, W);
    l *= 4;
  }
  for i in logN/2+2..logN-1 by 2 {
    cftmd2(l, A, W);
    l *= 4;
  }

  const n = AD(1).length;
  if ((l << 2) == n) {
    for j in 0..l-1 {
      butterfly(1.0, 1.0, 1.0, A(j), A(j+l), A(j+2*l), A(j+3*l));
    }
  } else {
    for j in 0..l-1 {
      var x0 = A(j).real;
      var x1 = A(j).imag;
      A(j).real = x0+x1;
      A(j).imag = x0-x1;
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

  A(0) = x0 + x2;
  A(2) = x0 - x2;
  A(1) = _complex(x1.real - x3.imag, x1.imag + x3.real);
  A(3) = _complex(x1.real + x3.imag, x1.imag - x3.real);

  x0 = A(4) + A(5);
  x1 = A(4) - A(5);
  x2 = A(6) + A(7);
  x3 = A(6) - A(7);
  A(4) = x0 + x2;
  A(6) = _complex(x2.imag - x0.imag, x0.real - x2.real);
  x0 = _complex(x0.real - x3.imag, x1.imag + x3.real);
  A(5) = wk1r * _complex(x0.real - x0.imag, x0.real + x0.imag);
  x0 = _complex(x3.imag + x1.real, x3.real - x1.imag);
  A(7) = wk1r * _complex(x0.imag - x0.real, x0.imag + x0.real);

  var k1 = 1;
  for j in 8..n-1 by 8 {
    var wk2 = W(k1);
    var wk1 = W(2*k1);
    var wk3 = _complex(wk1.real - 2* wk2.imag * wk1.imag, 
                       2 * wk2.imag * wk1.real - wk1.imag);

    butterfly(wk1, wk2, wk3, A(j), A(j+1), A(j+2), A(j+3));

    k1 += 1;
  }
}


def cftmd0(l, A, W) {
  var wk1r = W(1).real;
  const m = l << 2;

  for j in 0..l-1 by 2 {
    butterfly(1.0, 1.0, 1.0, A(j), A(j+l), A(j+2*l), A(j+3*l));
  }

  for j in m..l+m by 2 {
    butterfly(_complex(wk1r, wk1r), -1.0i, _complex(-wk1r, wk1r), A(j), A(j+l), A(j+2*l), A(j+3*l));
  }
}

def cftmd1(l, A, W) {
  const m = l << 2;
  const m2 = 2*m;
  const n = A.domain(1).length;

  cftmd0(l, A, W);
  var k1 = 1;
  for k in m2..n-1 by m2 {
    var wk2 = W[k1];
    var wk1 = W[2*k1];
    var wk3 = _complex(wk1.real - 2 * wk2.imag * wk1.imag,
                       2 * wk2.imag * wk1.real - wk1.imag);
    for j in k..l+k-1 by 2 {
      butterfly(wk1, wk2, wk3, A(j), A(j+l), A(j+2*l), A(j+3*l));
    }

    wk1 = W[2*k1+1];
    wk3 = _complex(wk1.real - 2 * wk2.real * wk1.imag,
                   2 * wk2.real * wk1.real - wk1.imag);

    for j in k+m..l+k+m-1 by 2 {
      butterfly(wk1, _complex(-wk2.imag, wk2.real), wk3, A(j), A(j+l), A(j+2*l), A(j+3*l));
    }

    k1 += 1;
  }
}

def cftmd2(l, A, W) {
}

def prand_array(X) {
}

def butterfly(wk1: complex, wk2: complex, wk3: complex, 
              inout a, inout b, inout c, inout d) {
  var x0 = a + b;
  var x1 = a - b;
  var x2 = c + d;
  var x3 = c - d;

  a = x0 + x2;
  x0 -= x2;
  c = wk2 * x0;
  x0 = _complex(x1.real - x3.imag, x1.imag + x3.real);
  b = wk1 * x0;
  x0 = _complex(x1.real + x3.imag, x1.imag - x3.real);
  d = wk3 * x0;
}
