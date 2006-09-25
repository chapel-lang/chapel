config var logN = 5;
const N = 1 << logN;

var DW = [0..N/4-1];

var A: [DW] complex;

twiddles(A);


def twiddles(W: [0..?n] complex) {
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
  writeln("W is: ", W);
}
