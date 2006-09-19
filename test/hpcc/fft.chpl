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

  const D = [1..N];
  const DW = [1..N/4];

  var A: [D] complex;
  var B: [D] complex;
  var W: [DW] complex;

  prand_array(A);

  B = A;                       // save A for verification step

  twiddles(W);
  W = bit_reverse(W);

  A.imag = -A.imag;            // conjugate data

  A = bit_reverse(A);

  dfft(A, W);

  A.real =  A.real / N;        // conjugate and scale data
  A.imag = -A.imag / N;

  var fftTimer = timer();
  fftTimer.start();

  A = bit_reverse(A);
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

def bit_reverse(W) {
  return W;
/*
//  const WD = W.domain;
  const n = WD.extent();
//  const mask: uint = 0x0102040810204080;
  const shift = (log(n) / log(2)): integer;

  for i in 1..n {
    const ndx = MTA_BIT_MAT_OR(mask, MTA_BIT_MAT_OR(i, mask));
    ndx = MTA_ROTATE_LEFT(ndx, shift);
    v(2*ndx) = w(2*i);
    v(2*ndx + 1) = w(2*i + 1);
  }

  return V;
*/
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

//fun twiddles(W: [?DW] float) {
def twiddles(W: [?WD] complex) {
  // TODO: workaround until I figure out how to query domain's length:
  const n = (1<<logN)/4;
  // const n = WD(1).length();
  const delta = atan(1.0) / n;
  const i = 1.0;

  W(1) = 1.0;
  // TODO: need to figure out the best way to write this _complex
  W(n/2 + 1) = let cosDeltaN = cos(delta * n)
                in _complex(cosDeltaN, cosDeltaN);
  for i in 2..n/2 {
    // TODO: this is a bit unfortunate -- do we have other options?
    const deltaI = delta * 2*(i-1);
    const x = cos(deltaI);
    const y = sin(deltaI);
    W(i)         = _complex(x, y);
    W(n - i + 2) = _complex(y, x);
  }
  if (debug) then writeln("in twiddles(), W is: ", W);
}
