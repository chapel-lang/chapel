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


def bitReverse(W: [?WD] complex) {
  const n = WD(1).length;
  const reverse = lg(n);
  var V: [WD] complex;
  for i in WD {
    var ndx = bitReverse(i:uint(64), numBits=reverse);
    V(ndx:int) = W(i); // BLC: unfortunate cast
  }
  return V;
}

def dfft(a, w) {
/*
  cf1st(n, a, w);

  var l = 8;
  for i in 4..logn/2 by 2 {
    l *= 4;
    cftmd1(n, l, a, w);
  }
//  for i = 
*/
}

def prand_array(X) {
}

