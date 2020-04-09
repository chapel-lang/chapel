config var logN = 5;
const N = 1 << logN;

var DW = {1..N/4};

var A: [DW] complex;

twiddles(A);


proc twiddles(W: [?WD] complex) {
  const n = WD.dim(0).size;
  const delta = atan(1.0) / n;
  const i = 1.0;

  W(1) = 1.0;
  // TODO: need to figure out the best way to write this _complex
  W(n/2 + 1) = let cosDeltaN = cos(delta * n)
                in (cosDeltaN, cosDeltaN):complex;
  for i in 2..n/2 {
    const deltaI = delta * 2*(i-1);
    const x = cos(deltaI);
    const y = sin(deltaI);
    W(i)         = (x, y):complex;
    W(n - i + 2) = (y, x):complex;
  }
  writeln("W is: ", W);
}
