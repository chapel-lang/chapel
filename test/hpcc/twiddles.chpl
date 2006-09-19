config var logN = 5;
const N = 1 << logN;

var D = [1..N];
var DW = [1..N/4];

var A: [DW] complex;

twiddles(A);


// BLC: What would this look like if we translated to 0-based
// coordinates locally?
def twiddles(W: [?WD] complex) {
  // TODO: workaround until I figure out how to query domain's length:
  const n = N/4;
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
  writeln("W is: ", W);
}
