config const n = 4;
config const epsilon = 1.0e-5;

const stencil = (/(-1, 0), (1, 0), (0, -1), (0, 1)/);

const D = [1..n, 1..n] distributed(block);
const BigD = D.expand(1);
const SouthD = D.exterior(1, 0);

var A, Temp : [BigD] float;

Temp(SouthD) = 1.0;

do {
  [i in D] Temp(i) = mysum reduce A(i + stencil) / 4.0;
  if abs(A - Temp) / max > epsilon {
    A = Temp;
    break;
  }
  [i in D] A(i) = Temp(i + stencil) / + / 4.0;
} while abs(A - Temp) / max > epsilon;

writeln(A);
